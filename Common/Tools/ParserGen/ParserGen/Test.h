/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Parser::Test

���ļ�ʹ��Vczh Parsing Generator�����Զ�����
***********************************************************************/

#ifndef VCZH_PARSING_TESTXML2
#define VCZH_PARSING_TESTXML2

#include "..\..\..\..\Libraries\GacUI\Public\Source\Vlpp.h"

namespace vczh
{
	namespace whoknows
	{
		namespace xmlutility
		{
			enum class TestXmlParserTokenIndex
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
			class TestXmlNode;
			class TestXmlAmbiguousNode;
			class TestXmlText;
			class TestXmlCData;
			class TestXmlAttribute;
			class TestXmlComment;
			class TestXmlElement;
			class TestXmlInstruction;
			class TestXmlDocument;
			class TestXmlAnimal;
			class TestXmlBird;
			class TestXmlGoose;
			class TestXmlDuck;
			class TestXmlBlackHole;

			class TestXmlNode abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<TestXmlNode>
			{
			public:
				class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
				{
				public:
					virtual void Visit(TestXmlAmbiguousNode* node)=0;
					virtual void Visit(TestXmlText* node)=0;
					virtual void Visit(TestXmlCData* node)=0;
					virtual void Visit(TestXmlAttribute* node)=0;
					virtual void Visit(TestXmlComment* node)=0;
					virtual void Visit(TestXmlElement* node)=0;
					virtual void Visit(TestXmlInstruction* node)=0;
					virtual void Visit(TestXmlDocument* node)=0;
				};

				virtual void Accept(TestXmlNode::IVisitor* visitor)=0;

			};

