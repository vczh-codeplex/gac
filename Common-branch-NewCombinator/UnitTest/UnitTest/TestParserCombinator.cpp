#include "..\..\Source\UnitTest\UnitTest.h"

/*
Rule<Expression> Factor(L"Value"), Term(L"Term"), Exp(L"Exp")

Factor
	= create<ValueExpression, Expression>(tok(NUMBER)[&ValueExpression::Number]))
	| tok(L"(") + use(Exp) + tok(L")")
	;

Term
	= use(Factor)
	| create<BinaryExpression, Expression>(Term[&BinaryExpression::Left] + (tok(L"*")|tok(K"/"))[&BinaryExpression::Operator] + Factor[&BinaryExpression::Right])
	;

Exp
	= use(Term)
	| create<BinaryExpression, Expression>(Exp[&BinaryExpression::Left] + (tok(L"+")|tok(K"-"))[&BinaryExpression::Operator] + Term[&BinaryExpression::Right])
	;
*/

TEST_CASE(TestParserCombinator)
{
}