#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Stream\FileStream.h"
#include "..\..\..\..\Common\Source\Stream\Accessor.h"
#include "..\..\..\..\Common\Source\Stream\CharFormat.h"
#include "..\..\..\..\Common\Source\Parsing\ParsingTable.h"
#include <Windows.h>

using namespace vl::collections;
using namespace vl::stream;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::tabling;

#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result=SetupWindowsDirect2DRenderer();
#endif
}

namespace test
{

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
GrammarColorizer
***********************************************************************/

	class GrammarColorizer : public GuiTextBoxRegexColorizer
	{
	protected:
		Ptr<ParsingRestrictParser>				grammarParser;
		volatile bool							finalizing;

		SpinLock								parsingTreeLock;
		Ptr<ParsingTreeObject>					parsingTreeNode;
		Ptr<ParserDecl>							parsingTreeDecl;

		SpinLock								parsingTextLock;
		WString									parsingText;
		volatile bool							isParsingRunning;
		SpinLock								parsingRunningEvent;

		void ParsingProcInternal()
		{
			while(true)
			{
				WString currentParsingText;
				{
					SpinLock::Scope scope(parsingTextLock);
					currentParsingText=parsingText;
					parsingText=L"";
				}
				if(currentParsingText==L"")
				{
					isParsingRunning=false;
					break;
				}

				ParsingError error;
				Ptr<ParsingTreeObject> node=grammarParser->Parse(currentParsingText, L"ParserDecl", error).Cast<ParsingTreeObject>();
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
			parsingRunningEvent.Leave();
		}

		static void ParsingProc(void* argument)
		{
			((GrammarColorizer*)argument)->ParsingProcInternal();
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
			grammarParser=CreateBootstrapParser();
		}

		TypeSymbol* FindScope(ParsingTreeNode* node)
		{
			if(!node) return 0;
			vint index=parsingTreeDecl->nodeTypeMap.Keys().IndexOf(node);
			return index==-1?FindScope(node->GetParent()):parsingTreeDecl->nodeTypeMap.Values().Get(index);
		}

		TypeSymbol* FindType(TypeSymbol* scope, const WString& name)
		{
			if(!scope) return 0;
			if(name==L"") return 0;
			vint index=scope->subTypes.Keys().IndexOf(name);
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
							vint index=type->subTypes.Keys().IndexOf(typeName);
							if(index!=-1) return type->subTypes.Values().Get(index).Obj();
						}
					}
				}
			}
			return 0;
		}
	public:
		GrammarColorizer()
			:isParsingRunning(false)
			,finalizing(false)
		{
			InitializeColorizer();
			InitializeParser();
		}

		~GrammarColorizer()
		{
			finalizing=true;
			parsingRunningEvent.Enter();
			parsingRunningEvent.Leave();
		}

		void SubmitCurrentText(const wchar_t* text)
		{
			{
				// copy the text because this is a cross thread accessible data
				SpinLock::Scope scope(parsingTextLock);
				parsingText=text;
			}
			if(!isParsingRunning)
			{
				isParsingRunning=true;
				parsingRunningEvent.Enter();
				ThreadPoolLite::Queue(&ParsingProc, this);
			}
		}

		void ColorizeTokenContextSensitive(vint lineIndex, const wchar_t* text, vint start, vint length, vint& token, vint& contextState)override
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
TestWindow
***********************************************************************/

	class TestWindow : public GuiWindow
	{
	protected:
		GuiMultilineTextBox*					textBoxGrammar;
		Ptr<GrammarColorizer>					colorizer;

		void textBoxGrammar_TextChanged(GuiGraphicsComposition* composition, GuiEventArgs& arguments)
		{
			WString text=textBoxGrammar->GetText();
			colorizer->SubmitCurrentText(text.Buffer());
		}
	public:
		TestWindow()
			:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
		{

			SetText(L"GacUISrc Test Application");
			SetClientSize(Size(640, 480));
			GetBoundsComposition()->SetPreferredMinSize(Size(320, 240));
			MoveToScreenCenter();

			textBoxGrammar=g::NewMultilineTextBox();
			textBoxGrammar->GetBoundsComposition()->SetAlignmentToParent(Margin(3, 3, 3, 3));
			GetBoundsComposition()->AddChild(textBoxGrammar->GetBoundsComposition());
			colorizer=new GrammarColorizer;
			textBoxGrammar->SetColorizer(colorizer);
			textBoxGrammar->TextChanged.AttachMethod(this, &TestWindow::textBoxGrammar_TextChanged);

			{
				FileStream fileStream(L"..\\GacUISrcCodepackedTest\\Resources\\CalculatorDefinition.txt", FileStream::ReadOnly);
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);
				textBoxGrammar->SetText(reader.ReadToEnd());
				textBoxGrammar->Select(TextPos(), TextPos());
			}
		}
	};
}
using namespace test;

void GuiMain()
{
	TestWindow window;
	GetApplication()->Run(&window);
}