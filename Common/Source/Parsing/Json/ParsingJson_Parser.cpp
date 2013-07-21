#include "ParsingJson_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace json
		{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n"L""
L"\r\n"L"class Node"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Literal:Node"
L"\r\n"L"{"
L"\r\n"L"\tenum Value"
L"\r\n"L"\t{"
L"\r\n"L"\t\tTrue,"
L"\r\n"L"\t\tFalse,"
L"\r\n"L"\t\tNull,"
L"\r\n"L"\t}"
L"\r\n"L""
L"\r\n"L"\tValue value;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class String:Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content(JsonUnescapingString);"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Number:Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken content;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Array:Node"
L"\r\n"L"{"
L"\r\n"L"\tNode[] items;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ObjectField:Node"
L"\r\n"L"{"
L"\r\n"L"\ttoken name(JsonUnescapingString);"
L"\r\n"L"\tNode value;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Object:Node"
L"\r\n"L"{"
L"\r\n"L"\tObjectField[] fields;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token TRUEVALUE = \"true\";"
L"\r\n"L"token FALSEVALUE = \"false\";"
L"\r\n"L"token NULLVALUE = \"null\";"
L"\r\n"L"token OBJOPEN = \"\\{\";"
L"\r\n"L"token OBJCLOSE = \"\\}\";"
L"\r\n"L"token ARROPEN = \"\\[\";"
L"\r\n"L"token ARRCLOSE = \"\\]\";"
L"\r\n"L"token COMMA = \",\";"
L"\r\n"L"token COLON = \":\";"
L"\r\n"L"token NUMBER = \"[\\-]?\\d+(.\\d+)?([eE][+\\-]?\\d+)?\";"
L"\r\n"L"token STRING = \"\"\"([^\\\\\"\"]|\\\\[^u]|\\\\u\\d{4})*\"\"\";"
L"\r\n"L""
L"\r\n"L"discardtoken SPACE = \"\\s+\";"
L"\r\n"L""
L"\r\n"L"rule Node JLiteral"
L"\r\n"L"\t= STRING:content as String"
L"\r\n"L"\t= NUMBER:content as Number"
L"\r\n"L"\t= \"true\" as Literal with {value = \"True\"}"
L"\r\n"L"\t= \"false\" as Literal with {value = \"False\"}"
L"\r\n"L"\t= \"null\" as Literal with {value = \"Null\"}"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule ObjectField JField"
L"\r\n"L"\t= STRING:name \":\" JValue:value as ObjectField"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Object JObject"
L"\r\n"L"\t= \"{\" [JField:fields {\",\" JField:fields} ] \"}\" as Object"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Array JArray"
L"\r\n"L"\t= \"[\" [JValue:items {\",\" JValue:items} ] \"]\" as Array"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Node JValue"
L"\r\n"L"\t= !JLiteral"
L"\r\n"L"\t= !JObject"
L"\r\n"L"\t= !JArray"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Node JRoot"
L"\r\n"L"\t= !JObject"
L"\r\n"L"\t= !JArray"
L"\r\n"L"\t;"
;

			vl::WString JsonGetParserTextBuffer()
			{
				return parserTextBuffer;
			}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void JsonUnescapingString(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class JsonTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				bool SetMember(JsonLiteral::JsonValue::Type& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
				{
					vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
					if(token)
					{
						if(token->GetValue()==L"True") { member=JsonLiteral::JsonValue::True; return true; }
						else if(token->GetValue()==L"False") { member=JsonLiteral::JsonValue::False; return true; }
						else if(token->GetValue()==L"Null") { member=JsonLiteral::JsonValue::Null; return true; }
						else { member=JsonLiteral::JsonValue::True; return false; }
					}
					member=JsonLiteral::JsonValue::True;
					return false;
				}

				void Fill(vl::Ptr<JsonNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<JsonLiteral> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->value, obj->GetMember(L"value"), tokens);
				}

				void Fill(vl::Ptr<JsonString> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						JsonUnescapingString(tree->content, tokens);
					}
				}

				void Fill(vl::Ptr<JsonNumber> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->content, obj->GetMember(L"content"), tokens);
				}

				void Fill(vl::Ptr<JsonArray> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->items, obj->GetMember(L"items"), tokens);
				}

				void Fill(vl::Ptr<JsonObjectField> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->name, obj->GetMember(L"name"), tokens))
					{
						JsonUnescapingString(tree->name, tokens);
					}
					SetMember(tree->value, obj->GetMember(L"value"), tokens);
				}

				void Fill(vl::Ptr<JsonObject> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->fields, obj->GetMember(L"fields"), tokens);
				}

				vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"Literal")
					{
						vl::Ptr<JsonLiteral> tree = new JsonLiteral;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"String")
					{
						vl::Ptr<JsonString> tree = new JsonString;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Number")
					{
						vl::Ptr<JsonNumber> tree = new JsonNumber;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Array")
					{
						vl::Ptr<JsonArray> tree = new JsonArray;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"ObjectField")
					{
						vl::Ptr<JsonObjectField> tree = new JsonObjectField;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Object")
					{
						vl::Ptr<JsonObject> tree = new JsonObject;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else 
						return 0;
				}
			};

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> JsonConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				JsonTreeConverter converter;
				vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
				converter.SetMember(tree, node, tokens);
				return tree;
			}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

			vl::Ptr<JsonLiteral> JsonLiteral::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonLiteral>();
			}

			vl::Ptr<JsonString> JsonString::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonString>();
			}

			vl::Ptr<JsonNumber> JsonNumber::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonNumber>();
			}

			vl::Ptr<JsonArray> JsonArray::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonArray>();
			}

			vl::Ptr<JsonObjectField> JsonObjectField::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonObjectField>();
			}

			vl::Ptr<JsonObject> JsonObject::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return JsonConvertParsingTreeNode(node, tokens).Cast<JsonObject>();
			}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

			void JsonLiteral::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void JsonString::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void JsonNumber::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void JsonArray::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void JsonObjectField::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void JsonObject::Accept(JsonNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
Parser Function
***********************************************************************/

			vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"JRoot");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"JRoot");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node)
				{
					return JsonConvertParsingTreeNode(node, state.GetTokens()).Cast<JsonNode>();
				}
				return 0;
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> JsonLoadTable()
			{
			    vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();
			    vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			    vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(parserTextBuffer, L"ParserDecl", errors);
			    vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);
			    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, false, errors);
			    return table;
			}

		}
	}
}
