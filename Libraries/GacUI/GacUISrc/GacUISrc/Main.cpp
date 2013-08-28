#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "FileSystemInformation.h"
#include "..\..\Source\GraphicsElement\WindowsDirect2D\GuiGraphicsWindowsDirect2D.h"
#include "..\..\Source\Reflection\GuiReflectionBasic.h"
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
Symbol
***********************************************************************/

class ParsingScopeSymbol;
class ParsingScope;

class ParsingScopeSymbol : public Object
{
};

class ParsingScope : public Object
{
};

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

	void CollectSubTypes(Ptr<ParsingTreeArray> types, Dictionary<ParsingTreeNode*, TypeSymbol*>& nodeTypeMap)
	{
		if(types)
		{
			for(int i=0;i<types->Count();i++)
			{
				Ptr<ParsingTreeObject> type=types->GetItem(i).Cast<ParsingTreeObject>();
				if(type)
				{
					CollectSubType(type, nodeTypeMap);
				}
			}
		}
	}

	void CollectSubType(Ptr<ParsingTreeObject> type, Dictionary<ParsingTreeNode*, TypeSymbol*>& nodeTypeMap)
	{
		Ptr<ParsingTreeToken> name=type->GetMember(L"name").Cast<ParsingTreeToken>();
		if(name && !subTypes.Keys().Contains(name->GetValue()))
		{
			Ptr<TypeSymbol> symbol=new TypeSymbol;
			symbol->typeName=name->GetValue();
			symbol->parent=this;
			subTypes.Add(symbol->typeName, symbol);
			symbol->CollectSubTypes(type->GetMember(L"subTypes").Cast<ParsingTreeArray>(), nodeTypeMap);
			nodeTypeMap.Add(type.Obj(), symbol.Obj());
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
		Ptr<ParsingTreeArray> defs=parserDecl->GetMember(L"definitions").Cast<ParsingTreeArray>();
		if(defs)
		{
			vint count=defs->Count();
			for(vint i=0;i<count;i++)
			{
				Ptr<ParsingTreeObject> defObject=defs->GetItem(i).Cast<ParsingTreeObject>();
				if(defObject)
				{
					if(defObject->GetType()==L"TokenDef")
					{
						Ptr<ParsingTreeToken> name=defObject->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							tokens.Add(name->GetValue());
						}
					}
					else if(defObject->GetType()==L"RuleDef")
					{
						Ptr<ParsingTreeToken> name=defObject->GetMember(L"name").Cast<ParsingTreeToken>();
						if(name)
						{
							rules.Add(name->GetValue());
						}
					}
					else
					{
						CollectSubType(defObject, nodeTypeMap);
					}
				}
			}
		}
	}

	TypeSymbol* FindScope(ParsingTreeNode* node)
	{
		if(!node) return 0;
		int index=nodeTypeMap.Keys().IndexOf(node);
		return index==-1?FindScope(node->GetParent()):nodeTypeMap.Values().Get(index);
	}

	TypeSymbol* FindType(TypeSymbol* scope, const WString& name)
	{
		if(!scope) return 0;
		if(name==L"") return 0;
		int index=scope->subTypes.Keys().IndexOf(name);
		if(index!=-1) return scope->subTypes.Values().Get(index).Obj();
		return FindType(scope->parent, name);
	}

	TypeSymbol* FindType(TypeSymbol* scope, ParsingTreeObject* object, ParsingTreeToken* foundToken)
	{
		if(scope && object)
		{
			if(!foundToken)
			{
				foundToken=object->GetMember(L"name").Cast<ParsingTreeToken>().Obj();
			}
			if(object->GetMember(L"name")==foundToken)
			{
				WString typeName=foundToken->GetValue();
				if(object->GetType()==L"PrimitiveTypeObj")
				{
					return FindType(scope, typeName);
				}
				else if(object->GetType()==L"SubTypeObj")
				{
					TypeSymbol* type=FindType(scope, object->GetMember(L"parentType").Cast<ParsingTreeObject>().Obj(), 0);
					if(type)
					{
						int index=type->subTypes.Keys().IndexOf(typeName);
						if(index!=-1) return type->subTypes.Values().Get(index).Obj();
					}
				}
			}
			else if(object->GetMember(L"ambiguousType")==foundToken)
			{
				WString typeName=foundToken->GetValue();
				if(object->GetType()==L"ClassTypeDef")
				{
					return FindType(scope, typeName);
				}
			}
		}
		return 0;
	}
};

