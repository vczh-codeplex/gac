#include "ParsingJson_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace json
		{
/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void UnescapingString(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

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
						UnescapingString(tree->content, tokens);
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
						UnescapingString(tree->name, tokens);
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
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"String")
					{
						vl::Ptr<JsonString> tree = new JsonString;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Number")
					{
						vl::Ptr<JsonNumber> tree = new JsonNumber;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Array")
					{
						vl::Ptr<JsonArray> tree = new JsonArray;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"ObjectField")
					{
						vl::Ptr<JsonObjectField> tree = new JsonObjectField;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<JsonNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Object")
					{
						vl::Ptr<JsonObject> tree = new JsonObject;
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

			vl::Ptr<JsonNode> JsonParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"JRoot");
				vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
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
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(14-vl::parsing::tabling::ParsingTable::UserTokenStart, 0, 32, 6);
				table->SetTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(1, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(2, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(3, vl::parsing::tabling::ParsingTable::TokenInfo(L"TRUE", L"true"));
				table->SetTokenInfo(4, vl::parsing::tabling::ParsingTable::TokenInfo(L"FALSE", L"false"));
				table->SetTokenInfo(5, vl::parsing::tabling::ParsingTable::TokenInfo(L"NULL", L"null"));
				table->SetTokenInfo(6, vl::parsing::tabling::ParsingTable::TokenInfo(L"OBJOPEN", L"\\{"));
				table->SetTokenInfo(7, vl::parsing::tabling::ParsingTable::TokenInfo(L"OBJCLOSE", L"\\}"));
				table->SetTokenInfo(8, vl::parsing::tabling::ParsingTable::TokenInfo(L"ARROPEN", L"\\["));
				table->SetTokenInfo(9, vl::parsing::tabling::ParsingTable::TokenInfo(L"ARRCLOSE", L"\\]"));
				table->SetTokenInfo(10, vl::parsing::tabling::ParsingTable::TokenInfo(L"COMMA", L","));
				table->SetTokenInfo(11, vl::parsing::tabling::ParsingTable::TokenInfo(L"COLON", L":"));
				table->SetTokenInfo(12, vl::parsing::tabling::ParsingTable::TokenInfo(L"NUMBER", L"[\\-]?\\d+.\\d+([eE][+\\-]?\\d+)?"));
				table->SetTokenInfo(13, vl::parsing::tabling::ParsingTable::TokenInfo(L"STRING", L"\"([^\\\\\"]|\\\\[^u]|\\\\u\\d{4})*\""));
				table->SetStateInfo(0, vl::parsing::tabling::ParsingTable::StateInfo(L"JLiteral", L"JLiteral.RootStart", L"¡ñ $<JLiteral>"));
				table->SetStateInfo(1, vl::parsing::tabling::ParsingTable::StateInfo(L"JLiteral", L"JLiteral.Start", L"¡¤ <JLiteral>"));
				table->SetStateInfo(2, vl::parsing::tabling::ParsingTable::StateInfo(L"JLiteral", L"JLiteral.1", L"<JLiteral>: STRING : content as String¡ñ\r\n<JLiteral>: NUMBER : content as Number¡ñ\r\n<JLiteral>: \"true\" as Literal with { value = \"True\" }¡ñ\r\n<JLiteral>: \"false\" as Literal with { value = \"False\" }¡ñ\r\n<JLiteral>: \"null\" as Literal with { value = \"Null\" }¡ñ"));
				table->SetStateInfo(3, vl::parsing::tabling::ParsingTable::StateInfo(L"JLiteral", L"JLiteral.RootEnd", L"$<JLiteral> ¡ñ"));
				table->SetStateInfo(4, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.RootEnd", L"$<JField> ¡ñ"));
				table->SetStateInfo(5, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.3", L"<JObject>: \"{\" [ JField : fields { \",\" JField : fields } ] \"}\" as Object¡ñ"));
				table->SetStateInfo(6, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.4", L"<JObject>: \"{\" [ JField : fields { \",\"¡ñ JField : fields } ] \"}\" as Object"));
				table->SetStateInfo(7, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.3", L"<JArray>: \"[\" [ JValue : items { \",\" JValue : items } ] \"]\" as Array¡ñ"));
				table->SetStateInfo(8, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.4", L"<JArray>: \"[\" [ JValue : items { \",\"¡ñ JValue : items } ] \"]\" as Array"));
				table->SetStateInfo(9, vl::parsing::tabling::ParsingTable::StateInfo(L"JValue", L"JValue.RootEnd", L"$<JValue> ¡ñ"));
				table->SetStateInfo(10, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.RootEnd", L"$<JObject> ¡ñ"));
				table->SetStateInfo(11, vl::parsing::tabling::ParsingTable::StateInfo(L"JRoot", L"JRoot.RootEnd", L"$<JRoot> ¡ñ"));
				table->SetStateInfo(12, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.1", L"<JField>: STRING : name¡ñ \":\" JValue : value as ObjectField"));
				table->SetStateInfo(13, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.RootEnd", L"$<JArray> ¡ñ"));
				table->SetStateInfo(14, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.1", L"<JObject>: \"{\"¡ñ [ JField : fields { \",\" JField : fields } ] \"}\" as Object"));
				table->SetStateInfo(15, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.1", L"<JArray>: \"[\"¡ñ [ JValue : items { \",\" JValue : items } ] \"]\" as Array"));
				table->SetStateInfo(16, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.2", L"<JField>: STRING : name \":\"¡ñ JValue : value as ObjectField"));
				table->SetStateInfo(17, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.RootStart", L"¡ñ $<JField>"));
				table->SetStateInfo(18, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.Start", L"¡¤ <JField>"));
				table->SetStateInfo(19, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.RootStart", L"¡ñ $<JObject>"));
				table->SetStateInfo(20, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.Start", L"¡¤ <JObject>"));
				table->SetStateInfo(21, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.RootStart", L"¡ñ $<JArray>"));
				table->SetStateInfo(22, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.Start", L"¡¤ <JArray>"));
				table->SetStateInfo(23, vl::parsing::tabling::ParsingTable::StateInfo(L"JValue", L"JValue.RootStart", L"¡ñ $<JValue>"));
				table->SetStateInfo(24, vl::parsing::tabling::ParsingTable::StateInfo(L"JValue", L"JValue.Start", L"¡¤ <JValue>"));
				table->SetStateInfo(25, vl::parsing::tabling::ParsingTable::StateInfo(L"JRoot", L"JRoot.RootStart", L"¡ñ $<JRoot>"));
				table->SetStateInfo(26, vl::parsing::tabling::ParsingTable::StateInfo(L"JRoot", L"JRoot.Start", L"¡¤ <JRoot>"));
				table->SetStateInfo(27, vl::parsing::tabling::ParsingTable::StateInfo(L"JField", L"JField.3", L"<JField>: STRING : name \":\" JValue : value as ObjectField¡ñ"));
				table->SetStateInfo(28, vl::parsing::tabling::ParsingTable::StateInfo(L"JObject", L"JObject.2", L"<JObject>: \"{\" [ JField : fields¡ñ { \",\" JField : fields } ] \"}\" as Object\r\n<JObject>: \"{\" [ JField : fields ¡ñ{ \",\" JField : fields } ] \"}\" as Object"));
				table->SetStateInfo(29, vl::parsing::tabling::ParsingTable::StateInfo(L"JArray", L"JArray.2", L"<JArray>: \"[\" [ JValue : items¡ñ { \",\" JValue : items } ] \"]\" as Array\r\n<JArray>: \"[\" [ JValue : items ¡ñ{ \",\" JValue : items } ] \"]\" as Array"));
				table->SetStateInfo(30, vl::parsing::tabling::ParsingTable::StateInfo(L"JValue", L"JValue.1", L"<JValue>: !JLiteral¡ñ\r\n<JValue>: !JObject¡ñ\r\n<JValue>: !JArray¡ñ"));
				table->SetStateInfo(31, vl::parsing::tabling::ParsingTable::StateInfo(L"JRoot", L"JRoot.1", L"<JRoot>: !JObject¡ñ\r\n<JRoot>: !JArray¡ñ"));
				table->SetRuleInfo(0, vl::parsing::tabling::ParsingTable::RuleInfo(L"JLiteral", L"Node", 0));
				table->SetRuleInfo(1, vl::parsing::tabling::ParsingTable::RuleInfo(L"JField", L"ObjectField", 17));
				table->SetRuleInfo(2, vl::parsing::tabling::ParsingTable::RuleInfo(L"JObject", L"Object", 19));
				table->SetRuleInfo(3, vl::parsing::tabling::ParsingTable::RuleInfo(L"JArray", L"Array", 21));
				table->SetRuleInfo(4, vl::parsing::tabling::ParsingTable::RuleInfo(L"JValue", L"Node", 23));
				table->SetRuleInfo(5, vl::parsing::tabling::ParsingTable::RuleInfo(L"JRoot", L"Node", 25));
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 1);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(0, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"True"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"False"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"Null"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Number", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"String", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 3);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 9);
						item->stackPattern.Add(24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 10);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
						item->stackPattern.Add(26);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 26, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 9);
						item->stackPattern.Add(24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(5, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 13);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
						item->stackPattern.Add(26);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 26, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 9);
						item->stackPattern.Add(24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 6);
						item->stackPattern.Add(24);
						item->stackPattern.Add(16);
						item->stackPattern.Add(6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"ObjectField", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 6, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"fields", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 8);
						item->stackPattern.Add(24);
						item->stackPattern.Add(8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"items", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"True"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"False"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"Null"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Number", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"String", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 16);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Object", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(14, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 14, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(14, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"True"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"False"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"Null"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Array", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Number", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"String", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(15, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"True"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"False"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"Null"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Number", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 16, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"String", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(16, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 18);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(17, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 20);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(19, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(20, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 22);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(21, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(22, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 24);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(23, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"True"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"False"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Literal", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"value", L"Null"));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Number", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"String", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 26);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(25, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 26, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 15);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 26, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 8, bag);
				}
				table->Initialize();
				return table;
			}

		}
	}
}
