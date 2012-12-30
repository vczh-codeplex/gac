#include <string.h>
#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\Accessor.h"
#include "..\..\Source\Stream\CharFormat.h"
#include "..\..\Source\Collections\Operation.h"

#include "..\..\Source\Parsing\ParsingDefinitions.h"
#include "..\..\Source\Parsing\ParsingAnalyzer.h"
#include "..\..\Source\Parsing\ParsingAutomaton.h"
#include "..\..\Source\Parsing\ParsingTable.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::analyzing;
using namespace vl::parsing::tabling;

extern WString GetPath();

namespace test
{
	void LogError(List<Ptr<ParsingError>>& errors, StreamWriter& writer)
	{
		if(errors.Count()>0)
		{
			writer.WriteLine(L"=============================================================");
			writer.WriteLine(L"Errors");
			writer.WriteLine(L"=============================================================");
			FOREACH(Ptr<ParsingError>, error, errors)
			{
				writer.WriteLine(error->errorMessage);
			}
		}
	}

	template<typename TLoggable>
	void LogParsingData(TLoggable loggable, const WString& fileName, const WString& name, List<Ptr<ParsingError>>& errors=*(List<Ptr<ParsingError>>*)0)
	{
		FileStream fileStream(GetPath()+fileName, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(name);
		writer.WriteLine(L"=============================================================");
		Log(loggable, writer);

		if(&errors)
		{
			LogError(errors, writer);
		}
	}

	Ptr<ParsingTable> CreateTable(Ptr<ParsingDefinition> definition, const WString& name)
	{
		ParsingSymbolManager symbolManager;
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &symbolManager, errors);
		LogParsingData(definition, L"Parsing."+name+L".Definition.txt", L"Grammar Definition", errors);
		TEST_ASSERT(errors.Count()==0);

		Ptr<Automaton> epsilonPDA=CreateEpsilonPDA(definition, &symbolManager);
		Ptr<Automaton> nondeterministicPDA=CreateNondeterministicPDAFromEpsilonPDA(epsilonPDA);
		Ptr<Automaton> jointPDA=CreateJointPDAFromNondeterministicPDA(nondeterministicPDA);
		
		LogParsingData(epsilonPDA, L"Parsing."+name+L".EPDA.txt", L"Epsilon PDA");
		LogParsingData(nondeterministicPDA, L"Parsing."+name+L".NPDA.txt", L"Non-deterministic PDA");
		LogParsingData(jointPDA, L"Parsing."+name+L".JPDA.txt", L"Joint PDA");

		CompactJointPDA(jointPDA);
		LogParsingData(jointPDA, L"Parsing."+name+L".JPDA-Compacted.txt", L"Compacted Joint PDA");

		MarkLeftRecursiveInJointPDA(jointPDA, errors);
		LogParsingData(jointPDA, L"Parsing."+name+L".JPDA-Marked.txt", L"Compacted Joint PDA", errors);
		TEST_ASSERT(errors.Count()==0);

		Ptr<ParsingTable> table=GenerateTable(definition, jointPDA, errors);
		LogParsingData(table, L"Parsing."+name+L".Table.txt", L"Table", errors);
		TEST_ASSERT(errors.Count()==0);

		return table;
	}

