#include "ParsingDefinitions.h"

namespace vl
{
	namespace parsing
	{
		namespace definitions
		{
			using namespace collections;

/***********************************************************************
�Ծ�
***********************************************************************/

			Ptr<ParsingDefinition> CreateParserDefinition()
			{
				ParsingDefinitionWriter definitionWriter;

				definitionWriter
					.Type(
						Class(L"TypeObj")
						)

					.Type(
						Class(L"PrimitiveTypeObj", Type(L"TypeObj"))
							.Member(L"name", TokenType())
						)

					.Type(
						Class(L"TokenTypeObj", Type(L"TypeObj"))
						)

					.Type(
						Class(L"SubTypeObj", Type(L"TypeObj"))
							.Member(L"parentType", Type(L"TypeObj"))
							.Member(L"name", TokenType())
						)

					.Type(
						Class(L"ArrayTypeObj", Type(L"TypeObj"))
							.Member(L"elementType", Type(L"TypeObj"))
						)
					//-------------------------------------
					.Type(
						Class(L"TypeDef")
							.Member(L"name", TokenType())
						)

					.Type(
						Class(L"ClassMemberDef")
							.Member(L"type", Type(L"TypeObj"))
							.Member(L"name", TokenType())
							.Member(L"unescapingFunction", TokenType())
						)

					.Type(
						Class(L"ClassTypeDef", Type(L"TypeDef"))								
							.Member(L"ambiguousType", TokenType())
							.Member(L"parentType", Type(L"TypeObj"))
							.Member(L"members", Type(L"ClassMemberDef").Array())
							.Member(L"subTypes", Type(L"TypeDef").Array())
						)

					.Type(
						Class(L"EnumMemberDef")
							.Member(L"name", TokenType())
						)

					.Type(
						Class(L"EnumTypeDef", Type(L"TypeDef"))
							.Member(L"members", Type(L"EnumMemberDef").Array())
						)
					//-------------------------------------
					.Type(
						Class(L"GrammarDef")
						)

					.Type(
						Class(L"PrimitiveGrammarDef", Type(L"GrammarDef"))
							.Member(L"name", TokenType())
						)

					.Type(
						Class(L"TextGrammarDef", Type(L"GrammarDef"))
							.Member(L"text", TokenType())
						)

					.Type(
						Class(L"SequenceGrammarDef", Type(L"GrammarDef"))
							.Member(L"first", Type(L"GrammarDef"))
							.Member(L"second", Type(L"GrammarDef"))
						)

					.Type(
						Class(L"AlternativeGrammarDef", Type(L"GrammarDef"))
							.Member(L"first", Type(L"GrammarDef"))
							.Member(L"second", Type(L"GrammarDef"))
						)

					.Type(
						Class(L"LoopGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
						)

					.Type(
						Class(L"OptionalGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
						)

					.Type(
						Class(L"CreateGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
							.Member(L"type", Type(L"TypeObj"))
						)

					.Type(
						Class(L"AssignGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
							.Member(L"memberName", TokenType())
						)

					.Type(
						Class(L"UseGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
						)

					.Type(
						Class(L"SetterGrammarDef", Type(L"GrammarDef"))
							.Member(L"grammar", Type(L"GrammarDef"))
							.Member(L"memberName", TokenType())
							.Member(L"value", TokenType())
						)
					//-------------------------------------
					.Type(
						Class(L"TokenDef")
							.SubType(
								Enum(L"DiscardOption")
									.Member(L"DiscardToken")
									.Member(L"KeepToken")
								)
							.Member(L"name", TokenType())
							.Member(L"regex", TokenType())
							.Member(L"discard", Type(L"DiscardOption"))
						)

					.Type(
						Class(L"RuleDef")
							.Member(L"name", TokenType())
							.Member(L"type", Type(L"TypeObj"))
							.Member(L"grammars", Type(L"GrammarDef").Array())
						)

					.Type(
						Class(L"ParserDef")
							.Member(L"types", Type(L"TypeDef").Array())
							.Member(L"tokens", Type(L"TokenDef").Array())
							.Member(L"rules", Type(L"RuleDef").Array())
						)
					//-------------------------------------
					.Token(L"CLASS",		L"class")
					.Token(L"AMBIGUOUS",	L"ambiguous")
					.Token(L"ENUM",			L"enum")
					.Token(L"TOKEN",		L"token")
					.Token(L"DISCARDTOKEN",	L"discardtoken")
					.Token(L"RULE",			L"rule")
					.Token(L"AS",			L"as")
					.Token(L"WITH",			L"with")

					.Token(L"OPEN",			L"/{")
					.Token(L"CLOSE",		L"/}")
					.Token(L"SEMICOLON",	L";")
					.Token(L"COLON",		L":")
					.Token(L"COMMA",		L",")
					.Token(L"DOT",			L".")
					.Token(L"ASSIGN",		L"/=")
					.Token(L"USING",		L"/!")
					.Token(L"OR",			L"/|")
					.Token(L"OPTOPEN"	,	L"/[")
					.Token(L"OPTCLOSE"	,	L"/]")
					.Token(L"PREOPEN"	,	L"/(")
					.Token(L"PRECLOSE"	,	L"/)")

					.Token(L"NAME",			L"[a-zA-Z_]/w*")
					.Token(L"STRING",		L"\"([^\"]|\"\")*\"")
					.Discard(L"SPACE",		L"/s+")
					//-------------------------------------
					.Rule(L"Type", Type(L"TypeObj"))
						.Imply(
							(Rule(L"NAME")[L"name"])
								.As(Type(L"PrimitiveTypeObj"))
							)
						.Imply(
							Text(L"token")
								.As(Type(L"TokenTypeObj"))
							)
						.Imply(
							(Rule(L"Type")[L"parentType"] + Text(L".") + Rule(L"NAME")[L"name"])
								.As(Type(L"SubTypeObj"))
							)
						.Imply(
							(Rule(L"Type")[L"elementType"] + Text(L"[") + Text(L"]"))
								.As(Type(L"ArrayTypeObj"))
							)
						.EndRule()
					//-------------------------------------
					.Rule(L"EnumMember", Type(L"EnumMemberDef"))
						.Imply(
							(Rule(L"NAME")[L"name"] + Text(L","))
								.As(Type(L"EnumMemberDef"))
							)
						.EndRule()
					.Rule(L"Enum", Type(L"EnumTypeDef"))
						.Imply(
							(Text(L"enum") + Rule(L"NAME")[L"name"] + Text(L"{") + *(Rule(L"EnumMember")[L"members"]) + Text(L"}"))
								.As(Type(L"EnumTypeDef"))
							)
						.EndRule()
					.Rule(L"ClassMember", Type(L"ClassMemberDef"))
						.Imply(
							(Rule(L"Type")[L"type"] + Rule(L"NAME")[L"name"] + Opt(Text(L"(") + Rule(L"NAME")[L"unescapingFunction"] + Text(L")")) + Text(L";"))
								.As(Type(L"ClassMemberDef"))
							)
						.EndRule()
					.Rule(L"Class", Type(L"ClassTypeDef"))
						.Imply(
							(
								Text(L"class") + Rule(L"NAME")[L"name"]
								+ Opt(Text(L"ambiguous") + Text(L"(") + Rule(L"NAME")[L"ambiguousType"] + Text(L")"))
								+ Opt(Text(L":") + Rule(L"Type")[L"parentType"]) + Text(L"{")
								+ *(Rule(L"ClassMember")[L"members"] | Rule(L"TypeDecl")[L"subTypes"])
								+ Text(L"}")
								)
								.As(Type(L"ClassTypeDef"))
							)
						.EndRule()
					.Rule(L"TypeDecl", Type(L"TypeDef"))
						.Imply(!Rule(L"Enum") | !Rule(L"Class"))
						.EndRule()
					//------------------------------------
					.Rule(L"PrimitiveGrammar", Type(L"GrammarDef"))
						.Imply(
							(Rule(L"NAME")[L"name"])
								.As(Type(L"PrimitiveGrammarDef"))
							)
						.Imply(
							(Rule(L"STRING")[L"text"])
								.As(Type(L"TextGrammarDef"))
							)
						.Imply(
							(Rule(L"PrimitiveGrammar")[L"grammar"] + Text(L":") + Rule(L"NAME")[L"memberName"])
								.As(Type(L"AssignGrammarDef"))
							)
						.Imply(
							(Text(L"!") + Rule(L"PrimitiveGrammar")[L"grammar"])
								.As(Type(L"UseGrammarDef"))
							)
						.Imply(
							(Text(L"[") + Rule(L"Grammar")[L"grammar"] + Text(L"]"))
								.As(Type(L"OptionalGrammarDef"))
							)
						.Imply(
							(Text(L"{") + Rule(L"Grammar")[L"grammar"] + Text(L"}"))
								.As(Type(L"LoopGrammarDef"))
							)
						.Imply(
							(Text(L"(") + !Rule(L"Grammar") + Text(L")"))
							)
						.EndRule()

					.Rule(L"SequenceGrammar", Type(L"GrammarDef"))
						.Imply(
							!Rule(L"PrimitiveGrammar")
							)
						.Imply(
							(Rule(L"SequenceGrammar")[L"first"] + Rule(L"PrimitiveGrammar")[L"second"])
								.As(Type(L"SequenceGrammarDef"))
							)
						.EndRule()

					.Rule(L"AlternativeGrammar", Type(L"GrammarDef"))
						.Imply(
							!Rule(L"SequenceGrammar")
							)
						.Imply(
							(Rule(L"AlternativeGrammar")[L"first"] + Text(L"|") + Rule(L"SequenceGrammar")[L"second"])
								.As(Type(L"AlternativeGrammarDef"))
							)
						.EndRule()

					.Rule(L"Grammar", Type(L"GrammarDef"))
						.Imply(
							!Rule(L"AlternativeGrammar")
							)
						.Imply(
							(Rule(L"Grammar")[L"grammar"] + Text(L"as") + Rule(L"Type")[L"type"])
								.As(Type(L"CreateGrammarDef"))
							)
						.Imply(
							(Rule(L"Grammar")[L"grammar"] + Text(L"with") + Text(L"{") + Rule(L"NAME")[L"memberName"] + Text(L"=") + Rule(L"STRING")[L"value"] + Text(L"}"))
								.As(Type(L"SetterGrammarDef"))
							)
						.EndRule()
					//------------------------------------
					.Rule(L"TokenDecl", Type(L"TokenDef"))
						.Imply(
							(Text(L"token") + Rule(L"NAME")[L"name"] + Text(L"=") + Rule(L"STRING")[L"regex"] + Text(L";"))
								.As(Type(L"TokenDef"))
								.Set(L"discard", L"KeepToken")
							)
						.Imply(
							(Text(L"discardtoken") + Rule(L"NAME")[L"name"] + Text(L"=") + Rule(L"STRING")[L"regex"] + Text(L";"))
								.As(Type(L"TokenDef"))
								.Set(L"discard", L"DiscardToken")
							)
						.EndRule()

					.Rule(L"RuleFragmentDecl", Type(L"GrammarDef"))
						.Imply(
							Text(L"=") + !Rule(L"Grammar") + Text(L";")
							)
						.EndRule()

					.Rule(L"RuleDecl", Type(L"RuleDef"))
						.Imply(
							(Text(L"rule") + Rule(L"Type")[L"type"] + Rule(L"NAME")[L"name"] + *(Text(L"=") + Rule(L"Grammar")[L"grammars"]) + Text(L";"))
								.As(Type(L"RuleDef"))
							)
						.EndRule()
					//------------------------------------
					.Rule(L"ParserDecl", Type(L"ParserDef"))
						.Imply(
							(
								*(
									Rule(L"TypeDecl")[L"types"] |
									Rule(L"TokenDecl")[L"tokens"] |
									Rule(L"RuleDecl")[L"rules"]
									)
								+(
									Rule(L"TypeDecl")[L"types"] |
									Rule(L"TokenDecl")[L"tokens"] |
									Rule(L"RuleDecl")[L"rules"]
									)
								)
								.As(Type(L"ParserDef"))
							)
						.EndRule()
					;

				return definitionWriter.Definition();
			}

			WString DeserializeString(Ptr<ParsingTreeToken> token)
			{
				const WString& value=token->GetValue();
				if(value.Length()>=2 && value[0]==L'"' && value[value.Length()-1]==L'"')
				{
					Array<wchar_t> chars(value.Length());
					memset(&chars[0], 0, chars.Count()*sizeof(wchar_t));
					const wchar_t* reading=value.Buffer()+1;
					wchar_t* writing=&chars[0];
					while(*reading)
					{
						if(*reading!=L'"')
						{
							*writing++=*reading++;
						}
						else if(reading[1]!=L'"')
						{
							break;
						}
						else
						{
							*writing++=L'"';
							reading+=2;
						}
					}
					return &chars[0];
				}
				return L"";
			}

			void SetName(WString& target, Ptr<ParsingTreeNode> node)
			{
				Ptr<ParsingTreeToken> token=node.Cast<ParsingTreeToken>();
				if(token)
				{
					target=token->GetValue();
				}
			}

			void SetText(WString& target, Ptr<ParsingTreeNode> node)
			{
				Ptr<ParsingTreeToken> token=node.Cast<ParsingTreeToken>();
				if(token)
				{
					target=DeserializeString(token);
				}
			}

			template<typename T>
			void SetArray(List<Ptr<T>>& target, Ptr<ParsingTreeNode> node)
			{
				Ptr<ParsingTreeArray> source=node.Cast<ParsingTreeArray>();
				if(source)
				{
					for(vint i=0;i<source->Count();i++)
					{
						target.Add(Deserialize(source->GetItem(i).Cast<ParsingTreeObject>()).Cast<T>());
					}
				}
			}

			template<typename T>
			void SetMember(Ptr<T>& target, Ptr<ParsingTreeNode> node)
			{
				Ptr<ParsingTreeObject> source=node.Cast<ParsingTreeObject>();
				if(source)
				{
					target=Deserialize(source).Cast<T>();
				}
			}

			Ptr<ParsingTreeCustomBase> Deserialize(Ptr<ParsingTreeObject> node)
			{
				if(!node)
				{
					return 0;
				}
				else if(node->GetType()==L"PrimitiveTypeObj")
				{
					Ptr<ParsingDefinitionPrimitiveType> target=new ParsingDefinitionPrimitiveType;
					SetName(target->name, node->GetMember(L"name"));
					return target;
				}
				else if(node->GetType()==L"TokenTypeObj")
				{
					Ptr<ParsingDefinitionTokenType> target=new ParsingDefinitionTokenType;
					return target;
				}
				else if(node->GetType()==L"SubTypeObj")
				{
					Ptr<ParsingDefinitionSubType> target=new ParsingDefinitionSubType;
					SetMember(target->parentType, node->GetMember(L"parentType"));
					SetName(target->subTypeName, node->GetMember(L"name"));
					return target;
				}
				else if(node->GetType()==L"ArrayTypeObj")
				{
					Ptr<ParsingDefinitionArrayType> target=new ParsingDefinitionArrayType;
					SetMember(target->elementType, node->GetMember(L"elementType"));
					return target;
				}
				else if(node->GetType()==L"ClassMemberDef")
				{
					Ptr<ParsingDefinitionClassMemberDefinition> target=new ParsingDefinitionClassMemberDefinition;
					SetMember(target->type, node->GetMember(L"type"));
					SetName(target->name, node->GetMember(L"name"));
					SetName(target->unescapingFunction, node->GetMember(L"unescapingFunction"));
					return target;
				}
				else if(node->GetType()==L"ClassTypeDef")
				{
					Ptr<ParsingDefinitionClassDefinition> target=new ParsingDefinitionClassDefinition;
					SetName(target->ambiguousType, node->GetMember(L"ambiguousType"));
					SetMember(target->parentType, node->GetMember(L"parentType"));
					SetName(target->name, node->GetMember(L"name"));
					SetArray(target->members, node->GetMember(L"members"));
					SetArray(target->subTypes, node->GetMember(L"subTypes"));
					return target;
				}
				else if(node->GetType()==L"EnumMemberDef")
				{
					Ptr<ParsingDefinitionEnumMemberDefinition> target=new ParsingDefinitionEnumMemberDefinition;
					SetName(target->name, node->GetMember(L"name"));
					return target;
				}
				else if(node->GetType()==L"EnumTypeDef")
				{
					Ptr<ParsingDefinitionEnumDefinition> target=new ParsingDefinitionEnumDefinition;
					SetName(target->name, node->GetMember(L"name"));
					SetArray(target->members, node->GetMember(L"members"));
					return target;
				}
				else if(node->GetType()==L"PrimitiveGrammarDef")
				{
					Ptr<ParsingDefinitionPrimitiveGrammar> target=new ParsingDefinitionPrimitiveGrammar;
					SetName(target->name, node->GetMember(L"name"));
					return target;
				}
				else if(node->GetType()==L"TextGrammarDef")
				{
					Ptr<ParsingDefinitionTextGrammar> target=new ParsingDefinitionTextGrammar;
					SetText(target->text, node->GetMember(L"text"));
					return target;
				}
				else if(node->GetType()==L"SequenceGrammarDef")
				{
					Ptr<ParsingDefinitionSequenceGrammar> target=new ParsingDefinitionSequenceGrammar;
					SetMember(target->first, node->GetMember(L"first"));
					SetMember(target->second, node->GetMember(L"second"));
					return target;
				}
				else if(node->GetType()==L"AlternativeGrammarDef")
				{
					Ptr<ParsingDefinitionAlternativeGrammar> target=new ParsingDefinitionAlternativeGrammar;
					SetMember(target->first, node->GetMember(L"first"));
					SetMember(target->second, node->GetMember(L"second"));
					return target;
				}
				else if(node->GetType()==L"LoopGrammarDef")
				{
					Ptr<ParsingDefinitionLoopGrammar> target=new ParsingDefinitionLoopGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					return target;
				}
				else if(node->GetType()==L"OptionalGrammarDef")
				{
					Ptr<ParsingDefinitionOptionalGrammar> target=new ParsingDefinitionOptionalGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					return target;
				}
				else if(node->GetType()==L"CreateGrammarDef")
				{
					Ptr<ParsingDefinitionCreateGrammar> target=new ParsingDefinitionCreateGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					SetMember(target->type, node->GetMember(L"type"));
					return target;
				}
				else if(node->GetType()==L"AssignGrammarDef")
				{
					Ptr<ParsingDefinitionAssignGrammar> target=new ParsingDefinitionAssignGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					SetName(target->memberName, node->GetMember(L"memberName"));
					return target;
				}
				else if(node->GetType()==L"UseGrammarDef")
				{
					Ptr<ParsingDefinitionUseGrammar> target=new ParsingDefinitionUseGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					return target;
				}
				else if(node->GetType()==L"SetterGrammarDef")
				{
					Ptr<ParsingDefinitionSetterGrammar> target=new ParsingDefinitionSetterGrammar;
					SetMember(target->grammar, node->GetMember(L"grammar"));
					SetName(target->memberName, node->GetMember(L"memberName"));
					SetText(target->value, node->GetMember(L"value"));
					return target;
				}
				else if(node->GetType()==L"TokenDef")
				{
					Ptr<ParsingDefinitionTokenDefinition> target=new ParsingDefinitionTokenDefinition;
					SetName(target->name, node->GetMember(L"name"));
					SetText(target->regex, node->GetMember(L"regex"));

					Ptr<ParsingTreeToken> token=node->GetMember(L"discard").Cast<ParsingTreeToken>();
					target->discard=(token && token->GetValue()==L"DiscardToken");
					return target;
				}
				else if(node->GetType()==L"RuleDef")
				{
					Ptr<ParsingDefinitionRuleDefinition> target=new ParsingDefinitionRuleDefinition;
					SetName(target->name, node->GetMember(L"name"));
					SetMember(target->type, node->GetMember(L"type"));
					SetArray(target->grammars, node->GetMember(L"grammars"));
					return target;
				}
				else if(node->GetType()==L"ParserDef")
				{
					Ptr<ParsingDefinition> target=new ParsingDefinition;
					SetArray(target->types, node->GetMember(L"types"));
					SetArray(target->tokens, node->GetMember(L"tokens"));
					SetArray(target->rules, node->GetMember(L"rules"));
					return target;
				}
				else
				{
					return 0;
				}
			}

			Ptr<ParsingDefinition> DeserializeDefinition(Ptr<ParsingTreeNode> node)
			{
				return Deserialize(node.Cast<ParsingTreeObject>()).Cast<ParsingDefinition>();
			}
		}
	}
}