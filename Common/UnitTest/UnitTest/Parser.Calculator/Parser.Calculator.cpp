#include "Parser.Calculator.h"

namespace test
{
	namespace parser
	{
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
					Fill(tree, obj, tokens);
					Fill(tree.Cast<CalExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BinaryExpression")
				{
					vl::Ptr<CalBinaryExpression> tree = new CalBinaryExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<CalExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FunctionExpression")
				{
					vl::Ptr<CalFunctionExpression> tree = new CalFunctionExpression;
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

		vl::Ptr<CalExpression> CalParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"Exp");
			vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
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
			vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(12-vl::parsing::tabling::ParsingTable::UserTokenStart, 1, 30, 5);
			table->SetTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
			table->SetTokenInfo(1, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
			table->SetTokenInfo(2, vl::parsing::tabling::ParsingTable::TokenInfo(L"", L""));
			table->SetTokenInfo(3, vl::parsing::tabling::ParsingTable::TokenInfo(L"NAME", L"[a-zA-Z_]/w*"));
			table->SetTokenInfo(4, vl::parsing::tabling::ParsingTable::TokenInfo(L"NUMBER", L"/d+(./d+)?"));
			table->SetTokenInfo(5, vl::parsing::tabling::ParsingTable::TokenInfo(L"ADD", L"/+"));
			table->SetTokenInfo(6, vl::parsing::tabling::ParsingTable::TokenInfo(L"SUB", L"-"));
			table->SetTokenInfo(7, vl::parsing::tabling::ParsingTable::TokenInfo(L"MUL", L"/*"));
			table->SetTokenInfo(8, vl::parsing::tabling::ParsingTable::TokenInfo(L"DIV", L"//"));
			table->SetTokenInfo(9, vl::parsing::tabling::ParsingTable::TokenInfo(L"LEFT", L"/("));
			table->SetTokenInfo(10, vl::parsing::tabling::ParsingTable::TokenInfo(L"RIGHT", L"/)"));
			table->SetTokenInfo(11, vl::parsing::tabling::ParsingTable::TokenInfo(L"COMMA", L","));
			table->SetDiscardTokenInfo(0, vl::parsing::tabling::ParsingTable::TokenInfo(L"SPACE", L"/s+"));
			table->SetStateInfo(0, vl::parsing::tabling::ParsingTable::StateInfo(L"Number", L"Number.RootStart", L"¡ñ $<Number>"));
			table->SetStateInfo(1, vl::parsing::tabling::ParsingTable::StateInfo(L"Number", L"Number.Start", L"¡¤ <Number>"));
			table->SetStateInfo(2, vl::parsing::tabling::ParsingTable::StateInfo(L"Number", L"Number.1", L"<Number>: NUMBER : value as NumberExpression¡ñ"));
			table->SetStateInfo(3, vl::parsing::tabling::ParsingTable::StateInfo(L"Number", L"Number.RootEnd", L"$<Number> ¡ñ"));
			table->SetStateInfo(4, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.RootEnd", L"$<Factor> ¡ñ"));
			table->SetStateInfo(5, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.3", L"<Term>: Term : firstOperand \"*\"¡ñ Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }\r\n<Term>: Term : firstOperand \"/\"¡ñ Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }"));
			table->SetStateInfo(6, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.RootEnd", L"$<Term> ¡ñ"));
			table->SetStateInfo(7, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.4", L"<Call>: NAME : functionName \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression¡ñ"));
			table->SetStateInfo(8, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.5", L"<Call>: NAME : functionName \"(\" [ Exp : arguments { \",\"¡ñ Exp : arguments } ] \")\" as FunctionExpression"));
			table->SetStateInfo(9, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.1", L"<Factor>: !Number | !Call¡ñ\r\n<Factor>: \"(\" !Exp \")\"¡ñ"));
			table->SetStateInfo(10, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.3", L"<Exp>: Exp : firstOperand \"+\"¡ñ Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }\r\n<Exp>: Exp : firstOperand \"-\"¡ñ Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }"));
			table->SetStateInfo(11, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.RootEnd", L"$<Exp> ¡ñ"));
			table->SetStateInfo(12, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.1", L"<Call>: NAME : functionName¡ñ \"(\" [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression"));
			table->SetStateInfo(13, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.2", L"<Factor>: \"(\"¡ñ !Exp \")\""));
			table->SetStateInfo(14, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.RootEnd", L"$<Call> ¡ñ"));
			table->SetStateInfo(15, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.2", L"<Call>: NAME : functionName \"(\"¡ñ [ Exp : arguments { \",\" Exp : arguments } ] \")\" as FunctionExpression"));
			table->SetStateInfo(16, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.RootStart", L"¡ñ $<Call>"));
			table->SetStateInfo(17, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.Start", L"¡¤ <Call>"));
			table->SetStateInfo(18, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.RootStart", L"¡ñ $<Factor>"));
			table->SetStateInfo(19, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.Start", L"¡¤ <Factor>"));
			table->SetStateInfo(20, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.RootStart", L"¡ñ $<Term>"));
			table->SetStateInfo(21, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.Start", L"¡¤ <Term>"));
			table->SetStateInfo(22, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.RootStart", L"¡ñ $<Exp>"));
			table->SetStateInfo(23, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.Start", L"¡¤ <Exp>"));
			table->SetStateInfo(24, vl::parsing::tabling::ParsingTable::StateInfo(L"Call", L"Call.3", L"<Call>: NAME : functionName \"(\" [ Exp : arguments¡ñ { \",\" Exp : arguments } ] \")\" as FunctionExpression\r\n<Call>: NAME : functionName \"(\" [ Exp : arguments ¡ñ{ \",\" Exp : arguments } ] \")\" as FunctionExpression"));
			table->SetStateInfo(25, vl::parsing::tabling::ParsingTable::StateInfo(L"Factor", L"Factor.3", L"<Factor>: \"(\" !Exp¡ñ \")\""));
			table->SetStateInfo(26, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.1", L"<Term>: !Factor¡ñ\r\n<Term>: Term : firstOperand \"*\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }¡ñ\r\n<Term>: Term : firstOperand \"/\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }¡ñ"));
			table->SetStateInfo(27, vl::parsing::tabling::ParsingTable::StateInfo(L"Term", L"Term.2", L"<Term>: Term : firstOperand¡ñ \"*\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Mul\" }\r\n<Term>: Term : firstOperand¡ñ \"/\" Factor : secondOperand as BinaryExpression with { binaryOperator = \"Div\" }"));
			table->SetStateInfo(28, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.1", L"<Exp>: !Term¡ñ\r\n<Exp>: Exp : firstOperand \"+\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }¡ñ\r\n<Exp>: Exp : firstOperand \"-\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }¡ñ"));
			table->SetStateInfo(29, vl::parsing::tabling::ParsingTable::StateInfo(L"Exp", L"Exp.2", L"<Exp>: Exp : firstOperand¡ñ \"+\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Add\" }\r\n<Exp>: Exp : firstOperand¡ñ \"-\" Term : secondOperand as BinaryExpression with { binaryOperator = \"Sub\" }"));
			table->SetRuleInfo(0, vl::parsing::tabling::ParsingTable::RuleInfo(L"Number", L"NumberExpression", 0));
			table->SetRuleInfo(1, vl::parsing::tabling::ParsingTable::RuleInfo(L"Call", L"FunctionExpression", 16));
			table->SetRuleInfo(2, vl::parsing::tabling::ParsingTable::RuleInfo(L"Factor", L"Expression", 18));
			table->SetRuleInfo(3, vl::parsing::tabling::ParsingTable::RuleInfo(L"Term", L"Expression", 20));
			table->SetRuleInfo(4, vl::parsing::tabling::ParsingTable::RuleInfo(L"Exp", L"Expression", 22));
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 1);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(0, 0, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(1, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 3);
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
					item->stackPattern.Add(19);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 1, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 5, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 6, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 7, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 8, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 10, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(2, 11, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(5, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(5, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 5, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(5, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 14);
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
					item->stackPattern.Add(19);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 1, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 5, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 6, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 7, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 8, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 10, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(19);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(7, 11, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(8, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(8, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(8, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 4);
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 11);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(1, 6);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 1, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(5, 10);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Add"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 5, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(6, 10);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Sub"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 6, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(7, 5);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Mul"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 7, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(21);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(8, 5);
					item->stackPattern.Add(5);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::LeftRecursiveReduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"firstOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Setter, 0, L"binaryOperator", L"Div"));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"BinaryExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 8, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 9);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 10, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(21);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(21);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(5);
					item->stackPattern.Add(23);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Using, 0, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(15);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(11, 8);
					item->stackPattern.Add(5);
					item->stackPattern.Add(10);
					item->stackPattern.Add(8);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 5, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"secondOperand", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Reduce, 8, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Item, 0, L"arguments", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(9, 11, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(10, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(10, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 10, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(10, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 15);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(12, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(13, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(13, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 13, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(13, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(15, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(15, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 15, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(15, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(10, 7);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"FunctionExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(15, 10, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 17);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(16, 0, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(17, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 19);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(18, 0, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(19, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(19, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(19, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 21);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(20, 0, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(21, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(21, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(21, 9, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(0, 23);
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(22, 0, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(3, 12);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"functionName", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(23, 3, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(4, 2);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 19, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Assign, 0, L"value", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Create, 0, L"NumberExpression", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(23, 4, bag);
			}
			{
				vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag;
				{
					vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(9, 13);
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 23, L"", L""));
					item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::Shift, 21, L"", L""));
					bag->transitionItems.Add(item);
				}
				table->SetTransitionBag(23, 9, bag);
			}
			table->Initialize();
			return table;
		}

	}
}
