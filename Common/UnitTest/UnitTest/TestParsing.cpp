#include <string.h>
#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\Accessor.h"
#include "..\..\Source\Stream\CharFormat.h"
#include "..\..\Source\Collections\Operation.h"

#include "..\..\Source\Parsing\ParsingDefinitions.h"
#include "..\..\Source\Parsing\ParsingAnalyzer.h"
#include "..\..\Source\Parsing\ParsingAutomaton.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::parsing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::analyzing;

extern WString GetPath();

namespace test
{
	void GeneralTest(Ptr<ParsingDefinition> definition, const WString& name)
	{
		FileStream fileStream(GetPath()+L"Parsing."+name+L".Definition.txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Grammar");
		writer.WriteLine(L"=============================================================");
		Log(definition, writer);

		ParsingSymbolManager symbolManager;
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &symbolManager, errors);
		if(errors.Count()>0)
		{
			writer.WriteLine(L"=============================================================");
			writer.WriteLine(L"Errprs");
			writer.WriteLine(L"=============================================================");
			FOREACH(Ptr<ParsingError>, error, errors.Wrap())
			{
				writer.WriteLine(error->errorMessage);
			}

			encoderStream.Close();
			fileStream.Close();
		}
		TEST_ASSERT(errors.Count()==0);

		Ptr<Automaton> automaton=CreateEpsilonPDA(definition, &symbolManager);
		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Epsilon PDA");
		writer.WriteLine(L"=============================================================");
		Log(automaton, writer);
	}
}
using namespace test;

TEST_CASE(TestParsingExpression)
{
	Ptr<ParsingDefinition> definition;
	{
		ParsingDefinitionWriter definitionWriter;

		definitionWriter
			.Type(
				Class(L"Expression")
			)
			.Type(
				Class(L"NumberExpression", Type(L"Expression"))
					.Member(L"value", TokenType())
			)
			.Type(
				Class(L"BinaryExpression", Type(L"Expression"))
					.SubType(
						Enum(L"BinaryOperator")
							.Member(L"Add")
							.Member(L"Sub")
							.Member(L"Mul")
							.Member(L"Div")
					)
					.Member(L"firstOperand", Type(L"Expression"))
					.Member(L"secondOperand", Type(L"Expression"))
					.Member(L"binaryOperator", Type(L"BinaryOperator"))
				)
			.Type(
				Class(L"FunctionExpression", Type(L"Expression"))
					.Member(L"functionName", TokenType())
					.Member(L"arguments", Type(L"Expression").Array())
			)

			.Token(L"NAME",		L"[a-zA-Z_]/w*")
			.Token(L"NUMBER",	L"/d+(./d+)")
			.Token(L"ADD",		L"/+")
			.Token(L"SUB",		L"-")
			.Token(L"MUL",		L"/*")
			.Token(L"DIV",		L"//")
			.Token(L"LEFT",		L"/(")
			.Token(L"RIGHT",	L"/)")
			.Token(L"COMMA",	L",")

			.Rule(L"Number", Type(L"NumberExpression"))
				.Imply(Rule(L"NUMBER")[L"value"])
				.EndRule()

			.Rule(L"Call", Type(L"FunctionExpression"))
				.Imply(Rule(L"NAME")[L"functionName"] + Text(L"(") + Opt(Rule(L"Exp")[L"arguments"] + *(Text(L",") + Rule(L"Exp")[L"arguments"])) + Text(L")"))
				.EndRule()

			.Rule(L"Factor", Type(L"Expression"))
				.Imply(!Rule(L"Number") | !Rule(L"Call"))
				.EndRule()

			.Rule(L"Term", Type(L"Expression"))
				.Imply(!Rule(L"Factor"))
				.Imply(
					(Rule(L"Term")[L"firstOperand"] + Text(L"*") + Rule(L"Factor")[L"secondOperand"])
						.As(Type(L"BinaryExpression"))
						.Set(L"binaryOperator", L"Mul")
					)
				.Imply(
					(Rule(L"Term")[L"firstOperand"] + Text(L"/") + Rule(L"Factor")[L"secondOperand"])
						.As(Type(L"BinaryExpression"))
						.Set(L"binaryOperator", L"Div")
					)
				.EndRule()

			.Rule(L"Exp", Type(L"Expression"))
				.Imply(!Rule(L"Term"))
				.Imply(
					(Rule(L"Exp")[L"firstOperand"] + Text(L"+") + Rule(L"Term")[L"secondOperand"])
						.As(Type(L"BinaryExpression"))
						.Set(L"binaryOperator", L"Add")
					)
				.Imply(
					(Rule(L"Exp")[L"firstOperand"] + Text(L"-") + Rule(L"Term")[L"secondOperand"])
						.As(Type(L"BinaryExpression"))
						.Set(L"binaryOperator", L"Sub")
					)
				.EndRule()
			;

		definition=definitionWriter.Definition();
	}
	GeneralTest(definition, L"Calculator");
}