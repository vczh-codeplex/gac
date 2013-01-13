#include <string.h>
#include "..\..\Source\Exception.h"
#include "..\..\Source\UnitTest\UnitTest.h"
#include "..\..\Source\Stream\FileStream.h"
#include "..\..\Source\Stream\MemoryStream.h"
#include "..\..\Source\Stream\Accessor.h"
#include "..\..\Source\Stream\CharFormat.h"
#include "..\..\Source\Collections\Operation.h"
#include "..\..\Source\Parsing\ParsingAutomaton.h"
#include "..\..\Source\Parsing\Parsing.h"
#include "..\..\Source\Parsing\Xml\ParsingXml.h"
#include "..\..\Source\Parsing\Json\ParsingJson.h"
#include "Parser.Calculator\Parser.Calculator.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::analyzing;
using namespace vl::parsing::tabling;
using namespace vl::parsing::xml;
using namespace vl::parsing::json;
using namespace test::parser;

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

	Ptr<ParsingTreeNode> Parse(Ptr<ParsingTable> table, const WString& input, const WString& name, const WString& rule, vint index, bool showInput)
	{
		if(showInput)
		{
			unittest::UnitTest::PrintInfo(L"Parsing: "+input);
		}
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

					vint shiftReduceRangeIndex=0;
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
							if(result.shiftReduceRanges && showInput)
							{
								ParsingState::ShiftReduceRange range=result.shiftReduceRanges->Get(shiftReduceRangeIndex++);
								vint start=range.shiftToken->start;
								vint end=range.reduceToken->start+range.reduceToken->length;
								writer.WriteString(L"        ¡¾");
								writer.WriteString(input.Sub(start, end-start));
								writer.WriteLine(L"¡¿");
							}
							break;
						case ParsingTable::Instruction::LeftRecursiveReduce:
							writer.WriteLine(L"    LR-Reduce "+itow(ins.stateParameter)+L"["+table->GetStateInfo(ins.stateParameter).ruleName+L"]");
							break;
						}
					}

					if(result.tableTokenIndex==ParsingTable::TokenFinish)
					{
						if(result.shiftReduceRanges && showInput)
						{
							ParsingState::ShiftReduceRange range=result.shiftReduceRanges->Get(shiftReduceRangeIndex++);
							vint start=range.shiftToken->start;
							vint end=range.reduceToken->start+range.reduceToken->length;
							writer.WriteString(L"        ¡¾");
							writer.WriteString(input.Sub(start, end-start));
							writer.WriteLine(L"¡¿");
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
				WString originalInput;
				if(showInput)
				{
					originalInput=input;
				}
				Log(node, originalInput, writer);
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
					.Member(L"name", TokenType(), L"UnescapingName")
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
		Parse(table, inputs[i], L"NameList", L"NameList", i, true);
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
		Parse(table, inputs[i], L"Calculator", L"Exp", i, true);
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
		Parse(table, inputs[i], L"Statement", L"Stat", i, true);
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
		Parse(table, inputTexts[i][1], L"Syngram", inputTexts[i][0], i, true);
	}
	for(vint i=0;i<sizeof(inputDefs)/sizeof(*inputDefs);i++)
	{
		WString grammar=ParsingDefinitionToText(inputDefs[i]);
		Parse(table, grammar, L"Syngram_Bootstrap", L"ParserDecl", i, false);
	}
}

TEST_CASE(TestParsingBootstrap)
{
	Ptr<ParsingStrictParser> parser=CreateBootstrapStrictParser();
	TEST_ASSERT(parser);

	Ptr<ParsingDefinition> inputDefs[]=
	{
		CreateNameListDefinition(),
		CreateExpressionDefinition(),
		CreateStatementDefinition(),
		CreateParserDefinition(),
	};

	List<Ptr<ParsingError>> errors;
	for(vint i=0;i<sizeof(inputDefs)/sizeof(*inputDefs);i++)
	{
		WString grammar=ParsingDefinitionToText(inputDefs[i]);
		Ptr<ParsingTreeNode> node=parser->Parse(grammar, L"ParserDecl", errors);
		TEST_ASSERT(node);
		Ptr<ParsingDefinition> def=DeserializeDefinition(node);
		TEST_ASSERT(def);
		WString grammar2=ParsingDefinitionToText(def);
		TEST_ASSERT(grammar==grammar2);
	}
}