/***********************************************************************
ParserGrammarExecutor
***********************************************************************/

class ParserGrammarExecutor : public RepeatingParsingExecutor
{
protected:

	void OnContextFinishedAsync(RepeatingParsingOutput& context)override
	{
		context.semanticContext=new ParserDecl(context.node);
	}
public:
	ParserGrammarExecutor()
		:RepeatingParsingExecutor(CreateBootstrapAutoRecoverParser(), L"ParserDecl")
	{
	}
};

/***********************************************************************
ParserGrammarColorizer
***********************************************************************/

class ParserGrammarColorizer : public GuiGrammarColorizer
{
protected:
	vint									tokenIdType;
	vint									tokenIdToken;
	vint									tokenIdRule;
	vint									semanticType;
	vint									semanticGrammar;

	void OnSemanticColorize(SemanticColorizeContext& context)override
	{
		Ptr<ParserDecl> parserDecl=context.semanticContext.Cast<ParserDecl>();
		if(parserDecl)
		{
			if(context.semantic==semanticType)
			{
				TypeSymbol* scope=parserDecl->FindScope(context.tokenParent);
				if(parserDecl->FindType(scope, context.tokenParent, context.foundToken))
				{
					context.token=tokenIdType;
				}
			}
			else if(context.semantic==semanticGrammar)
			{
				WString name=context.foundToken->GetValue();
				if(parserDecl->tokens.Contains(name))
				{
					context.token=tokenIdToken;
				}
				else if(parserDecl->rules.Contains(name))
				{
					context.token=tokenIdRule;
				}
			}
		}
	}
public:
	ParserGrammarColorizer(Ptr<ParserGrammarExecutor> executor)
		:GuiGrammarColorizer(executor)
	{
		SetColor(L"Keyword", Color(0, 0, 255));
		SetColor(L"Attribute", Color(0, 0, 255));
		SetColor(L"String", Color(163, 21, 21));
		SetColor(L"Type", Color(43, 145, 175));
		SetColor(L"Token", Color(163, 73, 164));
		SetColor(L"Rule", Color(255, 127, 39));
		EndSetColors();

		tokenIdType=GetTokenId(L"Type");
		tokenIdToken=GetTokenId(L"Token");
		tokenIdRule=GetTokenId(L"Rule");
		semanticType=GetSemanticId(L"Type");
		semanticGrammar=GetSemanticId(L"Grammar");
	}

	~ParserGrammarColorizer()
	{
		EnsureColorizerFinished();
	}
};

/***********************************************************************
ParserGrammarAutoComplete
***********************************************************************/

class ParserGrammarAutoComplete : public GuiGrammarAutoComplete
{
protected:
	GuiMultilineTextBox*					textBoxScope;

	void OnContextFinishedAsync(const Context& context)override
	{
		//WString selectedTree;
		//{
		//	MemoryStream stream;
		//	{
		//		StreamWriter writer(stream);
		//		Log(context.modifiedNode.Obj(), L"", writer);
		//	}
		//	stream.SeekFromBegin(0);
		//	StreamReader reader(stream);
		//	selectedTree=reader.ReadToEnd();
		//}

		WString candidateTokenMessage, candidateTypeMessage;
		if(context.autoComplete)
		{
			Ptr<ParsingTable> table=GetParsingExecutor()->GetParser()->GetTable();
			FOREACH(vint, token, context.autoComplete->candidates)
			{
				const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(token+ParsingTable::UserTokenStart);
				candidateTokenMessage+=tokenInfo.name
					+(context.autoComplete->shownCandidates.Contains(token)?L"[SHOWN]":L"")
					+L": "
					+tokenInfo.regex
					+L"\r\n";
			}

			if(context.autoComplete->token)
			{
				candidateTypeMessage+=L"editing: "+context.autoComplete->token->GetValue()+L"\r\n";
				FOREACH(vint, type, context.autoComplete->types)
				{
					candidateTypeMessage+=L"type: "+GetAutoCompleteTypeName(type)+L"\r\n";
				}
			}
		}

		WString selectedMessage
			=L"================CANDIDATE-TOKENS================\r\n"
			+candidateTokenMessage
			+L"================CANDIDATE-TYPES================\r\n"
			+candidateTypeMessage
			+L"================RULE================\r\n"
			+context.rule+L"\r\n"
			+L"================CODE================\r\n"
			+context.modifiedCode+L"\r\n"
			//+L"================TREE================\r\n"
			//+selectedTree;
			;

		GetApplication()->InvokeInMainThread([=]()
		{
			textBoxScope->SetText(selectedMessage);
			textBoxScope->Select(TextPos(), TextPos());
		});
	}
public:
	ParserGrammarAutoComplete(Ptr<ParserGrammarExecutor> executor, GuiMultilineTextBox* _textBoxScope)
		:GuiGrammarAutoComplete(executor)
		,textBoxScope(_textBoxScope)
	{
	}

