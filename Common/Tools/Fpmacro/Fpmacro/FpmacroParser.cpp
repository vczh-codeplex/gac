#include "FpmacroParser.h"

namespace fpmacro
{
	namespace parser
	{
/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

		class FpmTreeConverter : public vl::parsing::ParsingTreeConverter
		{
		public:
			using vl::parsing::ParsingTreeConverter::SetMember;

			void Fill(vl::Ptr<FpmExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<FpmConcatExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expressions, obj->GetMember(L"expressions"), tokens);
			}

			void Fill(vl::Ptr<FpmArrayExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->elements, obj->GetMember(L"elements"), tokens);
			}

			void Fill(vl::Ptr<FpmInvokeExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->function, obj->GetMember(L"function"), tokens);
				SetMember(tree->arguments, obj->GetMember(L"arguments"), tokens);
			}

			void Fill(vl::Ptr<FpmBracketExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<FpmReferenceExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
			}

			void Fill(vl::Ptr<FpmTextExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->text, obj->GetMember(L"text"), tokens);
			}

			void Fill(vl::Ptr<FpmDefinition> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<FpmExpressionDefinition> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->expression, obj->GetMember(L"expression"), tokens);
			}

			void Fill(vl::Ptr<FpmReferenceDefinition> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->name, obj->GetMember(L"name"), tokens);
				SetMember(tree->parameters, obj->GetMember(L"parameters"), tokens);
				SetMember(tree->definitions, obj->GetMember(L"definitions"), tokens);
			}

			void Fill(vl::Ptr<FpmMacro> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->definitions, obj->GetMember(L"definitions"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"ConcatExpression")
				{
					vl::Ptr<FpmConcatExpression> tree = new FpmConcatExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ArrayExpression")
				{
					vl::Ptr<FpmArrayExpression> tree = new FpmArrayExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"InvokeExpression")
				{
					vl::Ptr<FpmInvokeExpression> tree = new FpmInvokeExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"BracketExpression")
				{
					vl::Ptr<FpmBracketExpression> tree = new FpmBracketExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ReferenceExpression")
				{
					vl::Ptr<FpmReferenceExpression> tree = new FpmReferenceExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"TextExpression")
				{
					vl::Ptr<FpmTextExpression> tree = new FpmTextExpression;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ExpressionDefinition")
				{
					vl::Ptr<FpmExpressionDefinition> tree = new FpmExpressionDefinition;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmDefinition>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"ReferenceDefinition")
				{
					vl::Ptr<FpmReferenceDefinition> tree = new FpmReferenceDefinition;
					Fill(tree, obj, tokens);
					Fill(tree.Cast<FpmDefinition>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"Macro")
				{
					vl::Ptr<FpmMacro> tree = new FpmMacro;
					Fill(tree, obj, tokens);
					return tree;
				}
				else 
					return 0;
			}
		};

		vl::Ptr<vl::parsing::ParsingTreeCustomBase> FpmConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			FpmTreeConverter converter;
			vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
			converter.SetMember(tree, node, tokens);
			return tree;
		}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

		vl::Ptr<FpmConcatExpression> FpmConcatExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmConcatExpression>();
		}

		vl::Ptr<FpmArrayExpression> FpmArrayExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmArrayExpression>();
		}

		vl::Ptr<FpmInvokeExpression> FpmInvokeExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmInvokeExpression>();
		}

		vl::Ptr<FpmBracketExpression> FpmBracketExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmBracketExpression>();
		}

		vl::Ptr<FpmReferenceExpression> FpmReferenceExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmReferenceExpression>();
		}

		vl::Ptr<FpmTextExpression> FpmTextExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmTextExpression>();
		}

		vl::Ptr<FpmExpressionDefinition> FpmExpressionDefinition::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmExpressionDefinition>();
		}

		vl::Ptr<FpmReferenceDefinition> FpmReferenceDefinition::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmReferenceDefinition>();
		}

		vl::Ptr<FpmMacro> FpmMacro::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return FpmConvertParsingTreeNode(node, tokens).Cast<FpmMacro>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void FpmConcatExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmArrayExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmInvokeExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmBracketExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmReferenceExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmTextExpression::Accept(FpmExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmExpressionDefinition::Accept(FpmDefinition::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void FpmReferenceDefinition::Accept(FpmDefinition::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> FpmParseFpmacroCodeAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"macro_start");
			vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<FpmMacro> FpmParseFpmacroCode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"macro_start");
			vl::Ptr<vl::parsing::tabling::ParsingStrictParser> parser=new vl::parsing::tabling::ParsingStrictParser;
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node)
			{
				return FpmConvertParsingTreeNode(node, state.GetTokens()).Cast<FpmMacro>();
			}
			return 0;
		}

