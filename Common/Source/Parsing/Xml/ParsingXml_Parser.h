/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::ParsingXml_Parser

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#ifndef VCZH_PARSING_XML_PARSINGXML_PARSER
#define VCZH_PARSING_XML_PARSINGXML_PARSER

#include "..\Parsing.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			struct XmlParserTokenIndex abstract
			{
				static const vl::vint INSTRUCTION_OPEN = 0;
				static const vl::vint INSTRUCTION_CLOSE = 1;
				static const vl::vint COMPLEX_ELEMENT_OPEN = 2;
				static const vl::vint SINGLE_ELEMENT_CLOSE = 3;
				static const vl::vint ELEMENT_OPEN = 4;
				static const vl::vint ELEMENT_CLOSE = 5;
				static const vl::vint EQUAL = 6;
				static const vl::vint NAME = 7;
				static const vl::vint ATTVALUE = 8;
				static const vl::vint COMMENT = 9;
				static const vl::vint CDATA = 10;
				static const vl::vint TEXT = 11;
				static const vl::vint SPACE = 12;
			};
			class XmlNode;
			class XmlText;
			class XmlCData;
			class XmlAttribute;
			class XmlComment;
			class XmlElement;
			class XmlInstruction;
			class XmlDocument;

			class XmlNode abstract : public vl::parsing::ParsingTreeCustomBase
			{
			public:
				class IVisitor : public vl::Interface
				{
				public:
					virtual void Visit(XmlText* node)=0;
					virtual void Visit(XmlCData* node)=0;
					virtual void Visit(XmlAttribute* node)=0;
					virtual void Visit(XmlComment* node)=0;
					virtual void Visit(XmlElement* node)=0;
					virtual void Visit(XmlInstruction* node)=0;
					virtual void Visit(XmlDocument* node)=0;
				};

				virtual void Accept(XmlNode::IVisitor* visitor)=0;

			};

			class XmlText : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlText> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlCData : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlCData> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlAttribute : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken value;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlAttribute> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlComment : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlComment> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlElement : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken closingName;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;
				vl::collections::List<vl::Ptr<XmlNode>> subNodes;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlElement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlInstruction : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlInstruction> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlDocument : public XmlNode
			{
			public:
				vl::collections::List<vl::Ptr<XmlNode>> prologs;
				vl::Ptr<XmlElement> rootElement;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlDocument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> XmlLoadTable();

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		}
	}
}
#endif