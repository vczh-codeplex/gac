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
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"CData")
					{
						vl::Ptr<XmlCData> tree = new XmlCData;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Attribute")
					{
						vl::Ptr<XmlAttribute> tree = new XmlAttribute;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Comment")
					{
						vl::Ptr<XmlComment> tree = new XmlComment;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Element")
					{
						vl::Ptr<XmlElement> tree = new XmlElement;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Instruction")
					{
						vl::Ptr<XmlInstruction> tree = new XmlInstruction;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Document")
					{
						vl::Ptr<XmlDocument> tree = new XmlDocument;
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

			vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XDocument");
				vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
				vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
				vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
				if(node)
				{
					return XmlConvertParsingTreeNode(node, state.GetTokens()).Cast<XmlDocument>();
				}
				return 0;
			}

			vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
			{
				vl::parsing::tabling::ParsingState state(input, table);
				state.Reset(L"XElement");
				vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
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
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(15-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 42, 8);
				table->SetTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(1, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(2, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
				table->SetTokenInfo(3, vl::parsing::tabling::ParsingTable::TokenInfo(L"INSTRUCTION_OPEN", L"/</?"));
				table->SetTokenInfo(4, vl::parsing::tabling::ParsingTable::TokenInfo(L"INSTRUCTION_CLOSE", L"/?/>"));
				table->SetTokenInfo(5, vl::parsing::tabling::ParsingTable::TokenInfo(L"COMPLEX_ELEMENT_OPEN", L"/<//"));
				table->SetTokenInfo(6, vl::parsing::tabling::ParsingTable::TokenInfo(L"SINGLE_ELEMENT_CLOSE", L"///>"));
				table->SetTokenInfo(7, vl::parsing::tabling::ParsingTable::TokenInfo(L"ELEMENT_OPEN", L"/<"));
				table->SetTokenInfo(8, vl::parsing::tabling::ParsingTable::TokenInfo(L"ELEMENT_CLOSE", L"/>"));
				table->SetTokenInfo(9, vl::parsing::tabling::ParsingTable::TokenInfo(L"EQUAL", L"/="));
				table->SetTokenInfo(10, vl::parsing::tabling::ParsingTable::TokenInfo(L"NAME", L"[a-zA-Z0-9:_/-]+"));
				table->SetTokenInfo(11, vl::parsing::tabling::ParsingTable::TokenInfo(L"ATTVALUE", L"\"[^<>\"]*\"|\'[^<>\']*\'"));
				table->SetTokenInfo(12, vl::parsing::tabling::ParsingTable::TokenInfo(L"COMMENT", L"/</!--([^/->]|-[^/->]|--[^>])*--/>"));
				table->SetTokenInfo(13, vl::parsing::tabling::ParsingTable::TokenInfo(L"CDATA", L"/</!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>"));
				table->SetTokenInfo(14, vl::parsing::tabling::ParsingTable::TokenInfo(L"TEXT", L"([^<>=\" /r/n/ta-zA-Z0-9:_/-])+|\"|\'"));
				table->SetDiscardTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"SPACE", L"/s+"));
				table->SetStateInfo(0, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.RootStart", L"¡ñ $<XAttribute>"));
				table->SetStateInfo(1, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.Start", L"¡¤ <XAttribute>"));
				table->SetStateInfo(2, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.1", L"<XAttribute>: NAME : name¡ñ \"=\" ATTVALUE : value as Attribute"));
				table->SetStateInfo(3, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.2", L"<XAttribute>: NAME : name \"=\"¡ñ ATTVALUE : value as Attribute"));
				table->SetStateInfo(4, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.3", L"<XAttribute>: NAME : name \"=\" ATTVALUE : value as Attribute¡ñ"));
				table->SetStateInfo(5, vl::parsing::tabling::ParsingTable::StateInfo(L"XAttribute", L"XAttribute.RootEnd", L"$<XAttribute> ¡ñ"));
				table->SetStateInfo(6, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.3", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element¡ñ"));
				table->SetStateInfo(7, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.4", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\"¡ñ { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" ¡ñ{ XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element"));
				table->SetStateInfo(8, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.3", L"<XInstruction>: \"<?\" NAME : name { XAttribute : attributes } \"?>\" as Instruction¡ñ"));
				table->SetStateInfo(9, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.RootEnd", L"$<XElement> ¡ñ"));
				table->SetStateInfo(10, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.5", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\"¡ñ NAME : closingName \">\" ) as Element"));
				table->SetStateInfo(11, vl::parsing::tabling::ParsingTable::StateInfo(L"XText", L"XText.1", L"<XText>: NAME : content | EQUAL : content | ATTVALUE : content | TEXT : content as Text¡ñ"));
				table->SetStateInfo(12, vl::parsing::tabling::ParsingTable::StateInfo(L"XCData", L"XCData.1", L"<XCData>: CDATA : content as CData¡ñ"));
				table->SetStateInfo(13, vl::parsing::tabling::ParsingTable::StateInfo(L"XComment", L"XComment.1", L"<XComment>: COMMENT : content as Comment¡ñ"));
				table->SetStateInfo(14, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.1", L"<XElement>: \"<\"¡ñ NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element"));
				table->SetStateInfo(15, vl::parsing::tabling::ParsingTable::StateInfo(L"XSubNode", L"XSubNode.RootEnd", L"$<XSubNode> ¡ñ"));
				table->SetStateInfo(16, vl::parsing::tabling::ParsingTable::StateInfo(L"XDocument", L"XDocument.RootEnd", L"$<XDocument> ¡ñ"));
				table->SetStateInfo(17, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.RootEnd", L"$<XInstruction> ¡ñ"));
				table->SetStateInfo(18, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.1", L"<XInstruction>: \"<?\"¡ñ NAME : name { XAttribute : attributes } \"?>\" as Instruction"));
				table->SetStateInfo(19, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.6", L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName¡ñ \">\" ) as Element"));
				table->SetStateInfo(20, vl::parsing::tabling::ParsingTable::StateInfo(L"XText", L"XText.RootEnd", L"$<XText> ¡ñ"));
				table->SetStateInfo(21, vl::parsing::tabling::ParsingTable::StateInfo(L"XCData", L"XCData.RootEnd", L"$<XCData> ¡ñ"));
				table->SetStateInfo(22, vl::parsing::tabling::ParsingTable::StateInfo(L"XComment", L"XComment.RootEnd", L"$<XComment> ¡ñ"));
				table->SetStateInfo(23, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.2", L"<XElement>: \"<\" NAME : name¡ñ { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name ¡ñ{ XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element"));
				table->SetStateInfo(24, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.2", L"<XInstruction>: \"<?\" NAME : name¡ñ { XAttribute : attributes } \"?>\" as Instruction\r\n<XInstruction>: \"<?\" NAME : name ¡ñ{ XAttribute : attributes } \"?>\" as Instruction"));
				table->SetStateInfo(25, vl::parsing::tabling::ParsingTable::StateInfo(L"XText", L"XText.RootStart", L"¡ñ $<XText>"));
				table->SetStateInfo(26, vl::parsing::tabling::ParsingTable::StateInfo(L"XText", L"XText.Start", L"¡¤ <XText>"));
				table->SetStateInfo(27, vl::parsing::tabling::ParsingTable::StateInfo(L"XCData", L"XCData.RootStart", L"¡ñ $<XCData>"));
				table->SetStateInfo(28, vl::parsing::tabling::ParsingTable::StateInfo(L"XCData", L"XCData.Start", L"¡¤ <XCData>"));
				table->SetStateInfo(29, vl::parsing::tabling::ParsingTable::StateInfo(L"XComment", L"XComment.RootStart", L"¡ñ $<XComment>"));
				table->SetStateInfo(30, vl::parsing::tabling::ParsingTable::StateInfo(L"XComment", L"XComment.Start", L"¡¤ <XComment>"));
				table->SetStateInfo(31, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.RootStart", L"¡ñ $<XElement>"));
				table->SetStateInfo(32, vl::parsing::tabling::ParsingTable::StateInfo(L"XElement", L"XElement.Start", L"¡¤ <XElement>"));
				table->SetStateInfo(33, vl::parsing::tabling::ParsingTable::StateInfo(L"XSubNode", L"XSubNode.RootStart", L"¡ñ $<XSubNode>"));
				table->SetStateInfo(34, vl::parsing::tabling::ParsingTable::StateInfo(L"XSubNode", L"XSubNode.Start", L"¡¤ <XSubNode>"));
				table->SetStateInfo(35, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.RootStart", L"¡ñ $<XInstruction>"));
				table->SetStateInfo(36, vl::parsing::tabling::ParsingTable::StateInfo(L"XInstruction", L"XInstruction.Start", L"¡¤ <XInstruction>"));
				table->SetStateInfo(37, vl::parsing::tabling::ParsingTable::StateInfo(L"XDocument", L"XDocument.RootStart", L"¡ñ $<XDocument>"));
				table->SetStateInfo(38, vl::parsing::tabling::ParsingTable::StateInfo(L"XDocument", L"XDocument.Start", L"¡¤ <XDocument>"));
				table->SetStateInfo(39, vl::parsing::tabling::ParsingTable::StateInfo(L"XSubNode", L"XSubNode.1", L"<XSubNode>: !XText | !XCData | !XComment | !XElement¡ñ"));
				table->SetStateInfo(40, vl::parsing::tabling::ParsingTable::StateInfo(L"XDocument", L"XDocument.1", L"<XDocument>: { XInstruction : prologs | XComment : prologs } XElement : rootElement as Document¡ñ"));
				table->SetStateInfo(41, vl::parsing::tabling::ParsingTable::StateInfo(L"XDocument", L"XDocument.2", L"<XDocument>: ¡ñ{ XInstruction : prologs | XComment : prologs } XElement : rootElement as Document"));
				table->SetRuleInfo(0, vl::parsing::tabling::ParsingTable::RuleInfo(L"XAttribute", L"Attribute", 0));
				table->SetRuleInfo(1, vl::parsing::tabling::ParsingTable::RuleInfo(L"XText", L"Text", 25));
				table->SetRuleInfo(2, vl::parsing::tabling::ParsingTable::RuleInfo(L"XCData", L"CData", 27));
				table->SetRuleInfo(3, vl::parsing::tabling::ParsingTable::RuleInfo(L"XComment", L"Comment", 29));
				table->SetRuleInfo(4, vl::parsing::tabling::ParsingTable::RuleInfo(L"XElement", L"Element", 31));
				table->SetRuleInfo(5, vl::parsing::tabling::ParsingTable::RuleInfo(L"XSubNode", L"Node", 33));
				table->SetRuleInfo(6, vl::parsing::tabling::ParsingTable::RuleInfo(L"XInstruction", L"Instruction", 35));
				table->SetRuleInfo(7, vl::parsing::tabling::ParsingTable::RuleInfo(L"XDocument", L"Document", 37));
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
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(1, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 3);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(2, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 4);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Attribute", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(3, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 5);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 8);
						item->stackPattern.Add(24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"attributes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Instruction", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 6);
						item->stackPattern.Add(23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"attributes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Element", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 7);
						item->stackPattern.Add(23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"attributes", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 2);
						item->stackPattern.Add(24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"attributes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 2);
						item->stackPattern.Add(23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"attributes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(4, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 9);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 16);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Document", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"rootElement", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 16);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"rootElement", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Document", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 15);
						item->stackPattern.Add(34);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(6, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(7, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 17);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(8, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 19);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"closingName", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(10, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 20);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 15);
						item->stackPattern.Add(34);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(11, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 21);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 15);
						item->stackPattern.Add(34);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(12, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 22);
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 15);
						item->stackPattern.Add(34);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 1, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 5, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(41);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(38);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"prologs", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 41, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->stackPattern.Add(34);
						item->stackPattern.Add(7);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"subNodes", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 7, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(13, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 23);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(14, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 24);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(18, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Element", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(19, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 6);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Element", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(23, 6, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 7);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(23, 8, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(23, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 8);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Instruction", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 4, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 2);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 24, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"name", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(24, 10, bag);
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
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(26, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 28);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(27, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(28, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 30);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(29, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(30, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 32);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(31, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(32, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 34);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(33, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 9, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 10, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 11, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 12, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(13, 12);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"CData", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 13, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(14, 11);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 34, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Text", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(34, 14, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 36);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(35, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(36, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 38);
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(37, 0, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 18);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 38, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 3, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 14);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 38, L"", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 7, bag);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
					{
						vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(12, 13);
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 38, L"", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"content", L""));
						item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"Comment", L""));
						bag->transitionItems.Add(item);
					}
					table->SetTransitionBag(38, 12, bag);
				}
				table->Initialize();
				return table;
			}

		}
	}
}
