#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "..\..\Source\GacUI.h"
#include "..\..\..\..\Common\Source\Tuple.h"
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
			Group<WString, WString>						typeNames;
			SortedList<WString>							tokenNames;
			SortedList<WString>							literalNames;
			SortedList<WString>							ruleNames;
			Group<WString, WString>						inheritedTypes;
			Group<WString, WString>						enumItems;
			Group<WString, Tuple<WString, WString>>		classFields;
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

		void CreateCache_SearchSubTypes(Ptr<Cache> cache, const WString prefix, Ptr<ParsingTreeObject> typeDef)
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
							if (auto subType = subNode.Cast<ParsingTreeObject>())
							{
								CreateCache_SearchSubTypes(cache, prefix + name->GetValue() + L".", subType);
							}
						}
					}
				}
			}
		}

		void CreateCache_CreateSymbols(Ptr<Cache> cache, Ptr<ParsingTreeObject> def)
		{
			if (def->GetType() == L"TokenDef")
			{
				if (auto name = def->GetMember(L"name").Cast<ParsingTreeToken>())
				{
					cache->tokenNames.Add(name->GetValue());
				}

				if (auto discard = def->GetMember(L"discard").Cast<ParsingTreeToken>())
				{
					if (discard->GetValue() == L"DiscardToken")
					{
						return;
					}
				}
				if (auto regex = def->GetMember(L"regex").Cast<ParsingTreeToken>())
				{
					auto escaped = DeserializeString(regex->GetValue());
					if (IsRegexEscapedLiteralString(escaped))
					{
						cache->literalNames.Add(SerializeString(UnescapeTextForRegex(escaped)));
					}
				}
			}
			else if (def->GetType() == L"RuleDef")
			{
				if (auto name = def->GetMember(L"name").Cast<ParsingTreeToken>())
				{
					cache->ruleNames.Add(name->GetValue());
				}
			}
			else
			{
				CreateCache_SearchSubTypes(cache, L"", def);
			}
		}

		void CreateCache_ResolveSymbols(Ptr<Cache> cache, const WString& prefix, Ptr<ParsingTreeObject> typeDef)
		{
			if (auto name = typeDef->GetMember(L"name").Cast<ParsingTreeToken>())
			{
				auto typePrefix = prefix + name->GetValue() + L".";
				if (typeDef->GetType() == L"EnumTypeDef")
				{
					if (auto members = typeDef->GetMember(L"members").Cast<ParsingTreeArray>())
					{
						FOREACH(Ptr<ParsingTreeNode>, subNode, members->GetItems())
						{
							if (auto member = subNode.Cast<ParsingTreeObject>())
							{
								if (auto nameToken = member->GetMember(L"name").Cast<ParsingTreeToken>())
								{
									cache->enumItems.Add(typePrefix, nameToken->GetValue());
								}
							}
						}
					}
				}
				else if (typeDef->GetType() == L"ClassTypeDef")
				{
					if (auto parentType = typeDef->GetMember(L"parentType").Cast<ParsingTreeObject>())
					{
						auto resolvedType = ResolveType(parentType.Obj(), cache, nullptr);
						if (resolvedType != L"")
						{
							cache->inheritedTypes.Add(resolvedType, prefix + name->GetValue());
						}
					}

					if (auto members = typeDef->GetMember(L"members").Cast<ParsingTreeArray>())
					{
						FOREACH(Ptr<ParsingTreeNode>, subNode, members->GetItems())
						{
							if (auto field = subNode.Cast<ParsingTreeObject>())
							{
								auto typeNode = field->GetMember(L"type").Cast<ParsingTreeObject>();
								auto nameToken = field->GetMember(L"name").Cast<ParsingTreeToken>();
								if (typeNode && nameToken)
								{
									auto resolvedType = ResolveType(typeNode.Obj(), cache, nullptr);
									cache->classFields.Add(typePrefix, Tuple<WString, WString>(nameToken->GetValue(), resolvedType));
								}
							}
						}
					}

					if (auto members = typeDef->GetMember(L"subTypes").Cast<ParsingTreeArray>())
					{
						FOREACH(Ptr<ParsingTreeNode>, subNode, members->GetItems())
						{
							if (auto subType = subNode.Cast<ParsingTreeObject>())
							{
								CreateCache_ResolveSymbols(cache, typePrefix, subType);
							}
						}
					}
				}
			}
		}

		Ptr<Object> CreateCacheAsync(const RepeatingParsingOutput& output)
		{
			auto cache = MakePtr<Cache>();
			if (auto definitions = output.node->GetMember(L"definitions").Cast<ParsingTreeArray>())
			{
				FOREACH(Ptr<ParsingTreeNode>, subNode, definitions->GetItems())
				{
					if (auto obj = subNode.Cast<ParsingTreeObject>())
					{
						CreateCache_CreateSymbols(cache, obj);
					}
				}
				
				FOREACH(Ptr<ParsingTreeNode>, subNode, definitions->GetItems())
				{
					if (auto obj = subNode.Cast<ParsingTreeObject>())
					{
						CreateCache_ResolveSymbols(cache, L"", obj);
					}
				}
			}
			return cache;
		}

		/***********************************************************************
		GetSemanticIdForToken
		***********************************************************************/

		void GetTypeScopes(ParsingTreeObject* typeObj, List<WString>& typeScopes, const RepeatingPartialParsingOutput* output)
		{
			typeScopes.Add(L"");
			auto typeScope = GetParent(typeObj, output);
			while (typeScope)
			{
				if (auto typeScopeObj = dynamic_cast<ParsingTreeObject*>(typeScope))
				{
					if (typeScopeObj->GetType() == L"ClassTypeDef")
					{
						if (auto classNameToken = GetMember(typeScopeObj, L"name", output).Cast<ParsingTreeToken>())
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
				typeScope = GetParent(typeScope, output);
			}
		}

		WString ResolveType(ParsingTreeObject* typeObj, Ptr<Cache> cache, const RepeatingPartialParsingOutput* output)
		{
			if (auto nameToken = GetMember(typeObj, L"name", output).Cast<ParsingTreeToken>())
			{
				auto name = nameToken->GetValue();
				if (typeObj->GetType() == L"PrimitiveTypeObj")
				{
					List<WString> typeScopes;
					GetTypeScopes(typeObj, typeScopes, output);

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
					if (auto parentTypeObj = GetMember(typeObj, L"parentType", output).Cast<ParsingTreeObject>())
					{
						auto resolvedType = ResolveType(parentTypeObj.Obj(), cache, output);
						if (cache->typeNames.Contains(resolvedType + L".", name))
						{
							return resolvedType + L"." + name;
						}
					}
				}
			}
			return L"";
		}

		vint GetSemanticIdForTokenAsync(const ParsingTokenContext& tokenContext, const RepeatingParsingOutput& output)
		{
			auto cache = output.cache.Cast<Cache>();
			if (tokenContext.tokenParent->GetType() == L"PrimitiveTypeObj" || tokenContext.tokenParent->GetType() == L"SubTypeObj")
			{
				if (tokenContext.field == L"name") // Type
				{
					if (ResolveType(tokenContext.tokenParent, cache, nullptr) != L"")
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

		void FindRootGrammar(ParsingTreeObject* grammarDef, ParsingTreeObject*& rootGrammarDef, ParsingTreeObject*& ruleDef, const RepeatingPartialParsingOutput* output)
		{
			ParsingTreeObject* lastGrammar = grammarDef;
			ParsingTreeNode* current = GetParent(grammarDef, output);
			while (lastGrammar && current)
			{
				if (auto grammars = dynamic_cast<ParsingTreeArray*>(current))
				{
					if (auto grammarsParent = dynamic_cast<ParsingTreeObject*>(GetParent(grammars, output)))
					{
						if (grammarsParent->GetType() == L"RuleDef")
						{
							rootGrammarDef = lastGrammar;
							ruleDef = grammarsParent;
						}
					}
					return;
				}
				lastGrammar = dynamic_cast<ParsingTreeObject*>(current);
				current = GetParent(current, output);
			}
		}

		WString ResolveRuleType(ParsingTreeObject* ruleDef, Ptr<Cache> cache, const RepeatingPartialParsingOutput* output)
		{
			if (auto typeObj = GetMember(ruleDef, L"type", output).Cast<ParsingTreeObject>())
			{
				return ResolveType(typeObj.Obj(), cache, output);
			}
			return L"";
		}

		WString ResolveGrammarType(ParsingTreeObject* grammarDef, Ptr<Cache> cache, const RepeatingPartialParsingOutput* output)
		{
			List<WString> members;
			if (grammarDef->GetType() == L"CreateGrammarDef")
			{
				if (auto typeObj = GetMember(grammarDef, L"type", output).Cast<ParsingTreeObject>())
				{
					auto resolvedType = ResolveType(typeObj.Obj(), cache, output);
					if (resolvedType != L"")
					{
						return resolvedType;
					}
				}
			}

			if (
				grammarDef->GetType() == L"LoopGrammarDef" ||
				grammarDef->GetType() == L"OptionalGrammarDef" ||
				grammarDef->GetType() == L"CreateGrammarDef" ||
				grammarDef->GetType() == L"AssignGrammarDef" ||
				grammarDef->GetType() == L"UseGrammarDef" ||
				grammarDef->GetType() == L"SetterGrammarDef")
			{
				members.Add(L"grammar");
			}
			else if (
				grammarDef->GetType() == L"SequenceGrammarDef" ||
				grammarDef->GetType() == L"AlternativeGrammarDef")
			{
				members.Add(L"first");
				members.Add(L"second");
			}

			FOREACH(WString, member, members)
			{
				if (auto subGrammarDef = GetMember(grammarDef, member, output).Cast<ParsingTreeObject>())
				{
					auto resolvedType = ResolveGrammarType(subGrammarDef.Obj(), cache, output);
					if (resolvedType != L"")
					{
						return resolvedType;
					}
				}
			}
			return L"";
		}

		void FindAvailableFields(ParsingTreeObject* grammarDef, Ptr<Cache> cache, List<Tuple<WString, WString, WString>>& fields, const RepeatingPartialParsingOutput* output)
		{
			ParsingTreeObject* rootGrammarDef = nullptr;
			ParsingTreeObject* ruleDef = nullptr;
			FindRootGrammar(grammarDef, rootGrammarDef, ruleDef, output);
					
			if (rootGrammarDef && ruleDef)
			{
				auto resolvedType = ResolveGrammarType(rootGrammarDef, cache, output);
				auto needInheritedType = false;
				if (resolvedType == L"")
				{
					resolvedType = ResolveRuleType(ruleDef, cache, output);
					needInheritedType = true;
				}

				if (resolvedType != L"")
				{
					List<WString> types;
					types.Add(resolvedType);
					vint current = 0;

					while (current < types.Count())
					{
						auto type = types[current++];
						vint index = cache->classFields.Keys().IndexOf(type + L".");
						if (index != -1)
						{
							const auto& values = cache->classFields.GetByIndex(index);
							for (vint i = 0; i < values.Count(); i++)
							{
								auto& value = values[i];
								fields.Add(Tuple<WString, WString, WString>(type, value.f0, value.f1));
							}
						}

						if (needInheritedType)
						{
							index = cache->inheritedTypes.Keys().IndexOf(type);
							if (index != -1)
							{
								FOREACH(WString, inheritedType, cache->inheritedTypes.GetByIndex(index))
								{
									if (!types.Contains(inheritedType))
									{
										types.Add(inheritedType);
									}
								}
							}
						}
					}
				}
			}
		}

		void GetCandidateItemsAsync(const ParsingTokenContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems)
		{
			auto cache = partialOutput.input.cache.Cast<Cache>();
			if (tokenContext.tokenParent->GetType() == L"PrimitiveTypeObj")
			{
				if (tokenContext.field == L"name") // Type
				{
					List<WString> typeScopes;
					GetTypeScopes(tokenContext.tokenParent, typeScopes, &partialOutput);
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
						})
						.Distinct())
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
					if (auto parentTypeObj = GetMember(tokenContext.tokenParent, L"parentType", &partialOutput).Cast<ParsingTreeObject>())
					{
						auto resolvedType = ResolveType(parentTypeObj.Obj(), cache, &partialOutput);
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
					FOREACH(WString, name, cache->literalNames)
					{
						ParsingCandidateItem item;
						item.semanticId = _literal;
						item.name = name;
						candidateItems.Add(item);
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"AssignGrammarDef")
			{
				if (tokenContext.field == L"memberName") // Field
				{
					List<Tuple<WString, WString, WString>> fields;
					FindAvailableFields(tokenContext.tokenParent, cache, fields, &partialOutput);

					for (vint i = 0; i < fields.Count(); i++)
					{
						ParsingCandidateItem item;
						item.semanticId = _field;
						item.name = fields[i].f1;
						candidateItems.Add(item);
					}
				}
			}
			else if (tokenContext.tokenParent->GetType() == L"SetterGrammarDef")
			{
				if (tokenContext.field == L"memberName") // Field
				{
					List<Tuple<WString, WString, WString>> fields;
					FindAvailableFields(tokenContext.tokenParent, cache, fields, &partialOutput);

					for (vint i = 0; i < fields.Count(); i++)
					{
						if (cache->enumItems.Keys().Contains(fields[i].f2))
						{
							ParsingCandidateItem item;
							item.semanticId = _field;
							item.name = fields[i].f1;
							candidateItems.Add(item);
						}
					}
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