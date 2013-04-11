/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Parser.Calculator

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#include "..\..\..\Source\Parsing\Parsing.h"

namespace test
{
	namespace parser
	{
		struct CalParserTokenIndex abstract
		{
			static const vl::vint NAME = 0;
			static const vl::vint NUMBER = 1;
			static const vl::vint ADD = 2;
			static const vl::vint SUB = 3;
			static const vl::vint MUL = 4;
			static const vl::vint DIV = 5;
			static const vl::vint LEFT = 6;
			static const vl::vint RIGHT = 7;
			static const vl::vint COMMA = 8;
			static const vl::vint SPACE = 9;
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
			struct CalBinaryOperator abstract
			{
				enum Type
				{
					Add,
					Sub,
					Mul,
					Div,
				};
			};

			vl::Ptr<CalExpression> firstOperand;
			vl::Ptr<CalExpression> secondOperand;
			CalBinaryOperator::Type binaryOperator;

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

		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> CalConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> CalLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

	}
}
