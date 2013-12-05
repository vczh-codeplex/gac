#include "WfExpression.h"

namespace vl
{
	namespace workflow
	{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n"L""
L"\r\n"L"class Expression"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum LiteralValue"
L"\r\n"L"{"
L"\r\n"L"\tNull,"
L"\r\n"L"\tTrue,"
L"\r\n"L"\tFalse,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class LiteralExpression"
L"\r\n"L"{"
L"\r\n"L"\tLiteralValue\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FloatingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class IntegerExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class StringExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FormatExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum UnaryOperator"
L"\r\n"L"{"
L"\r\n"L"\tPositive,"
L"\r\n"L"\tNegative,"
L"\r\n"L"\tNot,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class UnaryExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tUnaryOperator\t\top;"
L"\r\n"L"\tExpression\t\t\toperand;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum BinaryOperator"
L"\r\n"L"{"
L"\r\n"L"\tIndex,"
L"\r\n"L"\tExp,"
L"\r\n"L"\tAdd,"
L"\r\n"L"\tSub,"
L"\r\n"L"\tMul,"
L"\r\n"L"\tDiv,"
L"\r\n"L"\tConcat,"
L"\r\n"L"\tShl,"
L"\r\n"L"\tShr,"
L"\r\n"L"\tLT,"
L"\r\n"L"\tGT,"
L"\r\n"L"\tLE,"
L"\r\n"L"\tGE,"
L"\r\n"L"\tEQ,"
L"\r\n"L"\tNE,"
L"\r\n"L"\tXor,"
L"\r\n"L"\tAnd,"
L"\r\n"L"\tNot,"
L"\r\n"L"\tFailedThen,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BinaryExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tBinaryOperator\t\top;"
L"\r\n"L"\tExpression\t\t\tfirst;"
L"\r\n"L"\tExpression\t\t\tsecond;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token EXP = \"/^\";"
L"\r\n"L"token ADD = \"/+\";"
L"\r\n"L"token SUB = \"-\";"
L"\r\n"L"token MUL = \"/*\";"
L"\r\n"L"token DIV = \"/\\\";"
L"\r\n"L"token CONCAT = \"&\";"
L"\r\n"L"token LE = \"/<=\";"
L"\r\n"L"token GE = \"/>=\";"
L"\r\n"L"token LT = \"/<\";"
L"\r\n"L"token GT = \"/>\";"
L"\r\n"L"token EQ = \"=\";"
L"\r\n"L"token NE = \"!=\";"
L"\r\n"L"token FAILED_THEN = \"/?/?\";"
L"\r\n"L"token OPEN_ARRAY = \"/[\";"
L"\r\n"L"token CLOSE_ARRAY = \"/]\";"
L"\r\n"L"token OPEN_BRACE = \"/{\";"
L"\r\n"L"token CLOSE_BRACE = \"/}\";"
L"\r\n"L""
L"\r\n"L"token SHL = \"shl\";"
L"\r\n"L"token SHR = \"shr\";"
L"\r\n"L"token XOR = \"xor\";"
L"\r\n"L"token AND = \"and\";"
L"\r\n"L"token OR = \"or\";"
L"\r\n"L"token NOT = \"not\";"
L"\r\n"L"token NULL = \"null\";"
L"\r\n"L"token TRUE = \"true\";"
L"\r\n"L"token FALSE = \"false\";"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z_]/w*\";"
L"\r\n"L"token FLOAT = \"/d+(./d+)?\";"
L"\r\n"L"token INTEGER = \"/d+(./d+)?\";"
L"\r\n"L"token STRING = \"(\'[^\']*\')+|(\"\"[^\"\"]*\"\")+\";"
L"\r\n"L"token FORMATSTRING = \"/$((\'[^\']*\')+|(\"\"[^\"\"]*\"\")+)\";"
L"\r\n"L""
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule LiteralExpression Literal"
L"\r\n"L"\t= \"null\" as LiteralExpression with {value = \"Null\"}"
L"\r\n"L"\t= \"true\" as LiteralExpression with {value = \"True\"}"
L"\r\n"L"\t= \"false\" as LiteralExpression with {value = \"False\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule IntegerExpression Integer"
L"\r\n"L"\t= INTEGER : value as IntegerExpression"
L"\r\n"L"\t;"
L"\r\n"L"rule FloatingExpression FloatingPoint"
L"\r\n"L"\t= FLOAT : value as FloatingExpression"
L"\r\n"L"\t;"
L"\r\n"L"rule StringExpression String"
L"\r\n"L"\t= STRING : value as StringExpression"
L"\r\n"L"\t;"
L"\r\n"L"rule FormatExpression FormatString"
L"\r\n"L"\t= FORMATSTRING : value as FormatExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression Constant"
L"\r\n"L"\t= !Literal"
L"\r\n"L"\t= !Integer"
L"\r\n"L"\t= !FloatingPoint"
L"\r\n"L"\t= !String"
L"\r\n"L"\t= !FormatString"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression WorkflowExpression"
L"\r\n"L"\t= !Constant"
L"\r\n"L"\t;"
;

