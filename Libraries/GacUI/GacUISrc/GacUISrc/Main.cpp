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

Ptr<ParsingScopeSymbol> CreateSymbolFromNode(Ptr<ParsingTreeObject> obj, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder);
void CreateSubSymbols(ParsingScopeSymbol* symbol, Ptr<ParsingTreeObject> node, const WString& memberName, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder);

class GrammarSymbol : public ParsingScopeSymbol
{
public:
	GrammarSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder, const WString& semantic)
		:ParsingScopeSymbol(finder->Node(node->GetMember(L"name")).Cast<ParsingTreeToken>()->GetValue(), executor->GetSemanticId(semantic))
	{
		SetNode(node);
	}
};

class EnumFieldSymbol : public GrammarSymbol
{
public:
	EnumFieldSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:GrammarSymbol(node, executor, finder, L"EnumValue")
	{
	}
};

class ClassFieldSymbol : public GrammarSymbol
{
public:
	ClassFieldSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:GrammarSymbol(node, executor, finder, L"Field")
	{
	}
};

class TypeSymbol : public GrammarSymbol
{
public:
	TypeSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:GrammarSymbol(node, executor, finder, L"Type")
	{
	}
};

class EnumSymbol : public TypeSymbol
{
public:
	EnumSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:TypeSymbol(node, executor, finder)
	{
		CreateScope();
		CreateSubSymbols(this, node, L"members", executor, finder);
	}
};

class ClassSymbol : public TypeSymbol
{
public:
	ClassSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:TypeSymbol(node, executor, finder)
	{
		CreateScope();
		CreateSubSymbols(this, node, L"members", executor, finder);
		CreateSubSymbols(this, node, L"subTypes", executor, finder);
	}
};

class TokenSymbol : public GrammarSymbol
{
public:
	TokenSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:GrammarSymbol(node, executor, finder, L"Token")
	{
		CreateScope();
	}
};

class RuleSymbol : public GrammarSymbol
{
public:
	RuleSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		:GrammarSymbol(node, executor, finder, L"Rule")
	{
		CreateScope();
	}
};

class ParserDefSymbol : public ParsingScopeSymbol
{
public:
	ParserDefSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
	{
		SetNode(node.Obj());
		CreateScope();
		CreateSubSymbols(this, node, L"definitions", executor, finder);
	}
};


