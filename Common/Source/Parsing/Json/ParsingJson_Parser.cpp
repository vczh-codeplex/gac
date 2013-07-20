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
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(14-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 36, 6);
				#define SET_TOKEN_INFO(INDEX, NAME, REGEX) table->SetTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
				#define SET_DISCARD_TOKEN_INFO(INDEX, NAME, REGEX) table->SetDiscardTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
				#define SET_STATE_INFO(INDEX, RULE, STATE, EXPR) table->SetStateInfo(INDEX, vl::parsing::tabling::ParsingTable::StateInfo(RULE, STATE, EXPR));
				#define SET_RULE_INFO(INDEX, NAME, TYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, L"", STARTSTATE));
				#define SET_AMBIGUOUS_RULE_INFO(INDEX, NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE));
				#define BEGIN_TRANSITION_BAG(STATE, TOKEN) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag; table->SetTransitionBag(STATE, TOKEN, bag);
				#define BEGIN_TRANSITION_ITEM(TOKEN, TARGETSTATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(TOKEN, TARGETSTATE); bag->transitionItems.Add(item);
				#define END_TRANSITION_ITEM }
				#define END_TRANSITION_BAG }
				#define ITEM_STACK_PATTERN(STATE) item->stackPattern.Add(STATE);
				#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE, RULE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE, RULE));
				#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
				#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);
				#define END_LOOK_AHEAD }

				SET_TOKEN_INFO(0, L"", L"")
				SET_TOKEN_INFO(1, L"", L"")
				SET_TOKEN_INFO(2, L"", L"")
				SET_TOKEN_INFO(3, L"TRUEVALUE", L"true")
				SET_TOKEN_INFO(4, L"FALSEVALUE", L"false")
				SET_TOKEN_INFO(5, L"NULLVALUE", L"null")
				SET_TOKEN_INFO(6, L"OBJOPEN", L"\\{")
				SET_TOKEN_INFO(7, L"OBJCLOSE", L"\\}")
				SET_TOKEN_INFO(8, L"ARROPEN", L"\\[")
				SET_TOKEN_INFO(9, L"ARRCLOSE", L"\\]")
				SET_TOKEN_INFO(10, L"COMMA", L",")
				SET_TOKEN_INFO(11, L"COLON", L":")
				SET_TOKEN_INFO(12, L"NUMBER", L"[\\-]?\\d+(.\\d+)?([eE][+\\-]?\\d+)?")
				SET_TOKEN_INFO(13, L"STRING", L"\"([^\\\\\"]|\\\\[^u]|\\\\u\\d{4})*\"")

				SET_DISCARD_TOKEN_INFO(0, L"SPACE", L"\\s+")

				SET_STATE_INFO(0, L"JLiteral", L"JLiteral.RootStart", L"¡ñ $<JLiteral>")
				SET_STATE_INFO(1, L"JLiteral", L"JLiteral.Start", L"¡¤ <JLiteral>")
				SET_STATE_INFO(2, L"JLiteral", L"JLiteral.1", L"<JLiteral>: STRING : content as String¡ñ\r\n<JLiteral>: NUMBER : content as Number¡ñ\r\n<JLiteral>: \"true\" as Literal¡ñ with { value = \"True\" }\r\n<JLiteral>: \"false\" as Literal¡ñ with { value = \"False\" }\r\n<JLiteral>: \"null\" as Literal¡ñ with { value = \"Null\" }")
				SET_STATE_INFO(3, L"JLiteral", L"JLiteral.RootEnd", L"$<JLiteral> ¡ñ")
				SET_STATE_INFO(4, L"JField", L"JField.RootEnd", L"$<JField> ¡ñ")
				SET_STATE_INFO(5, L"JObject", L"JObject.3", L"<JObject>: \"{\" [ JField : fields { \",\" JField : fields } ] \"}\" as Object¡ñ")
				SET_STATE_INFO(6, L"JObject", L"JObject.4", L"<JObject>: \"{\" [ JField : fields { \",\"¡ñ JField : fields } ] \"}\" as Object")
				SET_STATE_INFO(7, L"JArray", L"JArray.3", L"<JArray>: \"[\" [ JValue : items { \",\" JValue : items } ] \"]\" as Array¡ñ")
				SET_STATE_INFO(8, L"JArray", L"JArray.4", L"<JArray>: \"[\" [ JValue : items { \",\"¡ñ JValue : items } ] \"]\" as Array")
				SET_STATE_INFO(9, L"JValue", L"JValue.RootEnd", L"$<JValue> ¡ñ")
				SET_STATE_INFO(10, L"JObject", L"JObject.RootEnd", L"$<JObject> ¡ñ")
				SET_STATE_INFO(11, L"JRoot", L"JRoot.RootEnd", L"$<JRoot> ¡ñ")
				SET_STATE_INFO(12, L"JField", L"JField.1", L"<JField>: STRING : name¡ñ \":\" JValue : value as ObjectField")
				SET_STATE_INFO(13, L"JArray", L"JArray.RootEnd", L"$<JArray> ¡ñ")
				SET_STATE_INFO(14, L"JObject", L"JObject.1", L"<JObject>: \"{\"¡ñ [ JField : fields { \",\" JField : fields } ] \"}\" as Object")
				SET_STATE_INFO(15, L"JArray", L"JArray.1", L"<JArray>: \"[\"¡ñ [ JValue : items { \",\" JValue : items } ] \"]\" as Array")
				SET_STATE_INFO(16, L"JField", L"JField.2", L"<JField>: STRING : name \":\"¡ñ JValue : value as ObjectField")
				SET_STATE_INFO(17, L"JField", L"JField.RootStart", L"¡ñ $<JField>")
				SET_STATE_INFO(18, L"JField", L"JField.Start", L"¡¤ <JField>")
				SET_STATE_INFO(19, L"JObject", L"JObject.RootStart", L"¡ñ $<JObject>")
				SET_STATE_INFO(20, L"JObject", L"JObject.Start", L"¡¤ <JObject>")
				SET_STATE_INFO(21, L"JArray", L"JArray.RootStart", L"¡ñ $<JArray>")
				SET_STATE_INFO(22, L"JArray", L"JArray.Start", L"¡¤ <JArray>")
				SET_STATE_INFO(23, L"JValue", L"JValue.RootStart", L"¡ñ $<JValue>")
				SET_STATE_INFO(24, L"JValue", L"JValue.Start", L"¡¤ <JValue>")
				SET_STATE_INFO(25, L"JRoot", L"JRoot.RootStart", L"¡ñ $<JRoot>")
				SET_STATE_INFO(26, L"JRoot", L"JRoot.Start", L"¡¤ <JRoot>")
				SET_STATE_INFO(27, L"JLiteral", L"JLiteral.2", L"<JLiteral>: STRING : content as String¡ñ\r\n<JLiteral>: NUMBER : content as Number¡ñ\r\n<JLiteral>: \"true\" as Literal with { value = \"True\" }¡ñ\r\n<JLiteral>: \"false\" as Literal with { value = \"False\" }¡ñ\r\n<JLiteral>: \"null\" as Literal with { value = \"Null\" }¡ñ")
				SET_STATE_INFO(28, L"JField", L"JField.3", L"<JField>: STRING : name \":\" JValue : value as ObjectField¡ñ")
				SET_STATE_INFO(29, L"JField", L"JField.4", L"<JField>: STRING : name \":\" JValue : value as ObjectField¡ñ")
				SET_STATE_INFO(30, L"JObject", L"JObject.2", L"<JObject>: \"{\" [ JField : fields¡ñ { \",\" JField : fields } ] \"}\" as Object\r\n<JObject>: \"{\" [ JField : fields ¡ñ{ \",\" JField : fields } ] \"}\" as Object")
				SET_STATE_INFO(31, L"JObject", L"JObject.5", L"<JObject>: \"{\" [ JField : fields { \",\" JField : fields } ] \"}\" as Object¡ñ")
				SET_STATE_INFO(32, L"JArray", L"JArray.2", L"<JArray>: \"[\" [ JValue : items¡ñ { \",\" JValue : items } ] \"]\" as Array\r\n<JArray>: \"[\" [ JValue : items ¡ñ{ \",\" JValue : items } ] \"]\" as Array")
				SET_STATE_INFO(33, L"JArray", L"JArray.5", L"<JArray>: \"[\" [ JValue : items { \",\" JValue : items } ] \"]\" as Array¡ñ")
				SET_STATE_INFO(34, L"JValue", L"JValue.1", L"<JValue>: !JLiteral¡ñ\r\n<JValue>: !JObject¡ñ\r\n<JValue>: !JArray¡ñ")
				SET_STATE_INFO(35, L"JRoot", L"JRoot.1", L"<JRoot>: !JObject¡ñ\r\n<JRoot>: !JArray¡ñ")

				SET_RULE_INFO(0, L"JLiteral", L"Node", 0)
				SET_RULE_INFO(1, L"JField", L"ObjectField", 17)
				SET_RULE_INFO(2, L"JObject", L"Object", 19)
				SET_RULE_INFO(3, L"JArray", L"Array", 21)
				SET_RULE_INFO(4, L"JValue", L"Node", 23)
				SET_RULE_INFO(5, L"JRoot", L"Node", 25)

				BEGIN_TRANSITION_BAG(0, 0)

					BEGIN_TRANSITION_ITEM(0, 1)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 3)

					BEGIN_TRANSITION_ITEM(3, 2)
					ITEM_INSTRUCTION(Setter, 0, L"value", L"True", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 4)

					BEGIN_TRANSITION_ITEM(4, 2)
					ITEM_INSTRUCTION(Setter, 0, L"value", L"False", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 5)

					BEGIN_TRANSITION_ITEM(5, 2)
					ITEM_INSTRUCTION(Setter, 0, L"value", L"Null", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 12)

					BEGIN_TRANSITION_ITEM(12, 2)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Number", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 13)

					BEGIN_TRANSITION_ITEM(13, 2)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"String", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 1)

					BEGIN_TRANSITION_ITEM(1, 3)
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 4)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 7)

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 9)

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 10)

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 1)

					BEGIN_TRANSITION_ITEM(1, 10)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 11)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JRoot");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 4)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 7)

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 9)

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(5, 10)

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Create, 0, L"Object", L"", L"JObject");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 13)

					BEGIN_TRANSITION_ITEM(13, 12)
					ITEM_INSTRUCTION(Shift, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 1)

					BEGIN_TRANSITION_ITEM(1, 13)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 11)
					ITEM_STACK_PATTERN(26)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 26, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JRoot");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 4)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 9)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 7)

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 5)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 9)

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(9, 7)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 10)

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(14)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 6)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(16)
					ITEM_STACK_PATTERN(6)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"ObjectField", L"", L"JField");
					ITEM_INSTRUCTION(Reduce, 6, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"fields", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(15)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_STACK_PATTERN(8)
					ITEM_INSTRUCTION(Create, 0, L"Array", L"", L"JArray");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"JValue");
					ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"items", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 3)

					BEGIN_TRANSITION_ITEM(3, 2)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"True", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 4)

					BEGIN_TRANSITION_ITEM(4, 2)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"False", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 5)

					BEGIN_TRANSITION_ITEM(5, 2)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"Null", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 12)

					BEGIN_TRANSITION_ITEM(12, 2)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Number", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 13)

					BEGIN_TRANSITION_ITEM(13, 2)
					ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"String", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 11)

					BEGIN_TRANSITION_ITEM(11, 16)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 7)

					BEGIN_TRANSITION_ITEM(7, 5)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 13)

					BEGIN_TRANSITION_ITEM(13, 12)
					ITEM_INSTRUCTION(Shift, 14, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 3)

					BEGIN_TRANSITION_ITEM(3, 2)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"True", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 4)

					BEGIN_TRANSITION_ITEM(4, 2)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"False", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 5)

					BEGIN_TRANSITION_ITEM(5, 2)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"Null", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 9)

					BEGIN_TRANSITION_ITEM(9, 7)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 12)

					BEGIN_TRANSITION_ITEM(12, 2)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Number", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(15, 13)

					BEGIN_TRANSITION_ITEM(13, 2)
					ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"String", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 3)

					BEGIN_TRANSITION_ITEM(3, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"True", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 4)

					BEGIN_TRANSITION_ITEM(4, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"False", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 5)

					BEGIN_TRANSITION_ITEM(5, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"Null", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 12)

					BEGIN_TRANSITION_ITEM(12, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Number", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(16, 13)

					BEGIN_TRANSITION_ITEM(13, 2)
					ITEM_INSTRUCTION(Shift, 16, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"String", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(17, 0)

					BEGIN_TRANSITION_ITEM(0, 18)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(18, 13)

					BEGIN_TRANSITION_ITEM(13, 12)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(19, 0)

					BEGIN_TRANSITION_ITEM(0, 20)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(20, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(21, 0)

					BEGIN_TRANSITION_ITEM(0, 22)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(23, 0)

					BEGIN_TRANSITION_ITEM(0, 24)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 3)

					BEGIN_TRANSITION_ITEM(3, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"True", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 4)

					BEGIN_TRANSITION_ITEM(4, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"False", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 5)

					BEGIN_TRANSITION_ITEM(5, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Setter, 0, L"value", L"Null", L"");
					ITEM_INSTRUCTION(Create, 0, L"Literal", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 12)

					BEGIN_TRANSITION_ITEM(12, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Number", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 13)

					BEGIN_TRANSITION_ITEM(13, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"String", L"", L"JLiteral");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 0)

					BEGIN_TRANSITION_ITEM(0, 26)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 6)

					BEGIN_TRANSITION_ITEM(6, 14)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 8)

					BEGIN_TRANSITION_ITEM(8, 15)
					ITEM_INSTRUCTION(Shift, 26, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				table->Initialize();
				return table;

				#undef SET_TOKEN_INFO
				#undef SET_DISCARD_TOKEN_INFO
				#undef SET_STATE_INFO
				#undef SET_RULE_INFO
				#undef SET_AMBIGUOUS_RULE_INFO
				#undef BEGIN_TRANSITION_BAG
				#undef BEGIN_TRANSITION_ITEM
				#undef END_TRANSITION_ITEM
				#undef END_TRANSITION_BAG
				#undef ITEM_STACK_PATTERN
				#undef ITEM_INSTRUCTION
				#undef BEGIN_LOOK_AHEAD
				#undef LOOK_AHEAD
				#undef END_LOOK_AHEAD
			}

		}
	}
}
