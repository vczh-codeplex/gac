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
					//-------------------------------------
					;

				return definitionWriter.Definition();
			}
		}
	}
}