	~ParserGrammarAutoComplete()
	{
		EnsureAutoCompleteFinished();
	}
};

/***********************************************************************
TextBoxColorizerWindow
***********************************************************************/

class AutoCompleteWindow : public GuiWindow
{
protected:
	GuiTab*									tabIntellisense;
	GuiMultilineTextBox*					textBoxEditor;
	GuiMultilineTextBox*					textBoxScope;
	GuiMultilineTextBox*					textBoxGrammar;

	Ptr<ParserGrammarColorizer>				colorizer;
	Ptr<ParserGrammarAutoComplete>			autoComplete;
	Ptr<ParserGrammarExecutor>				executor;

public:
	AutoCompleteWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		SetText(L"GacUISrc Test Application");
		SetClientSize(Size(800, 600));

		GuiSelectableButton::MutexGroupController* controller=new GuiSelectableButton::MutexGroupController;
		AddComponent(controller);

		tabIntellisense=g::NewTab();
		tabIntellisense->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		GetBoundsComposition()->AddChild(tabIntellisense->GetBoundsComposition());
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Code Editor");

			GuiTableComposition* table=new GuiTableComposition;
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			table->SetRowsAndColumns(1, 3);
			table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(1, GuiCellOption::AbsoluteOption(5));
			table->SetColumnOption(2, GuiCellOption::AbsoluteOption(300));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);

				textBoxEditor=g::NewMultilineTextBox();
				textBoxEditor->SetVerticalAlwaysVisible(false);
				textBoxEditor->SetHorizontalAlwaysVisible(false);
				textBoxEditor->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				cell->AddChild(textBoxEditor->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 2, 1, 1);

				textBoxScope=g::NewMultilineTextBox();
				textBoxScope->SetVerticalAlwaysVisible(false);
				textBoxScope->SetHorizontalAlwaysVisible(false);
				textBoxScope->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textBoxScope->SetReadonly(true);
				cell->AddChild(textBoxScope->GetBoundsComposition());
			}
			page->GetContainer()->GetBoundsComposition()->AddChild(table);
		}
		{
			GuiTabPage* page=tabIntellisense->CreatePage();
			page->SetText(L"Grammar");

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->SetReadonly(true);
			textBoxGrammar->SetVerticalAlwaysVisible(false);
			textBoxGrammar->SetHorizontalAlwaysVisible(false);
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			page->GetContainer()->GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			textBoxGrammar->SetColorizer(new ParserGrammarColorizer(new ParserGrammarExecutor));
		}
		{
			executor=new ParserGrammarExecutor;
			colorizer=new ParserGrammarColorizer(executor);
			autoComplete=new ParserGrammarAutoComplete(executor, textBoxScope);

			textBoxEditor->SetColorizer(colorizer);
			textBoxEditor->SetAutoComplete(autoComplete);

			FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
			BomDecoder decoder;
			DecoderStream decoderStream(fileStream, decoder);
			StreamReader reader(decoderStream);
			textBoxEditor->SetText(reader.ReadToEnd());
			textBoxEditor->Select(TextPos(), TextPos());
		}
		{
			Ptr<ParsingDefinition> definition=CreateParserDefinition();
			MemoryStream stream;
			{
				StreamWriter writer(stream);
				Log(definition, writer);
			}
			stream.SeekFromBegin(0);
			StreamReader reader(stream);
			textBoxGrammar->SetText(reader.ReadToEnd());
			textBoxGrammar->Select(TextPos(), TextPos());
		}

		// set the preferred minimum client 600
		this->GetBoundsComposition()->SetPreferredMinSize(Size(800, 600));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}

	~AutoCompleteWindow()
	{
	}
};

extern void UnitTestInGuiMain();

void GuiMain()
{
	UnitTestInGuiMain();
	AutoCompleteWindow window;
	GetApplication()->Run(&window);
}