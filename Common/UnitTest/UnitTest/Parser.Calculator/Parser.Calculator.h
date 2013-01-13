/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::Parser.Calculator

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/

#include "..\..\..\Source\Parsing\Parsing.h"

namespace test
{
	namespace parser
	{
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

		extern vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
	}
}
