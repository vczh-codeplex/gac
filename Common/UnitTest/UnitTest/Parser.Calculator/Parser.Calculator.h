/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Parser.Calculator

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#include "..\..\..\Source\Parsing\Parsing.h"
#include "..\..\..\Source\Parsing\ParsingDefinitions.h"
#include "..\..\..\Source\Parsing\ParsingAutomaton.h"

namespace test
{
	namespace parser
	{
		enum class CalParserTokenIndex
		{
			NAME = 0,
			NUMBER = 1,
			ADD = 2,
			SUB = 3,
			MUL = 4,
			DIV = 5,
			LEFT = 6,
			RIGHT = 7,
			COMMA = 8,
			SPACE = 9,
		};
		class CalExpression;
		class CalNumberExpression;
		class CalBinaryExpression;
		class CalFunctionExpression;

		class CalExpression abstract : public vl::parsing::ParsingTreeCustomBase
		{
		public:
			class IVisitor : public vl::Interface
			{
			public:
				virtual void Visit(CalNumberExpression* node)=0;
				virtual void Visit(CalBinaryExpression* node)=0;
				virtual void Visit(CalFunctionExpression* node)=0;
			};

			virtual void Accept(CalExpression::IVisitor* visitor)=0;

		};

		class CalNumberExpression : public CalExpression
		{
		public:
			vl::parsing::ParsingToken value;

			void Accept(CalExpression::IVisitor* visitor)override;

			static vl::Ptr<CalNumberExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class CalBinaryExpression : public CalExpression
		{
		public:
			enum class CalBinaryOperator
			{
				Add,
				Sub,
				Mul,
				Div,
			};

			vl::Ptr<CalExpression> firstOperand;
			vl::Ptr<CalExpression> secondOperand;
			CalBinaryOperator binaryOperator;

			void Accept(CalExpression::IVisitor* visitor)override;

			static vl::Ptr<CalBinaryExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class CalFunctionExpression : public CalExpression
		{
		public:
			vl::parsing::ParsingToken functionName;
			vl::collections::List<vl::Ptr<CalExpression>> arguments;

			void Accept(CalExpression::IVisitor* visitor)override;

			static vl::Ptr<CalFunctionExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		extern vl::WString CalGetParserTextBuffer();
		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> CalConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> CalLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

		extern vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);
		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

	}
}
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			DECL_TYPE_INFO(test::parser::CalExpression)
			DECL_TYPE_INFO(test::parser::CalNumberExpression)
			DECL_TYPE_INFO(test::parser::CalBinaryExpression)
			DECL_TYPE_INFO(test::parser::CalBinaryExpression::CalBinaryOperator)
			DECL_TYPE_INFO(test::parser::CalFunctionExpression)
#endif

			extern bool CalLoadTypes();
		}
	}
}
