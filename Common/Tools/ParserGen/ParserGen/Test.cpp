#include "Test.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

			extern void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeCData(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeComment(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeTextFragment(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

			class XmlTreeConverter : public vl::parsing::ParsingTreeConverter
			{
			public:
				using vl::parsing::ParsingTreeConverter::SetMember;

				Ptr<ParsingTreeCustomBase> ConvertClass(Ptr<ParsingTreeObject> obj, const TokenList& tokens)override
				{
					if(obj->GetType()==L"Node")
					{
					}
					else if(obj->GetType()==L"Text")
					{
					}
					else if(obj->GetType()==L"CData")
					{
					}
					else if(obj->GetType()==L"Attribute")
					{
					}
					else if(obj->GetType()==L"Comment")
					{
					}
					else if(obj->GetType()==L"Element")
					{
					}
					else if(obj->GetType()==L"Instruction")
					{
					}
					else if(obj->GetType()==L"Document")
					{
					}
					else 
						return 0;
				}
			};

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				XmlTreeConverter converter;
				vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
				converter.SetMember(tree, node, tokens);
				return tree;
			}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

			vl::Ptr<XmlNode> XmlNode::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlNode>();
			}

			vl::Ptr<XmlText> XmlText::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlText>();
			}

			vl::Ptr<XmlCData> XmlCData::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlCData>();
			}

			vl::Ptr<XmlAttribute> XmlAttribute::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlAttribute>();
			}

			vl::Ptr<XmlComment> XmlComment::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlComment>();
			}

			vl::Ptr<XmlElement> XmlElement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlElement>();
			}

			vl::Ptr<XmlInstruction> XmlInstruction::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
			{
				return XmlConvertParsingTreeNode(node, tokens).Cast<XmlInstruction>();
			}

			vl::Ptr<XmlDocument> XmlDocument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)
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

		}
	}
}
