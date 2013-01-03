#include <string.h>
#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\MemoryStream.h"
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

		if(&errors && errors.Count()>0)
		{
			writer.WriteLine(L"=============================================================");
			writer.WriteLine(L"Errors");
			writer.WriteLine(L"=============================================================");
			FOREACH(Ptr<ParsingError>, error, errors)
			{
				writer.WriteLine(error->errorMessage);
			}
			unittest::UnitTest::PrintInfo(L"Errors are logged in \""+fileName+L"\".");
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

	Ptr<ParsingTreeNode> Parse(Ptr<ParsingTable> table, const WString& input, const WString& name, const WString& rule, vint index)
	{
		unittest::UnitTest::PrintInfo(L"Parsing: "+input);
		bool meetTokenFinish=false;
		Ptr<ParsingTreeNode> node;
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
				TEST_ASSERT(token.token!=-1);
				if(table->IsInputToken(token.token))
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
					TEST_ASSERT(!meetTokenFinish);
					switch(result.tableTokenIndex)
					{
					case ParsingTable::TokenBegin:
						writer.WriteString(L"$TokenBegin => ");
						break;
					case ParsingTable::TokenFinish:
						writer.WriteString(L"$TokenFinish => ");
						meetTokenFinish=true;
						break;
					case ParsingTable::TryReduce:
						writer.WriteString(L"TryReduce => ");
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

					TEST_ASSERT(builder.Run(result));
				}
				else
				{
					break;
				}
			}

			writer.WriteLine(L"=============================================================");
			writer.WriteLine(L"Tree");
			writer.WriteLine(L"=============================================================");
			node=builder.GetNode();
			if(node)
			{
				Log(node, writer);
			}
		}
		TEST_ASSERT(meetTokenFinish);
		TEST_ASSERT(node);
		return node;
	}
}
using namespace test;

namespace test
{
	Ptr<ParsingDefinition> CreateNameListDefinition()
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

		return definitionWriter.Definition();
	}
}
using namespace test;

TEST_CASE(TestParseNameList)
{
	Ptr<ParsingDefinition> definition=CreateNameListDefinition();
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

namespace test
{
	Ptr<ParsingDefinition> CreateExpressionDefinition()
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

		return definitionWriter.Definition();
	}
}
using namespace test;

TEST_CASE(TestParsingExpression)
{
	Ptr<ParsingDefinition> definition=CreateExpressionDefinition();
	Ptr<ParsingTable> table=CreateTable(definition, L"Calculator");
	const wchar_t* inputs[]=
	{
		L"0",
		L"1+2",
		L"1*2+3*4",
		L"(1+2)*(3+4)",
		L"exp()-pow(cos(1.57*2), 4)",
		L"1+1+1+1+1+1",
	};
	for(vint i=0;i<sizeof(inputs)/sizeof(*inputs);i++)
	{
		Parse(table, inputs[i], L"Calculator", L"Exp", i);
	}
}

