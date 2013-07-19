#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "FileSystemInformation.h"
#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include "..\..\..\..\Common\Source\Parsing\ParsingDefinitions.h"
#include "..\..\..\..\Common\Source\Stream\MemoryStream.h"

using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::tabling;
using namespace vl::parsing::definitions;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}


/***********************************************************************
SymbolLookup
***********************************************************************/

class TypeSymbol : public Object
{
public:
	WString									typeName;
	Dictionary<WString, Ptr<TypeSymbol>>	subTypes;
	TypeSymbol*								parent;

	TypeSymbol()
		:parent(0)
	{
	}

	void CollectTypes(Ptr<ParsingTreeArray> types, Dictionary<ParsingTreeNode*, TypeSymbol*>& nodeTypeMap)
	{
		if(types)
		{
			for(int i=0;i<types->Count();i++)
			{
				Ptr<ParsingTreeObject> type=types->GetItem(i).Cast<ParsingTreeObject>();
				if(type)
				{
					Ptr<ParsingTreeToken> name=type->GetMember(L"name").Cast<ParsingTreeToken>();
					if(name && !subTypes.Keys().Contains(name->GetValue()))
					{
						Ptr<TypeSymbol> symbol=new TypeSymbol;
						symbol->typeName=name->GetValue();
						symbol->parent=this;
						subTypes.Add(symbol->typeName, symbol);
						symbol->CollectTypes(type->GetMember(L"subTypes").Cast<ParsingTreeArray>(), nodeTypeMap);
						nodeTypeMap.Add(type.Obj(), symbol.Obj());
					}
				}
			}
		}
	}
};

class ParserDecl : public TypeSymbol
{
public:
	SortedList<WString>							tokens;
	SortedList<WString>							rules;
	Dictionary<ParsingTreeNode*, TypeSymbol*>	nodeTypeMap;

	ParserDecl(Ptr<ParsingTreeObject> parserDecl)
	{
		nodeTypeMap.Add(parserDecl.Obj(), this);
		CollectTypes(parserDecl->GetMember(L"types").Cast<ParsingTreeArray>(), nodeTypeMap);
		{
			Ptr<ParsingTreeArray> items=parserDecl->GetMember(L"tokens").Cast<ParsingTreeArray>();
			if(items)
			{
				for(int i=0;i<items->Count();i++)
				{
					Ptr<ParsingTreeObject> type=items->GetItem(i).Cast<ParsingTreeObject>();
					if(type)
					{
						Ptr<ParsingTreeToken> name=type->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							tokens.Add(name->GetValue());
						}
					}
				}
			}
		}
		{
			Ptr<ParsingTreeArray> items=parserDecl->GetMember(L"rules").Cast<ParsingTreeArray>();
			if(items)
			{
				for(int i=0;i<items->Count();i++)
				{
					Ptr<ParsingTreeObject> type=items->GetItem(i).Cast<ParsingTreeObject>();
					if(type)
					{
						Ptr<ParsingTreeToken> name=type->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							rules.Add(name->GetValue());
						}
					}
				}
			}
		}
	}
};

/***********************************************************************
RepeatingTaskExecutor
***********************************************************************/

template<typename T>
class RepeatingTaskExecutor : public Object
{
protected:
	virtual void							Execute(const T& input)=0;
private:
	SpinLock								inputLock;
	T										inputData;
	volatile bool							inputDataAvailable;
	SpinLock								executingEvent;
	volatile bool							executing;

	void ExecutingProcInternal()
	{
		while(true)
		{
			bool currentInputDataAvailable;
			T currentInputData;
			{
				SpinLock::Scope scope(inputLock);
				currentInputData=inputData;
				currentInputDataAvailable=inputDataAvailable;
				inputDataAvailable=false;
			}
			if(!currentInputDataAvailable)
			{
				executing=false;
				break;
			}
			Execute(currentInputData);
		}
		executingEvent.Leave();
	}

	static void ExecutingProc(void* argument)
	{
		((RepeatingTaskExecutor<T>*)argument)->ExecutingProcInternal();
	}
public:
	RepeatingTaskExecutor()
		:inputDataAvailable(false)
		,executing(false)
	{
	}

	~RepeatingTaskExecutor()
	{
		executingEvent.Enter();
		executingEvent.Leave();
	}

	void SubmitTask(const T& input)
	{
		{
			// copy the text because this is a cross thread accessible data
			SpinLock::Scope scope(inputLock);
			inputData=input;
			inputDataAvailable=true;
		}
		if(!executing)
		{
			executing=true;
			executingEvent.Enter();
			ThreadPoolLite::Queue(&ExecutingProc, this);
		}
	}
};

/***********************************************************************
GrammarColorizer
***********************************************************************/

