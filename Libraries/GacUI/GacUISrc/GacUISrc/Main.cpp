#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Regex\RegexExpression.h"
#include "..\..\Source\Controls\TextEditorPackage\LanguageService\GuiLanguageColorizer.h"
#include "..\..\Source\Controls\TextEditorPackage\LanguageService\GuiLanguageAutoComplete.h"
#include "..\..\Source\Controls\TextEditorPackage\LanguageService\GuiLanguageService.h"
#include <Windows.h>

using namespace vl::regex;
using namespace vl::regex_internal;
using namespace vl::parsing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::tabling;
using namespace vl::stream;

//#define GUI_GRAPHICS_RENDERER_GDI
#define GUI_GRAPHICS_RENDERER_DIRECT2D

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
#ifdef GUI_GRAPHICS_RENDERER_GDI
	int result=SetupWindowsGDIRenderer();
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	int result = SetupWindowsDirect2DRenderer();
#endif

	ThreadLocalStorage::DisposeStorages();
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return result;
}

extern void UnitTestInGuiMain();

namespace autocomplete
{


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
	protected:
		WString GetDisplayInternal(vint semanticId)
		{
			return literalString;
		}
	public:
		WString literalString;

		EnumFieldSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
			:GrammarSymbol(node, executor, finder, L"EnumValue")
		{
			WString value=finder->Node(node->GetMember(L"name")).Cast<ParsingTreeToken>()->GetValue();
			literalString=SerializeString(value);
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
	protected:
		WString GetDisplayInternal(vint semanticId)
		{
			return semanticId==literalId?literalString:ParsingScopeSymbol::GetDisplayInternal(semanticId);
		}
	public:
		vint literalId;
		WString literalString;

		TokenSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
			:GrammarSymbol(node, executor, finder, L"Token")
			,literalId(-1)
		{
			WString value=finder->Node(node->GetMember(L"regex")).Cast<ParsingTreeToken>()->GetValue();
			WString regex=DeserializeString(value);
			if(IsRegexEscapedListeralString(regex))
			{
				literalString=SerializeString(UnescapeTextForRegex(regex));
				AddSemanticId(literalId=executor->GetSemanticId(L"Literal"));
			}
		}
	};