/***********************************************************************
Table Generation
***********************************************************************/

		vl::Ptr<vl::parsing::tabling::ParsingTable> FpmLoadTable()
		{
			vl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(15-vl::parsing::tabling::ParsingTable::UserTokenStart, 0, 89, 11);
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
			#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE));
			#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;
			#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);
			#define END_LOOK_AHEAD }

			SET_TOKEN_INFO(0, L"", L"")
			SET_TOKEN_INFO(1, L"", L"")
			SET_TOKEN_INFO(2, L"", L"")
			SET_TOKEN_INFO(3, L"BRACKET_OPEN", L"/(")
			SET_TOKEN_INFO(4, L"BRACKET_CLOSE", L"/)")
			SET_TOKEN_INFO(5, L"ARRAY", L"/$array")
			SET_TOKEN_INFO(6, L"DEFINE", L"/$/$define")
			SET_TOKEN_INFO(7, L"BEGIN", L"/$/$begin")
			SET_TOKEN_INFO(8, L"END", L"/$/$end")
			SET_TOKEN_INFO(9, L"COMMA", L",")
			SET_TOKEN_INFO(10, L"NAME", L"/$[a-zA-Z_]/w*")
			SET_TOKEN_INFO(11, L"NEW_LINE", L"[/r/n]+")
			SET_TOKEN_INFO(12, L"SPACE", L"[ /t]+")
			SET_TOKEN_INFO(13, L"TEXT_FRAGMENT", L"(////[^/r/n]*|///*([^*]|/*+[^*//])*/*+//)|\"([^\\\\\"]|\\\\\\.)*\"|[^/$(), /t/r/n\"]+|/$/(/./)|[//\"]")
			SET_TOKEN_INFO(14, L"BRACKET", L"/(/)")


			SET_STATE_INFO(0, L"reference_exp", L"reference_exp.RootStart", L"¡ñ $<reference_exp>")
			SET_STATE_INFO(1, L"reference_exp", L"reference_exp.Start", L"¡¤ <reference_exp>")
			SET_STATE_INFO(2, L"reference_exp", L"reference_exp.1", L"<reference_exp>: NAME : name as ReferenceExpression¡ñ")
			SET_STATE_INFO(3, L"reference_exp", L"reference_exp.RootEnd", L"$<reference_exp> ¡ñ")
			SET_STATE_INFO(4, L"unit_exp_nc_nb", L"unit_exp_nc_nb.5", L"<unit_exp_nc_nb>: reference_exp : function \"(\"¡ñ [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(5, L"unit_exp_nc_nb", L"unit_exp_nc_nb.7", L"<unit_exp_nc_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : function \"()\" as InvokeExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : expressions unit_exp_nc_nb : expressions as ConcatExpression¡ñ\r\n<unit_exp_nc_nb>: TEXT_FRAGMENT : text as TextExpression¡ñ\r\n<unit_exp_nc_nb>: SPACE : text as TextExpression¡ñ")
			SET_STATE_INFO(6, L"unit_exp_nc_nb", L"unit_exp_nc_nb.1", L"<unit_exp_nc_nb>: \"$array\"¡ñ \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(7, L"unit_exp_nc_nb", L"unit_exp_nc_nb.9", L"<unit_exp_nc_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\"¡ñ exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(8, L"unit_exp_nc_nb", L"unit_exp_nc_nb.11", L"<unit_exp_nc_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\"¡ñ exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(9, L"exp_nc", L"exp_nc.RootEnd", L"$<exp_nc> ¡ñ")
			SET_STATE_INFO(10, L"unit_exp_nb", L"unit_exp_nb.7", L"<unit_exp_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression¡ñ\r\n<unit_exp_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression¡ñ\r\n<unit_exp_nb>: reference_exp : function \"()\" as InvokeExpression¡ñ\r\n<unit_exp_nb>: reference_exp : expressions unit_exp_nb : expressions as ConcatExpression¡ñ\r\n<unit_exp_nb>: TEXT_FRAGMENT : text as TextExpression¡ñ\r\n<unit_exp_nb>: SPACE : text as TextExpression¡ñ\r\n<unit_exp_nb>: \",\" : text as TextExpression¡ñ")
			SET_STATE_INFO(11, L"unit_exp_nb", L"unit_exp_nb.9", L"<unit_exp_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\"¡ñ exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(12, L"unit_exp_nb", L"unit_exp_nb.11", L"<unit_exp_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\"¡ñ exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(13, L"unit_exp_nb", L"unit_exp_nb.5", L"<unit_exp_nb>: reference_exp : function \"(\"¡ñ [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(14, L"unit_exp_nb", L"unit_exp_nb.1", L"<unit_exp_nb>: \"$array\"¡ñ \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(15, L"unit_exp_nc", L"unit_exp_nc.2", L"<unit_exp_nc>: BRACKET : text as TextExpression¡ñ\r\n<unit_exp_nc>: \"(\" exp : expression \")\" as BracketExpression¡ñ")
			SET_STATE_INFO(16, L"unit_exp", L"unit_exp.2", L"<unit_exp>: BRACKET : text as TextExpression¡ñ\r\n<unit_exp>: \"(\" exp : expression \")\" as BracketExpression¡ñ")
			SET_STATE_INFO(17, L"exp", L"exp.RootEnd", L"$<exp> ¡ñ")
			SET_STATE_INFO(18, L"exp_def", L"exp_def.RootEnd", L"$<exp_def> ¡ñ")
			SET_STATE_INFO(19, L"ref_def", L"ref_def.RootEnd", L"$<ref_def> ¡ñ")
			SET_STATE_INFO(20, L"ref_def", L"ref_def.11", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE¡ñ { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition\r\n<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE ¡ñ{ def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(21, L"def", L"def.RootEnd", L"$<def> ¡ñ")
			SET_STATE_INFO(22, L"macro_start", L"macro_start.RootEnd", L"$<macro_start> ¡ñ")
			SET_STATE_INFO(23, L"macro_start", L"macro_start.3", L"<macro_start>: [ NEW_LINE ] def : definitions { NEW_LINE def : definitions } [ NEW_LINE ] as Macro¡ñ\r\n<macro_start>: [ NEW_LINE ] def : definitions { NEW_LINE¡ñ def : definitions } [ NEW_LINE ] as Macro")
			SET_STATE_INFO(24, L"unit_exp_nc", L"unit_exp_nc.3", L"<unit_exp_nc>: \"(\"¡ñ exp : expression \")\" as BracketExpression")
			SET_STATE_INFO(25, L"unit_exp_nc_nb", L"unit_exp_nc_nb.RootEnd", L"$<unit_exp_nc_nb> ¡ñ")
			SET_STATE_INFO(26, L"unit_exp_nc", L"unit_exp_nc.RootEnd", L"$<unit_exp_nc> ¡ñ")
			SET_STATE_INFO(27, L"unit_exp_nc_nb", L"unit_exp_nc_nb.4", L"<unit_exp_nc_nb>: \"$array\" \"(\"¡ñ [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(28, L"unit_exp_nb", L"unit_exp_nb.RootEnd", L"$<unit_exp_nb> ¡ñ")
			SET_STATE_INFO(29, L"unit_exp", L"unit_exp.RootEnd", L"$<unit_exp> ¡ñ")
			SET_STATE_INFO(30, L"unit_exp", L"unit_exp.3", L"<unit_exp>: \"(\"¡ñ exp : expression \")\" as BracketExpression")
			SET_STATE_INFO(31, L"unit_exp_nb", L"unit_exp_nb.4", L"<unit_exp_nb>: \"$array\" \"(\"¡ñ [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(32, L"ref_def", L"ref_def.7", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition¡ñ")
			SET_STATE_INFO(33, L"ref_def", L"ref_def.1", L"<ref_def>: \"$$define\"¡ñ [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(34, L"ref_def", L"ref_def.2", L"<ref_def>: \"$$define\" [ SPACE ]¡ñ NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(35, L"ref_def", L"ref_def.3", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name¡ñ ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(36, L"ref_def", L"ref_def.5", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" )¡ñ ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(37, L"ref_def", L"ref_def.4", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\"¡ñ [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(38, L"ref_def", L"ref_def.8", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\"¡ñ NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition\r\n<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions¡ñ NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(39, L"ref_def", L"ref_def.6", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters¡ñ { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition\r\n<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters ¡ñ{ \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(40, L"ref_def", L"ref_def.9", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\"¡ñ NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition")
			SET_STATE_INFO(41, L"unit_exp_nc_nb", L"unit_exp_nc_nb.RootStart", L"¡ñ $<unit_exp_nc_nb>")
			SET_STATE_INFO(42, L"unit_exp_nc_nb", L"unit_exp_nc_nb.Start", L"¡¤ <unit_exp_nc_nb>")
			SET_STATE_INFO(43, L"unit_exp_nc", L"unit_exp_nc.RootStart", L"¡ñ $<unit_exp_nc>")
			SET_STATE_INFO(44, L"unit_exp_nc", L"unit_exp_nc.Start", L"¡¤ <unit_exp_nc>")
			SET_STATE_INFO(45, L"exp_nc", L"exp_nc.RootStart", L"¡ñ $<exp_nc>")
			SET_STATE_INFO(46, L"exp_nc", L"exp_nc.Start", L"¡¤ <exp_nc>")
			SET_STATE_INFO(47, L"unit_exp_nb", L"unit_exp_nb.RootStart", L"¡ñ $<unit_exp_nb>")
			SET_STATE_INFO(48, L"unit_exp_nb", L"unit_exp_nb.Start", L"¡¤ <unit_exp_nb>")
			SET_STATE_INFO(49, L"unit_exp", L"unit_exp.RootStart", L"¡ñ $<unit_exp>")
			SET_STATE_INFO(50, L"unit_exp", L"unit_exp.Start", L"¡¤ <unit_exp>")
			SET_STATE_INFO(51, L"exp", L"exp.RootStart", L"¡ñ $<exp>")
			SET_STATE_INFO(52, L"exp", L"exp.Start", L"¡¤ <exp>")
			SET_STATE_INFO(53, L"exp_def", L"exp_def.RootStart", L"¡ñ $<exp_def>")
			SET_STATE_INFO(54, L"exp_def", L"exp_def.Start", L"¡¤ <exp_def>")
			SET_STATE_INFO(55, L"ref_def", L"ref_def.RootStart", L"¡ñ $<ref_def>")
			SET_STATE_INFO(56, L"ref_def", L"ref_def.Start", L"¡¤ <ref_def>")
			SET_STATE_INFO(57, L"def", L"def.RootStart", L"¡ñ $<def>")
			SET_STATE_INFO(58, L"def", L"def.Start", L"¡¤ <def>")
			SET_STATE_INFO(59, L"macro_start", L"macro_start.RootStart", L"¡ñ $<macro_start>")
			SET_STATE_INFO(60, L"macro_start", L"macro_start.Start", L"¡¤ <macro_start>")
			SET_STATE_INFO(61, L"macro_start", L"macro_start.1", L"<macro_start>: [ NEW_LINE ]¡ñ def : definitions { NEW_LINE def : definitions } [ NEW_LINE ] as Macro")
			SET_STATE_INFO(62, L"reference_exp", L"reference_exp.2", L"<reference_exp>: NAME : name as ReferenceExpression¡ñ")
			SET_STATE_INFO(63, L"unit_exp_nc_nb", L"unit_exp_nc_nb.2", L"<unit_exp_nc_nb>: reference_exp : function¡ñ \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression\r\n<unit_exp_nc_nb>: reference_exp : function¡ñ \"()\" as InvokeExpression")
			SET_STATE_INFO(64, L"unit_exp_nc_nb", L"unit_exp_nc_nb.3", L"<unit_exp_nc_nb>: reference_exp : expressions¡ñ unit_exp_nc_nb : expressions as ConcatExpression")
			SET_STATE_INFO(65, L"unit_exp_nc_nb", L"unit_exp_nc_nb.6", L"<unit_exp_nc_nb>: \"$array\" \"(\" [ exp_nc : elements¡ñ { \",\" exp_nc : elements } ] \")\" as ArrayExpression\r\n<unit_exp_nc_nb>: \"$array\" \"(\" [ exp_nc : elements ¡ñ{ \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(66, L"unit_exp_nc_nb", L"unit_exp_nc_nb.8", L"<unit_exp_nc_nb>: reference_exp : function \"(\" [ exp_nc : arguments¡ñ { \",\" exp_nc : arguments } ] \")\" as InvokeExpression\r\n<unit_exp_nc_nb>: reference_exp : function \"(\" [ exp_nc : arguments ¡ñ{ \",\" exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(67, L"unit_exp_nc_nb", L"unit_exp_nc_nb.10", L"<unit_exp_nc_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : function \"()\" as InvokeExpression¡ñ\r\n<unit_exp_nc_nb>: reference_exp : expressions unit_exp_nc_nb : expressions as ConcatExpression¡ñ\r\n<unit_exp_nc_nb>: TEXT_FRAGMENT : text as TextExpression¡ñ\r\n<unit_exp_nc_nb>: SPACE : text as TextExpression¡ñ")
			SET_STATE_INFO(68, L"unit_exp_nc", L"unit_exp_nc.1", L"<unit_exp_nc>: !unit_exp_nc_nb¡ñ\r\n<unit_exp_nc>: BRACKET : text as TextExpression¡ñ\r\n<unit_exp_nc>: \"(\" exp : expression \")\" as BracketExpression¡ñ")
			SET_STATE_INFO(69, L"unit_exp_nc", L"unit_exp_nc.4", L"<unit_exp_nc>: \"(\" exp : expression¡ñ \")\" as BracketExpression")
			SET_STATE_INFO(70, L"exp_nc", L"exp_nc.1", L"<exp_nc>: !reference_exp¡ñ\r\n<exp_nc>: unit_exp_nc : expressions { unit_exp_nc : expressions } [ reference_exp : expressions ] as ConcatExpression¡ñ")
			SET_STATE_INFO(71, L"exp_nc", L"exp_nc.2", L"<exp_nc>: unit_exp_nc : expressions¡ñ { unit_exp_nc : expressions } [ reference_exp : expressions ] as ConcatExpression\r\n<exp_nc>: unit_exp_nc : expressions ¡ñ{ unit_exp_nc : expressions } [ reference_exp : expressions ] as ConcatExpression")
			SET_STATE_INFO(72, L"exp_nc", L"exp_nc.3", L"<exp_nc>: unit_exp_nc : expressions { unit_exp_nc : expressions } [ reference_exp : expressions ] as ConcatExpression¡ñ")
			SET_STATE_INFO(73, L"unit_exp_nb", L"unit_exp_nb.2", L"<unit_exp_nb>: reference_exp : function¡ñ \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression\r\n<unit_exp_nb>: reference_exp : function¡ñ \"()\" as InvokeExpression")
			SET_STATE_INFO(74, L"unit_exp_nb", L"unit_exp_nb.3", L"<unit_exp_nb>: reference_exp : expressions¡ñ unit_exp_nb : expressions as ConcatExpression")
			SET_STATE_INFO(75, L"unit_exp_nb", L"unit_exp_nb.6", L"<unit_exp_nb>: \"$array\" \"(\" [ exp_nc : elements¡ñ { \",\" exp_nc : elements } ] \")\" as ArrayExpression\r\n<unit_exp_nb>: \"$array\" \"(\" [ exp_nc : elements ¡ñ{ \",\" exp_nc : elements } ] \")\" as ArrayExpression")
			SET_STATE_INFO(76, L"unit_exp_nb", L"unit_exp_nb.8", L"<unit_exp_nb>: reference_exp : function \"(\" [ exp_nc : arguments¡ñ { \",\" exp_nc : arguments } ] \")\" as InvokeExpression\r\n<unit_exp_nb>: reference_exp : function \"(\" [ exp_nc : arguments ¡ñ{ \",\" exp_nc : arguments } ] \")\" as InvokeExpression")
			SET_STATE_INFO(77, L"unit_exp_nb", L"unit_exp_nb.10", L"<unit_exp_nb>: \"$array\" \"(\" [ exp_nc : elements { \",\" exp_nc : elements } ] \")\" as ArrayExpression¡ñ\r\n<unit_exp_nb>: reference_exp : function \"(\" [ exp_nc : arguments { \",\" exp_nc : arguments } ] \")\" as InvokeExpression¡ñ\r\n<unit_exp_nb>: reference_exp : function \"()\" as InvokeExpression¡ñ\r\n<unit_exp_nb>: reference_exp : expressions unit_exp_nb : expressions as ConcatExpression¡ñ\r\n<unit_exp_nb>: TEXT_FRAGMENT : text as TextExpression¡ñ\r\n<unit_exp_nb>: SPACE : text as TextExpression¡ñ\r\n<unit_exp_nb>: \",\" : text as TextExpression¡ñ")
			SET_STATE_INFO(78, L"unit_exp", L"unit_exp.1", L"<unit_exp>: !unit_exp_nb¡ñ\r\n<unit_exp>: BRACKET : text as TextExpression¡ñ\r\n<unit_exp>: \"(\" exp : expression \")\" as BracketExpression¡ñ")
			SET_STATE_INFO(79, L"unit_exp", L"unit_exp.4", L"<unit_exp>: \"(\" exp : expression¡ñ \")\" as BracketExpression")
			SET_STATE_INFO(80, L"exp", L"exp.1", L"<exp>: !reference_exp¡ñ\r\n<exp>: unit_exp : expressions { unit_exp : expressions } [ reference_exp : expressions ] as ConcatExpression¡ñ")
			SET_STATE_INFO(81, L"exp", L"exp.2", L"<exp>: unit_exp : expressions¡ñ { unit_exp : expressions } [ reference_exp : expressions ] as ConcatExpression\r\n<exp>: unit_exp : expressions ¡ñ{ unit_exp : expressions } [ reference_exp : expressions ] as ConcatExpression")
			SET_STATE_INFO(82, L"exp", L"exp.3", L"<exp>: unit_exp : expressions { unit_exp : expressions } [ reference_exp : expressions ] as ConcatExpression¡ñ")
			SET_STATE_INFO(83, L"exp_def", L"exp_def.1", L"<exp_def>: exp : expression as ExpressionDefinition¡ñ")
			SET_STATE_INFO(84, L"exp_def", L"exp_def.2", L"<exp_def>: exp : expression as ExpressionDefinition¡ñ")
			SET_STATE_INFO(85, L"ref_def", L"ref_def.10", L"<ref_def>: \"$$define\" [ SPACE ] NAME : name ( \"(\" [ NAME : parameters { \",\" NAME : parameters } ] \")\" | \"()\" ) ( exp_def : definitions | \"$$begin\" NEW_LINE { def : definitions NEW_LINE } \"$$end\" ) as ReferenceDefinition¡ñ")
			SET_STATE_INFO(86, L"def", L"def.1", L"<def>: !exp_def¡ñ\r\n<def>: !ref_def¡ñ")
			SET_STATE_INFO(87, L"macro_start", L"macro_start.2", L"<macro_start>: [ NEW_LINE ] def : definitions¡ñ { NEW_LINE def : definitions } [ NEW_LINE ] as Macro\r\n<macro_start>: [ NEW_LINE ] def : definitions ¡ñ{ NEW_LINE def : definitions } [ NEW_LINE ] as Macro")
			SET_STATE_INFO(88, L"macro_start", L"macro_start.4", L"<macro_start>: [ NEW_LINE ] def : definitions { NEW_LINE def : definitions } [ NEW_LINE ] as Macro¡ñ")

			SET_RULE_INFO(0, L"reference_exp", L"ReferenceExpression", 0)
			SET_RULE_INFO(1, L"unit_exp_nc_nb", L"Expression", 41)
			SET_RULE_INFO(2, L"unit_exp_nc", L"Expression", 43)
			SET_RULE_INFO(3, L"exp_nc", L"Expression", 45)
			SET_RULE_INFO(4, L"unit_exp_nb", L"Expression", 47)
			SET_RULE_INFO(5, L"unit_exp", L"Expression", 49)
			SET_RULE_INFO(6, L"exp", L"Expression", 51)
			SET_RULE_INFO(7, L"exp_def", L"ExpressionDefinition", 53)
			SET_RULE_INFO(8, L"ref_def", L"ReferenceDefinition", 55)
			SET_RULE_INFO(9, L"def", L"Definition", 57)
			SET_RULE_INFO(10, L"macro_start", L"Macro", 59)

			BEGIN_TRANSITION_BAG(0, 0)

				BEGIN_TRANSITION_ITEM(0, 1)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(1, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 1)

				BEGIN_TRANSITION_ITEM(1, 3)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 3)

				BEGIN_TRANSITION_ITEM(3, 13)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"function", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(3, 4)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"function", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 4)

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 74, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 64, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 9)

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 74, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 74, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 64, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 11)

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 74, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 64, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 74, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 64, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(2, 14)

				BEGIN_TRANSITION_ITEM(14, 10)
				ITEM_STACK_PATTERN(48)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"function", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(14, 5)
				ITEM_STACK_PATTERN(42)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceExpression", L"");
				ITEM_INSTRUCTION(Reduce, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"function", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 4)

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(4, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 4, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 1)

				BEGIN_TRANSITION_ITEM(1, 25)
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 26)
				ITEM_STACK_PATTERN(44)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 2)

				BEGIN_TRANSITION_ITEM(2, 5)
				ITEM_STACK_PATTERN(64)
				ITEM_INSTRUCTION(Reduce, 64, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 4)

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 9)

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(5, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_STACK_PATTERN(44)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 44, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(6, 3)

				BEGIN_TRANSITION_ITEM(3, 27)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(7, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 7, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(8, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 8, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 1)

				BEGIN_TRANSITION_ITEM(1, 28)
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 29)
				ITEM_STACK_PATTERN(50)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 2)

				BEGIN_TRANSITION_ITEM(2, 10)
				ITEM_STACK_PATTERN(74)
				ITEM_INSTRUCTION(Reduce, 74, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 4)

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 11)

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(10, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_STACK_PATTERN(50)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 50, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(11, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 11, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(12, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 12, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 4)

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(13, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 13, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(14, 3)

				BEGIN_TRANSITION_ITEM(3, 31)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 1)

				BEGIN_TRANSITION_ITEM(1, 26)
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 9)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 4)

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				ITEM_INSTRUCTION(Create, 0, L"InvokeExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 9)

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(71)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(31)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 31, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(27)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 27, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(13)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 13, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 12)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(12)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 12, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 11)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(11)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 11, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(8)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 8, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 7)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(7)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 7, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"elements", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 8)
				ITEM_STACK_PATTERN(46)
				ITEM_STACK_PATTERN(4)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 4, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"arguments", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(15, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_STACK_PATTERN(71)
				ITEM_INSTRUCTION(Reduce, 71, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_STACK_PATTERN(46)
				ITEM_INSTRUCTION(Reduce, 46, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 71, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 1)

				BEGIN_TRANSITION_ITEM(1, 29)
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 18)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 17)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 4)

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 16)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(30)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 30, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(4, 15)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(24)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 24, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"BracketExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 11)

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(81)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(52)
				ITEM_STACK_PATTERN(54)
				ITEM_STACK_PATTERN(36)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ConcatExpression", L"");
				ITEM_INSTRUCTION(Reduce, 54, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"expression", L"");
				ITEM_INSTRUCTION(Create, 0, L"ExpressionDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 36, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(16, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_STACK_PATTERN(81)
				ITEM_INSTRUCTION(Reduce, 81, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_STACK_PATTERN(52)
				ITEM_INSTRUCTION(Reduce, 52, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"expressions", L"");
				ITEM_INSTRUCTION(Shift, 81, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 8)

				BEGIN_TRANSITION_ITEM(8, 32)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(20, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 20, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 1)

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(23, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 23, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(24, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 24, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 4)

				BEGIN_TRANSITION_ITEM(4, 5)
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(27, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 27, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(30, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 30, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 4)

				BEGIN_TRANSITION_ITEM(4, 10)
				ITEM_INSTRUCTION(Create, 0, L"ArrayExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(31, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 31, L"", L"");
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(32, 1)

				BEGIN_TRANSITION_ITEM(1, 19)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 22)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				ITEM_INSTRUCTION(Create, 0, L"Macro", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(1, 21)
				ITEM_STACK_PATTERN(58)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(32, 11)

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(61)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 61, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(60)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 60, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 23)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(23)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 23, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(11, 20)
				ITEM_STACK_PATTERN(58)
				ITEM_STACK_PATTERN(20)
				ITEM_INSTRUCTION(Create, 0, L"ReferenceDefinition", L"");
				ITEM_INSTRUCTION(Reduce, 58, L"", L"");
				ITEM_INSTRUCTION(Using, 0, L"", L"");
				ITEM_INSTRUCTION(Reduce, 20, L"", L"");
				ITEM_INSTRUCTION(Item, 0, L"definitions", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(33, 10)

				BEGIN_TRANSITION_ITEM(10, 35)
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(33, 12)

				BEGIN_TRANSITION_ITEM(12, 34)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(34, 10)

				BEGIN_TRANSITION_ITEM(10, 35)
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(35, 3)

				BEGIN_TRANSITION_ITEM(3, 37)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(35, 14)

				BEGIN_TRANSITION_ITEM(14, 36)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 7)

				BEGIN_TRANSITION_ITEM(7, 38)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(36, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 36, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(37, 4)

				BEGIN_TRANSITION_ITEM(4, 36)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(37, 10)

				BEGIN_TRANSITION_ITEM(10, 39)
				ITEM_INSTRUCTION(Item, 0, L"parameters", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(38, 11)

				BEGIN_TRANSITION_ITEM(11, 20)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(39, 4)

				BEGIN_TRANSITION_ITEM(4, 36)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(39, 9)

				BEGIN_TRANSITION_ITEM(9, 40)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(40, 10)

				BEGIN_TRANSITION_ITEM(10, 39)
				ITEM_INSTRUCTION(Item, 0, L"parameters", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(41, 0)

				BEGIN_TRANSITION_ITEM(0, 42)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(42, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(42, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(42, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(42, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(43, 0)

				BEGIN_TRANSITION_ITEM(0, 44)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(44, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(45, 0)

				BEGIN_TRANSITION_ITEM(0, 46)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 3)

				BEGIN_TRANSITION_ITEM(3, 24)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 5)

				BEGIN_TRANSITION_ITEM(5, 6)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Shift, 42, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 12)

				BEGIN_TRANSITION_ITEM(12, 5)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 13)

				BEGIN_TRANSITION_ITEM(13, 5)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Shift, 44, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(46, 14)

				BEGIN_TRANSITION_ITEM(14, 15)
				ITEM_INSTRUCTION(Shift, 46, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(47, 0)

				BEGIN_TRANSITION_ITEM(0, 48)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(48, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(48, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(48, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(48, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(48, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(49, 0)

				BEGIN_TRANSITION_ITEM(0, 50)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(50, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(51, 0)

				BEGIN_TRANSITION_ITEM(0, 52)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(52, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(53, 0)

				BEGIN_TRANSITION_ITEM(0, 54)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(54, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(55, 0)

				BEGIN_TRANSITION_ITEM(0, 56)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(56, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(57, 0)

				BEGIN_TRANSITION_ITEM(0, 58)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(58, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(59, 0)

				BEGIN_TRANSITION_ITEM(0, 60)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 11)

				BEGIN_TRANSITION_ITEM(11, 61)
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(60, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 60, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 3)

				BEGIN_TRANSITION_ITEM(3, 30)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 5)

				BEGIN_TRANSITION_ITEM(5, 14)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 6)

				BEGIN_TRANSITION_ITEM(6, 33)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 9)

				BEGIN_TRANSITION_ITEM(9, 10)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 10)

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

				BEGIN_TRANSITION_ITEM(10, 2)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Shift, 48, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"name", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 12)

				BEGIN_TRANSITION_ITEM(12, 10)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 13)

				BEGIN_TRANSITION_ITEM(13, 10)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Shift, 50, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
				END_TRANSITION_ITEM

			END_TRANSITION_BAG

			BEGIN_TRANSITION_BAG(61, 14)

				BEGIN_TRANSITION_ITEM(14, 16)
				ITEM_INSTRUCTION(Shift, 61, L"", L"");
				ITEM_INSTRUCTION(Shift, 58, L"", L"");
				ITEM_INSTRUCTION(Shift, 54, L"", L"");
				ITEM_INSTRUCTION(Shift, 52, L"", L"");
				ITEM_INSTRUCTION(Assign, 0, L"text", L"");
				ITEM_INSTRUCTION(Create, 0, L"TextExpression", L"");
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
