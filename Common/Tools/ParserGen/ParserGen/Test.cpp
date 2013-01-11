#include "Test.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			extern void XmlMergeTextFragment(vl::collections::List<vl::Ptr<XmlNode>>& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeAttributeValue(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeCData(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeComment(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);
			extern void XmlUnescapeTextFragment(vl::parsing::ParsingToken& value, vl::collections::List<vl::regex::RegexToken>& tokens);

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
