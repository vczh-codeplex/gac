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
L"\r\n"L"class Type"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ReferenceType : Type"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class SharedPointerType : Type"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\telement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class EnumerableType : Type"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\telement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class MapType : Type"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\tkey;"
L"\r\n"L"\tType\t\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FunctionType : Type"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\tresult;"
L"\r\n"L"\tType[]\t\t\t\t\targuments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ChildType : Type"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\tparent;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
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
L"\r\n"L"class ReferenceExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class MemberExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tparent;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class LiteralExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tLiteralValue\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FloatingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class IntegerExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class StringExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FormatExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tvalue;"
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
L"\r\n"L"\tUnaryOperator\t\t\top;"
L"\r\n"L"\tExpression\t\t\t\toperand;"
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
L"\r\n"L"\tOr,"
L"\r\n"L"\tNot,"
L"\r\n"L"\tFailedThen,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BinaryExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tBinaryOperator\t\t\top;"
L"\r\n"L"\tExpression\t\t\t\tfirst;"
L"\r\n"L"\tExpression\t\t\t\tsecond;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class LetExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tExpression\t\t\t\tvalue;"
L"\r\n"L"\tExpression\t\t\t\texp;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class IfExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tcondition;"
L"\r\n"L"\tExpression\t\t\t\ttrueBranch;"
L"\r\n"L"\tExpression\t\t\t\tfalseBranch;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum RangeBoundary"
L"\r\n"L"{"
L"\r\n"L"\tInclusive,"
L"\r\n"L"\tExclusive,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class RangeExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tbegin;"
L"\r\n"L"\tRangeBoundary\t\t\tbeginBoundary;"
L"\r\n"L"\tExpression\t\t\t\tend;"
L"\r\n"L"\tRangeBoundary\t\t\tendBoundary;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class SetTestingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\telement;"
L"\r\n"L"\tExpression\t\t\t\tcollection;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ConstructorArgument"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tkey;"
L"\r\n"L"\tExpression\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ConstructorExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tConstructorArgument[]\targuments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class InferExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum TypeCastingStrategy"
L"\r\n"L"{"
L"\r\n"L"\tStrong,"
L"\r\n"L"\tWeak,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TypeCastingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tTypeCastingStrategy\t\tstrategy;"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TypeTestingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TypeOfTypeExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TypeOfExpressionExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class CallExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tfunction;"
L"\r\n"L"\tExpression[]\t\t\targuments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token EXP = \"/^\";"
L"\r\n"L"token ADD = \"/+\";"
L"\r\n"L"token SUB = \"-\";"
L"\r\n"L"token MUL = \"/*\";"
L"\r\n"L"token DIV = \"//\";"
L"\r\n"L"token CONCAT = \"&\";"
L"\r\n"L"token LE = \"/</=\";"
L"\r\n"L"token GE = \"/>/=\";"
L"\r\n"L"token LT = \"/<\";"
L"\r\n"L"token GT = \"/>\";"
L"\r\n"L"token EQ = \"/=\";"
L"\r\n"L"token NE = \"/!/=\";"
L"\r\n"L"token FAILED_THEN = \"/?/?\";"
L"\r\n"L"token QUESTION_MARK = \"/?\";"
L"\r\n"L"token COLON = \":\";"
L"\r\n"L"token SEMICOLON = \";\";"
L"\r\n"L"token COMMA = \",\";"
L"\r\n"L"token DOT = \".\";"
L"\r\n"L"token OPEN_ARRAY = \"/[\";"
L"\r\n"L"token CLOSE_ARRAY = \"/]\";"
L"\r\n"L"token OPEN_BRACE = \"/{\";"
L"\r\n"L"token CLOSE_BRACE = \"/}\";"
L"\r\n"L"token OPEN_BRACKET = \"/(\";"
L"\r\n"L"token CLOSE_BRACKET = \"/)\";"
L"\r\n"L""
L"\r\n"L"token KEYWORD_SHL = \"shl\";"
L"\r\n"L"token KEYWORD_SHR = \"shr\";"
L"\r\n"L"token KEYWORD_XOR = \"xor\";"
L"\r\n"L"token KEYWORD_AND = \"and\";"
L"\r\n"L"token KEYWORD_OR = \"or\";"
L"\r\n"L"token KEYWORD_NOT = \"not\";"
L"\r\n"L"token KEYWORD_NULL= \"null\";"
L"\r\n"L"token KEYWORD_TRUE= \"true\";"
L"\r\n"L"token KEYWORD_FALSE = \"false\";"
L"\r\n"L"token KEYWORD_LET = \"let\";"
L"\r\n"L"token KEYWORD_IN = \"in\";"
L"\r\n"L"token KEYWORD_RANGE = \"range\";"
L"\r\n"L"token KEYWORD_OF = \"of\";"
L"\r\n"L"token KEYWORD_AS = \"as\";"
L"\r\n"L"token KEYWORD_IS = \"is\";"
L"\r\n"L"token KEYWORD_CAST = \"cast\";"
L"\r\n"L"token KEYWORD_FUNC = \"func\";"
L"\r\n"L"token KEYWORD_TYPEOF = \"typeof\";"
L"\r\n"L"token KEYWORD_TYPE = \"type\";"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z_]/w*\";"
L"\r\n"L"token FLOAT = \"/d+(./d+)?\";"
L"\r\n"L"token INTEGER = \"/d+(./d+)?\";"
L"\r\n"L"token STRING = \"(\'[^\']*\')+|(\"\"[^\"\"]*\"\")+\";"
L"\r\n"L"token FORMATSTRING = \"/$((\'[^\']*\')+|(\"\"[^\"\"]*\"\")+)\";"
L"\r\n"L""
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule Type WorkflowType"
L"\r\n"L"\t= NAME : name as ReferenceType"
L"\r\n"L"\t= \"func\" \"(\" [WorkflowType : arguments {\",\" WorkflowType : arguments}] \")\" \":\" WorkflowType : result as FunctionType"
L"\r\n"L"\t= WorkflowType : element \"^\" as SharedPointerType"
L"\r\n"L"\t= WorkflowType : element \"{\" \"}\" as EnumerableType"
L"\r\n"L"\t= WorkflowType : value \"[\" [WorkflowType : key] \"]\" as MapType"
L"\r\n"L"\t;"
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
L"\r\n"L"rule ConstructorArgument CtorFragment"
L"\r\n"L"\t= WorkflowExpression : key [ \":\" WorkflowExpression : value] as ConstructorArgument"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression Exp0"
L"\r\n"L"\t= !Literal"
L"\r\n"L"\t= !Integer"
L"\r\n"L"\t= !FloatingPoint"
L"\r\n"L"\t= !String"
L"\r\n"L"\t= !FormatString"
L"\r\n"L"\t= NAME : name as ReferenceExpression"
L"\r\n"L"\t= \"(\" !WorkflowExpression \")\""
L"\r\n"L"\t= \"let\" NAME : name \"in\" WorkflowExpression : value \"=\" WorkflowExpression : exp as LetExpression"
L"\r\n"L"\t= \"+\" Exp0 as UnaryExpression with {op = \"Positive\"}"
L"\r\n"L"\t= \"-\" Exp0 as UnaryExpression with {op = \"Negative\"}"
L"\r\n"L"\t= \"not\" Exp0 as UnaryExpression with {op = \"Not\"}"
L"\r\n"L"\t= \"range\""
L"\r\n"L"\t\t((\"(\" with {beginBoundary = \"Exclusive\"}) | (\"[\" with {beginBoundary = \"Inclusive\"}))"
L"\r\n"L"\t\tWorkflowExpression : begin \",\" WorkflowExpression : end"
L"\r\n"L"\t\t((\")\" with {beginBoundary = \"Exclusive\"}) | (\"]\" with {beginBoundary = \"Inclusive\"}))"
L"\r\n"L"\t\tas RangeExpression"
L"\r\n"L"\t= \"{\" {CtorFragment : arguments} \"}\" as ConstructorExpression"
L"\r\n"L"\t= Exp0 : parent \".\" NAME : name as MemberExpression"
L"\r\n"L"\t= Exp0 : first \"[\" WorkflowExpression : second \"]\" as BinaryExpression with {op = \"Index\"}"
L"\r\n"L"\t= Exp0 : element \"in\" WorkflowExpression : collection as SetTestingExpression"
L"\r\n"L"\t= Exp0 : expression \"of\" WorkflowType : type as InferExpression"
L"\r\n"L"\t= Exp0 : expression \"is\" WorkflowType : type as TypeTestingExpression"
L"\r\n"L"\t= Exp0 : expression \"as\" WorkflowType : type as TypeCastingExpression with {strategy = \"Weak\"}"
L"\r\n"L"\t= \"cast\" WorkflowType : type Exp0 : expression as TypeCastingExpression with {strategy = \"Strong\"}"
L"\r\n"L"\t= \"typeof\" \"(\" WorkflowType : type \")\" as TypeOfTypeExpression"
L"\r\n"L"\t= \"type\" \"(\" WorkflowExpression : expression \")\" as TypeOfExpressionExpression"
L"\r\n"L"\t= Exp0 : function \"(\" [WorkflowExpression : arguments {\",\" WorkflowExpression : arguments}] \")\" as CallExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression Exp1"
L"\r\n"L"\t= !Exp0"
L"\r\n"L"\t= Exp1 : first \"^\" Exp0 : second as BinaryExpression with {op = \"Exp\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp2"
L"\r\n"L"\t= !Exp1"
L"\r\n"L"\t= Exp2 : first \"*\" Exp1 : second as BinaryExpression with {op = \"Mul\"}"
L"\r\n"L"\t= Exp2 : first \"/\" Exp1 : second as BinaryExpression with {op = \"Div\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp3"
L"\r\n"L"\t= !Exp2"
L"\r\n"L"\t= Exp3 : first \"+\" Exp2 : second as BinaryExpression with {op = \"Add\"}"
L"\r\n"L"\t= Exp3 : first \"-\" Exp2 : second as BinaryExpression with {op = \"Sub\"}"
L"\r\n"L"\t= Exp3 : first \"&\" Exp2 : second as BinaryExpression with {op = \"Concat\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp4"
L"\r\n"L"\t= !Exp3"
L"\r\n"L"\t= Exp4 : first \"shl\" Exp3 : second as BinaryExpression with {op = \"Shl\"}"
L"\r\n"L"\t= Exp4 : first \"shr\" Exp3 : second as BinaryExpression with {op = \"Shr\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp5"
L"\r\n"L"\t= !Exp4"
L"\r\n"L"\t= Exp5 : first \"<\" Exp4 : second as BinaryExpression with {op = \"LT\"}"
L"\r\n"L"\t= Exp5 : first \">\" Exp4 : second as BinaryExpression with {op = \"GT\"}"
L"\r\n"L"\t= Exp5 : first \"<=\" Exp4 : second as BinaryExpression with {op = \"LE\"}"
L"\r\n"L"\t= Exp5 : first \">=\" Exp4 : second as BinaryExpression with {op = \"GE\"}"
L"\r\n"L"\t= Exp5 : first \"=\" Exp4 : second as BinaryExpression with {op = \"EQ\"}"
L"\r\n"L"\t= Exp5 : first \"!=\" Exp4 : second as BinaryExpression with {op = \"NE\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp6"
L"\r\n"L"\t= !Exp5"
L"\r\n"L"\t= Exp6 : first \"xor\" Exp5 : second as BinaryExpression with {op = \"Xor\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp7"
L"\r\n"L"\t= !Exp6"
L"\r\n"L"\t= Exp7 : first \"and\" Exp6 : second as BinaryExpression with {op = \"And\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp8"
L"\r\n"L"\t= !Exp7"
L"\r\n"L"\t= Exp8 : first \"or\" Exp7 : second as BinaryExpression with {op = \"Or\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp9"
L"\r\n"L"\t= !Exp8"
L"\r\n"L"\t= Exp9 : first \"??\" Exp8 : second as BinaryExpression with {op = \"FailedThen\"}"
L"\r\n"L"\t;"
L"\r\n"L"rule Expression Exp10"
L"\r\n"L"\t= !Exp9"
L"\r\n"L"\t= Exp9 : condition \"?\" Exp10 : trueBranch \":\" Exp10 : falseBranch as IfExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression WorkflowExpression"
L"\r\n"L"\t= !Exp10"
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
					else if(token->GetValue()==L"Or") { member=WfBinaryOperator::Or; return true; }
					else if(token->GetValue()==L"Not") { member=WfBinaryOperator::Not; return true; }
					else if(token->GetValue()==L"FailedThen") { member=WfBinaryOperator::FailedThen; return true; }
					else { member=WfBinaryOperator::Index; return false; }
				}
				member=WfBinaryOperator::Index;
				return false;
			}

			bool SetMember(WfRangeBoundary& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Inclusive") { member=WfRangeBoundary::Inclusive; return true; }
					else if(token->GetValue()==L"Exclusive") { member=WfRangeBoundary::Exclusive; return true; }
					else { member=WfRangeBoundary::Inclusive; return false; }
				}
				member=WfRangeBoundary::Inclusive;
				return false;
			}

			bool SetMember(WfTypeCastingStrategy& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Strong") { member=WfTypeCastingStrategy::Strong; return true; }
					else if(token->GetValue()==L"Weak") { member=WfTypeCastingStrategy::Weak; return true; }
					else { member=WfTypeCastingStrategy::Strong; return false; }
				}
				member=WfTypeCastingStrategy::Strong;
				return false;
			}

			void Fill(vl::Ptr<WfType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfReferenceType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfSharedPointerType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->element, obj->GetMember(L"element"), tokens);
			}

			void Fill(vl::Ptr<WfEnumerableType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->element, obj->GetMember(L"element"), tokens);
			}

			void Fill(vl::Ptr<WfMapType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->key, obj->GetMember(L"key"), tokens);
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfFunctionType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->result, obj->GetMember(L"result"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			void Fill(vl::Ptr<WfChildType> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->parent, obj->GetMember(L"parent"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfReferenceExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfMemberExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->parent, obj->GetMember(L"parent"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
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

			void Fill(vl::Ptr<WfLetExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
				SetMember(tree->exp, obj->GetMember(L"exp"), tokens);
			}

			void Fill(vl::Ptr<WfIfExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->condition, obj->GetMember(L"condition"), tokens);
				SetMember(tree->trueBranch, obj->GetMember(L"trueBranch"), tokens);
				SetMember(tree->falseBranch, obj->GetMember(L"falseBranch"), tokens);
			}

			void Fill(vl::Ptr<WfRangeExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->begin, obj->GetMember(L"begin"), tokens);
				SetMember(tree->beginBoundary, obj->GetMember(L"beginBoundary"), tokens);
				SetMember(tree->end, obj->GetMember(L"end"), tokens);
				SetMember(tree->endBoundary, obj->GetMember(L"endBoundary"), tokens);
			}

			void Fill(vl::Ptr<WfSetTestingExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->element, obj->GetMember(L"element"), tokens);
				SetMember(tree->collection, obj->GetMember(L"collection"), tokens);
			}

			void Fill(vl::Ptr<WfConstructorArgument> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->key, obj->GetMember(L"key"), tokens);
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfConstructorExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			void Fill(vl::Ptr<WfInferExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
			}

			void Fill(vl::Ptr<WfTypeCastingExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->strategy, obj->GetMember(L"strategy"), tokens);
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
			}

			void Fill(vl::Ptr<WfTypeTestingExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
			}

			void Fill(vl::Ptr<WfTypeOfTypeExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
			}

			void Fill(vl::Ptr<WfTypeOfExpressionExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfCallExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->function, obj->GetMember(L"function"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"ReferenceType")
				{
					vl::Ptr<WfReferenceType> tree = new WfReferenceType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"SharedPointerType")
				{
					vl::Ptr<WfSharedPointerType> tree = new WfSharedPointerType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"EnumerableType")
				{
					vl::Ptr<WfEnumerableType> tree = new WfEnumerableType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"MapType")
				{
					vl::Ptr<WfMapType> tree = new WfMapType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionType")
				{
					vl::Ptr<WfFunctionType> tree = new WfFunctionType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ChildType")
				{
					vl::Ptr<WfChildType> tree = new WfChildType;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfType>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ReferenceExpression")
				{
					vl::Ptr<WfReferenceExpression> tree = new WfReferenceExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"MemberExpression")
				{
					vl::Ptr<WfMemberExpression> tree = new WfMemberExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"LiteralExpression")
				{
					vl::Ptr<WfLiteralExpression> tree = new WfLiteralExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
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
				else if(obj->GetType()==L"LetExpression")
				{
					vl::Ptr<WfLetExpression> tree = new WfLetExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"IfExpression")
				{
					vl::Ptr<WfIfExpression> tree = new WfIfExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"RangeExpression")
				{
					vl::Ptr<WfRangeExpression> tree = new WfRangeExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"SetTestingExpression")
				{
					vl::Ptr<WfSetTestingExpression> tree = new WfSetTestingExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ConstructorArgument")
				{
					vl::Ptr<WfConstructorArgument> tree = new WfConstructorArgument;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ConstructorExpression")
				{
					vl::Ptr<WfConstructorExpression> tree = new WfConstructorExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"InferExpression")
				{
					vl::Ptr<WfInferExpression> tree = new WfInferExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TypeCastingExpression")
				{
					vl::Ptr<WfTypeCastingExpression> tree = new WfTypeCastingExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TypeTestingExpression")
				{
					vl::Ptr<WfTypeTestingExpression> tree = new WfTypeTestingExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TypeOfTypeExpression")
				{
					vl::Ptr<WfTypeOfTypeExpression> tree = new WfTypeOfTypeExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TypeOfExpressionExpression")
				{
					vl::Ptr<WfTypeOfExpressionExpression> tree = new WfTypeOfExpressionExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"CallExpression")
				{
					vl::Ptr<WfCallExpression> tree = new WfCallExpression;
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

		vl::Ptr<WfReferenceType> WfReferenceType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfReferenceType>();
		}

		vl::Ptr<WfSharedPointerType> WfSharedPointerType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfSharedPointerType>();
		}

		vl::Ptr<WfEnumerableType> WfEnumerableType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfEnumerableType>();
		}

		vl::Ptr<WfMapType> WfMapType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfMapType>();
		}

		vl::Ptr<WfFunctionType> WfFunctionType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFunctionType>();
		}

		vl::Ptr<WfChildType> WfChildType::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfChildType>();
		}

		vl::Ptr<WfReferenceExpression> WfReferenceExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfReferenceExpression>();
		}

		vl::Ptr<WfMemberExpression> WfMemberExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfMemberExpression>();
		}

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

		vl::Ptr<WfLetExpression> WfLetExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfLetExpression>();
		}

		vl::Ptr<WfIfExpression> WfIfExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfIfExpression>();
		}

		vl::Ptr<WfRangeExpression> WfRangeExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfRangeExpression>();
		}

		vl::Ptr<WfSetTestingExpression> WfSetTestingExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfSetTestingExpression>();
		}

		vl::Ptr<WfConstructorArgument> WfConstructorArgument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfConstructorArgument>();
		}

		vl::Ptr<WfConstructorExpression> WfConstructorExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfConstructorExpression>();
		}

		vl::Ptr<WfInferExpression> WfInferExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfInferExpression>();
		}

		vl::Ptr<WfTypeCastingExpression> WfTypeCastingExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfTypeCastingExpression>();
		}

		vl::Ptr<WfTypeTestingExpression> WfTypeTestingExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfTypeTestingExpression>();
		}

		vl::Ptr<WfTypeOfTypeExpression> WfTypeOfTypeExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfTypeOfTypeExpression>();
		}

		vl::Ptr<WfTypeOfExpressionExpression> WfTypeOfExpressionExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfTypeOfExpressionExpression>();
		}

		vl::Ptr<WfCallExpression> WfCallExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfCallExpression>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void WfReferenceType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfSharedPointerType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfEnumerableType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfMapType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfFunctionType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfChildType::Accept(WfType::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfReferenceExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfMemberExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfLiteralExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

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

		void WfLetExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfIfExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfRangeExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfSetTestingExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfConstructorExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfInferExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfTypeCastingExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfTypeTestingExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfTypeOfTypeExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfTypeOfExpressionExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfCallExpression::Accept(WfExpression::IVisitor* visitor)
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
		    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, true, errors);
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

			IMPL_TYPE_INFO_RENAME(WfType, Workflow::WfType)
			IMPL_TYPE_INFO_RENAME(WfReferenceType, Workflow::WfReferenceType)
			IMPL_TYPE_INFO_RENAME(WfSharedPointerType, Workflow::WfSharedPointerType)
			IMPL_TYPE_INFO_RENAME(WfEnumerableType, Workflow::WfEnumerableType)
			IMPL_TYPE_INFO_RENAME(WfMapType, Workflow::WfMapType)
			IMPL_TYPE_INFO_RENAME(WfFunctionType, Workflow::WfFunctionType)
			IMPL_TYPE_INFO_RENAME(WfChildType, Workflow::WfChildType)
			IMPL_TYPE_INFO_RENAME(WfExpression, Workflow::WfExpression)
			IMPL_TYPE_INFO_RENAME(WfLiteralValue, Workflow::WfLiteralValue)
			IMPL_TYPE_INFO_RENAME(WfReferenceExpression, Workflow::WfReferenceExpression)
			IMPL_TYPE_INFO_RENAME(WfMemberExpression, Workflow::WfMemberExpression)
			IMPL_TYPE_INFO_RENAME(WfLiteralExpression, Workflow::WfLiteralExpression)
			IMPL_TYPE_INFO_RENAME(WfFloatingExpression, Workflow::WfFloatingExpression)
			IMPL_TYPE_INFO_RENAME(WfIntegerExpression, Workflow::WfIntegerExpression)
			IMPL_TYPE_INFO_RENAME(WfStringExpression, Workflow::WfStringExpression)
			IMPL_TYPE_INFO_RENAME(WfFormatExpression, Workflow::WfFormatExpression)
			IMPL_TYPE_INFO_RENAME(WfUnaryOperator, Workflow::WfUnaryOperator)
			IMPL_TYPE_INFO_RENAME(WfUnaryExpression, Workflow::WfUnaryExpression)
			IMPL_TYPE_INFO_RENAME(WfBinaryOperator, Workflow::WfBinaryOperator)
			IMPL_TYPE_INFO_RENAME(WfBinaryExpression, Workflow::WfBinaryExpression)
			IMPL_TYPE_INFO_RENAME(WfLetExpression, Workflow::WfLetExpression)
			IMPL_TYPE_INFO_RENAME(WfIfExpression, Workflow::WfIfExpression)
			IMPL_TYPE_INFO_RENAME(WfRangeBoundary, Workflow::WfRangeBoundary)
			IMPL_TYPE_INFO_RENAME(WfRangeExpression, Workflow::WfRangeExpression)
			IMPL_TYPE_INFO_RENAME(WfSetTestingExpression, Workflow::WfSetTestingExpression)
			IMPL_TYPE_INFO_RENAME(WfConstructorArgument, Workflow::WfConstructorArgument)
			IMPL_TYPE_INFO_RENAME(WfConstructorExpression, Workflow::WfConstructorExpression)
			IMPL_TYPE_INFO_RENAME(WfInferExpression, Workflow::WfInferExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeCastingStrategy, Workflow::WfTypeCastingStrategy)
			IMPL_TYPE_INFO_RENAME(WfTypeCastingExpression, Workflow::WfTypeCastingExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeTestingExpression, Workflow::WfTypeTestingExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeOfTypeExpression, Workflow::WfTypeOfTypeExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeOfExpressionExpression, Workflow::WfTypeOfExpressionExpression)
			IMPL_TYPE_INFO_RENAME(WfCallExpression, Workflow::WfCallExpression)
			IMPL_TYPE_INFO_RENAME(WfType::IVisitor, Workflow::WfType::IVisitor)
			IMPL_TYPE_INFO_RENAME(WfExpression::IVisitor, Workflow::WfExpression::IVisitor)

			BEGIN_CLASS_MEMBER(WfType)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfType)

			BEGIN_CLASS_MEMBER(WfReferenceType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfReferenceType>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfReferenceType::*)(), [](WfReferenceType* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfReferenceType::*)(const vl::WString&), [](WfReferenceType* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfReferenceType)

			BEGIN_CLASS_MEMBER(WfSharedPointerType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfSharedPointerType>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(element)
			END_CLASS_MEMBER(WfSharedPointerType)

			BEGIN_CLASS_MEMBER(WfEnumerableType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfEnumerableType>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(element)
			END_CLASS_MEMBER(WfEnumerableType)

			BEGIN_CLASS_MEMBER(WfMapType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfMapType>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(key)
				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(WfMapType)

			BEGIN_CLASS_MEMBER(WfFunctionType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFunctionType>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(result)
				CLASS_MEMBER_FIELD(arguments)
			END_CLASS_MEMBER(WfFunctionType)

			BEGIN_CLASS_MEMBER(WfChildType)
				CLASS_MEMBER_BASE(WfType)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfChildType>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfChildType::*)(), [](WfChildType* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfChildType::*)(const vl::WString&), [](WfChildType* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfChildType)

			BEGIN_CLASS_MEMBER(WfExpression)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfExpression)

			BEGIN_ENUM_ITEM(WfLiteralValue)
				ENUM_ITEM_NAMESPACE(WfLiteralValue)
				ENUM_NAMESPACE_ITEM(Null)
				ENUM_NAMESPACE_ITEM(True)
				ENUM_NAMESPACE_ITEM(False)
			END_ENUM_ITEM(WfLiteralValue)

			BEGIN_CLASS_MEMBER(WfReferenceExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfReferenceExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfReferenceExpression::*)(), [](WfReferenceExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfReferenceExpression::*)(const vl::WString&), [](WfReferenceExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfReferenceExpression)

			BEGIN_CLASS_MEMBER(WfMemberExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfMemberExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfMemberExpression::*)(), [](WfMemberExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfMemberExpression::*)(const vl::WString&), [](WfMemberExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfMemberExpression)

			BEGIN_CLASS_MEMBER(WfLiteralExpression)
				CLASS_MEMBER_BASE(WfExpression)

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
				ENUM_NAMESPACE_ITEM(Or)
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

			BEGIN_CLASS_MEMBER(WfLetExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfLetExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfLetExpression::*)(), [](WfLetExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfLetExpression::*)(const vl::WString&), [](WfLetExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(value)
				CLASS_MEMBER_FIELD(exp)
			END_CLASS_MEMBER(WfLetExpression)

			BEGIN_CLASS_MEMBER(WfIfExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfIfExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(condition)
				CLASS_MEMBER_FIELD(trueBranch)
				CLASS_MEMBER_FIELD(falseBranch)
			END_CLASS_MEMBER(WfIfExpression)

			BEGIN_ENUM_ITEM(WfRangeBoundary)
				ENUM_ITEM_NAMESPACE(WfRangeBoundary)
				ENUM_NAMESPACE_ITEM(Inclusive)
				ENUM_NAMESPACE_ITEM(Exclusive)
			END_ENUM_ITEM(WfRangeBoundary)

			BEGIN_CLASS_MEMBER(WfRangeExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfRangeExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(begin)
				CLASS_MEMBER_FIELD(beginBoundary)
				CLASS_MEMBER_FIELD(end)
				CLASS_MEMBER_FIELD(endBoundary)
			END_CLASS_MEMBER(WfRangeExpression)

			BEGIN_CLASS_MEMBER(WfSetTestingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfSetTestingExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(element)
				CLASS_MEMBER_FIELD(collection)
			END_CLASS_MEMBER(WfSetTestingExpression)

			BEGIN_CLASS_MEMBER(WfConstructorArgument)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfConstructorArgument>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(key)
				CLASS_MEMBER_FIELD(value)
			END_CLASS_MEMBER(WfConstructorArgument)

			BEGIN_CLASS_MEMBER(WfConstructorExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfConstructorExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(arguments)
			END_CLASS_MEMBER(WfConstructorExpression)

			BEGIN_CLASS_MEMBER(WfInferExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfInferExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(type)
			END_CLASS_MEMBER(WfInferExpression)

			BEGIN_ENUM_ITEM(WfTypeCastingStrategy)
				ENUM_ITEM_NAMESPACE(WfTypeCastingStrategy)
				ENUM_NAMESPACE_ITEM(Strong)
				ENUM_NAMESPACE_ITEM(Weak)
			END_ENUM_ITEM(WfTypeCastingStrategy)

			BEGIN_CLASS_MEMBER(WfTypeCastingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTypeCastingExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(strategy)
				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(type)
			END_CLASS_MEMBER(WfTypeCastingExpression)

			BEGIN_CLASS_MEMBER(WfTypeTestingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTypeTestingExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(type)
			END_CLASS_MEMBER(WfTypeTestingExpression)

			BEGIN_CLASS_MEMBER(WfTypeOfTypeExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTypeOfTypeExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(type)
			END_CLASS_MEMBER(WfTypeOfTypeExpression)

			BEGIN_CLASS_MEMBER(WfTypeOfExpressionExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTypeOfExpressionExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfTypeOfExpressionExpression)

			BEGIN_CLASS_MEMBER(WfCallExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfCallExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(function)
				CLASS_MEMBER_FIELD(arguments)
			END_CLASS_MEMBER(WfCallExpression)

			BEGIN_CLASS_MEMBER(WfType::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfType::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfType_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfReferenceType* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfSharedPointerType* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfEnumerableType* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfMapType* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfFunctionType* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfType::IVisitor::*)(WfChildType* node))
			END_CLASS_MEMBER(WfType)

			BEGIN_CLASS_MEMBER(WfExpression::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfExpression::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfExpression_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfReferenceExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfMemberExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfLiteralExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFloatingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfIntegerExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfStringExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFormatExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfUnaryExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfBinaryExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfLetExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfIfExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfRangeExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfSetTestingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfConstructorExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfInferExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfTypeCastingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfTypeTestingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfTypeOfTypeExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfTypeOfExpressionExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfCallExpression* node))
			END_CLASS_MEMBER(WfExpression)

			class WfTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::workflow::WfType)
					ADD_TYPE_INFO(vl::workflow::WfReferenceType)
					ADD_TYPE_INFO(vl::workflow::WfSharedPointerType)
					ADD_TYPE_INFO(vl::workflow::WfEnumerableType)
					ADD_TYPE_INFO(vl::workflow::WfMapType)
					ADD_TYPE_INFO(vl::workflow::WfFunctionType)
					ADD_TYPE_INFO(vl::workflow::WfChildType)
					ADD_TYPE_INFO(vl::workflow::WfExpression)
					ADD_TYPE_INFO(vl::workflow::WfLiteralValue)
					ADD_TYPE_INFO(vl::workflow::WfReferenceExpression)
					ADD_TYPE_INFO(vl::workflow::WfMemberExpression)
					ADD_TYPE_INFO(vl::workflow::WfLiteralExpression)
					ADD_TYPE_INFO(vl::workflow::WfFloatingExpression)
					ADD_TYPE_INFO(vl::workflow::WfIntegerExpression)
					ADD_TYPE_INFO(vl::workflow::WfStringExpression)
					ADD_TYPE_INFO(vl::workflow::WfFormatExpression)
					ADD_TYPE_INFO(vl::workflow::WfUnaryOperator)
					ADD_TYPE_INFO(vl::workflow::WfUnaryExpression)
					ADD_TYPE_INFO(vl::workflow::WfBinaryOperator)
					ADD_TYPE_INFO(vl::workflow::WfBinaryExpression)
					ADD_TYPE_INFO(vl::workflow::WfLetExpression)
					ADD_TYPE_INFO(vl::workflow::WfIfExpression)
					ADD_TYPE_INFO(vl::workflow::WfRangeBoundary)
					ADD_TYPE_INFO(vl::workflow::WfRangeExpression)
					ADD_TYPE_INFO(vl::workflow::WfSetTestingExpression)
					ADD_TYPE_INFO(vl::workflow::WfConstructorArgument)
					ADD_TYPE_INFO(vl::workflow::WfConstructorExpression)
					ADD_TYPE_INFO(vl::workflow::WfInferExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeCastingStrategy)
					ADD_TYPE_INFO(vl::workflow::WfTypeCastingExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeTestingExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeOfTypeExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeOfExpressionExpression)
					ADD_TYPE_INFO(vl::workflow::WfCallExpression)
					ADD_TYPE_INFO(vl::workflow::WfType::IVisitor)
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
