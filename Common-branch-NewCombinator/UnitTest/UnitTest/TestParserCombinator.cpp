#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Parsing\ParsingGrammarRegex.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\CharFormat.h"

using namespace vl;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::stream;

extern WString GetPath();

namespace TestParsing_Expression_Helper
{
	class Expression : public Object
	{
	public:
	};

	class Value : public Expression
	{
	public:
		double				number;
	};

	class Operator : public Expression
	{
	public:
		wchar_t				op;
		Ptr<Expression>		left;
		Ptr<Expression>		right;
	};

	double ToId(const RegexToken& token)
	{
		return wtof(WString(token.reading, token.length));
	}

	wchar_t ToOp(RegexToken token)
	{
		return *token.reading;
	}

	class ExpressionGrammar : public RegexGrammar
	{
	public:
		ExpressionGrammar()
		{
			vint LBRACKET	=CreateToken(L"(",			L"/(");
			vint RBRACKET	=CreateToken(L"R",			L"/R");
			vint ADD		=CreateToken(L"+",			L"/+");
			vint SUB		=CreateToken(L"-",			L"-");
			vint MUL		=CreateToken(L"*",			L"/*");
			vint DIV		=CreateToken(L"/",			L"//");
			vint NUMBER		=CreateToken(L"NUMBER",		L"/d+(./d+)?");

			Rule<Ptr<Expression>>	Exp(L"Exp"), Term(L"Term"), Factor(L"Factor");

			Factor	= create<Value>(
						assign(transform(tok(NUMBER), &ToId), &Value::number)
						)
					;
			Factor	= tok(LBRACKET) + use(Exp) + tok(RBRACKET)
					;

			Term	= use(Factor)
					;
			Term	= create<Operator>(
						assign(Term, &Operator::left) + assign(transform(tok(MUL) | tok(DIV), &ToOp), &Operator::op) + assign(Factor, &Operator::right)
						)
					;

			Exp		= use(Term)
					;
			Exp		= create<Operator>(
						assign(Exp, &Operator::left) + assign(transform(tok(ADD) | tok(SUB), &ToOp), &Operator::op) + assign(Term, &Operator::right)
						)
					;

			{
				FileStream fileStream(GetPath()+L"Grammar_Expression_Log.txt", FileStream::WriteOnly);
				BomEncoder encoder(BomEncoder::Utf16);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);
				LogGrammarFromRule(Exp.GetRuleNode(), writer);
			}
		}
	};
}
using namespace TestParsing_Expression_Helper;

TEST_CASE(TestParsing_Expression)
{
	ExpressionGrammar grammar;
}