	void Parse(Ptr<ParsingTable> table, const WString& input, const WString& name, const WString& rule, vint index)
	{
		FileStream fileStream(GetPath()+L"Parsing."+name+L".["+itow(index)+L"].txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		ParsingState state(input, table);
		ParsingTreeBuilder builder;
		builder.Reset();

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Input");
		writer.WriteLine(L"=============================================================");
		writer.WriteLine(input);

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Tokens");
		writer.WriteLine(L"=============================================================");
		FOREACH(RegexToken, token, state.GetTokens())
		{
			WString tokenName;
			if(token.token==-1)
			{
				tokenName=L"<ERROR>";
			}
			else if(table->IsInputToken(token.token))
			{
				tokenName=table->GetTokenInfo(table->GetTableTokenIndex(token.token)).name;
			}
			else
			{
				tokenName=table->GetDiscardTokenInfo(table->GetTableDiscardTokenIndex(token.token)).name;
			}
			writer.WriteLine(tokenName+L": "+WString(token.reading, token.length));
		}

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Transition");
		writer.WriteLine(L"=============================================================");
		vint startState=state.Reset(rule);
		TEST_ASSERT(startState!=-1);
		writer.WriteLine(L"StartState: "+itow(startState)+L"["+table->GetStateInfo(startState).stateName+L"]");

		while(true)
		{
			ParsingState::TransitionResult result=state.ReadToken();
			if(result)
			{
				switch(result.tableTokenIndex)
				{
				case ParsingTable::TokenBegin:
					writer.WriteString(L"$TokenBegin => ");
					break;
				case ParsingTable::TokenFinish:
					writer.WriteString(L"$TokenFinish => ");
					break;
				default:
					writer.WriteString(table->GetTokenInfo(result.tableTokenIndex).name);
					writer.WriteString(L"[");
					writer.WriteString(WString(result.token->reading, result.token->length));
					writer.WriteString(L"] => ");
				}
				writer.WriteLine(itow(result.tableStateTarget)+L"["+table->GetStateInfo(result.tableStateTarget).stateName+L"]");

				writer.WriteString(L"    State Stack: ");
				FOREACH_INDEXER(vint, stateIndex, i, state.GetStateStack())
				{
					if(i!=0) writer.WriteString(L", ");
					writer.WriteString(itow(stateIndex)+L"["+table->GetStateInfo(stateIndex).stateName+L"]");
				}
				writer.WriteLine(L"");

				FOREACH(ParsingTable::Instruction, ins, result.transition->instructions)
				{
					switch(ins.instructionType)
					{
					case ParsingTable::Instruction::Create:
						writer.WriteLine(L"    Create "+ins.nameParameter);
						break;
					case ParsingTable::Instruction::Using:
						writer.WriteLine(L"    Using");
						break;
					case ParsingTable::Instruction::Assign:
						writer.WriteLine(L"    Assign "+ins.nameParameter);
						break;
					case ParsingTable::Instruction::Item:
						writer.WriteLine(L"    Item "+ins.nameParameter);
						break;
					case ParsingTable::Instruction::Setter:
						writer.WriteLine(L"    Setter "+ins.nameParameter+L" = "+ins.value);
						break;
					case ParsingTable::Instruction::Shift:
						writer.WriteLine(L"    Shift "+itow(ins.stateParameter)+L"["+table->GetStateInfo(ins.stateParameter).ruleName+L"]");
						break;
					case ParsingTable::Instruction::Reduce:
						writer.WriteLine(L"    Reduce "+itow(ins.stateParameter)+L"["+table->GetStateInfo(ins.stateParameter).ruleName+L"]");
						break;
					case ParsingTable::Instruction::LeftRecursiveReduce:
						writer.WriteLine(L"    LR-Reduce "+itow(ins.stateParameter)+L"["+table->GetStateInfo(ins.stateParameter).ruleName+L"]");
						break;
					}
				}
			}
			else
			{
				break;
			}
		}

		writer.WriteLine(L"=============================================================");
		writer.WriteLine(L"Tree");
		writer.WriteLine(L"=============================================================");
		Ptr<ParsingTreeNode> node=builder.GetNode();
		if(node)
		{
			Log(node, writer);
		}
	}
}
using namespace test;

TEST_CASE(TestParseNameList)
{
	Ptr<ParsingDefinition> definition;
	{
		ParsingDefinitionWriter definitionWriter;

		definitionWriter
			.Type(
				Class(L"NameItemExpression")
					.Member(L"name", TokenType())
				)
			.Type(
				Class(L"NameListExpression")
					.Member(L"names", Type(L"NameItemExpression").Array())
				)

			.Token(L"NAME", L"[a-zA-Z_]/w*")
			.Token(L"COMMA", L",")
			.Discard(L"SPACE", L"/s+")

			.Rule(L"Name", Type(L"NameItemExpression"))
				.Imply(
					(Rule(L"NAME")[L"name"])
						.As(Type(L"NameItemExpression"))
					)
				.EndRule()

			.Rule(L"NameList", Type(L"NameListExpression"))
				.Imply(
					(Rule(L"Name")[L"names"] + *(Text(L",") + Rule(L"Name")[L"names"]))
						.As(Type(L"NameListExpression"))
					)
				.EndRule()
			;

		definition=definitionWriter.Definition();
	}

	Ptr<ParsingTable> table=CreateTable(definition, L"NameList");
	const wchar_t* inputs[]=
	{
		L"vczh",
		L"vczh, genius",
		L"vczh, genius, programmer",
	};
	for(vint i=0;i<sizeof(inputs)/sizeof(*inputs);i++)
	{
		Parse(table, inputs[i], L"NameList", L"NameList", i);
	}
}

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
			.Token(L"NUMBER",	L"/d+(./d+)?")
			.Token(L"ADD",		L"/+")
			.Token(L"SUB",		L"-")
			.Token(L"MUL",		L"/*")
			.Token(L"DIV",		L"//")
			.Token(L"LEFT",		L"/(")
			.Token(L"RIGHT",	L"/)")
			.Token(L"COMMA",	L",")
			.Discard(L"SPACE", L"/s+")

			.Rule(L"Number", Type(L"NumberExpression"))
				.Imply(
					Rule(L"NUMBER")[L"value"]
						.As(Type(L"NumberExpression"))
					)
				.EndRule()

			.Rule(L"Call", Type(L"FunctionExpression"))
				.Imply(
					(Rule(L"NAME")[L"functionName"] + Text(L"(") + Opt(Rule(L"Exp")[L"arguments"] + *(Text(L",") + Rule(L"Exp")[L"arguments"])) + Text(L")"))
						.As(Type(L"FunctionExpression"))
					)
				.EndRule()

			.Rule(L"Factor", Type(L"Expression"))
				.Imply(!Rule(L"Number") | !Rule(L"Call"))
				.Imply(Text(L"(") + !Rule(L"Exp") + Text(L")"))
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

	Ptr<ParsingTable> table=CreateTable(definition, L"Calculator");
	const wchar_t* inputs[]=
	{
		L"0",
		L"1+2",
		L"1*2+3*4",
		L"(1+2)*(3+4)",
		L"exp()-pow(cos(1.57*2), 4)",
	};
	for(vint i=0;i<sizeof(inputs)/sizeof(*inputs);i++)
	{
		Parse(table, inputs[i], L"Calculator", L"Exp", i);
	}
}