void CreateSubSymbols(ParsingScopeSymbol* symbol, Ptr<ParsingTreeObject> node, const WString& memberName, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
{
	if(Ptr<ParsingTreeArray> members=finder->Node(node->GetMember(memberName)).Cast<ParsingTreeArray>())
	{
		FOREACH(Ptr<ParsingTreeNode>, node, members->GetItems())
		{
			if(Ptr<ParsingTreeObject> obj=finder->Node(node).Cast<ParsingTreeObject>())
			{
				symbol->GetScope()->AddSymbol(CreateSymbolFromNode(obj, executor, finder));
			}
		}
	}
}

Ptr<ParsingScopeSymbol> CreateSymbolFromNode(Ptr<ParsingTreeObject> obj, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
{
	if(obj->GetType()==L"EnumMemberDef")
	{
		return new EnumFieldSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"EnumTypeDef")
	{
		return new EnumSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"ClassMemberDef")
	{
		return new ClassFieldSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"ClassTypeDef")
	{
		return new ClassSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"TokenDef")
	{
		return new TokenSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"RuleDef")
	{
		return new RuleSymbol(obj, executor, finder);
	}
	else if(obj->GetType()==L"ParserDef")
	{
		return new ParserDefSymbol(obj, executor, finder);
	}
	else
	{
		return 0;
	}
}

LazyList<Ptr<ParsingScopeSymbol>> FindReferencedSymbols(ParsingTreeObject* obj, ParsingScopeFinder* finder)
{
	ParsingScope* scope=finder->GetScopeFromNode(obj);
	if(obj->GetType()==L"PrimitiveTypeObj")
	{
		WString name=obj->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue();
		return finder->GetSymbolsRecursively(scope, name);
	}
	else if(obj->GetType()==L"SubTypeObj")
	{
		if(Ptr<ParsingTreeObject> parentType=obj->GetMember(L"parentType").Cast<ParsingTreeObject>())
		{
			WString name=obj->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue();
			LazyList<Ptr<ParsingScopeSymbol>> types=FindReferencedSymbols(parentType.Obj(), finder);
			return types
				.SelectMany([=](Ptr<ParsingScopeSymbol> type)
				{
					return finder->GetSymbols(type->GetScope(), name);
				});
		}
	}
	else if(obj->GetType()==L"PrimitiveGrammarDef")
	{
		WString name=obj->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue();
		return finder->GetSymbolsRecursively(scope, name);
	}
	return LazyList<Ptr<ParsingScopeSymbol>>();
}

typedef List<Ptr<ParsingScopeSymbol>> TypeList;
typedef Ptr<TypeList> PtrTypeList;

PtrTypeList SearchAllTypes(ParsingTreeObject* obj, ParsingScopeFinder* finder)
{
	PtrTypeList allTypes=new TypeList;
	ParsingScope* scope=finder->GetScopeFromNode(obj);
	while(scope)
	{
		ParsingScope* parentScope=finder->ParentScope(scope->GetOwnerSymbol());
		if(parentScope)
		{
			scope=parentScope;
		}
		else
		{
			break;
		}
	}

	CopyFrom(
		*allTypes.Obj(),
		From(finder->GetSymbolsRecursively(scope))
			.Where([](Ptr<ParsingScopeSymbol> symbol)
			{
				return symbol.Cast<TypeSymbol>();
			})
		);
	vint last=0;

	while(true)
	{
		vint count=allTypes->Count();
		CopyFrom(
			*allTypes.Obj(),
			From(*allTypes.Obj())
				.Skip(last)
				.SelectMany([=](Ptr<ParsingScopeSymbol> symbol)
				{
					return finder->GetSymbols(symbol->GetScope());
				})
				.Where([](Ptr<ParsingScopeSymbol> symbol)
				{
					return symbol.Cast<TypeSymbol>();
				}),
			true
			);
		if(allTypes->Count()==count)
		{
			break;
		}
		last=count;
	}

	return allTypes;
}

PtrTypeList IntersectTypes(PtrTypeList firstTypes, PtrTypeList secondTypes)
{
	if(!firstTypes)
	{
		return secondTypes;
	}
	else if(!secondTypes)
	{
		return firstTypes;
	}
	else
	{
		PtrTypeList types=new TypeList;
		CopyFrom(*types.Obj(), From(*firstTypes.Obj()).Intersect(*secondTypes.Obj()));
		return types;
	}
}

PtrTypeList SearchGrammarTypes(ParsingTreeObject* obj, ParsingScopeFinder* finder)
{
	if(obj->GetType()==L"SequenceGrammarDef" || obj->GetType()==L"AlternativeGrammarDef")
	{
		PtrTypeList firstTypes=SearchGrammarTypes(finder->Node(obj->GetMember(L"first")).Cast<ParsingTreeObject>().Obj(), finder);
		PtrTypeList secondTypes=SearchGrammarTypes(finder->Node(obj->GetMember(L"second")).Cast<ParsingTreeObject>().Obj(), finder);
		return IntersectTypes(firstTypes, secondTypes);
	}
	else if(
		obj->GetType()==L"LoopGrammarDef"
		|| obj->GetType()==L"OptionalGrammarDef"
		|| obj->GetType()==L"AssignGrammarDef"
		|| obj->GetType()==L"UseGrammarDef"
		|| obj->GetType()==L"SetterGrammarDef")
	{
		return SearchGrammarTypes(finder->Node(obj->GetMember(L"grammar")).Cast<ParsingTreeObject>().Obj(), finder);
	}
	else if(obj->GetType()==L"CreateGrammarDef")
	{
		Ptr<ParsingScopeSymbol> type=FindReferencedSymbols(finder->Node(obj->GetMember(L"type")).Cast<ParsingTreeObject>().Obj(), finder)
			.Where([](Ptr<ParsingScopeSymbol> symbol)
			{
				return symbol.Cast<TypeSymbol>();
			})
			.First(0);
		if(type)
		{
			PtrTypeList types=new List<Ptr<ParsingScopeSymbol>>;
			types->Add(type);
			return types;
		}
	}
	return 0;
}

LazyList<Ptr<ParsingScopeSymbol>> DetermineGrammarTypes(ParsingTreeObject* obj, ParsingScopeFinder* finder)
{
	PtrTypeList selectedTypes;
	ParsingTreeObject* lastObj=0;
	while(obj)
	{
		if(obj->GetType()==L"SequenceGrammarDef")
		{
			ParsingTreeObject* first=dynamic_cast<ParsingTreeObject*>(finder->Node(obj->GetMember(L"first").Obj()));
			ParsingTreeObject* second=dynamic_cast<ParsingTreeObject*>(finder->Node(obj->GetMember(L"second").Obj()));
			PtrTypeList alternativeTypes=lastObj==first?SearchGrammarTypes(second, finder):SearchGrammarTypes(first, finder);
			selectedTypes=IntersectTypes(selectedTypes, alternativeTypes);
		}
		else if(obj->GetType()==L"CreateGrammarDef")
		{
			Ptr<ParsingScopeSymbol> type=FindReferencedSymbols(finder->Node(obj->GetMember(L"type")).Cast<ParsingTreeObject>().Obj(), finder)
				.Where([](Ptr<ParsingScopeSymbol> symbol)
				{
					return symbol.Cast<TypeSymbol>();
				})
				.First(0);
			if(type)
			{
				PtrTypeList types=new List<Ptr<ParsingScopeSymbol>>;
				types->Add(type);
				selectedTypes=types;
			}
		}
		else if(obj->GetType()==L"AssignGrammarDef" || obj->GetType()==L"SetterGrammarDef")
		{
			ParsingTreeObject* grammar=dynamic_cast<ParsingTreeObject*>(finder->Node(obj->GetMember(L"grammar").Obj()));
			PtrTypeList alternativeTypes=SearchGrammarTypes(grammar, finder);
			selectedTypes=IntersectTypes(selectedTypes, alternativeTypes);
		}
		lastObj=obj;
		obj=dynamic_cast<ParsingTreeObject*>(finder->ParentNode(obj));
	}

	return selectedTypes?selectedTypes:SearchAllTypes(obj, finder);
}

LazyList<Ptr<ParsingScopeSymbol>> FindPossibleSymbols(ParsingTreeObject* obj, const WString& field, ParsingScopeFinder* finder)
{
	ParsingScope* scope=finder->GetScopeFromNode(obj);
	if(obj->GetType()==L"PrimitiveTypeObj")
	{
		if(field==L"name")
		{
			return finder->GetSymbolsRecursively(scope);
		}
	}
	else if(obj->GetType()==L"SubTypeObj")
	{
		if(field==L"name")
		{
			if(Ptr<ParsingTreeObject> parentType=obj->GetMember(L"parentType").Cast<ParsingTreeObject>())
			{
				WString name=obj->GetMember(L"name").Cast<ParsingTreeToken>()->GetValue();
				LazyList<Ptr<ParsingScopeSymbol>> types=FindReferencedSymbols(parentType.Obj(), finder);
				return types
					.SelectMany([=](Ptr<ParsingScopeSymbol> type)
					{
						return finder->GetSymbols(type->GetScope());
					});
			}
		}
	}
	else if(obj->GetType()==L"PrimitiveGrammarDef")
	{
		if(field==L"name")
		{
			return finder->GetSymbolsRecursively(scope);
		}
	}
	else if(obj->GetType()==L"TextGrammarDef")
	{
		if(field==L"text")
		{
		}
	}
	else if(obj->GetType()==L"AssignGrammarDef")
	{
		if(field==L"memberName")
		{
			return DetermineGrammarTypes(obj, finder)
				.SelectMany([=](Ptr<ParsingScopeSymbol> type)
				{
					return finder->GetSymbols(type->GetScope());
				})
				.Where([](Ptr<ParsingScopeSymbol> type)
				{
					return type.Cast<ClassFieldSymbol>();
				});
		}
	}
	else if(obj->GetType()==L"SetterGrammarDef")
	{
		if(field==L"memberName")
		{
			return DetermineGrammarTypes(obj, finder)
				.SelectMany([=](Ptr<ParsingScopeSymbol> type)
				{
					return finder->GetSymbols(type->GetScope());
				})
				.Where([](Ptr<ParsingScopeSymbol> type)
				{
					return type.Cast<ClassFieldSymbol>();
				});
		}
		else if(field==L"value")
		{
			WString memberName=finder->Node(obj->GetMember(L"memberName")).Cast<ParsingTreeToken>()->GetValue();
			Ptr<ParsingScopeSymbol> field=FindPossibleSymbols(obj, L"memberName", finder)
				.Where([=](Ptr<ParsingScopeSymbol> type)
				{
					return type->GetName()==memberName;
				})
				.First(0);
			if(field)
			{
				Ptr<ParsingTreeObject> type=finder->Node(field->GetNode()->GetMember(L"type")).Cast<ParsingTreeObject>();
				return FindReferencedSymbols(type.Obj(), finder)
					.SelectMany([=](Ptr<ParsingScopeSymbol> type)
					{
						return finder->GetSymbols(type->GetScope());
					})
					.Where([](Ptr<ParsingScopeSymbol> type)
					{
						return type.Cast<EnumFieldSymbol>();
					});
			}
		}
	}
	return LazyList<Ptr<ParsingScopeSymbol>>();
}

/***********************************************************************
ParserGrammarExecutor
***********************************************************************/

class ParserGrammarExecutor : public RepeatingParsingExecutor
{
protected:

	void OnContextFinishedAsync(RepeatingParsingOutput& context)override
	{
		context.finder=new ParsingScopeFinder();
		context.symbol=CreateSymbolFromNode(context.node, this, context.finder.Obj());
		context.finder->InitializeQueryCache(context.symbol.Obj());
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
		if(Ptr<ParsingScopeSymbol> symbol=FindReferencedSymbols(context.tokenParent, input.finder.Obj()).First(0))
		{
			vint semanticId=From(symbol->GetSemanticIds())
				.Intersect(*context.acceptableSemanticIds.Obj())
				.First(-1);
			if(semanticId!=-1)
			{
				context.semanticId=semanticId;
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

	void LogResult(Context& context)
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

		WString candidateTokenMessage, candidateTypeMessage, candidateSymbolMessage;
		if(context.autoComplete)
		{
			Ptr<ParsingTable> table=GetParsingExecutor()->GetParser()->GetTable();
			FOREACH(vint, token, context.autoComplete->candidates)
			{
				const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(token+ParsingTable::UserTokenStart);
				candidateTokenMessage
					+=tokenInfo.name
					+(context.autoComplete->shownCandidates.Contains(token)?L"[SHOWN]":L"")
					+L": "
					+tokenInfo.regex
					+L"\r\n";
			}

			if(context.autoComplete->foundToken)
			{
				candidateTypeMessage+=L"editing: "+context.autoComplete->foundToken->GetValue()+L"\r\n";
				if(context.autoComplete->acceptableSemanticIds)
				{
					FOREACH(vint, type, *context.autoComplete->acceptableSemanticIds.Obj())
					{
						candidateTypeMessage
							+=L"type: "
							+GetParsingExecutor()->GetSemanticName(type)
							+L"\r\n";
					}

					FOREACH(Ptr<ParsingScopeSymbol>, symbol, context.autoComplete->candidateSymbols)
					{
						candidateSymbolMessage
							+=symbol->GetName()
							+L"\r\n";
					}
				}
			}
		}

		WString selectedMessage
			=L"================CANDIDATE-TOKENS================\r\n"
			+candidateTokenMessage
			+L"================CANDIDATE-TYPES================\r\n"
			+candidateTypeMessage
			+L"================CANDIDATE-SYMBOLS================\r\n"
			+candidateSymbolMessage
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

	void OnContextFinishedAsync(Context& context)override
	{
		if(context.autoComplete && context.autoComplete->acceptableSemanticIds)
		{
			ParsingTreeNode* originalNode=context.originalNode.Obj();
			ParsingTreeNode* replacedNode=context.modifiedNode.Obj();
			ParsingScope* originalScope=context.input.finder->GetScopeFromNode(originalNode);
			if(originalScope)
			{
				ParsingScopeSymbol* originalSymbol=originalScope->GetOwnerSymbol();
				Ptr<ParsingScopeFinder> newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(0, 0, originalNode, replacedNode));
				Ptr<ParsingScopeSymbol> replacedSymbol=CreateSymbolFromNode(newFinder->Obj(originalSymbol->GetNode()), GetParsingExecutor().Obj(), newFinder.Obj());
				
				if(replacedSymbol)
				{
					newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(originalSymbol, replacedSymbol.Obj(), originalNode, replacedNode));
					newFinder->InitializeQueryCache(replacedSymbol.Obj(), context.input.finder.Obj());
					LazyList<Ptr<ParsingScopeSymbol>> symbols=FindPossibleSymbols(context.autoComplete->tokenParent, context.autoComplete->field, newFinder.Obj());
					CopyFrom(
						context.autoComplete->candidateSymbols,
						From(symbols)
							.Where([&context](Ptr<ParsingScopeSymbol> symbol)
							{
								return From(symbol->GetSemanticIds()).Intersect(*context.autoComplete->acceptableSemanticIds.Obj()).First(-1)!=-1;
							})
						);
				}
			}
		}
		LogResult(context);
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