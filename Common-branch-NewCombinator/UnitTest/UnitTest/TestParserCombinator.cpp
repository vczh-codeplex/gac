#include "..\..\Source\UnitTest\UnitTest.h"

/*
Rule<Expression> Factor(L"Value"), Term(L"Term"), Exp(L"Exp")

Factor
	= tok(NUMBER)[&ValueExpression::Number])
	| tok(L"(") >> Exp << tok(L")")
	;

Term
	= Factor
	| Term[&ValueExpression::Left] + (tok(L"*")|tok(K"/"))[&ValueExpression::Operator] + Factor[&ValueExpression::Right]
	;

Exp
	= Term
	| Exp[&ValueExpression::Left] + (tok(L"*")|tok(K"/"))[&ValueExpression::Operator] + Term[&ValueExpression::Right]
	;
*/

TEST_CASE(TestParserCombinator)
{
}