/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Test

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#ifndef VCZH_PARSING_XML
#define VCZH_PARSING_XML

#include "..\..\..\..\Libraries\GacUI\Public\Source\Vlpp.h"

namespace vczh
{
	namespace whoknows
	{
		namespace xmlutility
		{
			enum class XmlParserTokenIndex
			{
				INSTRUCTION_OPEN = 0,
				INSTRUCTION_CLOSE = 1,
				COMPLEX_ELEMENT_OPEN = 2,
				SINGLE_ELEMENT_CLOSE = 3,
				ELEMENT_OPEN = 4,
				ELEMENT_CLOSE = 5,
				EQUAL = 6,
				NAME = 7,
				ATTVALUE = 8,
				COMMENT = 9,
				CDATA = 10,
				TEXT = 11,
				SPACE = 12,
			};
			class XmlNode;
			class XmlAmbiguousNode;
			class XmlText;
			class XmlCData;
			class XmlAttribute;
			class XmlComment;
			class XmlElement;
			class XmlInstruction;
			class XmlDocument;
			class XmlFirstClass;
			class XmlFirstClassChild;
			class XmlThirdClassChild;

			class XmlNode abstract : public vl::parsing::ParsingTreeCustomBase
			{
			public:
				class IVisitor : public vl::Interface
				{
				public:
					virtual void Visit(XmlAmbiguousNode* node)=0;
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

			class XmlAmbiguousNode : public XmlNode
			{
			public:
				vl::collections::List<vl::Ptr<XmlNode>> items;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlAmbiguousNode> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
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
				vl::collections::List<vl::Ptr<XmlInstruction>> instructions;
				vl::collections::List<vl::Ptr<XmlComment>> comments;
				vl::Ptr<XmlElement> rootElement;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlDocument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			enum class XmlDirectEnum
			{
				A,
				B,
				C,
			};

			class XmlFirstClass abstract : public vl::parsing::ParsingTreeCustomBase
			{
			public:
				class IVisitor : public vl::Interface
				{
				public:
					virtual void Visit(XmlFirstClassChild* node)=0;
				};

				virtual void Accept(XmlFirstClass::IVisitor* visitor)=0;

				class XmlSecondClass;
				class XmlSecondClassChild;

				enum class XmlIndirectEnum
				{
					D,
					E,
					F,
				};

				class XmlSecondClass abstract : public vl::parsing::ParsingTreeCustomBase
				{
				public:
					class IVisitor : public vl::Interface
					{
					public:
						virtual void Visit(XmlFirstClass::XmlSecondClassChild* node)=0;
					};

					virtual void Accept(XmlFirstClass::XmlSecondClass::IVisitor* visitor)=0;

					class XmlThirdClass;

					class XmlThirdClass abstract : public vl::parsing::ParsingTreeCustomBase
					{
					public:
						class IVisitor : public vl::Interface
						{
						public:
							virtual void Visit(XmlThirdClassChild* node)=0;
						};

						virtual void Accept(XmlFirstClass::XmlSecondClass::XmlThirdClass::IVisitor* visitor)=0;

						vl::parsing::ParsingToken field1;
					};

				};

				class XmlSecondClassChild : public XmlSecondClass
				{
				public:
					vl::parsing::ParsingToken field3;

					void Accept(XmlFirstClass::XmlSecondClass::IVisitor* visitor)override;

					static vl::Ptr<XmlFirstClass::XmlSecondClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
				};

				vl::parsing::ParsingToken field4;
			};

			class XmlFirstClassChild : public XmlFirstClass
			{
			public:
				vl::parsing::ParsingToken field2;

				void Accept(XmlFirstClass::IVisitor* visitor)override;

				static vl::Ptr<XmlFirstClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlThirdClassChild : public XmlFirstClass::XmlSecondClass::XmlThirdClass
			{
			public:
				vl::parsing::ParsingToken field5;

				void Accept(XmlFirstClass::XmlSecondClass::XmlThirdClass::IVisitor* visitor)override;

				static vl::Ptr<XmlThirdClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::WString XmlGetParserTextBuffer();
			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> XmlLoadTable();

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<XmlDocument> XmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> XmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
			extern vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
			extern vl::Ptr<XmlElement> XmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

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
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlAmbiguousNode)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlNode)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlCData)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlDirectEnum)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlComment)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClassChild)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlElement)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlAttribute)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlThirdClassChild)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlInstruction)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlDocument)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlIndirectEnum)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClass)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClass::XmlThirdClass)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlFirstClass::XmlSecondClassChild)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::XmlText)
#endif

			extern bool XmlLoadTypes();
		}
	}
}
#endif