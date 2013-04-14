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
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<FpmMacro> FpmParseFpmacroCode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"macro_start");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
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
		    vl::WString grammar = 
L"\r\n"L""
L"\r\n"L"class Expression"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ConcatExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression[] expressions;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ArrayExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression[] elements;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class InvokeExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression function;"
L"\r\n"L"\tExpression[] arguments;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class BracketExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\tExpression expression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ReferenceExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken name;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class TextExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken text;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Definition"
L"\r\n"L"{"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ExpressionDefinition : Definition"
L"\r\n"L"{"
L"\r\n"L"\tExpression expression;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class ReferenceDefinition : Definition"
L"\r\n"L"{"
L"\r\n"L"\ttoken name;"
L"\r\n"L"\ttoken[] parameters;"
L"\r\n"L"\tDefinition[] definitions;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class Macro"
L"\r\n"L"{"
L"\r\n"L"\tDefinition[] definitions;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token BRACKET_OPEN = \"/(\";"
L"\r\n"L"token BRACKET_CLOSE = \"/)\";"
L"\r\n"L"token ARRAY = \"/$array\";"
L"\r\n"L"token DEFINE = \"/$/$define\";"
L"\r\n"L"token BEGIN = \"/$/$begin\";"
L"\r\n"L"token END = \"/$/$end\";"
L"\r\n"L"token COMMA = \",\";"
L"\r\n"L"token NAME = \"/$[a-zA-Z_]/w*\";"
L"\r\n"L"token NEW_LINE = \"[/r/n]+\";"
L"\r\n"L"token SPACE = \"[ /t]+\";"
L"\r\n"L""
L"\r\n"L"token TEXT_FRAGMENT = \"(////[^/r/n]*|///*([^*]|/*+[^*//])*/*+//)|\"\"([^\\\\\"\"]|\\\\\\.)*\"\"|[^/$(), /t/r/n\"\"]+|/$/(/./)|[//\"\"]\";"
L"\r\n"L"token BRACKET = \"/(/)\";"
L"\r\n"L""
L"\r\n"L"rule ReferenceExpression reference_exp"
L"\r\n"L"\t= NAME : name as ReferenceExpression"
L"\r\n"L"\t;"
L"\r\n"L"\t"
L"\r\n"L"rule Expression unit_exp_nc_nb"
L"\r\n"L"\t= \"$array\" \"(\" [exp_nc : elements {\",\" exp_nc : elements}] \")\" as ArrayExpression"
L"\r\n"L"\t= reference_exp : function \"(\" [exp_nc : arguments {\",\" exp_nc : arguments}] \")\" as InvokeExpression"
L"\r\n"L"\t= reference_exp : function \"()\" as InvokeExpression"
L"\r\n"L"\t= reference_exp : expressions unit_exp_nc_nb : expressions as ConcatExpression"
L"\r\n"L"\t= TEXT_FRAGMENT : text as TextExpression"
L"\r\n"L"\t= SPACE : text as TextExpression"
L"\r\n"L"\t;"
L"\r\n"L"\t"
L"\r\n"L"rule Expression unit_exp_nc"
L"\r\n"L"\t= !unit_exp_nc_nb"
L"\r\n"L"\t= BRACKET : text as TextExpression"
L"\r\n"L"\t= \"(\" exp : expression  \")\" as BracketExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression exp_nc"
L"\r\n"L"\t= !reference_exp"
L"\r\n"L"\t= unit_exp_nc : expressions {unit_exp_nc : expressions} [reference_exp : expressions] as ConcatExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression unit_exp_nb"
L"\r\n"L"\t= \"$array\" \"(\" [exp_nc : elements {\",\" exp_nc : elements}] \")\" as ArrayExpression"
L"\r\n"L"\t= reference_exp : function \"(\" [exp_nc : arguments {\",\" exp_nc : arguments}] \")\" as InvokeExpression"
L"\r\n"L"\t= reference_exp : function \"()\" as InvokeExpression"
L"\r\n"L"\t= reference_exp : expressions unit_exp_nb : expressions as ConcatExpression"
L"\r\n"L"\t= TEXT_FRAGMENT : text as TextExpression"
L"\r\n"L"\t= SPACE : text as TextExpression"
L"\r\n"L"\t= \",\" : text as TextExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression unit_exp"
L"\r\n"L"\t= !unit_exp_nb"
L"\r\n"L"\t= BRACKET : text as TextExpression"
L"\r\n"L"\t= \"(\" exp : expression  \")\" as BracketExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Expression exp"
L"\r\n"L"\t= !reference_exp"
L"\r\n"L"\t= unit_exp : expressions {unit_exp : expressions} [reference_exp : expressions] as ConcatExpression"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule ExpressionDefinition exp_def"
L"\r\n"L"\t= exp : expression as ExpressionDefinition"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule ReferenceDefinition ref_def"
L"\r\n"L"\t= \"$$define\" [SPACE] NAME : name (\"(\" [NAME : parameters {\",\" NAME : parameters}] \")\" | \"()\") SPACE"
L"\r\n"L"\t\t( exp_def : definitions"
L"\r\n"L"\t\t| \"$$begin\" NEW_LINE {def : definitions NEW_LINE } \"$$end\""
L"\r\n"L"\t\t) as ReferenceDefinition"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Definition def"
L"\r\n"L"\t= !exp_def"
L"\r\n"L"\t= !ref_def"
L"\r\n"L"\t;"
L"\r\n"L""
L"\r\n"L"rule Macro macro_start"
L"\r\n"L"\t= [NEW_LINE] def : definitions {NEW_LINE def : definitions} [NEW_LINE] as Macro"
L"\r\n"L"\t;"
;
		    vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();
		    vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
		    vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(grammar, L"ParserDecl", errors);
		    vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);
		    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, true, errors);
		    return table;
		}

	}
}