		vl::WString WfGetParserTextBuffer()
		{
			return parserTextBuffer;
		}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

		class WfTreeConverter : public vl::parsing::ParsingTreeConverter
		{
		public:
			using vl::parsing::ParsingTreeConverter::SetMember;

			bool SetMember(WfLiteralValue& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Null") { member=WfLiteralValue::Null; return true; }
					else if(token->GetValue()==L"True") { member=WfLiteralValue::True; return true; }
					else if(token->GetValue()==L"False") { member=WfLiteralValue::False; return true; }
					else { member=WfLiteralValue::Null; return false; }
				}
				member=WfLiteralValue::Null;
				return false;
			}

			bool SetMember(WfUnaryOperator& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Positive") { member=WfUnaryOperator::Positive; return true; }
					else if(token->GetValue()==L"Negative") { member=WfUnaryOperator::Negative; return true; }
					else if(token->GetValue()==L"Not") { member=WfUnaryOperator::Not; return true; }
					else { member=WfUnaryOperator::Positive; return false; }
				}
				member=WfUnaryOperator::Positive;
				return false;
			}

			bool SetMember(WfBinaryOperator& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Index") { member=WfBinaryOperator::Index; return true; }
					else if(token->GetValue()==L"Exp") { member=WfBinaryOperator::Exp; return true; }
					else if(token->GetValue()==L"Add") { member=WfBinaryOperator::Add; return true; }
					else if(token->GetValue()==L"Sub") { member=WfBinaryOperator::Sub; return true; }
					else if(token->GetValue()==L"Mul") { member=WfBinaryOperator::Mul; return true; }
					else if(token->GetValue()==L"Div") { member=WfBinaryOperator::Div; return true; }
					else if(token->GetValue()==L"Concat") { member=WfBinaryOperator::Concat; return true; }
					else if(token->GetValue()==L"Shl") { member=WfBinaryOperator::Shl; return true; }
					else if(token->GetValue()==L"Shr") { member=WfBinaryOperator::Shr; return true; }
					else if(token->GetValue()==L"LT") { member=WfBinaryOperator::LT; return true; }
					else if(token->GetValue()==L"GT") { member=WfBinaryOperator::GT; return true; }
					else if(token->GetValue()==L"LE") { member=WfBinaryOperator::LE; return true; }
					else if(token->GetValue()==L"GE") { member=WfBinaryOperator::GE; return true; }
					else if(token->GetValue()==L"EQ") { member=WfBinaryOperator::EQ; return true; }
					else if(token->GetValue()==L"NE") { member=WfBinaryOperator::NE; return true; }
					else if(token->GetValue()==L"Xor") { member=WfBinaryOperator::Xor; return true; }
					else if(token->GetValue()==L"And") { member=WfBinaryOperator::And; return true; }
					else if(token->GetValue()==L"Not") { member=WfBinaryOperator::Not; return true; }
					else if(token->GetValue()==L"FailedThen") { member=WfBinaryOperator::FailedThen; return true; }
					else { member=WfBinaryOperator::Index; return false; }
				}
				member=WfBinaryOperator::Index;
				return false;
			}

			void Fill(vl::Ptr<WfExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfLiteralExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfFloatingExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfIntegerExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfStringExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfFormatExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfUnaryExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->op, obj->GetMember(L"op"), tokens);
				SetMember(tree->operand, obj->GetMember(L"operand"), tokens);
			}

			void Fill(vl::Ptr<WfBinaryExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->op, obj->GetMember(L"op"), tokens);
				SetMember(tree->first, obj->GetMember(L"first"), tokens);
				SetMember(tree->second, obj->GetMember(L"second"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"LiteralExpression")
				{
					vl::Ptr<WfLiteralExpression> tree = new WfLiteralExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FloatingExpression")
				{
					vl::Ptr<WfFloatingExpression> tree = new WfFloatingExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"IntegerExpression")
				{
					vl::Ptr<WfIntegerExpression> tree = new WfIntegerExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"StringExpression")
				{
					vl::Ptr<WfStringExpression> tree = new WfStringExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FormatExpression")
				{
					vl::Ptr<WfFormatExpression> tree = new WfFormatExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"UnaryExpression")
				{
					vl::Ptr<WfUnaryExpression> tree = new WfUnaryExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BinaryExpression")
				{
					vl::Ptr<WfBinaryExpression> tree = new WfBinaryExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else 
					return 0;
			}
		};

		vl::Ptr<vl::parsing::ParsingTreeCustomBase> WfConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			WfTreeConverter converter;
			vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
			converter.SetMember(tree, node, tokens);
			return tree;
		}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

		vl::Ptr<WfLiteralExpression> WfLiteralExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfLiteralExpression>();
		}

		vl::Ptr<WfFloatingExpression> WfFloatingExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFloatingExpression>();
		}

		vl::Ptr<WfIntegerExpression> WfIntegerExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfIntegerExpression>();
		}

		vl::Ptr<WfStringExpression> WfStringExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfStringExpression>();
		}

		vl::Ptr<WfFormatExpression> WfFormatExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFormatExpression>();
		}

		vl::Ptr<WfUnaryExpression> WfUnaryExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfUnaryExpression>();
		}

		vl::Ptr<WfBinaryExpression> WfBinaryExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfBinaryExpression>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void WfFloatingExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfIntegerExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfStringExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfFormatExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfUnaryExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfBinaryExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowExpression");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseExpressionAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowExpression");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfExpression>();
			}
			return 0;
		}

		vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseExpression(input, table, errors);
		}