TEST_CASE(TestParsingTreeCharacterPosition)
{
	Ptr<ParsingStrictParser> parser;
	{
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingDefinition> definition=CreateExpressionDefinition();
		Ptr<ParsingTable> table=GenerateTable(definition, errors);
		TEST_ASSERT(table);
		parser=new ParsingStrictParser(table);
	}

	WString input=L"11+22*\r\n33+44";
	List<Ptr<ParsingError>> errors;
	Ptr<ParsingTreeNode> node=parser->Parse(input, L"Exp", errors);
	TEST_ASSERT(node);
	node->InitializeQueryCache();

	{
		ParsingTextPos pos(3);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeToken* token=dynamic_cast<ParsingTreeToken*>(foundNode);
		TEST_ASSERT(token);
		TEST_ASSERT(token->GetValue()==L"22");
	}
	{
		ParsingTextPos pos(4);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeToken* token=dynamic_cast<ParsingTreeToken*>(foundNode);
		TEST_ASSERT(token);
		TEST_ASSERT(token->GetValue()==L"22");
	}
	{
		ParsingTextPos pos(5);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(foundNode);
		TEST_ASSERT(obj);
		TEST_ASSERT(obj->GetMember(L"binaryOperator").Cast<ParsingTreeToken>()->GetValue()==L"Mul");
	}
	{
		ParsingTextPos pos(1, 1);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeToken* token=dynamic_cast<ParsingTreeToken*>(foundNode);
		TEST_ASSERT(token);
		TEST_ASSERT(token->GetValue()==L"33");
	}
	{
		ParsingTextPos pos(1, 2);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(foundNode);
		TEST_ASSERT(obj);
		TEST_ASSERT(obj->GetMember(L"binaryOperator").Cast<ParsingTreeToken>()->GetValue()==L"Add");
	}
	{
		ParsingTextPos pos(1, 3);
		ParsingTreeNode* foundNode=node->FindDeepestNode(pos);
		TEST_ASSERT(foundNode);
		ParsingTreeToken* token=dynamic_cast<ParsingTreeToken*>(foundNode);
		TEST_ASSERT(token);
		TEST_ASSERT(token->GetValue()==L"44");
	}
}

namespace test
{
	void ParseWithAutoRecover(Ptr<ParsingDefinition> definition, const WString& name, const WString& rule, List<WString>& inputs)
	{
		List<Ptr<ParsingError>> errors;
		Ptr<ParsingTable> table=GenerateTable(definition, errors);
		TEST_ASSERT(table);
		Ptr<ParsingAutoRecoverParser> parser=new ParsingAutoRecoverParser(table);

		FileStream fileStream(GetPath()+L"Parsing.AutoRecover."+name+L".txt", FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		FOREACH(WString, input, inputs)
		{
			unittest::UnitTest::PrintInfo(L"Parsing: "+input);
			writer.WriteLine(L"=============================================================");
			writer.WriteLine(input);
			writer.WriteLine(L"=============================================================");

			errors.Clear();
			Ptr<ParsingTreeNode> node=parser->Parse(input, rule, errors);
			if(node)
			{
				Log(node, input, writer);
			}
			writer.WriteLine(L"");
		}
	}
}
using namespace test;

TEST_CASE(TestAutoRecoverParser)
{
	Ptr<ParsingDefinition> definition=CreateExpressionDefinition();
	List<WString> inputs;
	inputs.Add(L"");
	inputs.Add(L"+");
	inputs.Add(L"1+");
	inputs.Add(L"+1");
	inputs.Add(L"(1");
	inputs.Add(L"1)");
	inputs.Add(L"1 2+3)");
	inputs.Add(L"(1 2+3");
	inputs.Add(L"()");
	ParseWithAutoRecover(definition, L"Calculator", L"Exp", inputs);
}

namespace test
{
	class CalExpressionEvaluationVisitor : public Object, public CalExpression::IVisitor
	{
	public:
		vint result;

