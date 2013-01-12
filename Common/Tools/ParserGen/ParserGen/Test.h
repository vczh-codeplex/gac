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
			class XmlFirstClass;
			class XmlFirstClassChild;
			class XmlThirdClassChild;

			class XmlNode : public vl::parsing::ParsingTreeCustomBase
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

				static vl::Ptr<XmlText> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlCData : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlCData> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlAttribute : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken value;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlAttribute> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlComment : public XmlNode
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlComment> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlElement : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken closingName;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;
				vl::collections::List<vl::Ptr<XmlNode>> subNodes;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlElement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlInstruction : public XmlNode
			{
			public:
				vl::parsing::ParsingToken name;
				vl::collections::List<vl::Ptr<XmlAttribute>> attributes;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlInstruction> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlDocument : public XmlNode
			{
			public:
				vl::collections::List<vl::Ptr<XmlInstruction>> instructions;
				vl::collections::List<vl::Ptr<XmlComment>> comments;
				vl::Ptr<XmlElement> rootElement;

				void Accept(XmlNode::IVisitor* visitor)override;

				static vl::Ptr<XmlDocument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			struct XmlDirectEnum abstract
			{
				enum Type
				{
					A,
					B,
					C,
				};
			};

			class XmlFirstClass : public vl::parsing::ParsingTreeCustomBase
			{
			public:
				class IVisitor : public Interface
				{
					public:
					virtual void Visit(XmlFirstClassChild* node)=0;
				};

				virtual void Accept(XmlFirstClass::IVisitor* visitor)=0;

				class XmlSecondClass;
				class XmlSecondClassChild;

				struct XmlIndirectEnum abstract
				{
					enum Type
					{
						D,
						E,
						F,
					};
				};

				class XmlSecondClass : public vl::parsing::ParsingTreeCustomBase
				{
				public:
					class IVisitor : public Interface
					{
						public:
						virtual void Visit(XmlFirstClass::XmlSecondClassChild* node)=0;
					};

					virtual void Accept(XmlFirstClass::XmlSecondClass::IVisitor* visitor)=0;

					class XmlThirdClass;

					class XmlThirdClass : public vl::parsing::ParsingTreeCustomBase
					{
					public:
						class IVisitor : public Interface
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

					static vl::Ptr<XmlFirstClass::XmlSecondClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
				};

				vl::parsing::ParsingToken field4;
			};

			class XmlFirstClassChild : public XmlFirstClass
			{
			public:
				vl::parsing::ParsingToken field2;

				void Accept(XmlFirstClass::IVisitor* visitor)override;

				static vl::Ptr<XmlFirstClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class XmlThirdClassChild : public XmlFirstClass::XmlSecondClass::XmlThirdClass
			{
			public:
				vl::parsing::ParsingToken field5;

				void Accept(XmlFirstClass::XmlSecondClass::XmlThirdClass::IVisitor* visitor)override;

				static vl::Ptr<XmlThirdClassChild> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> XmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);
		}
	}
}
