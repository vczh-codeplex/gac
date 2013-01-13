#include "Test.h"

namespace vczh
{
	namespace whoknows
	{
		namespace xmlutility
		{
/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeCData(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeComment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeTextFragment(vl::parsing::ParsingToken& value, const vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class XmlTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				bool SetMember(XmlDirectEnum::Type& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
				{
					vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
					if(token)
					{
						if(token->GetValue()==L"A") { member=XmlDirectEnum::A; return true; }
						else if(token->GetValue()==L"B") { member=XmlDirectEnum::B; return true; }
						else if(token->GetValue()==L"C") { member=XmlDirectEnum::C; return true; }
						else { member=XmlDirectEnum::A; return false; }
					}
					member=XmlDirectEnum::A;
					return false;
				}

				bool SetMember(XmlFirstClass::XmlIndirectEnum::Type& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
				{
					vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
					if(token)
					{
						if(token->GetValue()==L"D") { member=XmlFirstClass::XmlIndirectEnum::D; return true; }
						else if(token->GetValue()==L"E") { member=XmlFirstClass::XmlIndirectEnum::E; return true; }
						else if(token->GetValue()==L"F") { member=XmlFirstClass::XmlIndirectEnum::F; return true; }
						else { member=XmlFirstClass::XmlIndirectEnum::D; return false; }
					}
					member=XmlFirstClass::XmlIndirectEnum::D;
					return false;
				}

				void Fill(vl::Ptr<XmlNode> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlText> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					if(SetMember(tree->content, obj->GetMember(L"content"), tokens))
					{
						XmlUnescapeTextFragment(tree->content, tokens);
					}
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
					SetMember(tree->instructions, obj->GetMember(L"instructions"), tokens);
					SetMember(tree->comments, obj->GetMember(L"comments"), tokens);
					SetMember(tree->rootElement, obj->GetMember(L"rootElement"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field4, obj->GetMember(L"field4"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClass::XmlThirdClass> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field1, obj->GetMember(L"field1"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClass::XmlSecondClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field3, obj->GetMember(L"field3"), tokens);
				}

				void Fill(vl::Ptr<XmlFirstClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field2, obj->GetMember(L"field2"), tokens);
				}

				void Fill(vl::Ptr<XmlThirdClassChild> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
				{
					SetMember(tree->field5, obj->GetMember(L"field5"), tokens);
				}

				vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"Comment")
					{
						vl::Ptr<XmlComment> tree = new XmlComment;
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
					else if(obj->GetType()==L"Text")
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
					else if(obj->GetType()==L"FirstClass.SecondClassChild")
					{
						vl::Ptr<XmlFirstClass::XmlSecondClassChild> tree = new XmlFirstClass::XmlSecondClassChild;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass::XmlSecondClass>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"FirstClassChild")
					{
						vl::Ptr<XmlFirstClassChild> tree = new XmlFirstClassChild;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"Document")
					{
						vl::Ptr<XmlDocument> tree = new XmlDocument;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlNode>(), obj, tokens);
						return tree;
					}
					else if(obj->GetType()==L"ThirdClassChild")
					{
						vl::Ptr<XmlThirdClassChild> tree = new XmlThirdClassChild;
						Fill(tree, obj, tokens);
						Fill(tree.Cast<XmlFirstClass::XmlSecondClass::XmlThirdClass>(), obj, tokens);
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

			vl::Ptr<XmlComment> XmlComment::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlComment>();
			}

			vl::Ptr<XmlAttribute> XmlAttribute::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAttribute>();
			}

			vl::Ptr<XmlText> XmlText::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlText>();
			}

			vl::Ptr<XmlCData> XmlCData::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlCData>();
			}

			vl::Ptr<XmlFirstClass::XmlSecondClassChild> XmlFirstClass::XmlSecondClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlFirstClass::XmlSecondClassChild>();
			}

			vl::Ptr<XmlFirstClassChild> XmlFirstClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlFirstClassChild>();
			}

			vl::Ptr<XmlDocument> XmlDocument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlDocument>();
			}

			vl::Ptr<XmlThirdClassChild> XmlThirdClassChild::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlThirdClassChild>();
			}

			vl::Ptr<XmlElement> XmlElement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlElement>();
			}

			vl::Ptr<XmlInstruction> XmlInstruction::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlInstruction>();
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