	class RuleSymbol : public GrammarSymbol
	{
	public:
		RuleSymbol(Ptr<ParsingTreeObject> node, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
			:GrammarSymbol(node, executor, finder, L"Rule")
		{
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
		ParsingTreeObject* currentObj=obj;
		ParsingTreeObject* lastObj=0;
		while(currentObj)
		{
			if(currentObj->GetType()==L"SequenceGrammarDef")
			{
				ParsingTreeObject* first=dynamic_cast<ParsingTreeObject*>(finder->Node(currentObj->GetMember(L"first").Obj()));
				ParsingTreeObject* second=dynamic_cast<ParsingTreeObject*>(finder->Node(currentObj->GetMember(L"second").Obj()));
				PtrTypeList alternativeTypes=lastObj==first?SearchGrammarTypes(second, finder):SearchGrammarTypes(first, finder);
				selectedTypes=IntersectTypes(selectedTypes, alternativeTypes);
			}
			else if(currentObj->GetType()==L"CreateGrammarDef")
			{
				Ptr<ParsingScopeSymbol> type=FindReferencedSymbols(finder->Node(currentObj->GetMember(L"type")).Cast<ParsingTreeObject>().Obj(), finder)
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
			else if(currentObj->GetType()==L"AssignGrammarDef" || currentObj->GetType()==L"SetterGrammarDef")
			{
				ParsingTreeObject* grammar=dynamic_cast<ParsingTreeObject*>(finder->Node(currentObj->GetMember(L"grammar").Obj()));
				PtrTypeList alternativeTypes=SearchGrammarTypes(grammar, finder);
				selectedTypes=IntersectTypes(selectedTypes, alternativeTypes);
			}
			lastObj=currentObj;
			currentObj=dynamic_cast<ParsingTreeObject*>(finder->ParentNode(currentObj));
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
				return From(finder->GetSymbolsRecursively(scope))
					.Where([](Ptr<ParsingScopeSymbol> symbol)
					{
						return symbol.Cast<TokenSymbol>();
					});
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
GrammarLanguageProvider
***********************************************************************/

	class GrammarLanguageProvider : public Object, public ILanguageProvider
	{
	public:
		Ptr<ParsingScopeSymbol> CreateSymbolFromNode(Ptr<ParsingTreeObject> obj, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)
		{
			return autocomplete::CreateSymbolFromNode(obj, executor, finder);
		}

		LazyList<Ptr<ParsingScopeSymbol>> FindReferencedSymbols(ParsingTreeObject* obj, ParsingScopeFinder* finder)
		{
			return autocomplete::FindReferencedSymbols(obj, finder);
		}

		LazyList<Ptr<ParsingScopeSymbol>> FindPossibleSymbols(ParsingTreeObject* obj, const WString& field, ParsingScopeFinder* finder)
		{
			return autocomplete::FindPossibleSymbols(obj, field, finder);
		}
	};

/***********************************************************************
ParserParsingAnalyzer
***********************************************************************/

	class ParserParsingAnalyzer : public RepeatingParsingExecutor::IParsingAnalyzer
	{
	protected:
		RepeatingParsingExecutor*		executor;
		vint							_type = -1, _token = -1, _rule = -1, _field = -1, _enumValue = -1, _literal = -1;

		class Cache : public Object
		{
		public:
			Group<WString, WString>		typeNames;
			SortedList<WString>			tokenNames;
			SortedList<WString>			ruleNames;
		};
	public:
		void Attach(RepeatingParsingExecutor* _executor)
		{
			executor = _executor;
			_type = executor->GetSemanticId(L"Type");
			_token = executor->GetSemanticId(L"Token");
			_rule = executor->GetSemanticId(L"Rule");
			_field = executor->GetSemanticId(L"Field");
			_enumValue = executor->GetSemanticId(L"EnumValue");
			_literal = executor->GetSemanticId(L"Literal");
		}

		void Detach(RepeatingParsingExecutor* _executor)
		{
			executor = nullptr;
		}

		void CreateCache_SearchTypes(Ptr<Cache> cache, const WString prefix, Ptr<ParsingTreeObject> typeDef)
		{
			if (auto name = typeDef->GetMember(L"name").Cast<ParsingTreeToken>())
			{
				cache->typeNames.Add(prefix, name->GetValue());
				if (typeDef->GetType() == L"ClassTypeDef")
				{
					if (auto members = typeDef->GetMember(L"subTypes").Cast<ParsingTreeArray>())
					{
						FOREACH(Ptr<ParsingTreeNode>, subNode, members->GetItems())
						{
							if (auto obj = subNode.Cast<ParsingTreeObject>())
							{
								CreateCache_SearchTypes(cache, prefix + name->GetValue() + L".", obj);
							}
						}
					}
				}
			}
		}

		Ptr<Object> CreateCache(const RepeatingParsingOutput& output)
		{
			auto cache = MakePtr<Cache>();
			if (auto definitions = output.node->GetMember(L"definitions").Cast<ParsingTreeArray>())
			{
				FOREACH(Ptr<ParsingTreeNode>, subNode, definitions->GetItems())
				{
					if (auto obj = subNode.Cast<ParsingTreeObject>())
					{
						if (obj->GetType() == L"TokenDef")
						{
							if (auto name = obj->GetMember(L"name").Cast<ParsingTreeToken>())
							{
								cache->tokenNames.Add(name->GetValue());
							}
						}
						else if (obj->GetType() == L"RuleDef")
						{
							if (auto name = obj->GetMember(L"name").Cast<ParsingTreeToken>())
							{
								cache->ruleNames.Add(name->GetValue());
							}
						}
						else
						{
							CreateCache_SearchTypes(cache, L"", obj);
						}
					}
				}
			}
			return cache;
		}

		vint GetSemanticIdForToken(const ParsingTokenContext& tokenContext, const RepeatingParsingOutput& output)
		{
			if (tokenContext.tokenParent->GetType() == L"PrimitiveTypeObj")
			{
				if (tokenContext.field == L"name")
				{
					// @Semantic("Type")
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"SubTypeObj")
			{
				if (tokenContext.field == L"name")
				{
					// @Semantic("Type")
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"PrimitiveGrammarDef")
			{
				if (tokenContext.field == L"name")
				{
					// @Semantic("Token")
					// @Semantic("Rule")
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"TextGrammarDef")
			{
				if (tokenContext.field == L"text")
				{
					// @Semantic("Literal")
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"AssignGrammarDef")
			{
				if (tokenContext.field == L"memberName")
				{
					// @Semantic("Field")
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"SetterGrammarDef")
			{
				if (tokenContext.field == L"memberName")
				{
					// @Semantic("Field")
				}
				else if (tokenContext.field == L"value")
				{
					// @Semantic("EnumValue")
				}
			}
			return -1;
		}

		void GetCandidateItems(const ParsingTokenContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems)
		{
		}
	};

/***********************************************************************
ParserGrammarExecutor
***********************************************************************/

	class ParserGrammarExecutor : public RepeatingParsingExecutor
	{
	public:
		ParserGrammarExecutor()
			:RepeatingParsingExecutor(CreateBootstrapAutoRecoverParser(), L"ParserDecl", new ParserParsingAnalyzer)
		{
		}
	};

/***********************************************************************
ParserGrammarColorizer
***********************************************************************/

class ParserGrammarColorizer : public GuiGrammarColorizer
{
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
};

/***********************************************************************
ParserGrammarAutoComplete
***********************************************************************/

class ParserGrammarAutoComplete : public GuiGrammarAutoComplete
{
public:
	ParserGrammarAutoComplete(Ptr<ParserGrammarExecutor> executor)
		:GuiGrammarAutoComplete(executor)
	{
	}
};
}
using namespace autocomplete;

void GuiMain()
{
	GuiWindow window(GetCurrentTheme()->CreateWindowStyle());
	window.GetContainerComposition()->SetPreferredMinSize(Size(600, 480));
	{
		auto executor = MakePtr<ParserGrammarExecutor>();
		auto colorizer = MakePtr<ParserGrammarColorizer>(executor);
		auto autoComplete = MakePtr<ParserGrammarAutoComplete>(executor);
		
		auto textBox = g::NewMultilineTextBox();
		textBox->SetColorizer(colorizer);
		textBox->SetAutoComplete(autoComplete);
		
		MemoryStream memoryStream;
		{
			auto def = CreateParserDefinition();
			StreamWriter writer(memoryStream);
			Log(def, writer);
		}
		memoryStream.SeekFromBegin(0);
		StreamReader reader(memoryStream);
		textBox->SetText(reader.ReadToEnd());
		textBox->Select(TextPos(), TextPos());

		textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, 5, 5));
		window.AddChild(textBox);
	}
	window.ForceCalculateSizeImmediately();
	window.MoveToScreenCenter();
	GetApplication()->Run(&window);
}