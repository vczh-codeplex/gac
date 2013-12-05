/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::WfExpression

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#include "../../../../Common/Source/Parsing/Parsing.h"
#include "../../../../Common/Source/Parsing/ParsingDefinitions.h"
#include "../../../../Common/Source/Parsing/ParsingAutomaton.h"

namespace vl
{
	namespace workflow
	{
		enum class WfParserTokenIndex
		{
			EXP = 0,
			ADD = 1,
			SUB = 2,
			MUL = 3,
			DIV = 4,
			CONCAT = 5,
			LE = 6,
			GE = 7,
			LT = 8,
			GT = 9,
			EQ = 10,
			NE = 11,
			FAILED_THEN = 12,
			OPEN_ARRAY = 13,
			CLOSE_ARRAY = 14,
			OPEN_BRACE = 15,
			CLOSE_BRACE = 16,
			SHL = 17,
			SHR = 18,
			XOR = 19,
			AND = 20,
			OR = 21,
			NOT = 22,
			NULL = 23,
			TRUE = 24,
			FALSE = 25,
			NAME = 26,
			FLOAT = 27,
			INTEGER = 28,
			STRING = 29,
			FORMATSTRING = 30,
			SPACE = 31,
		};
		class WfExpression;
		class WfLiteralExpression;
		class WfFloatingExpression;
		class WfIntegerExpression;
		class WfStringExpression;
		class WfFormatExpression;
		class WfUnaryExpression;
		class WfBinaryExpression;

		class WfExpression abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfExpression>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(WfFloatingExpression* node)=0;
				virtual void Visit(WfIntegerExpression* node)=0;
				virtual void Visit(WfStringExpression* node)=0;
				virtual void Visit(WfFormatExpression* node)=0;
				virtual void Visit(WfUnaryExpression* node)=0;
				virtual void Visit(WfBinaryExpression* node)=0;
			};

			virtual void Accept(WfExpression::IVisitor* visitor)=0;

		};

		enum class WfLiteralValue
		{
			Null,
			True,
			False,
		};

		class WfLiteralExpression : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfLiteralExpression>
		{
		public:
			WfLiteralValue value;

			static vl::Ptr<WfLiteralExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfFloatingExpression : public WfExpression, vl::reflection::Description<WfFloatingExpression>
		{
		public:
			vl::parsing::ParsingToken value;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfFloatingExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfIntegerExpression : public WfExpression, vl::reflection::Description<WfIntegerExpression>
		{
		public:
			vl::parsing::ParsingToken value;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfIntegerExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfStringExpression : public WfExpression, vl::reflection::Description<WfStringExpression>
		{
		public:
			vl::parsing::ParsingToken value;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfStringExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfFormatExpression : public WfExpression, vl::reflection::Description<WfFormatExpression>
		{
		public:
			vl::parsing::ParsingToken value;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfFormatExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfUnaryOperator
		{
			Positive,
			Negative,
			Not,
		};

		class WfUnaryExpression : public WfExpression, vl::reflection::Description<WfUnaryExpression>
		{
		public:
			WfUnaryOperator op;
			vl::Ptr<WfExpression> operand;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfUnaryExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfBinaryOperator
		{
			Index,
			Exp,
			Add,
			Sub,
			Mul,
			Div,
			Concat,
			Shl,
			Shr,
			LT,
			GT,
			LE,
			GE,
			EQ,
			NE,
			Xor,
			And,
			Not,
			FailedThen,
		};

		class WfBinaryExpression : public WfExpression, vl::reflection::Description<WfBinaryExpression>
		{
		public:
			WfBinaryOperator op;
			vl::Ptr<WfExpression> first;
			vl::Ptr<WfExpression> second;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfBinaryExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		extern vl::WString WfGetParserTextBuffer();
		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> WfConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> WfLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
	}
}
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			DECL_TYPE_INFO(vl::workflow::WfExpression)
			DECL_TYPE_INFO(vl::workflow::WfLiteralValue)
			DECL_TYPE_INFO(vl::workflow::WfLiteralExpression)
			DECL_TYPE_INFO(vl::workflow::WfFloatingExpression)
			DECL_TYPE_INFO(vl::workflow::WfIntegerExpression)
			DECL_TYPE_INFO(vl::workflow::WfStringExpression)
			DECL_TYPE_INFO(vl::workflow::WfFormatExpression)
			DECL_TYPE_INFO(vl::workflow::WfUnaryOperator)
			DECL_TYPE_INFO(vl::workflow::WfUnaryExpression)
			DECL_TYPE_INFO(vl::workflow::WfBinaryOperator)
			DECL_TYPE_INFO(vl::workflow::WfBinaryExpression)
			DECL_TYPE_INFO(vl::workflow::WfExpression::IVisitor)

			namespace interface_proxy
			{
				class WfExpression_IVisitor : public ValueInterfaceRoot, public virtual vl::workflow::WfExpression::IVisitor
				{
				public:
					WfExpression_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::workflow::WfExpression::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new WfExpression_IVisitor(proxy);
					}

					void Visit(vl::workflow::WfFloatingExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfIntegerExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfStringExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfFormatExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfUnaryExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfBinaryExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				}
#endif

			extern bool WfLoadTypes();
		}
	}
}
