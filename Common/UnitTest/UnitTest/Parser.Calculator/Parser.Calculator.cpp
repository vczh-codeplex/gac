#include "Parser.Calculator.h"

namespace test
{
	namespace parser
	{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n"L""
L"\r\n"L"class Expression"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class NumberExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"    token value;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BinaryExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"    enum BinaryOperator"
L"\r\n"L"    {"
L"\r\n"L"        Add,"
L"\r\n"L"        Sub,"
L"\r\n"L"        Mul,"
L"\r\n"L"        Div,"
L"\r\n"L"    }"
L"\r\n"L""
L"\r\n"L"    Expression firstOperand;"
L"\r\n"L"    Expression secondOperand;"
L"\r\n"L"    BinaryOperator binaryOperator;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FunctionExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"    token functionName;"
L"\r\n"L"    Expression[] arguments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z_]/w*\";"
L"\r\n"L"token NUMBER = \"/d+(./d+)?\";"
L"\r\n"L"token ADD = \"/+\";"
L"\r\n"L"token SUB = \"-\";"
L"\r\n"L"token MUL = \"/*\";"
L"\r\n"L"token DIV = \"//\";"
L"\r\n"L"token LEFT = \"/(\";"
L"\r\n"L"token RIGHT = \"/)\";"
L"\r\n"L"token COMMA = \",\";"
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule NumberExpression Number"
L"\r\n"L"        = NUMBER : value as NumberExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule FunctionExpression Call"
L"\r\n"L"        = NAME : functionName \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule Expression Factor"
L"\r\n"L"        = !Number | !Call"
L"\r\n"L"        = \"(\" !Exp \")\""
L"\r\n"L"        ;"
L"\r\n"L"rule Expression Term"
L"\r\n"L"        = !Factor"
L"\r\n"L"        = Term : firstOperand \"*\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }"
L"\r\n"L"        = Term : firstOperand \"/\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }"
L"\r\n"L"        ;"
L"\r\n"L"rule Expression Exp"
L"\r\n"L"        = !Term"
L"\r\n"L"        = Exp : firstOperand \"+\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }"
L"\r\n"L"        = Exp : firstOperand \"-\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }"
L"\r\n"L"        ;"
L"\r\n"L""
;

		vl::WString CalGetParserTextBuffer()
		{
			return parserTextBuffer;
		}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

		class CalTreeConverter : public vl::parsing::ParsingTreeConverter
		{
		public:
			using vl::parsing::ParsingTreeConverter::SetMember;

			bool SetMember(CalBinaryExpression::CalBinaryOperator::Type& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Add") { member=CalBinaryExpression::CalBinaryOperator::Add; return true; }
					else if(token->GetValue()==L"Sub") { member=CalBinaryExpression::CalBinaryOperator::Sub; return true; }
					else if(token->GetValue()==L"Mul") { member=CalBinaryExpression::CalBinaryOperator::Mul; return true; }
					else if(token->GetValue()==L"Div") { member=CalBinaryExpression::CalBinaryOperator::Div; return true; }
					else { member=CalBinaryExpression::CalBinaryOperator::Add; return false; }
				}
				member=CalBinaryExpression::CalBinaryOperator::Add;
				return false;
			}

			void Fill(vl::Ptr<CalExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<CalNumberExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<CalBinaryExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->firstOperand, obj->GetMember(L"firstOperand"), tokens);
				SetMember(tree->secondOperand, obj->GetMember(L"secondOperand"), tokens);
				SetMember(tree->binaryOperator, obj->GetMember(L"binaryOperator"), tokens);
			}

			void Fill(vl::Ptr<CalFunctionExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->functionName, obj->GetMember(L"functionName"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"NumberExpression")
				{
					vl::Ptr<CalNumberExpression> tree = new CalNumberExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<CalExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BinaryExpression")
				{
					vl::Ptr<CalBinaryExpression> tree = new CalBinaryExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<CalExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionExpression")
				{
					vl::Ptr<CalFunctionExpression> tree = new CalFunctionExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<CalExpression>(), obj, tokens);
					return tree;
				}
				else 
					return 0;
			}
		};

