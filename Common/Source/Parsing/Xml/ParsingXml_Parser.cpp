#include "ParsingXml_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeCData(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeComment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class XmlTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				void Fill(vl::Ptr<XmlNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlText> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->content, obj->GetMember(L"content"), tokens);
				}

				void Fill(vl::Ptr<XmlCData> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeCData(tree->content, tokens);
					}
				}

				void Fill(vl::Ptr<XmlAttribute> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					if(SetMember(tree->value, obj->GetMember(L"value"), tokens))
					{
						XmlUnescapeAttributeValue(tree->value, tokens);
					}
				}

				void Fill(vl::Ptr<XmlComment> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeComment(tree->content, tokens);
					}
				}

				void Fill(vl::Ptr<XmlElement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					SetMember(tree->closingName, obj->GetMember(L"closingName"), tokens);
					SetMember(tree->attributes, obj->GetMember(L"attributes"), tokens);
					if(SetMember(tree->subNodes, obj->GetMember(L"subNodes"), tokens))
					{
						XmlMergeTextFragment(tree->subNodes, tokens);
					}
				}

				void Fill(vl::Ptr<XmlInstruction> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->name, obj->GetMember(L"name"), tokens);
					SetMember(tree->attributes, obj->GetMember(L"attributes"), tokens);
				}

				void Fill(vl::Ptr<XmlDocument> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->prologs, obj->GetMember(L"prologs"), tokens);
					SetMember(tree->rootElement, obj->GetMember(L"rootElement"), tokens);
				}

				vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"Text")
					{
						vl::Ptr<XmlText> tree = new XmlText;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"CData")
					{
						vl::Ptr<XmlCData> tree = new XmlCData;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Attribute")
					{
						vl::Ptr<XmlAttribute> tree = new XmlAttribute;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Comment")
					{
						vl::Ptr<XmlComment> tree = new XmlComment;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Element")
					{
						vl::Ptr<XmlElement> tree = new XmlElement;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Instruction")
					{
						vl::Ptr<XmlInstruction> tree = new XmlInstruction;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Document")
					{
						vl::Ptr<XmlDocument> tree = new XmlDocument;
						vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else 
						return 0;
				}
			};

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				XmlTreeConverter converter;
				vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
				converter.SetMember(tree, node, tokens);
				return tree;
			}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

			vl::Ptr<XmlText> XmlText::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlText>();
			}

			vl::Ptr<XmlCData> XmlCData::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlCData>();
			}

			vl::Ptr<XmlAttribute> XmlAttribute::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAttribute>();
			}

			vl::Ptr<XmlComment> XmlComment::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlComment>();
			}

			vl::Ptr<XmlElement> XmlElement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlElement>();
			}

			vl::Ptr<XmlInstruction> XmlInstruction::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlInstruction>();
			}

			vl::Ptr<XmlDocument> XmlDocument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlDocument>();
			}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

			void XmlText::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlCData::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlAttribute::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlComment::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlElement::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlInstruction::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlDocument::Accept(XmlNode::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
Parser Function
***********************************************************************/

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XDocument");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XDocument");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node)
				{
					return XmlConvertParsingTreeNode(node, state.GetTokens()).Cast<XmlDocument>();
				}
				return 0;
			}

			vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XElement");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				return node;
			}

			vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XElement");
				vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node)
				{
					return XmlConvertParsingTreeNode(node, state.GetTokens()).Cast<XmlElement>();
				}
				return 0;
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> XmlLoadTable()
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(15-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 49, 8);
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
				#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
				#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);
				#define END_LOOK_AHEAD }

				SET_TOKEN_INFO(0, L"", L"")
				SET_TOKEN_INFO(1, L"", L"")
				SET_TOKEN_INFO(2, L"", L"")
				SET_TOKEN_INFO(3, L"INSTRUCTION_OPEN", L"/</?")
				SET_TOKEN_INFO(4, L"INSTRUCTION_CLOSE", L"/?/>")
				SET_TOKEN_INFO(5, L"COMPLEX_ELEMENT_OPEN", L"/<//")
				SET_TOKEN_INFO(6, L"SINGLE_ELEMENT_CLOSE", L"///>")
				SET_TOKEN_INFO(7, L"ELEMENT_OPEN", L"/<")
				SET_TOKEN_INFO(8, L"ELEMENT_CLOSE", L"/>")
				SET_TOKEN_INFO(9, L"EQUAL", L"/=")
				SET_TOKEN_INFO(10, L"NAME", L"[a-zA-Z0-9:._/-]+")
				SET_TOKEN_INFO(11, L"ATTVALUE", L"\"[^<>\"]*\"|\'[^<>\']*\'")
				SET_TOKEN_INFO(12, L"COMMENT", L"/</!--([^/->]|-[^/->]|--[^>])*--/>")
				SET_TOKEN_INFO(13, L"CDATA", L"/</!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>")
				SET_TOKEN_INFO(14, L"TEXT", L"([^<>=\"\' /r/n/ta-zA-Z0-9:._/-])+|\"|\'")

				SET_DISCARD_TOKEN_INFO(0, L"SPACE", L"/s+")

				SET_STATE_INFO(0, L"XAttribute", L"XAttribute.RootStart", L"¡ñ $<XAttribute>")
				SET_STATE_INFO(1, L"XAttribute", L"XAttribute.Start", L"¡¤ <XAttribute>")
				SET_STATE_INFO(2, L"XAttribute", L"XAttribute.1", L"<XAttribute>: NAME : name¡ñ \"=\" ATTVALUE : value as Attribute")
				SET_STATE_INFO(3, L"XAttribute", L"XAttribute.2", L"<XAttribute>: NAME : name \"=\"¡ñ ATTVALUE : value as Attribute")
				SET_STATE_INFO(4, L"XAttribute", L"XAttribute.3", L"<XAttribute>: NAME : name \"=\" ATTVALUE : value as Attribute¡ñ")
				SET_STATE_INFO(5, L"XAttribute", L"XAttribute.RootEnd", L"$<XAttribute> ¡ñ")
				SET_STATE_INFO(6, L"XElement", L"XElement.3", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element¡ñ")
				SET_STATE_INFO(7, L"XElement", L"XElement.4", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\"¡ñ { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" ¡ñ{ XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element")
				SET_STATE_INFO(8, L"XInstruction", L"XInstruction.3", L"<XInstruction>: \"<?\" NAME : name { XAttribute : attributes } \"?>\" as Instruction¡ñ")
				SET_STATE_INFO(9, L"XElement", L"XElement.RootEnd", L"$<XElement> ¡ñ")
				SET_STATE_INFO(10, L"XText", L"XText.1", L"<XText>: NAME : content | EQUAL : content | ATTVALUE : content | TEXT : content as Text¡ñ")
				SET_STATE_INFO(11, L"XCData", L"XCData.1", L"<XCData>: CDATA : content as CData¡ñ")
				SET_STATE_INFO(12, L"XComment", L"XComment.1", L"<XComment>: COMMENT : content as Comment¡ñ")
				SET_STATE_INFO(13, L"XElement", L"XElement.1", L"<XElement>: \"<\"¡ñ NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element")
				SET_STATE_INFO(14, L"XElement", L"XElement.6", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\"¡ñ NAME : closingName \">\" ) as Element")
				SET_STATE_INFO(15, L"XSubNode", L"XSubNode.RootEnd", L"$<XSubNode> ¡ñ")
				SET_STATE_INFO(16, L"XDocument", L"XDocument.RootEnd", L"$<XDocument> ¡ñ")
				SET_STATE_INFO(17, L"XInstruction", L"XInstruction.RootEnd", L"$<XInstruction> ¡ñ")
				SET_STATE_INFO(18, L"XInstruction", L"XInstruction.1", L"<XInstruction>: \"<?\"¡ñ NAME : name { XAttribute : attributes } \"?>\" as Instruction")
				SET_STATE_INFO(19, L"XText", L"XText.RootEnd", L"$<XText> ¡ñ")
				SET_STATE_INFO(20, L"XCData", L"XCData.RootEnd", L"$<XCData> ¡ñ")
				SET_STATE_INFO(21, L"XComment", L"XComment.RootEnd", L"$<XComment> ¡ñ")
				SET_STATE_INFO(22, L"XElement", L"XElement.2", L"<XElement>: \"<\" NAME : name¡ñ { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name ¡ñ{ XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element")
				SET_STATE_INFO(23, L"XElement", L"XElement.7", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName¡ñ \">\" ) as Element")
				SET_STATE_INFO(24, L"XInstruction", L"XInstruction.2", L"<XInstruction>: \"<?\" NAME : name¡ñ { XAttribute : attributes } \"?>\" as Instruction\r\n<XInstruction>: \"<?\" NAME : name ¡ñ{ XAttribute : attributes } \"?>\" as Instruction")
				SET_STATE_INFO(25, L"XText", L"XText.RootStart", L"¡ñ $<XText>")
				SET_STATE_INFO(26, L"XText", L"XText.Start", L"¡¤ <XText>")
				SET_STATE_INFO(27, L"XCData", L"XCData.RootStart", L"¡ñ $<XCData>")
				SET_STATE_INFO(28, L"XCData", L"XCData.Start", L"¡¤ <XCData>")
				SET_STATE_INFO(29, L"XComment", L"XComment.RootStart", L"¡ñ $<XComment>")
				SET_STATE_INFO(30, L"XComment", L"XComment.Start", L"¡¤ <XComment>")
				SET_STATE_INFO(31, L"XElement", L"XElement.RootStart", L"¡ñ $<XElement>")
				SET_STATE_INFO(32, L"XElement", L"XElement.Start", L"¡¤ <XElement>")
				SET_STATE_INFO(33, L"XSubNode", L"XSubNode.RootStart", L"¡ñ $<XSubNode>")
				SET_STATE_INFO(34, L"XSubNode", L"XSubNode.Start", L"¡¤ <XSubNode>")
				SET_STATE_INFO(35, L"XInstruction", L"XInstruction.RootStart", L"¡ñ $<XInstruction>")
				SET_STATE_INFO(36, L"XInstruction", L"XInstruction.Start", L"¡¤ <XInstruction>")
				SET_STATE_INFO(37, L"XDocument", L"XDocument.RootStart", L"¡ñ $<XDocument>")
				SET_STATE_INFO(38, L"XDocument", L"XDocument.Start", L"¡¤ <XDocument>")
				SET_STATE_INFO(39, L"XAttribute", L"XAttribute.4", L"<XAttribute>: NAME : name \"=\" ATTVALUE : value as Attribute¡ñ")
				SET_STATE_INFO(40, L"XText", L"XText.2", L"<XText>: NAME : content | EQUAL : content | ATTVALUE : content | TEXT : content as Text¡ñ")
				SET_STATE_INFO(41, L"XCData", L"XCData.2", L"<XCData>: CDATA : content as CData¡ñ")
				SET_STATE_INFO(42, L"XComment", L"XComment.2", L"<XComment>: COMMENT : content as Comment¡ñ")
				SET_STATE_INFO(43, L"XElement", L"XElement.5", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element¡ñ")
				SET_STATE_INFO(44, L"XSubNode", L"XSubNode.1", L"<XSubNode>: !XText | !XCData | !XComment | !XElement¡ñ")
				SET_STATE_INFO(45, L"XInstruction", L"XInstruction.4", L"<XInstruction>: \"<?\" NAME : name { XAttribute : attributes } \"?>\" as Instruction¡ñ")
				SET_STATE_INFO(46, L"XDocument", L"XDocument.1", L"<XDocument>: { XInstruction : prologs | XComment : prologs } XElement : rootElement as Document¡ñ")
				SET_STATE_INFO(47, L"XDocument", L"XDocument.2", L"<XDocument>: ¡ñ{ XInstruction : prologs | XComment : prologs } XElement : rootElement as Document")
				SET_STATE_INFO(48, L"XDocument", L"XDocument.3", L"<XDocument>: { XInstruction : prologs | XComment : prologs } XElement : rootElement as Document¡ñ")

				SET_RULE_INFO(0, L"XAttribute", L"Attribute", 0)
				SET_RULE_INFO(1, L"XText", L"Text", 25)
				SET_RULE_INFO(2, L"XCData", L"CData", 27)
				SET_RULE_INFO(3, L"XComment", L"Comment", 29)
				SET_RULE_INFO(4, L"XElement", L"Element", 31)
				SET_RULE_INFO(5, L"XSubNode", L"Node", 33)
				SET_RULE_INFO(6, L"XInstruction", L"Instruction", 35)
				SET_RULE_INFO(7, L"XDocument", L"Document", 37)

				BEGIN_TRANSITION_BAG(0, 0)

					BEGIN_TRANSITION_ITEM(0, 1)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(1, 10)

					BEGIN_TRANSITION_ITEM(10, 2)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(2, 9)

					BEGIN_TRANSITION_ITEM(9, 3)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(3, 11)

					BEGIN_TRANSITION_ITEM(11, 4)
					ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 1)

					BEGIN_TRANSITION_ITEM(1, 5)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 4)

					BEGIN_TRANSITION_ITEM(4, 8)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 6)

					BEGIN_TRANSITION_ITEM(6, 6)
					ITEM_STACK_PATTERN(22)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					ITEM_INSTRUCTION(Reduce, 22, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 8)

					BEGIN_TRANSITION_ITEM(8, 7)
					ITEM_STACK_PATTERN(22)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					ITEM_INSTRUCTION(Reduce, 22, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(4, 10)

					BEGIN_TRANSITION_ITEM(10, 2)
					ITEM_STACK_PATTERN(24)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					ITEM_INSTRUCTION(Reduce, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(10, 2)
					ITEM_STACK_PATTERN(22)
					ITEM_INSTRUCTION(Create, 0, L"Attribute", L"", L"XAttribute");
					ITEM_INSTRUCTION(Reduce, 22, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"attributes", L"", L"");
					ITEM_INSTRUCTION(Shift, 22, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 1)

					BEGIN_TRANSITION_ITEM(1, 9)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 16)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"rootElement", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Document", L"", L"XDocument");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 16)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"rootElement", L"", L"");
					ITEM_INSTRUCTION(Create, 0, L"Document", L"", L"XDocument");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(34)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 5)

					BEGIN_TRANSITION_ITEM(5, 14)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(6, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Element", L"", L"XElement");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 5)

					BEGIN_TRANSITION_ITEM(5, 14)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(7, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 1)

					BEGIN_TRANSITION_ITEM(1, 17)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 3)

					BEGIN_TRANSITION_ITEM(3, 18)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 18)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(8, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Instruction", L"", L"XInstruction");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 1)

					BEGIN_TRANSITION_ITEM(1, 19)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(34)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 5)

					BEGIN_TRANSITION_ITEM(5, 14)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(10, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Text", L"", L"XText");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 1)

					BEGIN_TRANSITION_ITEM(1, 20)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(34)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 5)

					BEGIN_TRANSITION_ITEM(5, 14)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(11, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"CData", L"", L"XCData");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 1)

					BEGIN_TRANSITION_ITEM(1, 21)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(1, 15)
					ITEM_STACK_PATTERN(34)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 3)

					BEGIN_TRANSITION_ITEM(3, 18)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(3, 18)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 5)

					BEGIN_TRANSITION_ITEM(5, 14)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(47)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(38)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"prologs", L"", L"");
					ITEM_INSTRUCTION(Shift, 47, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(12, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_STACK_PATTERN(34)
					ITEM_STACK_PATTERN(7)
					ITEM_INSTRUCTION(Create, 0, L"Comment", L"", L"XComment");
					ITEM_INSTRUCTION(Reduce, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Using, 0, L"", L"", L"XSubNode");
					ITEM_INSTRUCTION(Reduce, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Item, 0, L"subNodes", L"", L"");
					ITEM_INSTRUCTION(Shift, 7, L"", L"", L"");
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(13, 10)

					BEGIN_TRANSITION_ITEM(10, 22)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(14, 10)

					BEGIN_TRANSITION_ITEM(10, 23)
					ITEM_INSTRUCTION(Assign, 0, L"closingName", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(18, 10)

					BEGIN_TRANSITION_ITEM(10, 24)
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 6)

					BEGIN_TRANSITION_ITEM(6, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 8)

					BEGIN_TRANSITION_ITEM(8, 7)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(22, 10)

					BEGIN_TRANSITION_ITEM(10, 2)
					ITEM_INSTRUCTION(Shift, 22, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(23, 8)

					BEGIN_TRANSITION_ITEM(8, 6)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 4)

					BEGIN_TRANSITION_ITEM(4, 8)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(24, 10)

					BEGIN_TRANSITION_ITEM(10, 2)
					ITEM_INSTRUCTION(Shift, 24, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"name", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(25, 0)

					BEGIN_TRANSITION_ITEM(0, 26)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(26, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(27, 0)

					BEGIN_TRANSITION_ITEM(0, 28)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(28, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(29, 0)

					BEGIN_TRANSITION_ITEM(0, 30)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(30, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(31, 0)

					BEGIN_TRANSITION_ITEM(0, 32)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(32, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(33, 0)

					BEGIN_TRANSITION_ITEM(0, 34)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 9)

					BEGIN_TRANSITION_ITEM(9, 10)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 10)

					BEGIN_TRANSITION_ITEM(10, 10)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 11)

					BEGIN_TRANSITION_ITEM(11, 10)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 13)

					BEGIN_TRANSITION_ITEM(13, 11)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(34, 14)

					BEGIN_TRANSITION_ITEM(14, 10)
					ITEM_INSTRUCTION(Shift, 34, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(35, 0)

					BEGIN_TRANSITION_ITEM(0, 36)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(36, 3)

					BEGIN_TRANSITION_ITEM(3, 18)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(37, 0)

					BEGIN_TRANSITION_ITEM(0, 38)
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 3)

					BEGIN_TRANSITION_ITEM(3, 18)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 7)

					BEGIN_TRANSITION_ITEM(7, 13)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					END_TRANSITION_ITEM

				END_TRANSITION_BAG

				BEGIN_TRANSITION_BAG(38, 12)

					BEGIN_TRANSITION_ITEM(12, 12)
					ITEM_INSTRUCTION(Shift, 38, L"", L"", L"");
					ITEM_INSTRUCTION(Assign, 0, L"content", L"", L"");
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
