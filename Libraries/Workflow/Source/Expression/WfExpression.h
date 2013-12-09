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
			QUESTION_MARK = 13,
			COLON = 14,
			SEMICOLON = 15,
			COMMA = 16,
			OPEN_ARRAY = 17,
			CLOSE_ARRAY = 18,
			OPEN_BRACE = 19,
			CLOSE_BRACE = 20,
			OPEN_BRACKET = 21,
			CLOSE_BRACKET = 22,
			KEYWORD_SHL = 23,
			KEYWORD_SHR = 24,
			KEYWORD_XOR = 25,
			KEYWORD_AND = 26,
			KEYWORD_OR = 27,
			KEYWORD_NOT = 28,
			KEYWORD_NULL = 29,
			KEYWORD_TRUE = 30,
			KEYWORD_FALSE = 31,
			KEYWORD_LET = 32,
			KEYWORD_IN = 33,
			KEYWORD_RANGE = 34,
			NAME = 35,
			FLOAT = 36,
			INTEGER = 37,
			STRING = 38,
			FORMATSTRING = 39,
			SPACE = 40,
		};
		class WfExpression;
		class WfLiteralExpression;
		class WfFloatingExpression;
		class WfIntegerExpression;
		class WfStringExpression;
		class WfFormatExpression;
		class WfUnaryExpression;
		class WfBinaryExpression;
		class WfLetExpression;
		class WfIfExpression;
		class WfRangeExpression;
		class WfElementExpression;

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
				virtual void Visit(WfLetExpression* node)=0;
				virtual void Visit(WfIfExpression* node)=0;
				virtual void Visit(WfRangeExpression* node)=0;
				virtual void Visit(WfElementExpression* node)=0;
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
			Or,
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

		class WfLetExpression : public WfExpression, vl::reflection::Description<WfLetExpression>
		{
		public:
			vl::parsing::ParsingToken name;
			vl::Ptr<WfExpression> value;
			vl::Ptr<WfExpression> exp;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfLetExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfIfExpression : public WfExpression, vl::reflection::Description<WfIfExpression>
		{
		public:
			vl::Ptr<WfExpression> condition;
			vl::Ptr<WfExpression> trueBranch;
			vl::Ptr<WfExpression> falseBranch;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfIfExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfRangeBoundary
		{
			Inclusive,
			Exclusive,
		};

		class WfRangeExpression : public WfExpression, vl::reflection::Description<WfRangeExpression>
		{
		public:
			vl::Ptr<WfExpression> begin;
			WfRangeBoundary beginBoundary;
			vl::Ptr<WfExpression> end;
			WfRangeBoundary endBoundary;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfRangeExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfElementExisting
		{
			Exists,
			NotExists,
		};

		class WfElementExpression : public WfExpression, vl::reflection::Description<WfElementExpression>
		{
		public:
			WfElementExisting test;
			vl::Ptr<WfExpression> element;
			vl::Ptr<WfExpression> collection;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfElementExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
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
			DECL_TYPE_INFO(vl::workflow::WfLetExpression)
			DECL_TYPE_INFO(vl::workflow::WfIfExpression)
			DECL_TYPE_INFO(vl::workflow::WfRangeBoundary)
			DECL_TYPE_INFO(vl::workflow::WfRangeExpression)
			DECL_TYPE_INFO(vl::workflow::WfElementExisting)
			DECL_TYPE_INFO(vl::workflow::WfElementExpression)
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

					void Visit(vl::workflow::WfLetExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfIfExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfRangeExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfElementExpression* node)override
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