namespace test
{
	Ptr<ParsingDefinition> CreateStatementDefinition()
	{
		ParsingDefinitionWriter definitionWriter;

		definitionWriter
			.Type(
				Class(L"ValueExpression")
					.Member(L"content", TokenType())
				)
			.Type(
				Class(L"ConditionExpression")
					.SubType(
						Enum(L"ConditionOperator")
							.Member(L"LT")
							.Member(L"LE")
							.Member(L"GT")
							.Member(L"GE")
							.Member(L"EQ")
							.Member(L"NE")
						)
					.Member(L"leftOperand", Type(L"ValueExpression"))
					.Member(L"rightOperand", Type(L"ValueExpression"))
					.Member(L"conditionOperator", Type(L"ConditionOperator"))
				)
			.Type(
				Class(L"Statement")
				)
			.Type(
				Class(L"IfElseStatement", Type(L"Statement"))
					.Member(L"condition", Type(L"ConditionExpression"))
					.Member(L"trueStatement", Type(L"Statement"))
					.Member(L"falseStatement", Type(L"Statement"))
				)
			.Type(
				Class(L"AssignStatement", Type(L"Statement"))
					.Member(L"leftOperand", Type(L"ValueExpression"))
					.Member(L"rightOperand", Type(L"ValueExpression"))
				)
			.Type(
				Class(L"ReturnStatement", Type(L"Statement"))
					.Member(L"result", Type(L"ValueExpression"))
				)
			.Type(
				Class(L"BlockStatement", Type(L"Statement"))
					.Member(L"statements", Type(L"Statement").Array())
				)

			.Token(L"IF",		L"if")
			.Token(L"THEN",		L"then")
			.Token(L"ELSE",		L"else")
			.Token(L"RETURN",	L"return")
			.Token(L"OPEN",		L"/{")
			.Token(L"CLOSE",	L"/}")
			.Token(L"LT",		L"/<")
			.Token(L"LE",		L"/</=")
			.Token(L"GT",		L"/>")
			.Token(L"GE",		L"/>/=")
			.Token(L"EQ",		L"/=/=")
			.Token(L"NE",		L"/!/=")
			.Token(L"ASSIGN",	L"/=")
			.Token(L"VALUE",	L"[a-zA-Z_]/w*")
			.Discard(L"SPACE",	L"/s+")

			.Rule(L"Value", Type(L"ValueExpression"))
				.Imply(
					(Rule(L"VALUE")[L"content"]).As(Type(L"ValueExpression"))
					)
				.EndRule()

			.Rule(L"Condition", Type(L"ConditionExpression"))
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L"<") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"LT")
					)
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L"<=") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"LE")
					)
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L">") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"GT")
					)
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L">=") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"GE")
					)
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L"==") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"EQ")
					)
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L"!=") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"ConditionExpression")).Set(L"conditionOperator", L"NE")
					)
				.EndRule()

			.Rule(L"Assign", Type(L"AssignStatement"))
				.Imply(
					(Rule(L"Value")[L"leftOperand"] + Text(L"=") + Rule(L"Value")[L"rightOperand"])
						.As(Type(L"AssignStatement"))
					)
				.EndRule()

			.Rule(L"Return", Type(L"ReturnStatement"))
				.Imply(
					(Text(L"return") + Rule(L"Value")[L"result"])
						.As(Type(L"ReturnStatement"))
					)
				.EndRule()

			.Rule(L"Block", Type(L"BlockStatement"))
				.Imply(
					(Text(L"{") + *Rule(L"Stat")[L"statements"] + Text(L"}"))
						.As(Type(L"BlockStatement"))
					)
				.EndRule()

			.Rule(L"IfElse", Type(L"IfElseStatement"))
				.Imply(
					(
						Text(L"if") + Rule(L"Condition")[L"condition"] +
						Text(L"then") + Rule(L"Stat")[L"trueStatement"] +
						Opt(Text(L"else") + Rule(L"Stat")[L"falseStatement"])
						)
						.As(Type(L"IfElseStatement"))
					)
				.EndRule()

			.Rule(L"Stat", Type(L"Statement"))
				.Imply(!Rule(L"Assign") | !Rule(L"Return") | !Rule(L"Block") | !Rule(L"IfElse"))
				.EndRule()
			;

		return definitionWriter.Definition();
	}
}
using namespace test;

TEST_CASE(TestParsingStatement)
{
	Ptr<ParsingDefinition> definition=CreateStatementDefinition();
	Ptr<ParsingTable> table=CreateTable(definition, L"Statement");
	const wchar_t* inputs[]=
	{
		L"a = b",
		L"return value",
		L"{}",
		L"{ a = b c = d return e }",
		L"if a < b then return a else return b",
		L"{ if a < b then return a else return b }",
		L"if a < b then if b < c then return c else return b",
		L"if a < b then if b < c then return c else return b else return a",
		L"if a > b then { a = b if c <= d then e = f g = h } else { if a != b then return a else return b }",
		L"if a < b then if a > b then if a <= b then if a >= b then if a == b then if a != b then return null",
	};
	for(vint i=0;i<sizeof(inputs)/sizeof(*inputs);i++)
	{
		Parse(table, inputs[i], L"Statement", L"Stat", i);
	}
}