/***********************************************************************
Table Generation
***********************************************************************/

		vl::Ptr<vl::parsing::tabling::ParsingTable> WfLoadTable()
		{
		    vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();
		    vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
		    vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(parserTextBuffer, L"ParserDecl", errors);
		    vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);
		    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, false, errors);
		    return table;
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
			using namespace vl::workflow;

			IMPL_TYPE_INFO_RENAME(WfExpression, Workflow::WfExpression)
			IMPL_TYPE_INFO_RENAME(WfLiteralValue, Workflow::WfLiteralValue)
			IMPL_TYPE_INFO_RENAME(WfLiteralExpression, Workflow::WfLiteralExpression)
			IMPL_TYPE_INFO_RENAME(WfFloatingExpression, Workflow::WfFloatingExpression)
			IMPL_TYPE_INFO_RENAME(WfIntegerExpression, Workflow::WfIntegerExpression)
			IMPL_TYPE_INFO_RENAME(WfStringExpression, Workflow::WfStringExpression)
			IMPL_TYPE_INFO_RENAME(WfFormatExpression, Workflow::WfFormatExpression)
			IMPL_TYPE_INFO_RENAME(WfUnaryOperator, Workflow::WfUnaryOperator)
			IMPL_TYPE_INFO_RENAME(WfUnaryExpression, Workflow::WfUnaryExpression)
			IMPL_TYPE_INFO_RENAME(WfBinaryOperator, Workflow::WfBinaryOperator)
			IMPL_TYPE_INFO_RENAME(WfBinaryExpression, Workflow::WfBinaryExpression)
			IMPL_TYPE_INFO_RENAME(WfExpression::IVisitor, Workflow::WfExpression::IVisitor)

			BEGIN_CLASS_MEMBER(WfExpression)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfExpression)

			BEGIN_ENUM_ITEM(WfLiteralValue)
				ENUM_ITEM_NAMESPACE(WfLiteralValue)
				ENUM_NAMESPACE_ITEM(Null)
				ENUM_NAMESPACE_ITEM(True)
				ENUM_NAMESPACE_ITEM(False)
			END_ENUM_ITEM(WfLiteralValue)

			BEGIN_CLASS_MEMBER(WfLiteralExpression)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfLiteralExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(WfLiteralExpression)

			BEGIN_CLASS_MEMBER(WfFloatingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFloatingExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfFloatingExpression::*)(), [](WfFloatingExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfFloatingExpression::*)(const vl::WString&), [](WfFloatingExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfFloatingExpression)

			BEGIN_CLASS_MEMBER(WfIntegerExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfIntegerExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfIntegerExpression::*)(), [](WfIntegerExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfIntegerExpression::*)(const vl::WString&), [](WfIntegerExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfIntegerExpression)

			BEGIN_CLASS_MEMBER(WfStringExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfStringExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfStringExpression::*)(), [](WfStringExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfStringExpression::*)(const vl::WString&), [](WfStringExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfStringExpression)

			BEGIN_CLASS_MEMBER(WfFormatExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFormatExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfFormatExpression::*)(), [](WfFormatExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfFormatExpression::*)(const vl::WString&), [](WfFormatExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfFormatExpression)

			BEGIN_ENUM_ITEM(WfUnaryOperator)
				ENUM_ITEM_NAMESPACE(WfUnaryOperator)
				ENUM_NAMESPACE_ITEM(Positive)
				ENUM_NAMESPACE_ITEM(Negative)
				ENUM_NAMESPACE_ITEM(Not)
			END_ENUM_ITEM(WfUnaryOperator)

			BEGIN_CLASS_MEMBER(WfUnaryExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfUnaryExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(op)
				CLASS_MEMBER_FIELD(operand)
			END_CLASS_MEMBER(WfUnaryExpression)

			BEGIN_ENUM_ITEM(WfBinaryOperator)
				ENUM_ITEM_NAMESPACE(WfBinaryOperator)
				ENUM_NAMESPACE_ITEM(Index)
				ENUM_NAMESPACE_ITEM(Exp)
				ENUM_NAMESPACE_ITEM(Add)
				ENUM_NAMESPACE_ITEM(Sub)
				ENUM_NAMESPACE_ITEM(Mul)
				ENUM_NAMESPACE_ITEM(Div)
				ENUM_NAMESPACE_ITEM(Concat)
				ENUM_NAMESPACE_ITEM(Shl)
				ENUM_NAMESPACE_ITEM(Shr)
				ENUM_NAMESPACE_ITEM(LT)
				ENUM_NAMESPACE_ITEM(GT)
				ENUM_NAMESPACE_ITEM(LE)
				ENUM_NAMESPACE_ITEM(GE)
				ENUM_NAMESPACE_ITEM(EQ)
				ENUM_NAMESPACE_ITEM(NE)
				ENUM_NAMESPACE_ITEM(Xor)
				ENUM_NAMESPACE_ITEM(And)
				ENUM_NAMESPACE_ITEM(Not)
				ENUM_NAMESPACE_ITEM(FailedThen)
			END_ENUM_ITEM(WfBinaryOperator)

			BEGIN_CLASS_MEMBER(WfBinaryExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfBinaryExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(op)
				CLASS_MEMBER_FIELD(first)
				CLASS_MEMBER_FIELD(second)
			END_CLASS_MEMBER(WfBinaryExpression)

			BEGIN_CLASS_MEMBER(WfExpression::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfExpression::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfExpression_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFloatingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfIntegerExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfStringExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFormatExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfUnaryExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfBinaryExpression* node))
			END_CLASS_MEMBER(WfExpression)

			class WfTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::workflow::WfExpression)
					ADD_TYPE_INFO(vl::workflow::WfLiteralValue)
					ADD_TYPE_INFO(vl::workflow::WfLiteralExpression)
					ADD_TYPE_INFO(vl::workflow::WfFloatingExpression)
					ADD_TYPE_INFO(vl::workflow::WfIntegerExpression)
					ADD_TYPE_INFO(vl::workflow::WfStringExpression)
					ADD_TYPE_INFO(vl::workflow::WfFormatExpression)
					ADD_TYPE_INFO(vl::workflow::WfUnaryOperator)
					ADD_TYPE_INFO(vl::workflow::WfUnaryExpression)
					ADD_TYPE_INFO(vl::workflow::WfBinaryOperator)
					ADD_TYPE_INFO(vl::workflow::WfBinaryExpression)
					ADD_TYPE_INFO(vl::workflow::WfExpression::IVisitor)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool WfLoadTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new WfTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}