		vl::Ptr<vl::parsing::ParsingTreeCustomBase> CalConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			CalTreeConverter converter;
			vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
			converter.SetMember(tree, node, tokens);
			return tree;
		}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

		vl::Ptr<CalNumberExpression> CalNumberExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return CalConvertParsingTreeNode(node, tokens).Cast<CalNumberExpression>();
		}

		vl::Ptr<CalBinaryExpression> CalBinaryExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return CalConvertParsingTreeNode(node, tokens).Cast<CalBinaryExpression>();
		}

		vl::Ptr<CalFunctionExpression> CalFunctionExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return CalConvertParsingTreeNode(node, tokens).Cast<CalFunctionExpression>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void CalNumberExpression::Accept(CalExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void CalBinaryExpression::Accept(CalExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void CalFunctionExpression::Accept(CalExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> CalParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"Exp");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"Exp");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node)
			{
				return CalConvertParsingTreeNode(node, state.GetTokens()).Cast<CalExpression>();
			}
			return 0;
		}

/***********************************************************************
Table Generation
***********************************************************************/

		vl::Ptr<vl::parsing::tabling::ParsingTable> CalLoadTable()
		{
			vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(12-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 34, 5);
			#define SET_TOKEN_INFO(INDEX, NAME, REGEX) table->SetTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
			#define SET_DISCARD_TOKEN_INFO(INDEX, NAME, REGEX) table->SetDiscardTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));
			#define SET_STATE_INFO(INDEX, RULE, STATE, EXPR) table->SetStateInfo(INDEX, vl::parsing::tabling::ParsingTable::StateInfo(RULE, STATE, EXPR));
			#define SET_RULE_INFO(INDEX, NAME, TYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, L"", STARTSTATE));
			#define SET_AMBIGUOUS_RULE_INFO(INDEX, NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE));
			#define BEGIN_TRANSITION_BAG(STATE, TOKEN) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag; table->SetTransitionBag(STATE, TOKEN, bag);
			#define BEGIN_TRANSITION_ITEM(TOKEN, TARGETSTATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(TOKEN, TARGETSTATE); bag->transitionItems.Add(item);
			#define END_TRANSITION_ITEM }
			#define END_TRANSITION_BAG }
			#define ITEM_STACK_PATTERN(STATE) item->stackPattern.Add(STATE);
			#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE, RULE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE, RULE));
			#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
			#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);
			#define END_LOOK_AHEAD }

			SET_TOKEN_INFO(0, L"", L"")
			SET_TOKEN_INFO(1, L"", L"")
			SET_TOKEN_INFO(2, L"", L"")
			SET_TOKEN_INFO(3, L"NAME", L"[a-zA-Z_]/w*")
			SET_TOKEN_INFO(4, L"NUMBER", L"/d+(./d+)?")
			SET_TOKEN_INFO(5, L"ADD", L"/+")
			SET_TOKEN_INFO(6, L"SUB", L"-")
			SET_TOKEN_INFO(7, L"MUL", L"/*")
			SET_TOKEN_INFO(8, L"DIV", L"//")
			SET_TOKEN_INFO(9, L"LEFT", L"/(")
			SET_TOKEN_INFO(10, L"RIGHT", L"/)")
			SET_TOKEN_INFO(11, L"COMMA", L",")

			SET_DISCARD_TOKEN_INFO(0, L"SPACE", L"/s+")

			SET_STATE_INFO(0, L"Number", L"Number.RootStart", L"¡ñ $<Number>")
			SET_STATE_INFO(1, L"Number", L"Number.Start", L"¡¤ <Number>")
			SET_STATE_INFO(2, L"Number", L"Number.1", L"<Number>: NUMBER : value as NumberExpression¡ñ")
			SET_STATE_INFO(3, L"Number", L"Number.RootEnd", L"$<Number> ¡ñ")
			SET_STATE_INFO(4, L"Factor", L"Factor.RootEnd", L"$<Factor> ¡ñ")
			SET_STATE_INFO(5, L"Term", L"Term.2", L"<Term>: Term : firstOperand \"/\"¡ñ Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }\r\n<Term>: Term : firstOperand \"*\"¡ñ Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }")
			SET_STATE_INFO(6, L"Term", L"Term.RootEnd", L"$<Term> ¡ñ")
			SET_STATE_INFO(7, L"Call", L"Call.4", L"<Call>: NAME : functionName \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression¡ñ")
			SET_STATE_INFO(8, L"Call", L"Call.5", L"<Call>: NAME : functionName \"(\" [ Exp : arguments { \",\"¡ñ Exp : arguments } ] \")\" as FunctionExpression")
			SET_STATE_INFO(9, L"Factor", L"Factor.1", L"<Factor>: !Number | !Call¡ñ\r\n<Factor>: \"(\" !Exp \")\"¡ñ")
			SET_STATE_INFO(10, L"Exp", L"Exp.3", L"<Exp>: Exp : firstOperand \"+\"¡ñ Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }\r\n<Exp>: Exp : firstOperand \"-\"¡ñ Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }")
			SET_STATE_INFO(11, L"Exp", L"Exp.RootEnd", L"$<Exp> ¡ñ")
			SET_STATE_INFO(12, L"Call", L"Call.1", L"<Call>: NAME : functionName¡ñ \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression")
			SET_STATE_INFO(13, L"Factor", L"Factor.2", L"<Factor>: \"(\"¡ñ !Exp \")\"")
			SET_STATE_INFO(14, L"Call", L"Call.RootEnd", L"$<Call> ¡ñ")
			SET_STATE_INFO(15, L"Call", L"Call.2", L"<Call>: NAME : functionName \"(\"¡ñ [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression")
			SET_STATE_INFO(16, L"Call", L"Call.RootStart", L"¡ñ $<Call>")
			SET_STATE_INFO(17, L"Call", L"Call.Start", L"¡¤ <Call>")
			SET_STATE_INFO(18, L"Factor", L"Factor.RootStart", L"¡ñ $<Factor>")
			SET_STATE_INFO(19, L"Factor", L"Factor.Start", L"¡¤ <Factor>")
			SET_STATE_INFO(20, L"Term", L"Term.RootStart", L"¡ñ $<Term>")
			SET_STATE_INFO(21, L"Term", L"Term.Start", L"¡¤ <Term>")
			SET_STATE_INFO(22, L"Exp", L"Exp.RootStart", L"¡ñ $<Exp>")
			SET_STATE_INFO(23, L"Exp", L"Exp.Start", L"¡¤ <Exp>")
			SET_STATE_INFO(24, L"Number", L"Number.2", L"<Number>: NUMBER : value as NumberExpression¡ñ")
			SET_STATE_INFO(25, L"Call", L"Call.3", L"<Call>: NAME : functionName \"(\" [ Exp : arguments¡ñ { \",\" Exp : arguments } ] \")\" as FunctionExpression\r\n<Call>: NAME : functionName \"(\" [ Exp : arguments ¡ñ{ \",\" Exp : arguments } ] \")\" as FunctionExpression")
			SET_STATE_INFO(26, L"Call", L"Call.6", L"<Call>: NAME : functionName \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression¡ñ")
			SET_STATE_INFO(27, L"Factor", L"Factor.3", L"<Factor>: \"(\" !Exp¡ñ \")\"")
			SET_STATE_INFO(28, L"Term", L"Term.1", L"<Term>: Term : firstOperand¡ñ \"/\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }\r\n<Term>: Term : firstOperand¡ñ \"*\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }")
			SET_STATE_INFO(29, L"Term", L"Term.3", L"<Term>: Term : firstOperand \"/\" Factor : secondOperand as BinaryExpression¡ñ with { binaryOperator = \"Div\" }\r\n<Term>: Term : firstOperand \"*\" Factor : secondOperand as BinaryExpression¡ñ with { binaryOperator = \"Mul\" }")
			SET_STATE_INFO(30, L"Term", L"Term.4", L"<Term>: Term : firstOperand \"/\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }¡ñ\r\n<Term>: !Factor¡ñ\r\n<Term>: Term : firstOperand \"*\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }¡ñ")
			SET_STATE_INFO(31, L"Exp", L"Exp.1", L"<Exp>: !Term¡ñ\r\n<Exp>: Exp : firstOperand \"+\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }¡ñ\r\n<Exp>: Exp : firstOperand \"-\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }¡ñ")
			SET_STATE_INFO(32, L"Exp", L"Exp.2", L"<Exp>: Exp : firstOperand¡ñ \"+\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }\r\n<Exp>: Exp : firstOperand¡ñ \"-\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }")
			SET_STATE_INFO(33, L"Exp", L"Exp.4", L"<Exp>: Exp : firstOperand \"+\" Term : secondOperand as BinaryExpression¡ñ with { binaryOperator = \"Add\" }\r\n<Exp>: Exp : firstOperand \"-\" Term : secondOperand as BinaryExpression¡ñ with { binaryOperator = \"Sub\" }")

			SET_RULE_INFO(0, L"Number", L"NumberExpression", 0)
			SET_RULE_INFO(1, L"Call", L"FunctionExpression", 16)
			SET_RULE_INFO(2, L"Factor", L"Expression", 18)
			SET_RULE_INFO(3, L"Term", L"Expression", 20)
			SET_RULE_INFO(4, L"Exp", L"Expression", 22)

			BEGIN_TRANSITION_BAG(0, 0)

				BEGIN_TRANSITION_ITEM(0, 1)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(1, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 1)

				BEGIN_TRANSITION_ITEM(1, 3)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 4)
				ITEM_STACK_PATTERN(19)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 5)

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 6)

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 7)

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 8)

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 10)

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 11)

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"NumberExpression", L"", L"Number");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 5, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 1)

				BEGIN_TRANSITION_ITEM(1, 14)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 4)
				ITEM_STACK_PATTERN(19)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 5)

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 6)

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 7)

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 8)

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 10)

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 11)

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(19)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"FunctionExpression", L"", L"Call");
				ITEM_INSTRUCTION(Reduce, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 1)

				BEGIN_TRANSITION_ITEM(1, 4)
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 11)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 6)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 5)

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 10)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Add", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 6)

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(6, 10)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Exp");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Sub", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 7)

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(7, 5)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Mul", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 8)

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(21)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(8, 5)
				ITEM_STACK_PATTERN(5)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(LeftRecursiveReduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"firstOperand", L"", L"");
				ITEM_INSTRUCTION(Create, 0, L"BinaryExpression", L"", L"Term");
				ITEM_INSTRUCTION(Setter, 0, L"binaryOperator", L"Div", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 10)

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 9)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Factor");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 7)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(9, 11)

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(21)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Term");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(23)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"", L"Exp");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(15)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 8)
				ITEM_STACK_PATTERN(5)
				ITEM_STACK_PATTERN(10)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 5, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"secondOperand", L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 10, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 9)

				BEGIN_TRANSITION_ITEM(9, 15)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 13, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 15, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 10)

				BEGIN_TRANSITION_ITEM(10, 7)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 0)

				BEGIN_TRANSITION_ITEM(0, 17)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(17, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(18, 0)

				BEGIN_TRANSITION_ITEM(0, 19)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(19, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(19, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(19, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 0)

				BEGIN_TRANSITION_ITEM(0, 21)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(21, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(21, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(21, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(22, 0)

				BEGIN_TRANSITION_ITEM(0, 23)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 3)

				BEGIN_TRANSITION_ITEM(3, 12)
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"functionName", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 4)

				BEGIN_TRANSITION_ITEM(4, 2)
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 19, L"", L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"value", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 9)

				BEGIN_TRANSITION_ITEM(9, 13)
				ITEM_INSTRUCTION(Shift, 23, L"", L"", L"");
				ITEM_INSTRUCTION(Shift, 21, L"", L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			table->Initialize();
			return table;

			#undef SET_TOKEN_INFO
			#undef SET_DISCARD_TOKEN_INFO
			#undef SET_STATE_INFO
			#undef SET_RULE_INFO
			#undef SET_AMBIGUOUS_RULE_INFO
			#undef BEGIN_TRANSITION_BAG
			#undef BEGIN_TRANSITION_ITEM
			#undef END_TRANSITION_ITEM
			#undef END_TRANSITION_BAG
			#undef ITEM_STACK_PATTERN
			#undef ITEM_INSTRUCTION
			#undef BEGIN_LOOK_AHEAD
			#undef LOOK_AHEAD
			#undef END_LOOK_AHEAD
		}

	}
}