			class TestXmlAmbiguousNode : public TestXmlNode, vl::reflection::Description<TestXmlAmbiguousNode>
			{
			public:
				vl::collections::List<vl::Ptr<TestXmlNode>> items;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlAmbiguousNode> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlText : public TestXmlNode, vl::reflection::Description<TestXmlText>
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlText> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlCData : public TestXmlNode, vl::reflection::Description<TestXmlCData>
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlCData> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlAttribute : public TestXmlNode, vl::reflection::Description<TestXmlAttribute>
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken value;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlAttribute> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlComment : public TestXmlNode, vl::reflection::Description<TestXmlComment>
			{
			public:
				vl::parsing::ParsingToken content;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlComment> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlElement : public TestXmlNode, vl::reflection::Description<TestXmlElement>
			{
			public:
				vl::parsing::ParsingToken name;
				vl::parsing::ParsingToken closingName;
				vl::collections::List<vl::Ptr<TestXmlAttribute>> attributes;
				vl::collections::List<vl::Ptr<TestXmlNode>> subNodes;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlElement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlInstruction : public TestXmlNode, vl::reflection::Description<TestXmlInstruction>
			{
			public:
				vl::parsing::ParsingToken name;
				vl::collections::List<vl::Ptr<TestXmlAttribute>> attributes;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlInstruction> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlDocument : public TestXmlNode, vl::reflection::Description<TestXmlDocument>
			{
			public:
				vl::collections::List<vl::Ptr<TestXmlInstruction>> instructions;
				vl::collections::List<vl::Ptr<TestXmlComment>> comments;
				vl::Ptr<TestXmlElement> rootElement;

				void Accept(TestXmlNode::IVisitor* visitor)override;

				static vl::Ptr<TestXmlDocument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			enum class TestXmlDirectEnum
			{
				A,
				B,
				C,
			};

			class TestXmlAnimal abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<TestXmlAnimal>
			{
			public:
				class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
				{
				public:
					virtual void Visit(TestXmlGoose* node)=0;
					virtual void Visit(TestXmlDuck* node)=0;
				};

				virtual void Accept(TestXmlAnimal::IVisitor* visitor)=0;

				class TestXmlEmptyClass;
				class TestXmlBody;
				class TestXmlLeg;
				class TestXmlTail;

				enum class TestXmlIndirectEnum
				{
					D,
					E,
					F,
				};

				class TestXmlEmptyClass : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<TestXmlEmptyClass>
				{
				public:

					static vl::Ptr<TestXmlAnimal::TestXmlEmptyClass> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
				};

				class TestXmlBody abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<TestXmlBody>
				{
				public:
					class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
					{
					public:
						virtual void Visit(TestXmlAnimal::TestXmlLeg* node)=0;
						virtual void Visit(TestXmlAnimal::TestXmlTail* node)=0;
					};

					virtual void Accept(TestXmlAnimal::TestXmlBody::IVisitor* visitor)=0;

					class TestXmlFur;

					class TestXmlFur abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<TestXmlFur>
					{
					public:
						class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
						{
						public:
							virtual void Visit(TestXmlBlackHole* node)=0;
						};

						virtual void Accept(TestXmlAnimal::TestXmlBody::TestXmlFur::IVisitor* visitor)=0;

						vl::parsing::ParsingToken title;

						static vl::Ptr<TestXmlAnimal::TestXmlBody::TestXmlFur> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
					};

				};

				class TestXmlLeg : public TestXmlBody, vl::reflection::Description<TestXmlLeg>
				{
				public:
					vl::parsing::ParsingToken id;

					void Accept(TestXmlAnimal::TestXmlBody::IVisitor* visitor)override;

					static vl::Ptr<TestXmlAnimal::TestXmlLeg> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
				};

				class TestXmlTail : public TestXmlBody, vl::reflection::Description<TestXmlTail>
				{
				public:
					vl::parsing::ParsingToken id;

					void Accept(TestXmlAnimal::TestXmlBody::IVisitor* visitor)override;

					static vl::Ptr<TestXmlAnimal::TestXmlTail> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
				};

				vl::parsing::ParsingToken name;
			};

			class TestXmlBird abstract : public TestXmlAnimal, vl::reflection::Description<TestXmlBird>
			{
			public:
				vl::parsing::ParsingToken id;
			};

			class TestXmlGoose : public TestXmlBird, vl::reflection::Description<TestXmlGoose>
			{
			public:
				vl::parsing::ParsingToken title;

				void Accept(TestXmlBird::IVisitor* visitor)override;

				static vl::Ptr<TestXmlGoose> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlDuck : public TestXmlBird, vl::reflection::Description<TestXmlDuck>
			{
			public:
				vl::parsing::ParsingToken title;

				void Accept(TestXmlBird::IVisitor* visitor)override;

				static vl::Ptr<TestXmlDuck> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			class TestXmlBlackHole : public TestXmlAnimal::TestXmlBody::TestXmlFur, vl::reflection::Description<TestXmlBlackHole>
			{
			public:
				vl::parsing::ParsingToken id;

				void Accept(TestXmlAnimal::TestXmlBody::TestXmlFur::IVisitor* visitor)override;

				static vl::Ptr<TestXmlBlackHole> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			};

			extern vl::WString TestXmlGetParserTextBuffer();
			extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> TestXmlConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
			extern vl::Ptr<vl::parsing::tabling::ParsingTable> TestXmlLoadTable();

			extern vl::Ptr<vl::parsing::ParsingTreeNode> TestXmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> TestXmlParseDocumentAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);
			extern vl::Ptr<TestXmlDocument> TestXmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
			extern vl::Ptr<TestXmlDocument> TestXmlParseDocument(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);

			extern vl::Ptr<vl::parsing::ParsingTreeNode> TestXmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
			extern vl::Ptr<vl::parsing::ParsingTreeNode> TestXmlParseElementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);
			extern vl::Ptr<TestXmlElement> TestXmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
			extern vl::Ptr<TestXmlElement> TestXmlParseElement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);
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
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlNode)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAmbiguousNode)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlText)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlCData)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAttribute)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlComment)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlElement)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlInstruction)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlDocument)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlDirectEnum)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlIndirectEnum)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlEmptyClass)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::TestXmlFur)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlLeg)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlTail)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlBird)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlGoose)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlDuck)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlBlackHole)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlNode::IVisitor)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::IVisitor)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::IVisitor)
			DECL_TYPE_INFO(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::TestXmlFur::IVisitor)

			namespace interface_proxy
			{
				class TestXmlNode_IVisitor : public ValueInterfaceRoot, public virtual vczh::whoknows::xmlutility::TestXmlNode::IVisitor
				{
				public:
					TestXmlNode_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vczh::whoknows::xmlutility::TestXmlNode::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TestXmlNode_IVisitor(proxy);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlAmbiguousNode* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlText* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlCData* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlAttribute* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlComment* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlElement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlInstruction* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlDocument* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				class TestXmlAnimal_IVisitor : public ValueInterfaceRoot, public virtual vczh::whoknows::xmlutility::TestXmlAnimal::IVisitor
				{
				public:
					TestXmlAnimal_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vczh::whoknows::xmlutility::TestXmlAnimal::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TestXmlAnimal_IVisitor(proxy);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlGoose* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlDuck* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				class TestXmlAnimal_TestXmlBody_IVisitor : public ValueInterfaceRoot, public virtual vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::IVisitor
				{
				public:
					TestXmlAnimal_TestXmlBody_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TestXmlAnimal_TestXmlBody_IVisitor(proxy);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlLeg* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlTail* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				class TestXmlAnimal_TestXmlBody_TestXmlFur_IVisitor : public ValueInterfaceRoot, public virtual vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::TestXmlFur::IVisitor
				{
				public:
					TestXmlAnimal_TestXmlBody_TestXmlFur_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vczh::whoknows::xmlutility::TestXmlAnimal::TestXmlBody::TestXmlFur::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new TestXmlAnimal_TestXmlBody_TestXmlFur_IVisitor(proxy);
					}

					void Visit(vczh::whoknows::xmlutility::TestXmlBlackHole* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				}
#endif

			extern bool TestXmlLoadTypes();
		}
	}
}
#endif