class GrammarColorizer : public GuiTextBoxRegexColorizer, public RepeatingTaskExecutor<WString>
{
protected:
	Ptr<ParsingGeneralParser>				grammarParser;
	SpinLock								parsingTreeLock;
	Ptr<ParsingTreeObject>					parsingTreeNode;
	Ptr<ParserDecl>							parsingTreeDecl;

	void Execute(const WString& input)override
	{
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTreeObject> node=grammarParser->Parse(input, L"ParserDecl", errors).Cast<ParsingTreeObject>();
		Ptr<ParserDecl> decl;
		if(node)
		{
			node->InitializeQueryCache();
			decl=new ParserDecl(node);
		}
		{
			SpinLock::Scope scope(parsingTreeLock);
			parsingTreeNode=node;
			parsingTreeDecl=decl;
			node=0;
		}
		RestartColorizer();
	}

	void InitializeColorizer()
	{
		text::ColorEntry entry=GetCurrentTheme()->GetDefaultTextBoxColorEntry();
		SetDefaultColor(entry);

		entry.normal.text=Color(163, 21, 21);
		AddToken(L"\"([^\\\\\"]|\\\\/.)*\"", entry);

		entry.normal.text=Color(0, 0, 255);
		AddToken(L"class|enum|token|discardtoken|rule|as|with", entry);

		// 2 -- token
		AddToken(L"[a-zA-Z_]/w*", GetDefaultColor());
			
		// 3 -- type name
		entry.normal.text=Color(43, 145, 175);
		AddExtraToken(entry);
			
		// 4 -- token name
		entry.normal.text=Color(163, 73, 164);
		AddExtraToken(entry);
			
		// 5 -- rule name
		entry.normal.text=Color(255, 127, 39);
		AddExtraToken(entry);

		Setup();
	}

	void InitializeParser()
	{
		grammarParser=CreateBootstrapAutoRecoverParser();
	}

	TypeSymbol* FindScope(ParsingTreeNode* node)
	{
		if(!node) return 0;
		int index=parsingTreeDecl->nodeTypeMap.Keys().IndexOf(node);
		return index==-1?FindScope(node->GetParent()):parsingTreeDecl->nodeTypeMap.Values().Get(index);
	}

	TypeSymbol* FindType(TypeSymbol* scope, const WString& name)
	{
		if(!scope) return 0;
		if(name==L"") return 0;
		int index=scope->subTypes.Keys().IndexOf(name);
		if(index!=-1) return scope->subTypes.Values().Get(index).Obj();
		return FindType(scope->parent, name);
	}

	TypeSymbol* FindType(TypeSymbol* scope, ParsingTreeObject* object)
	{
		if(scope && object)
		{
			Ptr<ParsingTreeToken> name=object->GetMember(L"name").Cast<ParsingTreeToken>();
			if(name)
			{
				WString typeName=name->GetValue();
				if(object->GetType()==L"PrimitiveTypeObj")
				{
					return FindType(scope, typeName);
				}
				else if(object->GetType()==L"SubTypeObj")
				{
					TypeSymbol* type=FindType(scope, object->GetMember(L"parentType").Cast<ParsingTreeObject>().Obj());
					if(type)
					{
						int index=type->subTypes.Keys().IndexOf(typeName);
						if(index!=-1) return type->subTypes.Values().Get(index).Obj();
					}
				}
			}
		}
		return 0;
	}
public:
	GrammarColorizer()
	{
		InitializeColorizer();
		InitializeParser();
	}

	void ColorizeTokenContextSensitive(int lineIndex, const wchar_t* text, vint start, vint length, vint& token, int& contextState)override
	{
		SpinLock::Scope scope(parsingTreeLock);
		if(parsingTreeNode && token==2)
		{
			ParsingTextPos pos(lineIndex, start);
			ParsingTreeNode* foundNode=parsingTreeNode->FindDeepestNode(pos);
			if(foundNode)
			{
				ParsingTreeToken* foundToken=dynamic_cast<ParsingTreeToken*>(foundNode);
				if(foundToken)
				{
					ParsingTreeObject* tokenParent=dynamic_cast<ParsingTreeObject*>(foundNode->GetParent());
					if(tokenParent)
					{
						if(tokenParent->GetType()==L"ClassTypeDef" && tokenParent->GetMember(L"name")==foundNode)
						{
							token=3;
						}
						else if(tokenParent->GetType()==L"TokenDef" && tokenParent->GetMember(L"name")==foundNode)
						{
							token=4;
						}
						else if(tokenParent->GetType()==L"RuleDef" && tokenParent->GetMember(L"name")==foundNode)
						{
							token=5;
						}
						else if(tokenParent->GetType()==L"PrimitiveGrammarDef" && tokenParent->GetMember(L"name")==foundNode)
						{
							WString name=foundToken->GetValue();
							if(parsingTreeDecl->tokens.Contains(name))
							{
								token=4;
							}
							else if(parsingTreeDecl->rules.Contains(name))
							{
								token=5;
							}
						}
						else if((tokenParent->GetType()==L"PrimitiveTypeObj" || tokenParent->GetType()==L"SubTypeObj") && tokenParent->GetMember(L"name")==foundNode)
						{
							TypeSymbol* scope=FindScope(tokenParent);
							if(FindType(scope, tokenParent))
							{
								token=3;
							}
						}
					}
				}
			}
		}
	}
};

