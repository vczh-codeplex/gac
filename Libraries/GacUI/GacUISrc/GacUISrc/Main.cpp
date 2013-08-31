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
SymbolLookup
***********************************************************************/

class GrammarSymbol : public ParsingScopeSymbol
{
public:
	GrammarSymbol(Ptr<ParsingTreeObject> node)
		:ParsingScopeSymbol(node->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue())
	{
		SetNode(node.Obj());
	}
};

class EnumFieldSymbol : public GrammarSymbol
{
public:
	EnumFieldSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class ClassFieldSymbol : public GrammarSymbol
{
public:
	ClassFieldSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class TypeSymbol : public GrammarSymbol
{
public:
	TypeSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class EnumSymbol : public TypeSymbol
{
public:
	EnumSymbol(Ptr<ParsingTreeObject> node)
		:TypeSymbol(node)
	{
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"members").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
				{
					GetScope()->AddSymbol(new EnumFieldSymbol(obj));
				}
			}
		}
	}
};

class ClassSymbol : public TypeSymbol
{
public:
	ClassSymbol(Ptr<ParsingTreeObject> node)
		:TypeSymbol(node)
	{
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"members").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
				{
					GetScope()->AddSymbol(new ClassFieldSymbol(obj));
				}
			}
		}
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"subTypes").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				Ptr<ParsingTreeObject> type=node.Cast<ParsingTreeObject>();
				if(type->GetType()==L"EnumTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new EnumSymbol(obj));
					}
				}
				else if(type->GetType()==L"ClassTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new ClassSymbol(obj));
					}
				}
			}
		}
	}
};

class TokenSymbol : public GrammarSymbol
{
public:
	TokenSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class RuleSymbol : public GrammarSymbol
{
public:
	RuleSymbol(Ptr<ParsingTreeObject> node)
		:GrammarSymbol(node)
	{
	}
};

class ParserDefSymbol : public ParsingScopeSymbol
{
public:
	ParserDefSymbol(Ptr<ParsingTreeObject> node)
		:ParsingScopeSymbol(L"")
	{
		SetNode(node.Obj());
		CreateScope();
		if(Ptr<ParsingTreeArray> members=node->GetMember(L"definitions").Cast<ParsingTreeArray>())
		{
			FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
			{
				Ptr<ParsingTreeObject> type=node.Cast<ParsingTreeObject>();
				if(type->GetType()==L"EnumTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new EnumSymbol(obj));
					}
				}
				else if(type->GetType()==L"ClassTypeDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new ClassSymbol(obj));
					}
				}
				else if(type->GetType()==L"TokenDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new TokenSymbol(obj));
					}
				}
				else if(type->GetType()==L"RuleDef")
				{
					if(Ptr<ParsingTreeObject> obj=node.Cast<ParsingTreeObject>())
					{
						GetScope()->AddSymbol(new RuleSymbol(obj));
					}
				}
			}
		}
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
		context.symbol=new ParserDefSymbol(context.node);
		context.finder=new ParsingScopeFinder(context.symbol.Obj());
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

	void OnSemanticColorize(SemanticColorizeContext& context, const RepeatingParsingOutput& input)override
	{
		List<WString> names;
		{
			ParsingTreeObject* type=context.tokenParent;
			while(type)
			{
				if(type->GetType()==L"PrimitiveTypeObj")
				{
					names.Add(type->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue());
				}
				if(type->GetType()==L"SubTypeObj")
				{
					names.Add(type->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue());
					type=dynamic_cast<ParsingTreeObject*>(type->GetMember(L"parentType").Obj());
				}
				else
				{
					break;
				}
			}
		}

		if(names.Count()==0)
		{
			names.Add(context.foundToken->GetValue());
		}

		ParsingScope* scope=input.finder->GetScopeFromNode(context.foundToken);
		Ptr<ParsingScopeSymbol> type=0;
		for(vint i=names.Count()-1;i>=0;i--)
		{
			if(type)
			{
				type=input.finder->GetSymbols(type->GetScope(), names[i])
					.First(0);
			}
			else if(i==names.Count()-1)
			{
				type=input.finder->GetSymbolsRecursively(scope, names[i])
					.First(0);
			}
			else
			{
				break;
			}
		}

		if(type.Cast<TypeSymbol>())
		{
			context.semantic=GetParsingExecutor()->GetSemanticId(L"Type");
		}
		else if(type.Cast<TokenSymbol>())
		{
			context.semantic=GetParsingExecutor()->GetSemanticId(L"Token");
		}
		else if(type.Cast<RuleSymbol>())
		{
			context.semantic=GetParsingExecutor()->GetSemanticId(L"Rule");
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
				FOREACH(vint, type, context.autoComplete->semantics)
				{
					candidateTypeMessage+=L"type: "+GetParsingExecutor()->GetSemanticName(type)+L"\r\n";
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
	Ptr<AutoCompleteWindow> window=new AutoCompleteWindow();
	GetApplication()->Run(window.Obj());
}