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
L"\r\n"L"class OrderedNameExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class OrderedLambdaExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tbody;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class MemberExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tparent;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ChildExpression : Expression"
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
L"\r\n"L"\tAssign,"
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
L"\r\n"L"enum SetTesting"
L"\r\n"L"{"
L"\r\n"L"\tIn,"
L"\r\n"L"\tNotIn,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class SetTestingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tSetTesting\t\t\t\ttest;"
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
L"\r\n"L"enum TypeTesting"
L"\r\n"L"{"
L"\r\n"L"\tIsType,"
L"\r\n"L"\tIsNotType,"
L"\r\n"L"\tIsNull,"
L"\r\n"L"\tIsNotNull,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TypeTestingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tTypeTesting\t\t\t\ttest;"
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
L"\r\n"L"class AttachEventExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tevent;"
L"\r\n"L"\tExpression\t\t\t\tfunction;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class DetachEventExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\thandler;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BindExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum ObserveType"
L"\r\n"L"{"
L"\r\n"L"\tSimpleObserve,"
L"\r\n"L"\tExtendedObserve,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ObserveExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tparent;"
L"\r\n"L"\tObserveType\t\t\t\tobserveType;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tExpression[]\t\t\tevents;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class CallExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tfunction;"
L"\r\n"L"\tExpression[]\t\t\targuments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Statement"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class VariableStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BreakStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ContinueStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ReturnStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class DeleteStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class RaiseExceptionStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class IfStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tStatement\t\t\t\ttrueBranch;"
L"\r\n"L"\tStatement\t\t\t\tfalseBranch;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class SwitchCase"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tStatement\t\t\t\tstatement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class SwitchStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"\tSwitchCase[]\t\t\tcaseBranches;"
L"\r\n"L"\tStatement\t\t\t\tdefaultBranch;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class WhileStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\tcondition;"
L"\r\n"L"\tStatement\t\t\t\tstatement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum ForEachDirection"
L"\r\n"L"{"
L"\r\n"L"\tNormal,"
L"\r\n"L"\tReversed,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ForEachStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tForEachDirection\t\tdirection;"
L"\r\n"L"\tExpression\t\t\t\tcollection;"
L"\r\n"L"\tStatement\t\t\t\tstatement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TryStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tStatement\t\t\t\tprotectedStatement;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tStatement\t\t\t\tcatchStatement;"
L"\r\n"L"\tStatement\t\t\t\tfinallyStatement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BlockStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tStatement[]\t\t\t\tstatements;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ExpressionStatement : Statement"
L"\r\n"L"{"
L"\r\n"L"\tExpression\t\t\t\texpression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Declaration"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class NamespaceDeclaration : Declaration"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tDeclaration[]\t\t\tdeclarations;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FunctionArgument"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"enum FunctionAnonymity"
L"\r\n"L"{"
L"\r\n"L"\tNamed,"
L"\r\n"L"\tAnonymous,"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FunctionDeclaration : Declaration"
L"\r\n"L"{"
L"\r\n"L"\tFunctionAnonymity\t\tanonymity;"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tFunctionArgument[]\t\targuments;"
L"\r\n"L"\tStatement\t\t\t\tstatement;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FunctionExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tFunctionDeclaration\t\tfunction;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class NewTypeExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tType\t\t\t\t\ttype;"
L"\r\n"L"\tExpression[]\t\t\targuments;"
L"\r\n"L"\tFunctionDeclaration[]\tfunctions;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ModuleUsingItem"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ModuleUsingPath"
L"\r\n"L"{"
L"\r\n"L"\tModuleUsingItem[]\t\titems;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Module"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\t\tname;"
L"\r\n"L"\tModuleUsingPath[]\t\tpaths;"
L"\r\n"L"\tDeclaration[]\t\t\tdeclarations;"
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
L"\r\n"L"token SCOPE_DELIMITER = \"::\";"
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
L"\r\n"L"token KEYWORD_NEW = \"new\";"
L"\r\n"L"token KEYWORD_OF = \"of\";"
L"\r\n"L"token KEYWORD_AS = \"as\";"
L"\r\n"L"token KEYWORD_IS = \"is\";"
L"\r\n"L"token KEYWORD_CAST = \"cast\";"
L"\r\n"L"token KEYWORD_FUNC = \"func\";"
L"\r\n"L"token KEYWORD_TYPEOF = \"typeof\";"
L"\r\n"L"token KEYWORD_TYPE = \"type\";"
L"\r\n"L"token KEYWORD_BIND = \"bind\";"
L"\r\n"L"token KEYWORD_OBSERVE = \"observe\";"
L"\r\n"L"token KEYWORD_ON = \"on\";"
L"\r\n"L"token KEYWORD_ATTACH = \"attach\";"
L"\r\n"L"token KEYWORD_DETACH = \"detach\";"
L"\r\n"L"token KEYWORD_VAR = \"var\";"
L"\r\n"L"token KEYWORD_BREAK = \"break\";"
L"\r\n"L"token KEYWORD_CONTINUE = \"continue\";"
L"\r\n"L"token KEYWORD_RETURN = \"return\";"
L"\r\n"L"token KEYWORD_DELETE = \"delete\";"
L"\r\n"L"token KEYWORD_RAISE = \"raise\";"
L"\r\n"L"token KEYWORD_IF = \"if\";"
L"\r\n"L"token KEYWORD_ELSE = \"else\";"
L"\r\n"L"token KEYWORD_SWITCH = \"switch\";"
L"\r\n"L"token KEYWORD_CASE = \"case\";"
L"\r\n"L"token KEYWORD_DEFAULT = \"default\";"
L"\r\n"L"token KEYWORD_WHILE = \"while\";"
L"\r\n"L"token KEYWORD_FOR = \"for\";"
L"\r\n"L"token KEYWORD_REVERSED = \"reversed\";"
L"\r\n"L"token KEYWORD_TRY = \"try\";"
L"\r\n"L"token KEYWORD_CATCH = \"catch\";"
L"\r\n"L"token KEYWORD_FINALLY = \"finally\";"
L"\r\n"L"token KEYWORD_USING = \"using\";"
L"\r\n"L"token KEYWORD_NAMESPACE = \"namespace\";"
L"\r\n"L"token KEYWORD_MODULE = \"module\";"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z_]/w*\";"
L"\r\n"L"token ORDERED_NAME = \"/$[0-9]*\";"
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
L"\r\n"L"\t= WorkflowType : parent \"::\" NAME : name as ChildType"
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
L"\r\n"L"\t= ORDERED_NAME : name as OrderedNameExpression"
L"\r\n"L"\t= \"(\" !WorkflowExpression \")\""
L"\r\n"L"\t= \"[\" WorkflowExpression : body \"]\" as OrderedLambdaExpression"
L"\r\n"L"\t= Function : function as FunctionExpression"
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
L"\r\n"L"\t= \"new\" WorkflowType : type \"(\" [WorkflowExpression : arguments {\",\" WorkflowExpression : arguments}] \")\" [\"{\" {Function : functions} \"}\"] as NewTypeExpression"
L"\r\n"L"\t= \"bind\" \"(\" WorkflowExpression : expression \")\" as BindExpression"
L"\r\n"L"\t= Exp0 : parent \".\" \"observe\" \"(\" WorkflowExpression : expression [\"on\" WorkflowExpression : events {\",\" WorkflowExpression : events}]\")\" as ObserveExpression with {observeType = \"SimpleObserve\"}"
L"\r\n"L"\t= Exp0 : parent \".\" \"observe\" \"as\" NAME : name \"(\" WorkflowExpression : expression [\"on\" WorkflowExpression : events {\",\" WorkflowExpression : events}]\")\" as ObserveExpression with {observeType = \"ExtendedObserve\"}"
L"\r\n"L"\t= Exp0 : function \"(\" [WorkflowExpression : arguments {\",\" WorkflowExpression : arguments}] \")\" as CallExpression"
L"\r\n"L"\t= Exp0 : parent \".\" NAME : name as MemberExpression"
L"\r\n"L"\t= Exp0 : parent \"::\" NAME : name as ChildExpression"
L"\r\n"L"\t= Exp0 : first \"[\" WorkflowExpression : second \"]\" as BinaryExpression with {op = \"Index\"}"
L"\r\n"L"\t= Exp0 : element"
L"\r\n"L"\t\t((\"in\" with {test=\"In\"}) | (\"not\" \"in\" with {test=\"NotIn\"}))"
L"\r\n"L"\t\tWorkflowExpression : collection as SetTestingExpression"
L"\r\n"L"\t= Exp0 : expression \"of\" WorkflowType : type as InferExpression"
L"\r\n"L"\t= Exp0 : expression"
L"\r\n"L"\t\t("
L"\r\n"L"\t\t\t("
L"\r\n"L"\t\t\t\t("
L"\r\n"L"\t\t\t\t\t(\"is\" with {test=\"IsType\"})"
L"\r\n"L"\t\t\t\t\t| (\"is\" \"not\" with {test=\"IsNotType\"})"
L"\r\n"L"\t\t\t\t) WorkflowType : type as TypeTestingExpression"
L"\r\n"L"\t\t\t)"
L"\r\n"L"\t\t\t| (\"is\" \"null\" as TypeTestingExpression with {test=\"IsNull\"})"
L"\r\n"L"\t\t\t| (\"is\" \"not\" \"null\" as TypeTestingExpression with {test=\"IsNotNull\"})"
L"\r\n"L"\t\t)"
L"\r\n"L"\t= Exp0 : expression \"as\" WorkflowType : type as TypeCastingExpression with {strategy = \"Weak\"}"
L"\r\n"L"\t= \"cast\" WorkflowType : type Exp0 : expression as TypeCastingExpression with {strategy = \"Strong\"}"
L"\r\n"L"\t= \"typeof\" \"(\" WorkflowType : type \")\" as TypeOfTypeExpression"
L"\r\n"L"\t= \"type\" \"(\" WorkflowExpression : expression \")\" as TypeOfExpressionExpression"
L"\r\n"L"\t= \"attach\" \"(\" WorkflowExpression : event \",\" WorkflowExpression : function \")\" as AttachEventExpression"
L"\r\n"L"\t= \"detach\" \"(\" WorkflowExpression : handler \")\" as DetachEventExpression"
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
L"\r\n"L"rule Expression Exp11"
L"\r\n"L"\t= !Exp10"
L"\r\n"L"\t= Exp10 : first \"=\" Exp11 : second as BinaryExpression with {op = \"Assign\"}"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression WorkflowExpression"
L"\r\n"L"\t= !Exp11"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule VariableStatement Variable"
L"\r\n"L"\t= \"var\" NAME : name [\":\" WorkflowType : type] \"=\" WorkflowExpression : expression \";\" as VariableStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule BreakStatement Break"
L"\r\n"L"\t= \"break\" \";\" as BreakStatement"
L"\r\n"L"\t;"
L"\r\n"L"rule ContinueStatement Continue"
L"\r\n"L"\t= \"continue\" \";\" as ContinueStatement"
L"\r\n"L"\t;"
L"\r\n"L"rule ReturnStatement Return"
L"\r\n"L"\t= \"return\" [WorkflowExpression : expression] \";\" as ReturnStatement"
L"\r\n"L"\t;"
L"\r\n"L"rule DeleteStatement Delete"
L"\r\n"L"\t= \"delete\" WorkflowExpression : expression \";\" as DeleteStatement"
L"\r\n"L"\t;"
L"\r\n"L"rule RaiseExceptionStatement RaiseException"
L"\r\n"L"\t= \"raise\" WorkflowExpression : expression \";\" as RaiseExceptionStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule IfStatement If"
L"\r\n"L"\t= \"if\" \"(\" [\"var\" NAME : name \":\" WorkflowType : type \"=\"] WorkflowExpression : expression \")\" Block : trueBranch [\"else\" Block : falseBranch] as IfStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule SwitchCase SwitchCaseFragment"
L"\r\n"L"\t= \"case\" WorkflowExpression : expression \":\" Block : statement as SwitchCase"
L"\r\n"L"\t;"
L"\r\n"L"rule SwitchStatement Switch"
L"\r\n"L"\t= \"switch\" \"(\" WorkflowExpression : expression \")\" \"{\" {SwitchCaseFragment : caseBranches} [\"default\" \":\" Block : defaultBranch] \"}\" as SwitchStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule WhileStatement While"
L"\r\n"L"\t= \"while\" \"(\" WorkflowExpression : condition \")\" Block : statement as WhileStatement"
L"\r\n"L"\t;"
L"\r\n"L"rule ForEachStatement ForEach"
L"\r\n"L"\t= \"for\" \"(\" NAME : name "
L"\r\n"L"\t\t((\"in\" with {direction=\"Normal\"}) | (\"in\" \"reversed\" with {direction=\"Reversed\"}))"
L"\r\n"L"\t\tWorkflowExpression : collection \")\" Block : statement as ForEachStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule TryStatement Try"
L"\r\n"L"\t= \"try\" Block : protectedStatement [\"catch\" \"(\" NAME : name \")\" Block : catchStatement] [\"finally\" Block : finallyStatement] as TryStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule BlockStatement Block"
L"\r\n"L"\t= \"{\" {WorkflowStatement : statements} \"}\" as BlockStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule ExpressionStatement ExprStat"
L"\r\n"L"\t= WorkflowExpression : expression as ExpressionStatement"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Statement WorkflowStatement"
L"\r\n"L"\t= !Variable"
L"\r\n"L"\t= !Break"
L"\r\n"L"\t= !Continue"
L"\r\n"L"\t= !Return"
L"\r\n"L"\t= !Delete"
L"\r\n"L"\t= !RaiseException"
L"\r\n"L"\t= !If"
L"\r\n"L"\t= !Switch"
L"\r\n"L"\t= !While"
L"\r\n"L"\t= !ForEach"
L"\r\n"L"\t= !Try"
L"\r\n"L"\t= !Block"
L"\r\n"L"\t= !ExprStat"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule NamespaceDeclaration Namespace"
L"\r\n"L"\t= \"namespace\" NAME : name \"{\" {WorkflowDeclaration : declarations} \"}\" as NamespaceDeclaration"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule FunctionArgument FunctionArgumentFragment"
L"\r\n"L"\t= NAME : name \":\" WorkflowType : type as FunctionArgument"
L"\r\n"L"\t;"
L"\r\n"L"rule FunctionDeclaration Function"
L"\r\n"L"\t= \"func\" "
L"\r\n"L"\t\t("
L"\r\n"L"\t\t\t(NAME : name \"(\" with {anonymity=\"Named\"})"
L"\r\n"L"\t\t\t| (\"(\" with {anonymity=\"Anonymous\"})"
L"\r\n"L"\t\t)"
L"\r\n"L"\t\t[FunctionArgumentFragment : arguments {\",\" FunctionArgumentFragment : arguments}] \")\" Block : statement as FunctionDeclaration"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Declaration WorkflowDeclaration"
L"\r\n"L"\t= !Namespace"
L"\r\n"L"\t= !Function"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule ModuleUsingItem UsingItem"
L"\r\n"L"\t= NAME : name as ModuleUsingItem"
L"\r\n"L"\t;"
L"\r\n"L"rule ModuleUsingPath UsingPath"
L"\r\n"L"\t= \"using\" UsingItem : items {\"::\" UsingItem : items} as ModuleUsingPath"
L"\r\n"L"\t;"
L"\r\n"L"rule Module WorkflowModule"
L"\r\n"L"\t= \"module\" NAME : name {UsingPath : paths} {WorkflowDeclaration : declarations} as Module"
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
					else if(token->GetValue()==L"Assign") { member=WfBinaryOperator::Assign; return true; }
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

			bool SetMember(WfSetTesting& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"In") { member=WfSetTesting::In; return true; }
					else if(token->GetValue()==L"NotIn") { member=WfSetTesting::NotIn; return true; }
					else { member=WfSetTesting::In; return false; }
				}
				member=WfSetTesting::In;
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

			bool SetMember(WfTypeTesting& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"IsType") { member=WfTypeTesting::IsType; return true; }
					else if(token->GetValue()==L"IsNotType") { member=WfTypeTesting::IsNotType; return true; }
					else if(token->GetValue()==L"IsNull") { member=WfTypeTesting::IsNull; return true; }
					else if(token->GetValue()==L"IsNotNull") { member=WfTypeTesting::IsNotNull; return true; }
					else { member=WfTypeTesting::IsType; return false; }
				}
				member=WfTypeTesting::IsType;
				return false;
			}

			bool SetMember(WfObserveType& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"SimpleObserve") { member=WfObserveType::SimpleObserve; return true; }
					else if(token->GetValue()==L"ExtendedObserve") { member=WfObserveType::ExtendedObserve; return true; }
					else { member=WfObserveType::SimpleObserve; return false; }
				}
				member=WfObserveType::SimpleObserve;
				return false;
			}

			bool SetMember(WfForEachDirection& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Normal") { member=WfForEachDirection::Normal; return true; }
					else if(token->GetValue()==L"Reversed") { member=WfForEachDirection::Reversed; return true; }
					else { member=WfForEachDirection::Normal; return false; }
				}
				member=WfForEachDirection::Normal;
				return false;
			}

			bool SetMember(WfFunctionAnonymity& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Named") { member=WfFunctionAnonymity::Named; return true; }
					else if(token->GetValue()==L"Anonymous") { member=WfFunctionAnonymity::Anonymous; return true; }
					else { member=WfFunctionAnonymity::Named; return false; }
				}
				member=WfFunctionAnonymity::Named;
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

			void Fill(vl::Ptr<WfOrderedNameExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfOrderedLambdaExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->body, obj->GetMember(L"body"), tokens);
			}

			void Fill(vl::Ptr<WfMemberExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->parent, obj->GetMember(L"parent"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfChildExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
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
				SetMember(tree->test, obj->GetMember(L"test"), tokens);
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
				SetMember(tree->test, obj->GetMember(L"test"), tokens);
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

			void Fill(vl::Ptr<WfAttachEventExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->event, obj->GetMember(L"event"), tokens);
				SetMember(tree->function, obj->GetMember(L"function"), tokens);
			}

			void Fill(vl::Ptr<WfDetachEventExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->handler, obj->GetMember(L"handler"), tokens);
			}

			void Fill(vl::Ptr<WfBindExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfObserveExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->parent, obj->GetMember(L"parent"), tokens);
				SetMember(tree->observeType, obj->GetMember(L"observeType"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->events, obj->GetMember(L"events"), tokens);
			}

			void Fill(vl::Ptr<WfCallExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->function, obj->GetMember(L"function"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			void Fill(vl::Ptr<WfStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfVariableStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfBreakStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfContinueStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfReturnStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfDeleteStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfRaiseExceptionStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfIfStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->trueBranch, obj->GetMember(L"trueBranch"), tokens);
				SetMember(tree->falseBranch, obj->GetMember(L"falseBranch"), tokens);
			}

			void Fill(vl::Ptr<WfSwitchCase> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->statement, obj->GetMember(L"statement"), tokens);
			}

			void Fill(vl::Ptr<WfSwitchStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
				SetMember(tree->caseBranches, obj->GetMember(L"caseBranches"), tokens);
				SetMember(tree->defaultBranch, obj->GetMember(L"defaultBranch"), tokens);
			}

			void Fill(vl::Ptr<WfWhileStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->condition, obj->GetMember(L"condition"), tokens);
				SetMember(tree->statement, obj->GetMember(L"statement"), tokens);
			}

			void Fill(vl::Ptr<WfForEachStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->direction, obj->GetMember(L"direction"), tokens);
				SetMember(tree->collection, obj->GetMember(L"collection"), tokens);
				SetMember(tree->statement, obj->GetMember(L"statement"), tokens);
			}

			void Fill(vl::Ptr<WfTryStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->protectedStatement, obj->GetMember(L"protectedStatement"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->catchStatement, obj->GetMember(L"catchStatement"), tokens);
				SetMember(tree->finallyStatement, obj->GetMember(L"finallyStatement"), tokens);
			}

			void Fill(vl::Ptr<WfBlockStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->statements, obj->GetMember(L"statements"), tokens);
			}

			void Fill(vl::Ptr<WfExpressionStatement> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<WfDeclaration> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfNamespaceDeclaration> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->declarations, obj->GetMember(L"declarations"), tokens);
			}

			void Fill(vl::Ptr<WfFunctionArgument> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
			}

			void Fill(vl::Ptr<WfFunctionDeclaration> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->anonymity, obj->GetMember(L"anonymity"), tokens);
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
				SetMember(tree->statement, obj->GetMember(L"statement"), tokens);
			}

			void Fill(vl::Ptr<WfFunctionExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->function, obj->GetMember(L"function"), tokens);
			}

			void Fill(vl::Ptr<WfNewTypeExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->type, obj->GetMember(L"type"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
				SetMember(tree->functions, obj->GetMember(L"functions"), tokens);
			}

			void Fill(vl::Ptr<WfModuleUsingItem> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<WfModuleUsingPath> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->items, obj->GetMember(L"items"), tokens);
			}

			void Fill(vl::Ptr<WfModule> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->paths, obj->GetMember(L"paths"), tokens);
				SetMember(tree->declarations, obj->GetMember(L"declarations"), tokens);
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
				else if(obj->GetType()==L"OrderedNameExpression")
				{
					vl::Ptr<WfOrderedNameExpression> tree = new WfOrderedNameExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"OrderedLambdaExpression")
				{
					vl::Ptr<WfOrderedLambdaExpression> tree = new WfOrderedLambdaExpression;
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
				else if(obj->GetType()==L"ChildExpression")
				{
					vl::Ptr<WfChildExpression> tree = new WfChildExpression;
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
				else if(obj->GetType()==L"AttachEventExpression")
				{
					vl::Ptr<WfAttachEventExpression> tree = new WfAttachEventExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"DetachEventExpression")
				{
					vl::Ptr<WfDetachEventExpression> tree = new WfDetachEventExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BindExpression")
				{
					vl::Ptr<WfBindExpression> tree = new WfBindExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ObserveExpression")
				{
					vl::Ptr<WfObserveExpression> tree = new WfObserveExpression;
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
				else if(obj->GetType()==L"VariableStatement")
				{
					vl::Ptr<WfVariableStatement> tree = new WfVariableStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BreakStatement")
				{
					vl::Ptr<WfBreakStatement> tree = new WfBreakStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ContinueStatement")
				{
					vl::Ptr<WfContinueStatement> tree = new WfContinueStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ReturnStatement")
				{
					vl::Ptr<WfReturnStatement> tree = new WfReturnStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"DeleteStatement")
				{
					vl::Ptr<WfDeleteStatement> tree = new WfDeleteStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"RaiseExceptionStatement")
				{
					vl::Ptr<WfRaiseExceptionStatement> tree = new WfRaiseExceptionStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"IfStatement")
				{
					vl::Ptr<WfIfStatement> tree = new WfIfStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"SwitchCase")
				{
					vl::Ptr<WfSwitchCase> tree = new WfSwitchCase;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"SwitchStatement")
				{
					vl::Ptr<WfSwitchStatement> tree = new WfSwitchStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"WhileStatement")
				{
					vl::Ptr<WfWhileStatement> tree = new WfWhileStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ForEachStatement")
				{
					vl::Ptr<WfForEachStatement> tree = new WfForEachStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TryStatement")
				{
					vl::Ptr<WfTryStatement> tree = new WfTryStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BlockStatement")
				{
					vl::Ptr<WfBlockStatement> tree = new WfBlockStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ExpressionStatement")
				{
					vl::Ptr<WfExpressionStatement> tree = new WfExpressionStatement;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfStatement>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"NamespaceDeclaration")
				{
					vl::Ptr<WfNamespaceDeclaration> tree = new WfNamespaceDeclaration;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfDeclaration>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionArgument")
				{
					vl::Ptr<WfFunctionArgument> tree = new WfFunctionArgument;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionDeclaration")
				{
					vl::Ptr<WfFunctionDeclaration> tree = new WfFunctionDeclaration;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfDeclaration>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionExpression")
				{
					vl::Ptr<WfFunctionExpression> tree = new WfFunctionExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"NewTypeExpression")
				{
					vl::Ptr<WfNewTypeExpression> tree = new WfNewTypeExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ModuleUsingItem")
				{
					vl::Ptr<WfModuleUsingItem> tree = new WfModuleUsingItem;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ModuleUsingPath")
				{
					vl::Ptr<WfModuleUsingPath> tree = new WfModuleUsingPath;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"Module")
				{
					vl::Ptr<WfModule> tree = new WfModule;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
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

		vl::Ptr<WfOrderedNameExpression> WfOrderedNameExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfOrderedNameExpression>();
		}

		vl::Ptr<WfOrderedLambdaExpression> WfOrderedLambdaExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfOrderedLambdaExpression>();
		}

		vl::Ptr<WfMemberExpression> WfMemberExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfMemberExpression>();
		}

		vl::Ptr<WfChildExpression> WfChildExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfChildExpression>();
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

		vl::Ptr<WfAttachEventExpression> WfAttachEventExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfAttachEventExpression>();
		}

		vl::Ptr<WfDetachEventExpression> WfDetachEventExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfDetachEventExpression>();
		}

		vl::Ptr<WfBindExpression> WfBindExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfBindExpression>();
		}

		vl::Ptr<WfObserveExpression> WfObserveExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfObserveExpression>();
		}

		vl::Ptr<WfCallExpression> WfCallExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfCallExpression>();
		}

		vl::Ptr<WfVariableStatement> WfVariableStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfVariableStatement>();
		}

		vl::Ptr<WfBreakStatement> WfBreakStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfBreakStatement>();
		}

		vl::Ptr<WfContinueStatement> WfContinueStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfContinueStatement>();
		}

		vl::Ptr<WfReturnStatement> WfReturnStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfReturnStatement>();
		}

		vl::Ptr<WfDeleteStatement> WfDeleteStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfDeleteStatement>();
		}

		vl::Ptr<WfRaiseExceptionStatement> WfRaiseExceptionStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfRaiseExceptionStatement>();
		}

		vl::Ptr<WfIfStatement> WfIfStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfIfStatement>();
		}

		vl::Ptr<WfSwitchCase> WfSwitchCase::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfSwitchCase>();
		}

		vl::Ptr<WfSwitchStatement> WfSwitchStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfSwitchStatement>();
		}

		vl::Ptr<WfWhileStatement> WfWhileStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfWhileStatement>();
		}

		vl::Ptr<WfForEachStatement> WfForEachStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfForEachStatement>();
		}

		vl::Ptr<WfTryStatement> WfTryStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfTryStatement>();
		}

		vl::Ptr<WfBlockStatement> WfBlockStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfBlockStatement>();
		}

		vl::Ptr<WfExpressionStatement> WfExpressionStatement::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfExpressionStatement>();
		}

		vl::Ptr<WfNamespaceDeclaration> WfNamespaceDeclaration::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfNamespaceDeclaration>();
		}

		vl::Ptr<WfFunctionArgument> WfFunctionArgument::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFunctionArgument>();
		}

		vl::Ptr<WfFunctionDeclaration> WfFunctionDeclaration::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFunctionDeclaration>();
		}

		vl::Ptr<WfFunctionExpression> WfFunctionExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFunctionExpression>();
		}

		vl::Ptr<WfNewTypeExpression> WfNewTypeExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfNewTypeExpression>();
		}

		vl::Ptr<WfModuleUsingItem> WfModuleUsingItem::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfModuleUsingItem>();
		}

		vl::Ptr<WfModuleUsingPath> WfModuleUsingPath::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfModuleUsingPath>();
		}

		vl::Ptr<WfModule> WfModule::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfModule>();
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

		void WfOrderedNameExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfOrderedLambdaExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfMemberExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfChildExpression::Accept(WfExpression::IVisitor* visitor)
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

		void WfAttachEventExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfDetachEventExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfBindExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfObserveExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfCallExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfVariableStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfBreakStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfContinueStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfReturnStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfDeleteStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfRaiseExceptionStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfIfStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfSwitchStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfWhileStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfForEachStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfTryStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfBlockStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfExpressionStatement::Accept(WfStatement::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfNamespaceDeclaration::Accept(WfDeclaration::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfFunctionDeclaration::Accept(WfDeclaration::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfFunctionExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfNewTypeExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowDeclaration");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseDeclarationAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseDeclarationAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfDeclaration> WfParseDeclaration(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowDeclaration");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfDeclaration>();
			}
			return 0;
		}

		vl::Ptr<WfDeclaration> WfParseDeclaration(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseDeclaration(input, table, errors);
		}

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

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseModuleAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowModule");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseModuleAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseModuleAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfModule> WfParseModule(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowModule");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfModule>();
			}
			return 0;
		}

		vl::Ptr<WfModule> WfParseModule(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseModule(input, table, errors);
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseStatementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowStatement");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseStatementAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseStatementAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfStatement> WfParseStatement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowStatement");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfStatement>();
			}
			return 0;
		}

		vl::Ptr<WfStatement> WfParseStatement(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseStatement(input, table, errors);
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseTypeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowType");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseTypeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseTypeAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfType> WfParseType(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowType");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfType>();
			}
			return 0;
		}

		vl::Ptr<WfType> WfParseType(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseType(input, table, errors);
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
			IMPL_TYPE_INFO_RENAME(WfOrderedNameExpression, Workflow::WfOrderedNameExpression)
			IMPL_TYPE_INFO_RENAME(WfOrderedLambdaExpression, Workflow::WfOrderedLambdaExpression)
			IMPL_TYPE_INFO_RENAME(WfMemberExpression, Workflow::WfMemberExpression)
			IMPL_TYPE_INFO_RENAME(WfChildExpression, Workflow::WfChildExpression)
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
			IMPL_TYPE_INFO_RENAME(WfSetTesting, Workflow::WfSetTesting)
			IMPL_TYPE_INFO_RENAME(WfSetTestingExpression, Workflow::WfSetTestingExpression)
			IMPL_TYPE_INFO_RENAME(WfConstructorArgument, Workflow::WfConstructorArgument)
			IMPL_TYPE_INFO_RENAME(WfConstructorExpression, Workflow::WfConstructorExpression)
			IMPL_TYPE_INFO_RENAME(WfInferExpression, Workflow::WfInferExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeCastingStrategy, Workflow::WfTypeCastingStrategy)
			IMPL_TYPE_INFO_RENAME(WfTypeCastingExpression, Workflow::WfTypeCastingExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeTesting, Workflow::WfTypeTesting)
			IMPL_TYPE_INFO_RENAME(WfTypeTestingExpression, Workflow::WfTypeTestingExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeOfTypeExpression, Workflow::WfTypeOfTypeExpression)
			IMPL_TYPE_INFO_RENAME(WfTypeOfExpressionExpression, Workflow::WfTypeOfExpressionExpression)
			IMPL_TYPE_INFO_RENAME(WfAttachEventExpression, Workflow::WfAttachEventExpression)
			IMPL_TYPE_INFO_RENAME(WfDetachEventExpression, Workflow::WfDetachEventExpression)
			IMPL_TYPE_INFO_RENAME(WfBindExpression, Workflow::WfBindExpression)
			IMPL_TYPE_INFO_RENAME(WfObserveType, Workflow::WfObserveType)
			IMPL_TYPE_INFO_RENAME(WfObserveExpression, Workflow::WfObserveExpression)
			IMPL_TYPE_INFO_RENAME(WfCallExpression, Workflow::WfCallExpression)
			IMPL_TYPE_INFO_RENAME(WfStatement, Workflow::WfStatement)
			IMPL_TYPE_INFO_RENAME(WfVariableStatement, Workflow::WfVariableStatement)
			IMPL_TYPE_INFO_RENAME(WfBreakStatement, Workflow::WfBreakStatement)
			IMPL_TYPE_INFO_RENAME(WfContinueStatement, Workflow::WfContinueStatement)
			IMPL_TYPE_INFO_RENAME(WfReturnStatement, Workflow::WfReturnStatement)
			IMPL_TYPE_INFO_RENAME(WfDeleteStatement, Workflow::WfDeleteStatement)
			IMPL_TYPE_INFO_RENAME(WfRaiseExceptionStatement, Workflow::WfRaiseExceptionStatement)
			IMPL_TYPE_INFO_RENAME(WfIfStatement, Workflow::WfIfStatement)
			IMPL_TYPE_INFO_RENAME(WfSwitchCase, Workflow::WfSwitchCase)
			IMPL_TYPE_INFO_RENAME(WfSwitchStatement, Workflow::WfSwitchStatement)
			IMPL_TYPE_INFO_RENAME(WfWhileStatement, Workflow::WfWhileStatement)
			IMPL_TYPE_INFO_RENAME(WfForEachDirection, Workflow::WfForEachDirection)
			IMPL_TYPE_INFO_RENAME(WfForEachStatement, Workflow::WfForEachStatement)
			IMPL_TYPE_INFO_RENAME(WfTryStatement, Workflow::WfTryStatement)
			IMPL_TYPE_INFO_RENAME(WfBlockStatement, Workflow::WfBlockStatement)
			IMPL_TYPE_INFO_RENAME(WfExpressionStatement, Workflow::WfExpressionStatement)
			IMPL_TYPE_INFO_RENAME(WfDeclaration, Workflow::WfDeclaration)
			IMPL_TYPE_INFO_RENAME(WfNamespaceDeclaration, Workflow::WfNamespaceDeclaration)
			IMPL_TYPE_INFO_RENAME(WfFunctionArgument, Workflow::WfFunctionArgument)
			IMPL_TYPE_INFO_RENAME(WfFunctionAnonymity, Workflow::WfFunctionAnonymity)
			IMPL_TYPE_INFO_RENAME(WfFunctionDeclaration, Workflow::WfFunctionDeclaration)
			IMPL_TYPE_INFO_RENAME(WfFunctionExpression, Workflow::WfFunctionExpression)
			IMPL_TYPE_INFO_RENAME(WfNewTypeExpression, Workflow::WfNewTypeExpression)
			IMPL_TYPE_INFO_RENAME(WfModuleUsingItem, Workflow::WfModuleUsingItem)
			IMPL_TYPE_INFO_RENAME(WfModuleUsingPath, Workflow::WfModuleUsingPath)
			IMPL_TYPE_INFO_RENAME(WfModule, Workflow::WfModule)
			IMPL_TYPE_INFO_RENAME(WfType::IVisitor, Workflow::WfType::IVisitor)
			IMPL_TYPE_INFO_RENAME(WfExpression::IVisitor, Workflow::WfExpression::IVisitor)
			IMPL_TYPE_INFO_RENAME(WfStatement::IVisitor, Workflow::WfStatement::IVisitor)
			IMPL_TYPE_INFO_RENAME(WfDeclaration::IVisitor, Workflow::WfDeclaration::IVisitor)

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

			BEGIN_CLASS_MEMBER(WfOrderedNameExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfOrderedNameExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfOrderedNameExpression::*)(), [](WfOrderedNameExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfOrderedNameExpression::*)(const vl::WString&), [](WfOrderedNameExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfOrderedNameExpression)

			BEGIN_CLASS_MEMBER(WfOrderedLambdaExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfOrderedLambdaExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(body)
			END_CLASS_MEMBER(WfOrderedLambdaExpression)

			BEGIN_CLASS_MEMBER(WfMemberExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfMemberExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfMemberExpression::*)(), [](WfMemberExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfMemberExpression::*)(const vl::WString&), [](WfMemberExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfMemberExpression)

			BEGIN_CLASS_MEMBER(WfChildExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfChildExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfChildExpression::*)(), [](WfChildExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfChildExpression::*)(const vl::WString&), [](WfChildExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfChildExpression)

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
				ENUM_NAMESPACE_ITEM(Assign)
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

			BEGIN_ENUM_ITEM(WfSetTesting)
				ENUM_ITEM_NAMESPACE(WfSetTesting)
				ENUM_NAMESPACE_ITEM(In)
				ENUM_NAMESPACE_ITEM(NotIn)
			END_ENUM_ITEM(WfSetTesting)

			BEGIN_CLASS_MEMBER(WfSetTestingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfSetTestingExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(test)
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

			BEGIN_ENUM_ITEM(WfTypeTesting)
				ENUM_ITEM_NAMESPACE(WfTypeTesting)
				ENUM_NAMESPACE_ITEM(IsType)
				ENUM_NAMESPACE_ITEM(IsNotType)
				ENUM_NAMESPACE_ITEM(IsNull)
				ENUM_NAMESPACE_ITEM(IsNotNull)
			END_ENUM_ITEM(WfTypeTesting)

			BEGIN_CLASS_MEMBER(WfTypeTestingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTypeTestingExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(test)
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

			BEGIN_CLASS_MEMBER(WfAttachEventExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfAttachEventExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(event)
				CLASS_MEMBER_FIELD(function)
			END_CLASS_MEMBER(WfAttachEventExpression)

			BEGIN_CLASS_MEMBER(WfDetachEventExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfDetachEventExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(handler)
			END_CLASS_MEMBER(WfDetachEventExpression)

			BEGIN_CLASS_MEMBER(WfBindExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfBindExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfBindExpression)

			BEGIN_ENUM_ITEM(WfObserveType)
				ENUM_ITEM_NAMESPACE(WfObserveType)
				ENUM_NAMESPACE_ITEM(SimpleObserve)
				ENUM_NAMESPACE_ITEM(ExtendedObserve)
			END_ENUM_ITEM(WfObserveType)

			BEGIN_CLASS_MEMBER(WfObserveExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfObserveExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfObserveExpression::*)(), [](WfObserveExpression* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfObserveExpression::*)(const vl::WString&), [](WfObserveExpression* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_FIELD(observeType)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(events)
			END_CLASS_MEMBER(WfObserveExpression)

			BEGIN_CLASS_MEMBER(WfCallExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfCallExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(function)
				CLASS_MEMBER_FIELD(arguments)
			END_CLASS_MEMBER(WfCallExpression)

			BEGIN_CLASS_MEMBER(WfStatement)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfStatement)

			BEGIN_CLASS_MEMBER(WfVariableStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfVariableStatement>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfVariableStatement::*)(), [](WfVariableStatement* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfVariableStatement::*)(const vl::WString&), [](WfVariableStatement* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(type)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfVariableStatement)

			BEGIN_CLASS_MEMBER(WfBreakStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfBreakStatement>(), NO_PARAMETER)


			END_CLASS_MEMBER(WfBreakStatement)

			BEGIN_CLASS_MEMBER(WfContinueStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfContinueStatement>(), NO_PARAMETER)


			END_CLASS_MEMBER(WfContinueStatement)

			BEGIN_CLASS_MEMBER(WfReturnStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfReturnStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfReturnStatement)

			BEGIN_CLASS_MEMBER(WfDeleteStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfDeleteStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfDeleteStatement)

			BEGIN_CLASS_MEMBER(WfRaiseExceptionStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfRaiseExceptionStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfRaiseExceptionStatement)

			BEGIN_CLASS_MEMBER(WfIfStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfIfStatement>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfIfStatement::*)(), [](WfIfStatement* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfIfStatement::*)(const vl::WString&), [](WfIfStatement* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(type)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(trueBranch)
				CLASS_MEMBER_FIELD(falseBranch)
			END_CLASS_MEMBER(WfIfStatement)

			BEGIN_CLASS_MEMBER(WfSwitchCase)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfSwitchCase>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(statement)
			END_CLASS_MEMBER(WfSwitchCase)

			BEGIN_CLASS_MEMBER(WfSwitchStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfSwitchStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
				CLASS_MEMBER_FIELD(caseBranches)
				CLASS_MEMBER_FIELD(defaultBranch)
			END_CLASS_MEMBER(WfSwitchStatement)

			BEGIN_CLASS_MEMBER(WfWhileStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfWhileStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(condition)
				CLASS_MEMBER_FIELD(statement)
			END_CLASS_MEMBER(WfWhileStatement)

			BEGIN_ENUM_ITEM(WfForEachDirection)
				ENUM_ITEM_NAMESPACE(WfForEachDirection)
				ENUM_NAMESPACE_ITEM(Normal)
				ENUM_NAMESPACE_ITEM(Reversed)
			END_ENUM_ITEM(WfForEachDirection)

			BEGIN_CLASS_MEMBER(WfForEachStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfForEachStatement>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfForEachStatement::*)(), [](WfForEachStatement* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfForEachStatement::*)(const vl::WString&), [](WfForEachStatement* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(direction)
				CLASS_MEMBER_FIELD(collection)
				CLASS_MEMBER_FIELD(statement)
			END_CLASS_MEMBER(WfForEachStatement)

			BEGIN_CLASS_MEMBER(WfTryStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfTryStatement>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfTryStatement::*)(), [](WfTryStatement* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfTryStatement::*)(const vl::WString&), [](WfTryStatement* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(protectedStatement)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(catchStatement)
				CLASS_MEMBER_FIELD(finallyStatement)
			END_CLASS_MEMBER(WfTryStatement)

			BEGIN_CLASS_MEMBER(WfBlockStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfBlockStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(statements)
			END_CLASS_MEMBER(WfBlockStatement)

			BEGIN_CLASS_MEMBER(WfExpressionStatement)
				CLASS_MEMBER_BASE(WfStatement)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfExpressionStatement>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(expression)
			END_CLASS_MEMBER(WfExpressionStatement)

			BEGIN_CLASS_MEMBER(WfDeclaration)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfDeclaration)

			BEGIN_CLASS_MEMBER(WfNamespaceDeclaration)
				CLASS_MEMBER_BASE(WfDeclaration)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfNamespaceDeclaration>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfNamespaceDeclaration::*)(), [](WfNamespaceDeclaration* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfNamespaceDeclaration::*)(const vl::WString&), [](WfNamespaceDeclaration* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(declarations)
			END_CLASS_MEMBER(WfNamespaceDeclaration)

			BEGIN_CLASS_MEMBER(WfFunctionArgument)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFunctionArgument>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfFunctionArgument::*)(), [](WfFunctionArgument* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfFunctionArgument::*)(const vl::WString&), [](WfFunctionArgument* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(type)
			END_CLASS_MEMBER(WfFunctionArgument)

			BEGIN_ENUM_ITEM(WfFunctionAnonymity)
				ENUM_ITEM_NAMESPACE(WfFunctionAnonymity)
				ENUM_NAMESPACE_ITEM(Named)
				ENUM_NAMESPACE_ITEM(Anonymous)
			END_ENUM_ITEM(WfFunctionAnonymity)

			BEGIN_CLASS_MEMBER(WfFunctionDeclaration)
				CLASS_MEMBER_BASE(WfDeclaration)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFunctionDeclaration>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfFunctionDeclaration::*)(), [](WfFunctionDeclaration* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfFunctionDeclaration::*)(const vl::WString&), [](WfFunctionDeclaration* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_FIELD(anonymity)
				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(arguments)
				CLASS_MEMBER_FIELD(statement)
			END_CLASS_MEMBER(WfFunctionDeclaration)

			BEGIN_CLASS_MEMBER(WfFunctionExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFunctionExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(function)
			END_CLASS_MEMBER(WfFunctionExpression)

			BEGIN_CLASS_MEMBER(WfNewTypeExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfNewTypeExpression>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(type)
				CLASS_MEMBER_FIELD(arguments)
				CLASS_MEMBER_FIELD(functions)
			END_CLASS_MEMBER(WfNewTypeExpression)

			BEGIN_CLASS_MEMBER(WfModuleUsingItem)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfModuleUsingItem>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfModuleUsingItem::*)(), [](WfModuleUsingItem* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfModuleUsingItem::*)(const vl::WString&), [](WfModuleUsingItem* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
			END_CLASS_MEMBER(WfModuleUsingItem)

			BEGIN_CLASS_MEMBER(WfModuleUsingPath)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfModuleUsingPath>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(items)
			END_CLASS_MEMBER(WfModuleUsingPath)

			BEGIN_CLASS_MEMBER(WfModule)
				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfModule>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_name, NO_PARAMETER, vl::WString(WfModule::*)(), [](WfModule* node){ return node->name.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_name, {L"value"}, void(WfModule::*)(const vl::WString&), [](WfModule* node, const vl::WString& value){ node->name.value = value; })

				CLASS_MEMBER_PROPERTY(name, get_name, set_name)
				CLASS_MEMBER_FIELD(paths)
				CLASS_MEMBER_FIELD(declarations)
			END_CLASS_MEMBER(WfModule)

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
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfOrderedNameExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfOrderedLambdaExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfMemberExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfChildExpression* node))
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
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfAttachEventExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfDetachEventExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfBindExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfObserveExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfCallExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFunctionExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfNewTypeExpression* node))
			END_CLASS_MEMBER(WfExpression)

			BEGIN_CLASS_MEMBER(WfStatement::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfStatement::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfStatement_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfVariableStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfBreakStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfContinueStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfReturnStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfDeleteStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfRaiseExceptionStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfIfStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfSwitchStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfWhileStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfForEachStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfTryStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfBlockStatement* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfStatement::IVisitor::*)(WfExpressionStatement* node))
			END_CLASS_MEMBER(WfStatement)

			BEGIN_CLASS_MEMBER(WfDeclaration::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfDeclaration::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfDeclaration_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfDeclaration::IVisitor::*)(WfNamespaceDeclaration* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfDeclaration::IVisitor::*)(WfFunctionDeclaration* node))
			END_CLASS_MEMBER(WfDeclaration)

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
					ADD_TYPE_INFO(vl::workflow::WfOrderedNameExpression)
					ADD_TYPE_INFO(vl::workflow::WfOrderedLambdaExpression)
					ADD_TYPE_INFO(vl::workflow::WfMemberExpression)
					ADD_TYPE_INFO(vl::workflow::WfChildExpression)
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
					ADD_TYPE_INFO(vl::workflow::WfSetTesting)
					ADD_TYPE_INFO(vl::workflow::WfSetTestingExpression)
					ADD_TYPE_INFO(vl::workflow::WfConstructorArgument)
					ADD_TYPE_INFO(vl::workflow::WfConstructorExpression)
					ADD_TYPE_INFO(vl::workflow::WfInferExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeCastingStrategy)
					ADD_TYPE_INFO(vl::workflow::WfTypeCastingExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeTesting)
					ADD_TYPE_INFO(vl::workflow::WfTypeTestingExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeOfTypeExpression)
					ADD_TYPE_INFO(vl::workflow::WfTypeOfExpressionExpression)
					ADD_TYPE_INFO(vl::workflow::WfAttachEventExpression)
					ADD_TYPE_INFO(vl::workflow::WfDetachEventExpression)
					ADD_TYPE_INFO(vl::workflow::WfBindExpression)
					ADD_TYPE_INFO(vl::workflow::WfObserveType)
					ADD_TYPE_INFO(vl::workflow::WfObserveExpression)
					ADD_TYPE_INFO(vl::workflow::WfCallExpression)
					ADD_TYPE_INFO(vl::workflow::WfStatement)
					ADD_TYPE_INFO(vl::workflow::WfVariableStatement)
					ADD_TYPE_INFO(vl::workflow::WfBreakStatement)
					ADD_TYPE_INFO(vl::workflow::WfContinueStatement)
					ADD_TYPE_INFO(vl::workflow::WfReturnStatement)
					ADD_TYPE_INFO(vl::workflow::WfDeleteStatement)
					ADD_TYPE_INFO(vl::workflow::WfRaiseExceptionStatement)
					ADD_TYPE_INFO(vl::workflow::WfIfStatement)
					ADD_TYPE_INFO(vl::workflow::WfSwitchCase)
					ADD_TYPE_INFO(vl::workflow::WfSwitchStatement)
					ADD_TYPE_INFO(vl::workflow::WfWhileStatement)
					ADD_TYPE_INFO(vl::workflow::WfForEachDirection)
					ADD_TYPE_INFO(vl::workflow::WfForEachStatement)
					ADD_TYPE_INFO(vl::workflow::WfTryStatement)
					ADD_TYPE_INFO(vl::workflow::WfBlockStatement)
					ADD_TYPE_INFO(vl::workflow::WfExpressionStatement)
					ADD_TYPE_INFO(vl::workflow::WfDeclaration)
					ADD_TYPE_INFO(vl::workflow::WfNamespaceDeclaration)
					ADD_TYPE_INFO(vl::workflow::WfFunctionArgument)
					ADD_TYPE_INFO(vl::workflow::WfFunctionAnonymity)
					ADD_TYPE_INFO(vl::workflow::WfFunctionDeclaration)
					ADD_TYPE_INFO(vl::workflow::WfFunctionExpression)
					ADD_TYPE_INFO(vl::workflow::WfNewTypeExpression)
					ADD_TYPE_INFO(vl::workflow::WfModuleUsingItem)
					ADD_TYPE_INFO(vl::workflow::WfModuleUsingPath)
					ADD_TYPE_INFO(vl::workflow::WfModule)
					ADD_TYPE_INFO(vl::workflow::WfType::IVisitor)
					ADD_TYPE_INFO(vl::workflow::WfExpression::IVisitor)
					ADD_TYPE_INFO(vl::workflow::WfStatement::IVisitor)
					ADD_TYPE_INFO(vl::workflow::WfDeclaration::IVisitor)
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