namespace test
{
	WString ParsingDefinitionToText(Ptr<ParsingDefinition> definition)
	{
		MemoryStream stream;
		{
			StreamWriter writer(stream);
			Log(definition, writer);
		}
		stream.SeekFromBegin(0);
		StreamReader reader(stream);
		return reader.ReadToEnd();
	}
}
using namespace test;

TEST_CASE(TestParsingGrammar)
{
	Ptr<ParsingDefinition> definition;
	Ptr<ParsingDefinition> inputDefs[]=
	{
		CreateNameListDefinition(),
		CreateExpressionDefinition(),
		CreateStatementDefinition(),
		definition=CreateParserDefinition(),
	};
	const wchar_t* inputTexts[][2]=
	{
		{L"Type",			L"token"},
		{L"Type",			L"Item"},
		{L"Type",			L"Item[]"},
		{L"Type",			L"Item.SubItem"},
		{L"Type",			L"Item[][][]"},
		{L"Type",			L"Item.SubItem[]"},
		{L"Type",			L"Item[].SubItem[].AnotherSubItem[]"},

		{L"TypeDecl",		L"enum Boolean{ True, False, }"},
		{L"TypeDecl",		L"class Vector{ int x; int y; }"},
		{L"TypeDecl",		L"class ComplexVector : SimpleVector{"
								L"enum Ordering{ Ordered, Unordered, }"
								L"class Item{ string name; Ordering ordering; }"
								L"Item x; Item y;"
							L"}"},

		{L"Grammar",		L"Factor"},
		{L"Grammar",		L"\"*\""},
		{L"Grammar",		L"Factor : operand"},
		{L"Grammar",		L"Term \"*\" Factor"},
		{L"Grammar",		L"!Term \"*\" Factor:second"},
		{L"Grammar",		L"A B | C D"},
		{L"Grammar",		L"A {B}"},
		{L"Grammar",		L"A [B]"},
		{L"Grammar",		L"[Exp:argument {\",\" Exp:argument}]"},
		{L"Grammar",		L"Term:first \"*\" Factor:second as BinaryExpression with {Operator = \"Mul\"}"},
	};
	Ptr<ParsingTable> table=CreateTable(definition, L"Syngram");
	for(vint i=0;i<sizeof(inputTexts)/sizeof(*inputTexts);i++)
	{
		Parse(table, inputTexts[i][1], L"Syngram", inputTexts[i][0], i);
	}
	for(vint i=0;i<sizeof(inputDefs)/sizeof(*inputDefs);i++)
	{
		WString grammar=ParsingDefinitionToText(inputDefs[i]);
		Parse(table, grammar, L"Syngram_Bootstrap", L"ParserDecl", i);
	}
}

TEST_CASE(TestParsingBootstrap)
{
	Ptr<ParsingRestrictParser> parser=CreateBootstrapParser();
	TEST_ASSERT(parser);

	Ptr<ParsingDefinition> inputDefs[]=
	{
		CreateNameListDefinition(),
		CreateExpressionDefinition(),
		CreateStatementDefinition(),
		CreateParserDefinition(),
	};

	ParsingError firstError;
	for(vint i=0;i<sizeof(inputDefs)/sizeof(*inputDefs);i++)
	{
		WString grammar=ParsingDefinitionToText(inputDefs[i]);
		Ptr<ParsingTreeNode> node=parser->Parse(grammar, L"ParserDecl", firstError);
		TEST_ASSERT(node);
		Ptr<ParsingDefinition> def=DeserializeDefinition(node);
		TEST_ASSERT(def);
		WString grammar2=ParsingDefinitionToText(def);
		TEST_ASSERT(grammar==grammar2);
	}
}

TEST_CASE(TestParsingTreeCharacterPosition)
{
	Ptr<ParsingRestrictParser> parser;
	{
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingDefinition> definition=CreateExpressionDefinition();
		Ptr<ParsingTable> table=GenerateTable(definition, errors);
		TEST_ASSERT(table);
		parser=new ParsingRestrictParser(table);
	}

	WString input=L"11+22*\r\n33+44";
	ParsingError firstError;
	Ptr<ParsingTreeNode> node=parser->Parse(input, L"Exp", firstError);
	TEST_ASSERT(node);
	node->InitializeQueryCache();
}