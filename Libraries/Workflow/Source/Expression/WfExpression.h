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
			DOT = 17,
			OPEN_ARRAY = 18,
			CLOSE_ARRAY = 19,
			OPEN_BRACE = 20,
			CLOSE_BRACE = 21,
			OPEN_BRACKET = 22,
			CLOSE_BRACKET = 23,
			KEYWORD_SHL = 24,
			KEYWORD_SHR = 25,
			KEYWORD_XOR = 26,
			KEYWORD_AND = 27,
			KEYWORD_OR = 28,
			KEYWORD_NOT = 29,
			KEYWORD_NULL = 30,
			KEYWORD_TRUE = 31,
			KEYWORD_FALSE = 32,
			KEYWORD_LET = 33,
			KEYWORD_IN = 34,
			KEYWORD_RANGE = 35,
			KEYWORD_OF = 36,
			KEYWORD_AS = 37,
			KEYWORD_IS = 38,
			KEYWORD_CAST = 39,
			KEYWORD_FUNC = 40,
			KEYWORD_TYPEOF = 41,
			KEYWORD_TYPE = 42,
			NAME = 43,
			FLOAT = 44,
			INTEGER = 45,
			STRING = 46,
			FORMATSTRING = 47,
			SPACE = 48,
		};
		class WfType;
		class WfReferenceType;
		class WfSharedPointerType;
		class WfEnumerableType;
		class WfMapType;
		class WfFunctionType;
		class WfChildType;
		class WfExpression;
		class WfReferenceExpression;
		class WfMemberExpression;
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
		class WfSetTestingExpression;
		class WfConstructorArgument;
		class WfConstructorExpression;
		class WfInferExpression;
		class WfTypeCastingExpression;
		class WfTypeTestingExpression;
		class WfTypeOfTypeExpression;
		class WfTypeOfExpressionExpression;
		class WfCallExpression;

		class WfType abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfType>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(WfReferenceType* node)=0;
				virtual void Visit(WfSharedPointerType* node)=0;
				virtual void Visit(WfEnumerableType* node)=0;
				virtual void Visit(WfMapType* node)=0;
				virtual void Visit(WfFunctionType* node)=0;
				virtual void Visit(WfChildType* node)=0;
			};

			virtual void Accept(WfType::IVisitor* visitor)=0;

		};

		class WfReferenceType : public WfType, vl::reflection::Description<WfReferenceType>
		{
		public:
			vl::parsing::ParsingToken name;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfReferenceType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfSharedPointerType : public WfType, vl::reflection::Description<WfSharedPointerType>
		{
		public:
			vl::Ptr<WfType> element;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfSharedPointerType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfEnumerableType : public WfType, vl::reflection::Description<WfEnumerableType>
		{
		public:
			vl::Ptr<WfType> element;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfEnumerableType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfMapType : public WfType, vl::reflection::Description<WfMapType>
		{
		public:
			vl::Ptr<WfType> key;
			vl::Ptr<WfType> value;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfMapType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfFunctionType : public WfType, vl::reflection::Description<WfFunctionType>
		{
		public:
			vl::Ptr<WfType> result;
			vl::collections::List<vl::Ptr<WfType>> arguments;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfFunctionType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfChildType : public WfType, vl::reflection::Description<WfChildType>
		{
		public:
			vl::Ptr<WfType> parent;
			vl::parsing::ParsingToken name;

			void Accept(WfType::IVisitor* visitor)override;

			static vl::Ptr<WfChildType> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfExpression abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfExpression>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(WfReferenceExpression* node)=0;
				virtual void Visit(WfMemberExpression* node)=0;
				virtual void Visit(WfLiteralExpression* node)=0;
				virtual void Visit(WfFloatingExpression* node)=0;
				virtual void Visit(WfIntegerExpression* node)=0;
				virtual void Visit(WfStringExpression* node)=0;
				virtual void Visit(WfFormatExpression* node)=0;
				virtual void Visit(WfUnaryExpression* node)=0;
				virtual void Visit(WfBinaryExpression* node)=0;
				virtual void Visit(WfLetExpression* node)=0;
				virtual void Visit(WfIfExpression* node)=0;
				virtual void Visit(WfRangeExpression* node)=0;
				virtual void Visit(WfSetTestingExpression* node)=0;
				virtual void Visit(WfConstructorExpression* node)=0;
				virtual void Visit(WfInferExpression* node)=0;
				virtual void Visit(WfTypeCastingExpression* node)=0;
				virtual void Visit(WfTypeTestingExpression* node)=0;
				virtual void Visit(WfTypeOfTypeExpression* node)=0;
				virtual void Visit(WfTypeOfExpressionExpression* node)=0;
				virtual void Visit(WfCallExpression* node)=0;
			};

			virtual void Accept(WfExpression::IVisitor* visitor)=0;

		};

		enum class WfLiteralValue
		{
			Null,
			True,
			False,
		};

		class WfReferenceExpression : public WfExpression, vl::reflection::Description<WfReferenceExpression>
		{
		public:
			vl::parsing::ParsingToken name;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfReferenceExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfMemberExpression : public WfExpression, vl::reflection::Description<WfMemberExpression>
		{
		public:
			vl::Ptr<WfExpression> parent;
			vl::parsing::ParsingToken name;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfMemberExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfLiteralExpression : public WfExpression, vl::reflection::Description<WfLiteralExpression>
		{
		public:
			WfLiteralValue value;

			void Accept(WfExpression::IVisitor* visitor)override;

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

		class WfSetTestingExpression : public WfExpression, vl::reflection::Description<WfSetTestingExpression>
		{
		public:
			vl::Ptr<WfExpression> element;
			vl::Ptr<WfExpression> collection;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfSetTestingExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfConstructorArgument : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfConstructorArgument>
		{
		public:
			vl::Ptr<WfExpression> key;
			vl::Ptr<WfExpression> value;

			static vl::Ptr<WfConstructorArgument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfConstructorExpression : public WfExpression, vl::reflection::Description<WfConstructorExpression>
		{
		public:
			vl::collections::List<vl::Ptr<WfConstructorArgument>> arguments;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfConstructorExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfInferExpression : public WfExpression, vl::reflection::Description<WfInferExpression>
		{
		public:
			vl::Ptr<WfExpression> expression;
			vl::Ptr<WfType> type;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfInferExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfTypeCastingStrategy
		{
			Strong,
			Weak,
		};

		class WfTypeCastingExpression : public WfExpression, vl::reflection::Description<WfTypeCastingExpression>
		{
		public:
			WfTypeCastingStrategy strategy;
			vl::Ptr<WfExpression> expression;
			vl::Ptr<WfType> type;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfTypeCastingExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfTypeTestingExpression : public WfExpression, vl::reflection::Description<WfTypeTestingExpression>
		{
		public:
			vl::Ptr<WfExpression> expression;
			vl::Ptr<WfType> type;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfTypeTestingExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfTypeOfTypeExpression : public WfExpression, vl::reflection::Description<WfTypeOfTypeExpression>
		{
		public:
			vl::Ptr<WfType> type;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfTypeOfTypeExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfTypeOfExpressionExpression : public WfExpression, vl::reflection::Description<WfTypeOfExpressionExpression>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfTypeOfExpressionExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfCallExpression : public WfExpression, vl::reflection::Description<WfCallExpression>
		{
		public:
			vl::Ptr<WfExpression> function;
			vl::collections::List<vl::Ptr<WfExpression>> arguments;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfCallExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
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
			DECL_TYPE_INFO(vl::workflow::WfType)
			DECL_TYPE_INFO(vl::workflow::WfReferenceType)
			DECL_TYPE_INFO(vl::workflow::WfSharedPointerType)
			DECL_TYPE_INFO(vl::workflow::WfEnumerableType)
			DECL_TYPE_INFO(vl::workflow::WfMapType)
			DECL_TYPE_INFO(vl::workflow::WfFunctionType)
			DECL_TYPE_INFO(vl::workflow::WfChildType)
			DECL_TYPE_INFO(vl::workflow::WfExpression)
			DECL_TYPE_INFO(vl::workflow::WfLiteralValue)
			DECL_TYPE_INFO(vl::workflow::WfReferenceExpression)
			DECL_TYPE_INFO(vl::workflow::WfMemberExpression)
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
			DECL_TYPE_INFO(vl::workflow::WfSetTestingExpression)
			DECL_TYPE_INFO(vl::workflow::WfConstructorArgument)
			DECL_TYPE_INFO(vl::workflow::WfConstructorExpression)
			DECL_TYPE_INFO(vl::workflow::WfInferExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeCastingStrategy)
			DECL_TYPE_INFO(vl::workflow::WfTypeCastingExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeTestingExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeOfTypeExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeOfExpressionExpression)
			DECL_TYPE_INFO(vl::workflow::WfCallExpression)
			DECL_TYPE_INFO(vl::workflow::WfType::IVisitor)
			DECL_TYPE_INFO(vl::workflow::WfExpression::IVisitor)

			namespace interface_proxy
			{
				class WfType_IVisitor : public ValueInterfaceRoot, public virtual vl::workflow::WfType::IVisitor
				{
				public:
					WfType_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::workflow::WfType::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new WfType_IVisitor(proxy);
					}

					void Visit(vl::workflow::WfReferenceType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfSharedPointerType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfEnumerableType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfMapType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfFunctionType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfChildType* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

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

					void Visit(vl::workflow::WfReferenceExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfMemberExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfLiteralExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
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

					void Visit(vl::workflow::WfSetTestingExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfConstructorExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfInferExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfTypeCastingExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfTypeTestingExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfTypeOfTypeExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfTypeOfExpressionExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfCallExpression* node)override
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
