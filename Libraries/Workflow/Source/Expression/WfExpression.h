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
			SCOPE_DELIMITER = 14,
			COLON = 15,
			SEMICOLON = 16,
			COMMA = 17,
			DOT = 18,
			OPEN_ARRAY = 19,
			CLOSE_ARRAY = 20,
			OPEN_BRACE = 21,
			CLOSE_BRACE = 22,
			OPEN_BRACKET = 23,
			CLOSE_BRACKET = 24,
			KEYWORD_SHL = 25,
			KEYWORD_SHR = 26,
			KEYWORD_XOR = 27,
			KEYWORD_AND = 28,
			KEYWORD_OR = 29,
			KEYWORD_NOT = 30,
			KEYWORD_NULL = 31,
			KEYWORD_TRUE = 32,
			KEYWORD_FALSE = 33,
			KEYWORD_LET = 34,
			KEYWORD_IN = 35,
			KEYWORD_RANGE = 36,
			KEYWORD_NEW = 37,
			KEYWORD_OF = 38,
			KEYWORD_AS = 39,
			KEYWORD_IS = 40,
			KEYWORD_CAST = 41,
			KEYWORD_FUNC = 42,
			KEYWORD_TYPEOF = 43,
			KEYWORD_TYPE = 44,
			KEYWORD_BIND = 45,
			KEYWORD_OBSERVE = 46,
			KEYWORD_ON = 47,
			KEYWORD_ATTACH = 48,
			KEYWORD_DETACH = 49,
			KEYWORD_VAR = 50,
			KEYWORD_BREAK = 51,
			KEYWORD_CONTINUE = 52,
			KEYWORD_RETURN = 53,
			KEYWORD_DELETE = 54,
			KEYWORD_RAISE = 55,
			KEYWORD_IF = 56,
			KEYWORD_ELSE = 57,
			KEYWORD_SWITCH = 58,
			KEYWORD_CASE = 59,
			KEYWORD_DEFAULT = 60,
			KEYWORD_WHILE = 61,
			KEYWORD_FOR = 62,
			KEYWORD_REVERSED = 63,
			KEYWORD_TRY = 64,
			KEYWORD_CATCH = 65,
			KEYWORD_FINALLY = 66,
			KEYWORD_USING = 67,
			KEYWORD_NAMESPACE = 68,
			KEYWORD_MODULE = 69,
			NAME = 70,
			ORDERED_NAME = 71,
			FLOAT = 72,
			INTEGER = 73,
			STRING = 74,
			FORMATSTRING = 75,
			SPACE = 76,
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
		class WfOrderedNameExpression;
		class WfOrderedLambdaExpression;
		class WfMemberExpression;
		class WfChildExpression;
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
		class WfAttachEventExpression;
		class WfDetachEventExpression;
		class WfBindExpression;
		class WfObserveExpression;
		class WfCallExpression;
		class WfStatement;
		class WfVariableStatement;
		class WfBreakStatement;
		class WfContinueStatement;
		class WfReturnStatement;
		class WfDeleteStatement;
		class WfRaiseExceptionStatement;
		class WfIfStatement;
		class WfSwitchCase;
		class WfSwitchStatement;
		class WfWhileStatement;
		class WfForEachStatement;
		class WfTryStatement;
		class WfBlockStatement;
		class WfExpressionStatement;
		class WfDeclaration;
		class WfNamespaceDeclaration;
		class WfFunctionArgument;
		class WfFunctionDeclaration;
		class WfFunctionExpression;
		class WfNewTypeExpression;
		class WfModuleUsingItem;
		class WfModuleUsingPath;
		class WfModule;

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
				virtual void Visit(WfOrderedNameExpression* node)=0;
				virtual void Visit(WfOrderedLambdaExpression* node)=0;
				virtual void Visit(WfMemberExpression* node)=0;
				virtual void Visit(WfChildExpression* node)=0;
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
				virtual void Visit(WfAttachEventExpression* node)=0;
				virtual void Visit(WfDetachEventExpression* node)=0;
				virtual void Visit(WfBindExpression* node)=0;
				virtual void Visit(WfObserveExpression* node)=0;
				virtual void Visit(WfCallExpression* node)=0;
				virtual void Visit(WfFunctionExpression* node)=0;
				virtual void Visit(WfNewTypeExpression* node)=0;
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

		class WfOrderedNameExpression : public WfExpression, vl::reflection::Description<WfOrderedNameExpression>
		{
		public:
			vl::parsing::ParsingToken name;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfOrderedNameExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfOrderedLambdaExpression : public WfExpression, vl::reflection::Description<WfOrderedLambdaExpression>
		{
		public:
			vl::Ptr<WfExpression> body;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfOrderedLambdaExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfMemberExpression : public WfExpression, vl::reflection::Description<WfMemberExpression>
		{
		public:
			vl::Ptr<WfExpression> parent;
			vl::parsing::ParsingToken name;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfMemberExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfChildExpression : public WfExpression, vl::reflection::Description<WfChildExpression>
		{
		public:
			vl::Ptr<WfExpression> parent;
			vl::parsing::ParsingToken name;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfChildExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
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
			Assign,
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

		enum class WfSetTesting
		{
			In,
			NotIn,
		};

		class WfSetTestingExpression : public WfExpression, vl::reflection::Description<WfSetTestingExpression>
		{
		public:
			WfSetTesting test;
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

		enum class WfTypeTesting
		{
			IsType,
			IsNotType,
			IsNull,
			IsNotNull,
		};

		class WfTypeTestingExpression : public WfExpression, vl::reflection::Description<WfTypeTestingExpression>
		{
		public:
			WfTypeTesting test;
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

		class WfAttachEventExpression : public WfExpression, vl::reflection::Description<WfAttachEventExpression>
		{
		public:
			vl::Ptr<WfExpression> event;
			vl::Ptr<WfExpression> function;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfAttachEventExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfDetachEventExpression : public WfExpression, vl::reflection::Description<WfDetachEventExpression>
		{
		public:
			vl::Ptr<WfExpression> handler;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfDetachEventExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfBindExpression : public WfExpression, vl::reflection::Description<WfBindExpression>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfBindExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfObserveType
		{
			SimpleObserve,
			ExtendedObserve,
		};

		class WfObserveExpression : public WfExpression, vl::reflection::Description<WfObserveExpression>
		{
		public:
			vl::Ptr<WfExpression> parent;
			WfObserveType observeType;
			vl::parsing::ParsingToken name;
			vl::Ptr<WfExpression> expression;
			vl::collections::List<vl::Ptr<WfExpression>> events;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfObserveExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfCallExpression : public WfExpression, vl::reflection::Description<WfCallExpression>
		{
		public:
			vl::Ptr<WfExpression> function;
			vl::collections::List<vl::Ptr<WfExpression>> arguments;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfCallExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfStatement abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfStatement>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(WfVariableStatement* node)=0;
				virtual void Visit(WfBreakStatement* node)=0;
				virtual void Visit(WfContinueStatement* node)=0;
				virtual void Visit(WfReturnStatement* node)=0;
				virtual void Visit(WfDeleteStatement* node)=0;
				virtual void Visit(WfRaiseExceptionStatement* node)=0;
				virtual void Visit(WfIfStatement* node)=0;
				virtual void Visit(WfSwitchStatement* node)=0;
				virtual void Visit(WfWhileStatement* node)=0;
				virtual void Visit(WfForEachStatement* node)=0;
				virtual void Visit(WfTryStatement* node)=0;
				virtual void Visit(WfBlockStatement* node)=0;
				virtual void Visit(WfExpressionStatement* node)=0;
			};

			virtual void Accept(WfStatement::IVisitor* visitor)=0;

		};

		class WfVariableStatement : public WfStatement, vl::reflection::Description<WfVariableStatement>
		{
		public:
			vl::Ptr<WfType> type;
			vl::parsing::ParsingToken name;
			vl::Ptr<WfExpression> expression;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfVariableStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfBreakStatement : public WfStatement, vl::reflection::Description<WfBreakStatement>
		{
		public:

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfBreakStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfContinueStatement : public WfStatement, vl::reflection::Description<WfContinueStatement>
		{
		public:

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfContinueStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfReturnStatement : public WfStatement, vl::reflection::Description<WfReturnStatement>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfReturnStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfDeleteStatement : public WfStatement, vl::reflection::Description<WfDeleteStatement>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfDeleteStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfRaiseExceptionStatement : public WfStatement, vl::reflection::Description<WfRaiseExceptionStatement>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfRaiseExceptionStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfIfStatement : public WfStatement, vl::reflection::Description<WfIfStatement>
		{
		public:
			vl::Ptr<WfType> type;
			vl::parsing::ParsingToken name;
			vl::Ptr<WfExpression> expression;
			vl::Ptr<WfStatement> trueBranch;
			vl::Ptr<WfStatement> falseBranch;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfIfStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfSwitchCase : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfSwitchCase>
		{
		public:
			vl::Ptr<WfExpression> expression;
			vl::Ptr<WfStatement> statement;

			static vl::Ptr<WfSwitchCase> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfSwitchStatement : public WfStatement, vl::reflection::Description<WfSwitchStatement>
		{
		public:
			vl::Ptr<WfExpression> expression;
			vl::collections::List<vl::Ptr<WfSwitchCase>> caseBranches;
			vl::Ptr<WfStatement> defaultBranch;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfSwitchStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfWhileStatement : public WfStatement, vl::reflection::Description<WfWhileStatement>
		{
		public:
			vl::Ptr<WfExpression> condition;
			vl::Ptr<WfStatement> statement;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfWhileStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfForEachDirection
		{
			Normal,
			Reversed,
		};

		class WfForEachStatement : public WfStatement, vl::reflection::Description<WfForEachStatement>
		{
		public:
			vl::parsing::ParsingToken name;
			WfForEachDirection direction;
			vl::Ptr<WfExpression> collection;
			vl::Ptr<WfStatement> statement;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfForEachStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfTryStatement : public WfStatement, vl::reflection::Description<WfTryStatement>
		{
		public:
			vl::Ptr<WfStatement> protectedStatement;
			vl::parsing::ParsingToken name;
			vl::Ptr<WfStatement> catchStatement;
			vl::Ptr<WfStatement> finallyStatement;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfTryStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfBlockStatement : public WfStatement, vl::reflection::Description<WfBlockStatement>
		{
		public:
			vl::collections::List<vl::Ptr<WfStatement>> statements;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfBlockStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfExpressionStatement : public WfStatement, vl::reflection::Description<WfExpressionStatement>
		{
		public:
			vl::Ptr<WfExpression> expression;

			void Accept(WfStatement::IVisitor* visitor)override;

			static vl::Ptr<WfExpressionStatement> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfDeclaration abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfDeclaration>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(WfNamespaceDeclaration* node)=0;
				virtual void Visit(WfFunctionDeclaration* node)=0;
			};

			virtual void Accept(WfDeclaration::IVisitor* visitor)=0;

		};

		class WfNamespaceDeclaration : public WfDeclaration, vl::reflection::Description<WfNamespaceDeclaration>
		{
		public:
			vl::parsing::ParsingToken name;
			vl::collections::List<vl::Ptr<WfDeclaration>> declarations;

			void Accept(WfDeclaration::IVisitor* visitor)override;

			static vl::Ptr<WfNamespaceDeclaration> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfFunctionArgument : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfFunctionArgument>
		{
		public:
			vl::parsing::ParsingToken name;
			vl::Ptr<WfType> type;

			static vl::Ptr<WfFunctionArgument> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class WfFunctionAnonymity
		{
			Named,
			Anonymous,
		};

		class WfFunctionDeclaration : public WfDeclaration, vl::reflection::Description<WfFunctionDeclaration>
		{
		public:
			WfFunctionAnonymity anonymity;
			vl::parsing::ParsingToken name;
			vl::collections::List<vl::Ptr<WfFunctionArgument>> arguments;
			vl::Ptr<WfStatement> statement;

			void Accept(WfDeclaration::IVisitor* visitor)override;

			static vl::Ptr<WfFunctionDeclaration> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfFunctionExpression : public WfExpression, vl::reflection::Description<WfFunctionExpression>
		{
		public:
			vl::Ptr<WfFunctionDeclaration> function;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfFunctionExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfNewTypeExpression : public WfExpression, vl::reflection::Description<WfNewTypeExpression>
		{
		public:
			vl::Ptr<WfType> type;
			vl::collections::List<vl::Ptr<WfExpression>> arguments;
			vl::collections::List<vl::Ptr<WfFunctionDeclaration>> functions;

			void Accept(WfExpression::IVisitor* visitor)override;

			static vl::Ptr<WfNewTypeExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfModuleUsingItem : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfModuleUsingItem>
		{
		public:
			vl::parsing::ParsingToken name;

			static vl::Ptr<WfModuleUsingItem> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfModuleUsingPath : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfModuleUsingPath>
		{
		public:
			vl::collections::List<vl::Ptr<WfModuleUsingItem>> items;

			static vl::Ptr<WfModuleUsingPath> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class WfModule : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<WfModule>
		{
		public:
			vl::parsing::ParsingToken name;
			vl::collections::List<vl::Ptr<WfModuleUsingPath>> paths;
			vl::collections::List<vl::Ptr<WfDeclaration>> declarations;

			static vl::Ptr<WfModule> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		extern vl::WString WfGetParserTextBuffer();
		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> WfConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> WfLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfDeclaration> WfParseDeclaration(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfDeclaration> WfParseDeclaration(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseModuleAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseModuleAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfModule> WfParseModule(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfModule> WfParseModule(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseStatementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseStatementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfStatement> WfParseStatement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfStatement> WfParseStatement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseTypeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> WfParseTypeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<WfType> WfParseType(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<WfType> WfParseType(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
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
			DECL_TYPE_INFO(vl::workflow::WfOrderedNameExpression)
			DECL_TYPE_INFO(vl::workflow::WfOrderedLambdaExpression)
			DECL_TYPE_INFO(vl::workflow::WfMemberExpression)
			DECL_TYPE_INFO(vl::workflow::WfChildExpression)
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
			DECL_TYPE_INFO(vl::workflow::WfSetTesting)
			DECL_TYPE_INFO(vl::workflow::WfSetTestingExpression)
			DECL_TYPE_INFO(vl::workflow::WfConstructorArgument)
			DECL_TYPE_INFO(vl::workflow::WfConstructorExpression)
			DECL_TYPE_INFO(vl::workflow::WfInferExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeCastingStrategy)
			DECL_TYPE_INFO(vl::workflow::WfTypeCastingExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeTesting)
			DECL_TYPE_INFO(vl::workflow::WfTypeTestingExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeOfTypeExpression)
			DECL_TYPE_INFO(vl::workflow::WfTypeOfExpressionExpression)
			DECL_TYPE_INFO(vl::workflow::WfAttachEventExpression)
			DECL_TYPE_INFO(vl::workflow::WfDetachEventExpression)
			DECL_TYPE_INFO(vl::workflow::WfBindExpression)
			DECL_TYPE_INFO(vl::workflow::WfObserveType)
			DECL_TYPE_INFO(vl::workflow::WfObserveExpression)
			DECL_TYPE_INFO(vl::workflow::WfCallExpression)
			DECL_TYPE_INFO(vl::workflow::WfStatement)
			DECL_TYPE_INFO(vl::workflow::WfVariableStatement)
			DECL_TYPE_INFO(vl::workflow::WfBreakStatement)
			DECL_TYPE_INFO(vl::workflow::WfContinueStatement)
			DECL_TYPE_INFO(vl::workflow::WfReturnStatement)
			DECL_TYPE_INFO(vl::workflow::WfDeleteStatement)
			DECL_TYPE_INFO(vl::workflow::WfRaiseExceptionStatement)
			DECL_TYPE_INFO(vl::workflow::WfIfStatement)
			DECL_TYPE_INFO(vl::workflow::WfSwitchCase)
			DECL_TYPE_INFO(vl::workflow::WfSwitchStatement)
			DECL_TYPE_INFO(vl::workflow::WfWhileStatement)
			DECL_TYPE_INFO(vl::workflow::WfForEachDirection)
			DECL_TYPE_INFO(vl::workflow::WfForEachStatement)
			DECL_TYPE_INFO(vl::workflow::WfTryStatement)
			DECL_TYPE_INFO(vl::workflow::WfBlockStatement)
			DECL_TYPE_INFO(vl::workflow::WfExpressionStatement)
			DECL_TYPE_INFO(vl::workflow::WfDeclaration)
			DECL_TYPE_INFO(vl::workflow::WfNamespaceDeclaration)
			DECL_TYPE_INFO(vl::workflow::WfFunctionArgument)
			DECL_TYPE_INFO(vl::workflow::WfFunctionAnonymity)
			DECL_TYPE_INFO(vl::workflow::WfFunctionDeclaration)
			DECL_TYPE_INFO(vl::workflow::WfFunctionExpression)
			DECL_TYPE_INFO(vl::workflow::WfNewTypeExpression)
			DECL_TYPE_INFO(vl::workflow::WfModuleUsingItem)
			DECL_TYPE_INFO(vl::workflow::WfModuleUsingPath)
			DECL_TYPE_INFO(vl::workflow::WfModule)
			DECL_TYPE_INFO(vl::workflow::WfType::IVisitor)
			DECL_TYPE_INFO(vl::workflow::WfExpression::IVisitor)
			DECL_TYPE_INFO(vl::workflow::WfStatement::IVisitor)
			DECL_TYPE_INFO(vl::workflow::WfDeclaration::IVisitor)

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

					void Visit(vl::workflow::WfOrderedNameExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfOrderedLambdaExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfMemberExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfChildExpression* node)override
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

					void Visit(vl::workflow::WfAttachEventExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfDetachEventExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfBindExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfObserveExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfCallExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfFunctionExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfNewTypeExpression* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				class WfStatement_IVisitor : public ValueInterfaceRoot, public virtual vl::workflow::WfStatement::IVisitor
				{
				public:
					WfStatement_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::workflow::WfStatement::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new WfStatement_IVisitor(proxy);
					}

					void Visit(vl::workflow::WfVariableStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfBreakStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfContinueStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfReturnStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfDeleteStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfRaiseExceptionStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfIfStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfSwitchStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfWhileStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfForEachStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfTryStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfBlockStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfExpressionStatement* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				class WfDeclaration_IVisitor : public ValueInterfaceRoot, public virtual vl::workflow::WfDeclaration::IVisitor
				{
				public:
					WfDeclaration_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::workflow::WfDeclaration::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new WfDeclaration_IVisitor(proxy);
					}

					void Visit(vl::workflow::WfNamespaceDeclaration* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::workflow::WfFunctionDeclaration* node)override
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
