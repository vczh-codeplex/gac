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
L"\r\n"L"\ttoken content(JsonUnescapingString)\t\t\t\t@Color(\"String\");"
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
L"\r\n"L"\ttoken name(JsonUnescapingString)\t\t\t\t@Color(\"AttName\");"
L"\r\n"L"\tNode value;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Object:Node"
L"\r\n"L"{"
L"\r\n"L"\tObjectField[] fields;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token TRUEVALUE = \"true\"\t\t\t\t\t\t\t@Color(\"Keyword\");"
L"\r\n"L"token FALSEVALUE = \"false\"\t\t\t\t\t\t\t@Color(\"Keyword\");"
L"\r\n"L"token NULLVALUE = \"null\"\t\t\t\t\t\t\t@Color(\"Keyword\");"
L"\r\n"L"token OBJOPEN = \"\\{\"\t\t\t\t\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token OBJCLOSE = \"\\}\"\t\t\t\t\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token ARROPEN = \"\\[\"\t\t\t\t\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token ARRCLOSE = \"\\]\"\t\t\t\t\t\t\t\t@Color(\"Boundary\");"
L"\r\n"L"token COMMA = \",\";"
L"\r\n"L"token COLON = \":\";"
L"\r\n"L"token NUMBER = \"[\\-]?\\d+(.\\d+)?([eE][+\\-]?\\d+)?\"\t@Color(\"Number\");"
L"\r\n"L"token STRING = \"\"\"([^\\\\\"\"]|\\\\[^u]|\\\\u\\d{4})*\"\"\"\t\t@ContextColor();"
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

				bool SetMember(JsonLiteral::JsonValue& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
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

			vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex)
			{
				vl::parsing::tabling::ParsingState state(input, table, codeIndex);
				state.Reset(L"JRoot");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return JsonParseAsParsingTreeNode(input, table, errors, codeIndex);
			}

			vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex)
			{
				vl::parsing::tabling::ParsingState state(input, table, codeIndex);
				state.Reset(L"JRoot");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node && errors.Count()==0)
				{
					return JsonConvertParsingTreeNode(node, state.GetTokens()).Cast<JsonNode>();
				}
				return 0;
			}

			vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex)
			{
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				return JsonParse(input, table, errors, codeIndex);
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
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			using namespace vl::parsing::json;

			IMPL_TYPE_INFO_RENAME(JsonNode, system::JsonNode)
			IMPL_TYPE_INFO_RENAME(JsonLiteral, system::JsonLiteral)
			IMPL_TYPE_INFO_RENAME(JsonLiteral::JsonValue, system::JsonLiteral::JsonValue)
			IMPL_TYPE_INFO_RENAME(JsonString, system::JsonString)
			IMPL_TYPE_INFO_RENAME(JsonNumber, system::JsonNumber)
			IMPL_TYPE_INFO_RENAME(JsonArray, system::JsonArray)
			IMPL_TYPE_INFO_RENAME(JsonObjectField, system::JsonObjectField)
			IMPL_TYPE_INFO_RENAME(JsonObject, system::JsonObject)
			IMPL_TYPE_INFO_RENAME(JsonNode::IVisitor, system::JsonNode::IVisitor)

			BEGIN_CLASS_MEMBER(JsonNode)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(JsonNode)

			BEGIN_CLASS_MEMBER(JsonLiteral)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonLiteral>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(JsonLiteral)

			BEGIN_ENUM_ITEM(JsonLiteral::JsonValue)
				ENUM_ITEM_NAMESPACE(JsonLiteral::JsonValue)
				ENUM_NAMESPACE_ITEM(True)
				ENUM_NAMESPACE_ITEM(False)
				ENUM_NAMESPACE_ITEM(Null)
			END_ENUM_ITEM(JsonLiteral::JsonValue)

			BEGIN_CLASS_MEMBER(JsonString)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonString>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(JsonString::*)(), [](JsonString* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(JsonString::*)(const vl::WString&), [](JsonString* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(JsonString)

			BEGIN_CLASS_MEMBER(JsonNumber)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonNumber>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_content, NO_PARAMETER, vl::WString(JsonNumber::*)(), [](JsonNumber* node){ return node->content.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_content, {L"value"}, void(JsonNumber::*)(const vl::WString&), [](JsonNumber* node, const vl::WString& value){ node->content.value = value; })

				CLASS_MEMBER_PROPERTY(content, get_content, set_content)
			END_CLASS_MEMBER(JsonNumber)

			BEGIN_CLASS_MEMBER(JsonArray)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonArray>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(items)
			END_CLASS_MEMBER(JsonArray)

			BEGIN_CLASS_MEMBER(JsonObjectField)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonObjectField>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(JsonObjectField::*)(), [](JsonObjectField* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(JsonObjectField::*)(const vl::WString&), [](JsonObjectField* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(JsonObjectField)

			BEGIN_CLASS_MEMBER(JsonObject)
				CLASS_MEMBER_BASE(JsonNode)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<JsonObject>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(fields)
			END_CLASS_MEMBER(JsonObject)

			BEGIN_CLASS_MEMBER(JsonNode::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<JsonNode::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::JsonNode_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonLiteral* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonString* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonNumber* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonArray* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonObjectField* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(JsonNode::IVisitor::*)(JsonObject* node))
			END_CLASS_MEMBER(JsonNode)

			class JsonTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::parsing::json::JsonNode)
					ADD_TYPE_INFO(vl::parsing::json::JsonLiteral)
					ADD_TYPE_INFO(vl::parsing::json::JsonLiteral::JsonValue)
					ADD_TYPE_INFO(vl::parsing::json::JsonString)
					ADD_TYPE_INFO(vl::parsing::json::JsonNumber)
					ADD_TYPE_INFO(vl::parsing::json::JsonArray)
					ADD_TYPE_INFO(vl::parsing::json::JsonObjectField)
					ADD_TYPE_INFO(vl::parsing::json::JsonObject)
					ADD_TYPE_INFO(vl::parsing::json::JsonNode::IVisitor)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool JsonLoadTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new JsonTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}
