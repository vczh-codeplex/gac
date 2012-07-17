#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Parsing\ParsingGrammarRegex.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\CharFormat.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::stream;

extern WString GetPath();

/***********************************************************************
Expression
***********************************************************************/

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
			vint LBRACKET	=CreateToken(L"\"(\"",			L"/(");
			vint RBRACKET	=CreateToken(L"\")\"",			L"/)");
			vint ADD		=CreateToken(L"\"+\"",			L"/+");
			vint SUB		=CreateToken(L"\"-\"",			L"-");
			vint MUL		=CreateToken(L"\"*\"",			L"/*");
			vint DIV		=CreateToken(L"\"/\"",			L"//");
			vint NUMBER		=CreateToken(L"NUMBER",			L"/d+(./d+)?");

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

				BuildParser(Exp.GetRuleNode().Obj(), &writer);
			}
		}
	};
}
using namespace TestParsing_Expression_Helper;

TEST_CASE(TestParsing_Expression)
{
	ExpressionGrammar grammar;
}

/***********************************************************************
Type
***********************************************************************/

namespace TestParsing_Type_Helper
{
	class Type : public Object
	{
	};

	class PointerType : public Type
	{
	public:
		Ptr<Type>			elementType;
	};

	class ReferenceType : public Type
	{
	public:
		Ptr<Type>			elementType;
	};

	class ArrayType : public Type
	{
	public:
		Ptr<Type>			elementType;
	};

	class PrimitiveType : public Type
	{
	public:
		WString				name;
	};

	class GenericType : public Type
	{
	public:
		Ptr<Type>			type;
		List<Ptr<Type>>		arguments;
	};

	class SubType : public Type
	{
	public:
		Ptr<Type>			type;
		WString				name;
	};

	class FunctionType : public Type
	{
	public:
		Ptr<Type>			returnType;
		List<Ptr<Type>>		arguments;
	};

	WString ToName(const RegexToken& token)
	{
		return WString(token.reading, token.length);
	}

	class TypeGrammar : public RegexGrammar
	{
	public:
		TypeGrammar()
		{
			vint GOPEN		=CreateToken(L"\"<\"",			L"<");
			vint GCLOSE		=CreateToken(L"\">\"",			L">");
			vint AOPEN		=CreateToken(L"\"[\"",			L"/[");
			vint ACLOSE		=CreateToken(L"\"]\"",			L"/]");
			vint FOPEN		=CreateToken(L"\"(\"",			L"/(");
			vint FCLOSE		=CreateToken(L"\")\"",			L"/)");
			vint NAME		=CreateToken(L"NAME",			L"[a-zA-Z_]/w*");
			vint COMMA		=CreateToken(L"\",\"",			L",");
			vint SUB		=CreateToken(L"\"::\"",			L"::");
			vint POINTER	=CreateToken(L"\"*\"",			L"/*");
			vint REFERENCE	=CreateToken(L"\"&\"",			L"&");

			Rule<Ptr<Type>> Type(L"Type"), SubableType(L"SubableType");

			SubableType	= create<PrimitiveType>(assign(transform(tok(NAME), &ToName), &PrimitiveType::name));
			SubableType	= create<GenericType>(
							assign(SubableType, &GenericType::type) +
							tok(GOPEN) +
							assign(Type, &GenericType::arguments) + *(tok(COMMA) + assign(Type, &GenericType::arguments)) +
							tok(GCLOSE)
							);
			SubableType	= create<SubType>(assign(SubableType, &SubType::type) + tok(SUB) + assign(transform(tok(NAME), &ToName), &SubType::name));

			Type		= use(SubableType);
			Type		= create<PointerType>(assign(Type, &PointerType::elementType) + tok(POINTER));
			Type		= create<ReferenceType>(assign(Type, &ReferenceType::elementType) + tok(REFERENCE));
			Type		= create<ArrayType>(assign(Type, &ArrayType::elementType) + tok(AOPEN) + tok(ACLOSE));
			Type		= create<FunctionType>(
							assign(Type, &FunctionType::returnType) +
							tok(FOPEN) +
							assign(Type, &FunctionType::arguments) + *(tok(COMMA) + assign(Type, &FunctionType::arguments)) +
							tok(FCLOSE)
							);

			{
				FileStream fileStream(GetPath()+L"Grammar_Type_Log.txt", FileStream::WriteOnly);
				BomEncoder encoder(BomEncoder::Utf16);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);

				BuildParser(Type.GetRuleNode().Obj(), &writer);
			}
		}
	};
}
using namespace TestParsing_Type_Helper;

TEST_CASE(TestParsing_Type)
{
	TypeGrammar grammar;
}