		void Visit(CalNumberExpression* node)override
		{
			result=wtoi(node->value.value);
		}

		void Visit(CalBinaryExpression* node)override
		{
			node->firstOperand->Accept(this);
			vint a=result;
			node->secondOperand->Accept(this);
			vint b=result;
			switch(node->binaryOperator)
			{
			case CalBinaryExpression::CalBinaryOperator::Add:
				result=a+b;
				break;
			case CalBinaryExpression::CalBinaryOperator::Sub:
				result=a-b;
				break;
			case CalBinaryExpression::CalBinaryOperator::Mul:
				result=a*b;
				break;
			case CalBinaryExpression::CalBinaryOperator::Div:
				result=a/b;
				break;
			default:
			throw Exception(L"Unknown operator.");
			}
		}

		void Visit(CalFunctionExpression* node)override
		{
			throw Exception(L"Function calling is not supported.");
		}
	};
}
using namespace test;

TEST_CASE(TestGeneratedParser_Calculator)
{
	Ptr<ParsingTable> table=CalLoadTable();
	TEST_ASSERT(table);
	Ptr<CalExpression> exp=CalParseExpression(L"(1+2)*(3+4)", table);
	TEST_ASSERT(exp);

	CalExpressionEvaluationVisitor visitor;
	exp->Accept(&visitor);
	TEST_ASSERT(visitor.result==21);
}

namespace test
{
	template<typename T>
	void TestGeneratedParser(
		const wchar_t* input[],
		const wchar_t* output[],
		vint count,
		Ptr<ParsingTable> table,
		const WString& name,
		const WString& rule,
		Ptr<T>(*deserializer)(const WString&, Ptr<ParsingTable>),
		WString(*serializer)(Ptr<T>)
		)
	{
		for(vint i=0;i<count;i++)
		{
			Parse(table, input[i], name, rule, i, true);
			Ptr<T> node=deserializer(input[i], table);
			WString text=serializer(node);
			TEST_ASSERT(text==output[i]);
		}
	}

	WString XmlDocumentToString(Ptr<XmlDocument> doc)
	{
		return XmlToString(doc);
	}
}
using namespace test;

TEST_CASE(TestGeneratedParser_Json)
{
	const wchar_t* input[]=
	{
		L"{ }",
		L"[ ]",
		L"[ 1 ]",
		L"[ 1 , 2 ]",
		L"[ true, false, null, 1, \"abc\" ]",
		L"[ \"\\b\\f\\n\\r\\t\\\\\\\"abc\\u0041\\u0039\" ]",
		L"{ \"name\" : \"vczh\", \"scores\" : [100, 90, 80, {\"a\":\"b\"}], \"IDE\":{\"VC++\":\"Microsoft\"} }",
	};
	const wchar_t* output[]=
	{
		L"{}",
		L"[]",
		L"[1]",
		L"[1,2]",
		L"[true,false,null,1,\"abc\"]",
		L"[\"\\b\\f\\n\\r\\t\\\\\\\"abcA9\"]",
		L"{\"name\":\"vczh\",\"scores\":[100,90,80,{\"a\":\"b\"}],\"IDE\":{\"VC++\":\"Microsoft\"}}",
	};
	Ptr<ParsingTable> table=JsonLoadTable();
	TestGeneratedParser(input, output, sizeof(input)/sizeof(*input), table, L"Json", L"JRoot", &JsonParse, &JsonToString);
}

TEST_CASE(TestGeneratedParser_Xml)
{
	const wchar_t* input[]=
	{
		L"<name />",
	};
	const wchar_t* output[]=
	{
		L"<name/>",
	};
	Ptr<ParsingTable> table=XmlLoadTable();
	TestGeneratedParser(input, output, sizeof(input)/sizeof(*input), table, L"Xml", L"XDocument", &XmlParseDocument, &XmlDocumentToString);
}