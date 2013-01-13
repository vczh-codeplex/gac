/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::ParsingJson_Parser

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/

#include "..\Parsing.h"

namespace vl
{
	namespace parsing
	{
		namespace json
		{
			class JsonNode;
			class JsonLiteral;
			class JsonString;
			class JsonNumber;
			class JsonArray;
			class JsonObjectField;
			class JsonObject;

			class JsonNode abstract : public vl::parsing::ParsingTreeCustomBase
			{
			public:
				class IVisitor : public vl::Interface
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

			class JsonLiteral : public JsonNode
			{
			public:
				struct JsonValue abstract
				{
					enum Type
					{
						True,
						False,
						Null,
					};
				};

				JsonValue::Type value;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonLiteral> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonString : public JsonNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonString> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonNumber : public JsonNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonNumber> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonArray : public JsonNode
			{
			public:
				vl::collections::List<vl::Ptr<JsonNode>> items;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonArray> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonObjectField : public JsonNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::Ptr<JsonNode> value;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonObjectField> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class JsonObject : public JsonNode
			{
			public:
				vl::collections::List<vl::Ptr<JsonObjectField>> fields;

				void Accept(JsonNode::IVisitor* visitor)override;

				static vl::Ptr<JsonObject> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> JsonConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> JsonLoadTable();

			extern vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		}
	}
}
