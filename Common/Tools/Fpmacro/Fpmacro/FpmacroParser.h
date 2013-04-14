/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::FpmacroParser

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#ifndef FPMACRO_PARSER
#define FPMACRO_PARSER

#include "..\..\..\Source\Parsing\Parsing.h"
#include "..\..\..\Source\Parsing\ParsingAutomaton.h"

namespace fpmacro
{
	namespace parser
	{
		struct FpmParserTokenIndex abstract
		{
			static const vl::vint BRACKET_OPEN = 0;
			static const vl::vint BRACKET_CLOSE = 1;
			static const vl::vint ARRAY = 2;
			static const vl::vint DEFINE = 3;
			static const vl::vint BEGIN = 4;
			static const vl::vint END = 5;
			static const vl::vint COMMA = 6;
			static const vl::vint NAME = 7;
			static const vl::vint NEW_LINE = 8;
			static const vl::vint SPACE = 9;
			static const vl::vint TEXT_FRAGMENT = 10;
			static const vl::vint BRACKET = 11;
		};
		class FpmExpression;
		class FpmConcatExpression;
		class FpmArrayExpression;
		class FpmInvokeExpression;
		class FpmBracketExpression;
		class FpmReferenceExpression;
		class FpmTextExpression;
		class FpmDefinition;
		class FpmExpressionDefinition;
		class FpmReferenceDefinition;
		class FpmMacro;

		class FpmExpression abstract : public vl::parsing::ParsingTreeCustomBase
		{
		public:
			class IVisitor : public vl::Interface
			{
			public:
				virtual void Visit(FpmConcatExpression* node)=0;
				virtual void Visit(FpmArrayExpression* node)=0;
				virtual void Visit(FpmInvokeExpression* node)=0;
				virtual void Visit(FpmBracketExpression* node)=0;
				virtual void Visit(FpmReferenceExpression* node)=0;
				virtual void Visit(FpmTextExpression* node)=0;
			};

			virtual void Accept(FpmExpression::IVisitor* visitor)=0;

		};

		class FpmConcatExpression : public FpmExpression
		{
		public:
			vl::collections::List<vl::Ptr<FpmExpression>> expressions;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmConcatExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmArrayExpression : public FpmExpression
		{
		public:
			vl::collections::List<vl::Ptr<FpmExpression>> elements;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmArrayExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmInvokeExpression : public FpmExpression
		{
		public:
			vl::Ptr<FpmExpression> function;
			vl::collections::List<vl::Ptr<FpmExpression>> arguments;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmInvokeExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmBracketExpression : public FpmExpression
		{
		public:
			vl::Ptr<FpmExpression> expression;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmBracketExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmReferenceExpression : public FpmExpression
		{
		public:
			vl::parsing::ParsingToken name;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmReferenceExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmTextExpression : public FpmExpression
		{
		public:
			vl::parsing::ParsingToken text;

			void Accept(FpmExpression::IVisitor* visitor)override;

			static vl::Ptr<FpmTextExpression> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmDefinition abstract : public vl::parsing::ParsingTreeCustomBase
		{
		public:
			class IVisitor : public vl::Interface
			{
			public:
				virtual void Visit(FpmExpressionDefinition* node)=0;
				virtual void Visit(FpmReferenceDefinition* node)=0;
			};

			virtual void Accept(FpmDefinition::IVisitor* visitor)=0;

		};

		class FpmExpressionDefinition : public FpmDefinition
		{
		public:
			vl::Ptr<FpmExpression> expression;

			void Accept(FpmDefinition::IVisitor* visitor)override;

			static vl::Ptr<FpmExpressionDefinition> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmReferenceDefinition : public FpmDefinition
		{
		public:
			vl::parsing::ParsingToken name;
			vl::collections::List<vl::parsing::ParsingToken> parameters;
			vl::collections::List<vl::Ptr<FpmDefinition>> definitions;

			void Accept(FpmDefinition::IVisitor* visitor)override;

			static vl::Ptr<FpmReferenceDefinition> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class FpmMacro : public vl::parsing::ParsingTreeCustomBase
		{
		public:
			vl::collections::List<vl::Ptr<FpmDefinition>> definitions;

			static vl::Ptr<FpmMacro> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> FpmConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> FpmLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> FpmParseFpmacroCodeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);
		extern vl::Ptr<FpmMacro> FpmParseFpmacroCode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);

	}
}
#endif