/***********************************************************************
TextBoxColorizerWindow
***********************************************************************/

class TextBoxColorizerWindow : public GuiWindow
{
protected:
	GuiTab*									tabIntellisense;
	GuiMultilineTextBox*					textBoxGrammar;
	GuiMultilineTextBox*					textBoxEditor;

	void textBoxEditor_TextChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
	{
		GuiMultilineTextBox* textBox=dynamic_cast<GuiMultilineTextBox*>(composition->GetRelatedControl());
		GrammarColorizer* colorizer=dynamic_cast<GrammarColorizer*>(textBox->GetColorizer().Obj());

		WString text=textBox->GetText();
		colorizer->SubmitTask(text.Buffer());
	}
public:
	TextBoxColorizerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"GacUISrc Test Application");
		SetClientSize(Size(800, 600));

		tabIntellisense=g::NewTab();
		tabIntellisense->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GetBoundsComposition()->AddChild(tabIntellisense->GetBoundsComposition());
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Code Editor");

			textBoxEditor=g::NewMultilineTextBox();
			textBoxEditor->SetHorizontalAlwaysVisible(false);
			textBoxEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(textBoxEditor->GetBoundsComposition());
			textBoxEditor->SetColorizer(new GrammarColorizer);
			textBoxEditor->TextChanged.AttachMethod(this, &TextBoxColorizerWindow::textBoxEditor_TextChanged);

			{
				FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				textBoxEditor->SetText(reader.ReadToEnd());
				textBoxEditor->Select(TextPos(), TextPos());
			}
		}
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Grammar");

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->SetReadonly(true);
			textBoxGrammar->SetHorizontalAlwaysVisible(false);
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			textBoxGrammar->SetColorizer(new GrammarColorizer);
			textBoxGrammar->TextChanged.AttachMethod(this, &TextBoxColorizerWindow::textBoxEditor_TextChanged);

			{
				Ptr<ParsingDefinition> def=CreateParserDefinition();
				MemoryStream stream;
				{
					StreamWriter writer(stream);
					Log(def, writer);
				}
				stream.SeekFromBegin(0);
				StreamReader reader(stream);
				textBoxGrammar->SetText(reader.ReadToEnd());
				textBoxGrammar->Select(TextPos(), TextPos());
			}
		}
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Configuration");
		}

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(800, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~TextBoxColorizerWindow()
	{
	}
};

extern void UnitTestInGuiMain();

void GuiMain()
{
	UnitTestInGuiMain();
	TextBoxColorizerWindow window;
	GetApplication()->Run(&window);
}

void UnitTestInGuiMain()
{
#define ASSERT(x) do{if(!(x))throw 0;}while(0)

	GuiBoundsComposition* bounds=new GuiBoundsComposition;
	GuiControl* control=new GuiControl(new GuiControl::EmptyStyleController);
	bounds->AddChild(control->GetBoundsComposition());

	vint* rc1=ReferenceCounterOperator<GuiBoundsComposition>::CreateCounter(bounds);
	vint* rc2=ReferenceCounterOperator<GuiControl>::CreateCounter(control);

	ASSERT(*rc1==0);
	Ptr<GuiBoundsComposition> a1=bounds;
	ASSERT(*rc1==1);
	Ptr<GuiBoundsComposition> a2=bounds;
	ASSERT(*rc1==2);
	Ptr<DescriptableObject> a3=a1.Cast<DescriptableObject>();
	ASSERT(*rc1==3);
	Ptr<DescriptableObject> a4=a2;
	ASSERT(*rc1==4);
		
	ASSERT(*rc2==0);
	Ptr<GuiControl> b1=control;
	ASSERT(*rc2==1);
	Ptr<GuiControl> b2=control;
	ASSERT(*rc2==2);
	Ptr<DescriptableObject> b3=b1.Cast<DescriptableObject>();
	ASSERT(*rc2==3);
	Ptr<DescriptableObject> b4=b2;
	ASSERT(*rc2==4);

	b4=b3=b2=b1=0; // not released yet
	ASSERT(*rc2==0);
	control->SetText(L"Text");

	a4=a3=a2=a1=0; // all released
}