/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Test

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/

#include "..\..\..\Source\Parsing\Parsing.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			class XmlNode;
			class XmlText;
			class XmlCData;
			class XmlAttribute;
			class XmlComment;
			class XmlElement;
			class XmlInstruction;
			class XmlDocument;

			class XmlNode
			{
			public:
				class IVisitor : public Interface
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
			};

			class XmlCData : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

			class XmlAttribute : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken value;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

			class XmlComment : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

			class XmlElement : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken closingName;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;
				vl::collections::List<vl::Ptr<XmlNode>> subNodes;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

			class XmlInstruction : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

			class XmlDocument : public XmlNode
			{
			public:
				vl::collections::List<vl::Ptr<XmlInstruction>> instructions;
				vl::collections::List<vl::Ptr<XmlComment>> comments;
				vl::Ptr<XmlElement> rootElement;

				void Accept(XmlNode::IVisitor* visitor)override;
			};

		}
	}
}
