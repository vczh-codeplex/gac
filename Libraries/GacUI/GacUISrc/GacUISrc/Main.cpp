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

		/***********************************************************************
		CreateCache
		***********************************************************************/

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

		/***********************************************************************
		GetSemanticIdForToken
		***********************************************************************/

		void GetTypeScopes(ParsingTreeObject* typeObj, List<WString>& typeScopes)
		{
			typeScopes.Add(L"");
			auto typeScope = typeObj->GetParent();
			while (typeScope)
			{
				if (auto typeScopeObj = dynamic_cast<ParsingTreeObject*>(typeScope))
				{
					if (typeScopeObj->GetType() == L"ClassTypeDef")
					{
						if (auto classNameToken = typeScopeObj->GetMember(L"name").Cast<ParsingTreeToken>())
						{
							auto className = classNameToken->GetValue();
							for (vint i = 0; i < typeScopes.Count(); i++)
							{
								typeScopes[i] = className + L"." + typeScopes[i];
							}
							typeScopes.Add(L"");
						}
					}
				}
				typeScope = typeScope->GetParent();
			}
		}

		WString ResolveType(ParsingTreeObject* typeObj, Ptr<Cache> cache)
		{
			if (auto nameToken = typeObj->GetMember(L"name").Cast<ParsingTreeToken>())
			{
				auto name = nameToken->GetValue();
				if (typeObj->GetType() == L"PrimitiveTypeObj")
				{
					List<WString> typeScopes;
					GetTypeScopes(typeObj, typeScopes);

					FOREACH(WString, typeScope, typeScopes)
					{
						if (cache->typeNames.Contains(typeScope, name))
						{
							return typeScope + name;
						}
					}
				}
				else if (typeObj->GetType() == L"SubTypeObj")
				{
					if (auto parentTypeObj = typeObj->GetMember(L"parentType").Cast<ParsingTreeObject>())
					{
						auto resolvedType = ResolveType(parentTypeObj.Obj(), cache);
						if (cache->typeNames.Contains(resolvedType + L".", name))
						{
							return resolvedType + L"." + name;
						}
					}
				}
			}
			return L"";
		}

		vint GetSemanticIdForToken(const ParsingTokenContext& tokenContext, const RepeatingParsingOutput& output)
		{
			auto cache = output.cache.Cast<Cache>();
			if (tokenContext.tokenParent->GetType() == L"PrimitiveTypeObj" || tokenContext.tokenParent->GetType() == L"SubTypeObj")
			{
				if (tokenContext.field == L"name") // Type
				{
					if (ResolveType(tokenContext.tokenParent, cache) != L"")
					{
						return _type;
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"PrimitiveGrammarDef")
			{
				if (tokenContext.field == L"name") // Token, Rule
				{
					auto value = tokenContext.foundToken->GetValue();
					if (cache->tokenNames.Contains(value))
					{
						return _token;
					}
					else if (cache->ruleNames.Contains(value))
					{
						return _rule;
					}
				}
			}
			return -1;
		}

		/***********************************************************************
		GetCandidateItems
		***********************************************************************/

		void GetCandidateItems(const ParsingTokenContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems)
		{
			auto cache = partialOutput.input.cache.Cast<Cache>();
			if (tokenContext.tokenParent->GetType() == L"PrimitiveTypeObj")
			{
				if (tokenContext.field == L"name") // Type
				{
					List<WString> typeScopes;
					GetTypeScopes(tokenContext.tokenParent, typeScopes);
					FOREACH(WString, typeName, From(typeScopes)
						.SelectMany([=](const WString& typeScope)
						{
							vint index = cache->typeNames.Keys().IndexOf(typeScope);
							if (index == -1)
							{
								return LazyList<WString>();
							}
							else
							{
								return LazyList<WString>(cache->typeNames.GetByIndex(index));
							}
						}))
					{
						ParsingCandidateItem item;
						item.semanticId = _type;
						item.name = typeName;
						candidateItems.Add(item);
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"SubTypeObj")
			{
				if (tokenContext.field == L"name") // Type
				{
					if (auto parentTypeObj = tokenContext.tokenParent->GetMember(L"parentType").Cast<ParsingTreeObject>())
					{
						auto resolvedType = ResolveType(parentTypeObj.Obj(), cache);
						vint index = cache->typeNames.Keys().IndexOf(resolvedType + L".");
						if (index != -1)
						{
							FOREACH(WString, typeName, cache->typeNames.GetByIndex(index))
							{
								ParsingCandidateItem item;
								item.semanticId = _type;
								item.name = typeName;
								candidateItems.Add(item);
							}
						}
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"PrimitiveGrammarDef")
			{
				if (tokenContext.field == L"name") // Token, Rule
				{
					FOREACH(WString, name, cache->tokenNames)
					{
						ParsingCandidateItem item;
						item.semanticId = _token;
						item.name = name;
						candidateItems.Add(item);
					}
					FOREACH(WString, name, cache->ruleNames)
					{
						ParsingCandidateItem item;
						item.semanticId = _rule;
						item.name = name;
						candidateItems.Add(item);
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"TextGrammarDef")
			{
				if (tokenContext.field == L"text") // Literal
				{
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"AssignGrammarDef")
			{
				if (tokenContext.field == L"memberName") // Field
				{
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"SetterGrammarDef")
			{
				if (tokenContext.field == L"memberName") // Field
				{
				}
				else if (tokenContext.field == L"value") // EnumValue
				{
				}
			}
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