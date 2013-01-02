#include "ParsingDefinitions.h"

namespace vl
{
	namespace parsing
	{
		namespace definitions
		{
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
						)

					.Type(
						Class(L"ClassTypeDef", Type(L"TypeDef"))
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
					.Token(L"ENUM",			L"enum")
					.Token(L"TOKEN",		L"token")
					.Token(L"DISCARDTOKEN",	L"discardtoken")
					.Token(L"RULE",			L"rule")

					.Token(L"OPEN",			L"/{")
					.Token(L"CLOSE",		L"/}")
					.Token(L"SEMICOLON",	L";")
					.Token(L"COLON",		L":")
					.Token(L"COMMA",		L",")
					.Token(L"DOT",			L".")
					.Token(L"ASSIGN",		L"=")
					.Token(L"ARRAYTYPE",	L"/[/]")

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
							(Rule(L"Type")[L"elementType"] + Text(L"[]"))
								.As(Type(L"ArrayTypeObj"))
							)
						.EndRule()
					;

				return definitionWriter.Definition();
			}
		}
	}
}