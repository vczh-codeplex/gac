/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::ParsingJson_Parser

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#ifndef VCZH_PARSING_JSON_PARSINGJSON_PARSER
#define VCZH_PARSING_JSON_PARSINGJSON_PARSER

#include "..\Parsing.h"
#include "..\ParsingDefinitions.h"
#include "..\ParsingAutomaton.h"

namespace vl
{
	namespace parsing
	{
		namespace json
		{
			enum class JsonParserTokenIndex
			{
				TRUEVALUE = 0,
				FALSEVALUE = 1,
				NULLVALUE = 2,
				OBJOPEN = 3,
				OBJCLOSE = 4,
				ARROPEN = 5,
				ARRCLOSE = 6,
				COMMA = 7,
				COLON = 8,
				NUMBER = 9,
				STRING = 10,
				SPACE = 11,
			};
			class JsonNode;
			class JsonLiteral;
			class JsonString;
			class JsonNumber;
			class JsonArray;
			class JsonObjectField;
			class JsonObject;

			class JsonNode abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<JsonNode>
			{
			public:
				class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
				{
				public:
					virtual void Visit(JsonLiteral* node)=0;
					virtual void Visit(JsonString* node)=0;
					virtual void Visit(JsonNumber* node)=0;
					virtual void Visit(JsonArray* node)=0;
					virtual void Visit(JsonObjectField* node)=0;
					virtual void Visit(JsonObject* node)=0;
				};

				virtual void Accept(JsonNode::IVisitor* visitor)=0;

			};

			class JsonLiteral : public JsonNode, vl::reflection::Description<JsonLiteral>
			{
			public:
				enum class JsonValue
				{
					True,
					False,
					Null,
				};

				JsonValue value;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonLiteral> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonString : public JsonNode, vl::reflection::Description<JsonString>
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonString> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonNumber : public JsonNode, vl::reflection::Description<JsonNumber>
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonNumber> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonArray : public JsonNode, vl::reflection::Description<JsonArray>
			{
			public:
				vl::collections::List<vl::Ptr<JsonNode>> items;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonArray> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonObjectField : public JsonNode, vl::reflection::Description<JsonObjectField>
			{
			public:
				vl::parsing::ParsingToken name;
				vl::Ptr<JsonNode> value;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonObjectField> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonObject : public JsonNode, vl::reflection::Description<JsonObject>
			{
			public:
				vl::collections::List<vl::Ptr<JsonObjectField>> fields;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonObject> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::WString JsonGetParserTextBuffer();
			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> JsonConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> JsonLoadTable();

			extern vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> JsonParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

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
			DECL_TYPE_INFO(vl::parsing::json::JsonNode)
			DECL_TYPE_INFO(vl::parsing::json::JsonLiteral)
			DECL_TYPE_INFO(vl::parsing::json::JsonLiteral::JsonValue)
			DECL_TYPE_INFO(vl::parsing::json::JsonString)
			DECL_TYPE_INFO(vl::parsing::json::JsonNumber)
			DECL_TYPE_INFO(vl::parsing::json::JsonArray)
			DECL_TYPE_INFO(vl::parsing::json::JsonObjectField)
			DECL_TYPE_INFO(vl::parsing::json::JsonObject)
			DECL_TYPE_INFO(vl::parsing::json::JsonNode::IVisitor)

			namespace interface_proxy
			{
				class JsonNode_IVisitor : public ValueInterfaceRoot, public virtual vl::parsing::json::JsonNode::IVisitor
				{
				public:
					JsonNode_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::parsing::json::JsonNode::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new JsonNode_IVisitor(proxy);
					}

					void Visit(vl::parsing::json::JsonLiteral* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::parsing::json::JsonString* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::parsing::json::JsonNumber* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::parsing::json::JsonArray* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::parsing::json::JsonObjectField* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::parsing::json::JsonObject* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				}
#endif

			extern bool JsonLoadTypes();
		}
	}
}
#endif