			void XmlFirstClass::XmlSecondClassChild::Accept(XmlFirstClass::XmlSecondClass::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlFirstClassChild::Accept(XmlFirstClass::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void XmlThirdClassChild::Accept(XmlFirstClass::XmlSecondClass::XmlThirdClass::IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
Table Generation
***********************************************************************/

			vl::Ptr<vl::parsing::tabling::ParsingTable> XmlLoadTable()
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(15-vl::parsing::tablingParsingTable::UserTokenStart, 1, 42, 8);
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"";
					info.regex=L"";
					table->SetTokenInfo(0, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"";
					info.regex=L"";
					table->SetTokenInfo(1, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"";
					info.regex=L"";
					table->SetTokenInfo(2, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"INSTRUCTION_OPEN";
					info.regex=L"/</?";
					table->SetTokenInfo(3, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"INSTRUCTION_CLOSE";
					info.regex=L"/?/>";
					table->SetTokenInfo(4, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"COMPLEX_ELEMENT_OPEN";
					info.regex=L"/<//";
					table->SetTokenInfo(5, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"SINGLE_ELEMENT_CLOSE";
					info.regex=L"///>";
					table->SetTokenInfo(6, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"ELEMENT_OPEN";
					info.regex=L"/<";
					table->SetTokenInfo(7, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"ELEMENT_CLOSE";
					info.regex=L"/>";
					table->SetTokenInfo(8, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"EQUAL";
					info.regex=L"/=";
					table->SetTokenInfo(9, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"NAME";
					info.regex=L"[a-zA-Z0-9:_/-]+";
					table->SetTokenInfo(10, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"ATTVALUE";
					info.regex=L"\"([^\"&]|&/l+;)*\"";
					table->SetTokenInfo(11, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"COMMENT";
					info.regex=L"/</!--[^>]*/>";
					table->SetTokenInfo(12, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"CDATA";
					info.regex=L"/</!/[CDATA/[([^/]]|/][^/]]|/]/][^>])*/]/]/>";
					table->SetTokenInfo(13, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"TEXT";
					info.regex=L"([^<>\"& /r/n/ta-zA-Z0-9:_/-]|&/l+;)+";
					table->SetTokenInfo(14, info);
				}
				{
					vl::parsing::tabling::ParsingTable::TokenInfo info;
					info.name=L"SPACE";
					info.regex=L"/s+";
					table->SetDiscardTokenInfo(0, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XDocument";
					info.stateName=L"XDocument.RootStart";
					info.stateExpression=L"¡ñ $<XDocument>";
					table->SetStateInfo(0, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XDocument";
					info.stateName=L"XDocument.Start";
					info.stateExpression=L"¡¤ <XDocument>";
					table->SetStateInfo(1, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.1";
					info.stateExpression=L"<XElement>: \"<\"¡ñ NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element";
					table->SetStateInfo(2, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.1";
					info.stateExpression=L"<XInstruction>: \"<?\"¡ñ NAME : name { XAttribute : attributes } \"?>\" as Instruction";
					table->SetStateInfo(3, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XComment";
					info.stateName=L"XComment.1";
					info.stateExpression=L"<XComment>: COMMENT : content as Comment¡ñ";
					table->SetStateInfo(4, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.4";
					info.stateExpression=L"<XElement>: \"<\" NAME : name ¡ñ{ XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name¡ñ { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element";
					table->SetStateInfo(5, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.2";
					info.stateExpression=L"<XInstruction>: \"<?\" NAME : name¡ñ { XAttribute : attributes } \"?>\" as Instruction\r\n<XInstruction>: \"<?\" NAME : name ¡ñ{ XAttribute : attributes } \"?>\" as Instruction";
					table->SetStateInfo(6, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XComment";
					info.stateName=L"XComment.RootEnd";
					info.stateExpression=L"$<XComment> ¡ñ";
					table->SetStateInfo(7, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XText";
					info.stateName=L"XText.1";
					info.stateExpression=L"<XText>: NAME : content | EQUAL : content | TEXT : content as Text¡ñ";
					table->SetStateInfo(8, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XCData";
					info.stateName=L"XCData.1";
					info.stateExpression=L"<XCData>: CDATA : content as CData¡ñ";
					table->SetStateInfo(9, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.5";
					info.stateExpression=L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\"¡ñ NAME : closingName \">\" ) as Element";
					table->SetStateInfo(10, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XSubNode";
					info.stateName=L"XSubNode.RootEnd";
					info.stateExpression=L"$<XSubNode> ¡ñ";
					table->SetStateInfo(11, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.2";
					info.stateExpression=L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element¡ñ";
					table->SetStateInfo(12, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.3";
					info.stateExpression=L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\"¡ñ { XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element\r\n<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" ¡ñ{ XSubNode : subNodes } \"</\" NAME : closingName \">\" ) as Element";
					table->SetStateInfo(13, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.1";
					info.stateExpression=L"<XAttribute>: NAME : name¡ñ \"=\" ATTVALUE : value as Attribute";
					table->SetStateInfo(14, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.3";
					info.stateExpression=L"<XInstruction>: \"<?\" NAME : name { XAttribute : attributes } \"?>\" as Instruction¡ñ";
					table->SetStateInfo(15, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XText";
					info.stateName=L"XText.RootEnd";
					info.stateExpression=L"$<XText> ¡ñ";
					table->SetStateInfo(16, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XCData";
					info.stateName=L"XCData.RootEnd";
					info.stateExpression=L"$<XCData> ¡ñ";
					table->SetStateInfo(17, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.6";
					info.stateExpression=L"<XElement>: \"<\" NAME : name { XAttribute : attributes } ( \"/>\" | \">\" { XSubNode : subNodes } \"</\" NAME : closingName¡ñ \">\" ) as Element";
					table->SetStateInfo(18, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.RootEnd";
					info.stateExpression=L"$<XElement> ¡ñ";
					table->SetStateInfo(19, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XDocument";
					info.stateName=L"XDocument.RootEnd";
					info.stateExpression=L"$<XDocument> ¡ñ";
					table->SetStateInfo(20, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.2";
					info.stateExpression=L"<XAttribute>: NAME : name \"=\"¡ñ ATTVALUE : value as Attribute";
					table->SetStateInfo(21, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.RootEnd";
					info.stateExpression=L"$<XInstruction> ¡ñ";
					table->SetStateInfo(22, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.3";
					info.stateExpression=L"<XAttribute>: NAME : name \"=\" ATTVALUE : value as Attribute¡ñ";
					table->SetStateInfo(23, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.RootEnd";
					info.stateExpression=L"$<XAttribute> ¡ñ";
					table->SetStateInfo(24, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.RootStart";
					info.stateExpression=L"¡ñ $<XElement>";
					table->SetStateInfo(25, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XElement";
					info.stateName=L"XElement.Start";
					info.stateExpression=L"¡¤ <XElement>";
					table->SetStateInfo(26, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XCData";
					info.stateName=L"XCData.RootStart";
					info.stateExpression=L"¡ñ $<XCData>";
					table->SetStateInfo(27, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XCData";
					info.stateName=L"XCData.Start";
					info.stateExpression=L"¡¤ <XCData>";
					table->SetStateInfo(28, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.RootStart";
					info.stateExpression=L"¡ñ $<XAttribute>";
					table->SetStateInfo(29, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XAttribute";
					info.stateName=L"XAttribute.Start";
					info.stateExpression=L"¡¤ <XAttribute>";
					table->SetStateInfo(30, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XComment";
					info.stateName=L"XComment.RootStart";
					info.stateExpression=L"¡ñ $<XComment>";
					table->SetStateInfo(31, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XComment";
					info.stateName=L"XComment.Start";
					info.stateExpression=L"¡¤ <XComment>";
					table->SetStateInfo(32, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.RootStart";
					info.stateExpression=L"¡ñ $<XInstruction>";
					table->SetStateInfo(33, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XInstruction";
					info.stateName=L"XInstruction.Start";
					info.stateExpression=L"¡¤ <XInstruction>";
					table->SetStateInfo(34, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XText";
					info.stateName=L"XText.RootStart";
					info.stateExpression=L"¡ñ $<XText>";
					table->SetStateInfo(35, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XText";
					info.stateName=L"XText.Start";
					info.stateExpression=L"¡¤ <XText>";
					table->SetStateInfo(36, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XSubNode";
					info.stateName=L"XSubNode.RootStart";
					info.stateExpression=L"¡ñ $<XSubNode>";
					table->SetStateInfo(37, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XSubNode";
					info.stateName=L"XSubNode.Start";
					info.stateExpression=L"¡¤ <XSubNode>";
					table->SetStateInfo(38, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XDocument";
					info.stateName=L"XDocument.1";
					info.stateExpression=L"<XDocument>: { XInstruction : instructions | XComment : comments } XElement : rootElement as Document¡ñ";
					table->SetStateInfo(39, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XDocument";
					info.stateName=L"XDocument.2";
					info.stateExpression=L"<XDocument>: ¡ñ{ XInstruction : instructions | XComment : comments } XElement : rootElement as Document";
					table->SetStateInfo(40, info);
				}
				{
					vl::parsing::tabling::ParsingTable::StateInfo info;
					info.ruleName=L"XSubNode";
					info.stateName=L"XSubNode.1";
					info.stateExpression=L"<XSubNode>: !XText | !XCData | !XComment | !XElement¡ñ";
					table->SetStateInfo(41, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XDocument";
					info.type=L"Document";
					info.rootStartState=0;
					table->SetRuleInfo(0, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XElement";
					info.type=L"Element";
					info.rootStartState=25;
					table->SetRuleInfo(1, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XCData";
					info.type=L"CData";
					info.rootStartState=27;
					table->SetRuleInfo(2, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XAttribute";
					info.type=L"Attribute";
					info.rootStartState=29;
					table->SetRuleInfo(3, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XComment";
					info.type=L"Comment";
					info.rootStartState=31;
					table->SetRuleInfo(4, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XInstruction";
					info.type=L"Instruction";
					info.rootStartState=33;
					table->SetRuleInfo(5, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XText";
					info.type=L"Text";
					info.rootStartState=35;
					table->SetRuleInfo(6, info);
				}
				{
					vl::parsing::tabling::ParsingTable::RuleInfo info;
					info.name=L"XSubNode";
					info.type=L"Node";
					info.rootStartState=37;
					table->SetRuleInfo(7, info);
				}
				return table;
			}

/***********************************************************************
Table Generation
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

		}
	}
}
