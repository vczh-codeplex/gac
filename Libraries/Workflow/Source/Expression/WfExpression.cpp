#include "WfExpression.h"

namespace vl
{
	namespace workflow
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
L"\r\n"L"class FloatingExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class IntegerExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class StringExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"class FormatExpression : Expression"
L"\r\n"L"{"
L"\r\n"L"\ttoken\t\t\t\tvalue;"
L"\r\n"L"}"
L"\r\n"L""
L"\r\n"L"token NAME = \"[a-zA-Z_]/w*\";"
L"\r\n"L"token FLOAT = \"/d+(./d+)?\";"
L"\r\n"L"token INTEGER = \"/d+(./d+)?\";"
L"\r\n"L"token STRING = \"(\'[^\']*\')+|(\"\"[^\"\"]*\"\")+\";"
L"\r\n"L"token FORMATSTRING = \"/$((\'[^\']*\')+|(\"\"[^\"\"]*\"\")+)\";"
L"\r\n"L"discardtoken SPACE = \"/s+\";"
L"\r\n"L""
L"\r\n"L"rule IntegerExpression Integer"
L"\r\n"L"        = INTEGER : value as IntegerExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule FloatingExpression FloatingPoint"
L"\r\n"L"        = FLOAT : value as FloatingExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule StringExpression String"
L"\r\n"L"        = STRING : value as StringExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule FormatExpression FormatString"
L"\r\n"L"        = FORMATSTRING : value as FormatExpression"
L"\r\n"L"        ;"
L"\r\n"L"rule Expression WorkflowExpression"
L"\r\n"L"\t\t= !Integer"
L"\r\n"L"\t\t= !FloatingPoint"
L"\r\n"L"\t\t= !String"
L"\r\n"L"\t\t= !FormatString"
L"\r\n"L"\t\t;"
;

		vl::WString WfGetParserTextBuffer()
		{
			return parserTextBuffer;
		}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

		class WfTreeConverter : public vl::parsing::ParsingTreeConverter
		{
		public:
			using vl::parsing::ParsingTreeConverter::SetMember;

			void Fill(vl::Ptr<WfExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<WfFloatingExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfIntegerExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfStringExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			void Fill(vl::Ptr<WfFormatExpression> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->value, obj->GetMember(L"value"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"FloatingExpression")
				{
					vl::Ptr<WfFloatingExpression> tree = new WfFloatingExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"IntegerExpression")
				{
					vl::Ptr<WfIntegerExpression> tree = new WfIntegerExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"StringExpression")
				{
					vl::Ptr<WfStringExpression> tree = new WfStringExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"FormatExpression")
				{
					vl::Ptr<WfFormatExpression> tree = new WfFormatExpression;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<WfExpression>(), obj, tokens);
					return tree;
				}
				else 
					return 0;
			}
		};

		vl::Ptr<vl::parsing::ParsingTreeCustomBase> WfConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			WfTreeConverter converter;
			vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
			converter.SetMember(tree, node, tokens);
			return tree;
		}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

		vl::Ptr<WfFloatingExpression> WfFloatingExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFloatingExpression>();
		}

		vl::Ptr<WfIntegerExpression> WfIntegerExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfIntegerExpression>();
		}

		vl::Ptr<WfStringExpression> WfStringExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfStringExpression>();
		}

		vl::Ptr<WfFormatExpression> WfFormatExpression::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return WfConvertParsingTreeNode(node, tokens).Cast<WfFormatExpression>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void WfFloatingExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfIntegerExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfStringExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void WfFormatExpression::Accept(WfExpression::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowExpression");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> WfParseExpressionAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseExpressionAsParsingTreeNode(input, table, errors);
		}

		vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors)
		{
			vl::parsing::tabling::ParsingState state(input, table);
			state.Reset(L"WorkflowExpression");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return WfConvertParsingTreeNode(node, state.GetTokens()).Cast<WfExpression>();
			}
			return 0;
		}

		vl::Ptr<WfExpression> WfParseExpression(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return WfParseExpression(input, table, errors);
		}

/***********************************************************************
Table Generation
***********************************************************************/

		vl::Ptr<vl::parsing::tabling::ParsingTable> WfLoadTable()
		{
		    vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();
		    vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
		    vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(parserTextBuffer, L"ParserDecl", errors);
		    vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);
		    vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, false, errors);
		    return table;
		}

	}
}
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			using namespace vl::workflow;

			IMPL_TYPE_INFO_RENAME(WfExpression, Workflow::WfExpression)
			IMPL_TYPE_INFO_RENAME(WfFloatingExpression, Workflow::WfFloatingExpression)
			IMPL_TYPE_INFO_RENAME(WfIntegerExpression, Workflow::WfIntegerExpression)
			IMPL_TYPE_INFO_RENAME(WfStringExpression, Workflow::WfStringExpression)
			IMPL_TYPE_INFO_RENAME(WfFormatExpression, Workflow::WfFormatExpression)
			IMPL_TYPE_INFO_RENAME(WfExpression::IVisitor, Workflow::WfExpression::IVisitor)

			BEGIN_CLASS_MEMBER(WfExpression)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(WfExpression)

			BEGIN_CLASS_MEMBER(WfFloatingExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFloatingExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfFloatingExpression::*)(), [](WfFloatingExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfFloatingExpression::*)(const vl::WString&), [](WfFloatingExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfFloatingExpression)

			BEGIN_CLASS_MEMBER(WfIntegerExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfIntegerExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfIntegerExpression::*)(), [](WfIntegerExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfIntegerExpression::*)(const vl::WString&), [](WfIntegerExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfIntegerExpression)

			BEGIN_CLASS_MEMBER(WfStringExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfStringExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfStringExpression::*)(), [](WfStringExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfStringExpression::*)(const vl::WString&), [](WfStringExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfStringExpression)

			BEGIN_CLASS_MEMBER(WfFormatExpression)
				CLASS_MEMBER_BASE(WfExpression)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<WfFormatExpression>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_value, NO_PARAMETER, vl::WString(WfFormatExpression::*)(), [](WfFormatExpression* node){ return node->value.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_value, {L"value"}, void(WfFormatExpression::*)(const vl::WString&), [](WfFormatExpression* node, const vl::WString& value){ node->value.value = value; })

				CLASS_MEMBER_PROPERTY(value, get_value, set_value)
			END_CLASS_MEMBER(WfFormatExpression)

			BEGIN_CLASS_MEMBER(WfExpression::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<WfExpression::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::WfExpression_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFloatingExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfIntegerExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfStringExpression* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(WfExpression::IVisitor::*)(WfFormatExpression* node))
			END_CLASS_MEMBER(WfExpression)

			class WfTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::workflow::WfExpression)
					ADD_TYPE_INFO(vl::workflow::WfFloatingExpression)
					ADD_TYPE_INFO(vl::workflow::WfIntegerExpression)
					ADD_TYPE_INFO(vl::workflow::WfStringExpression)
					ADD_TYPE_INFO(vl::workflow::WfFormatExpression)
					ADD_TYPE_INFO(vl::workflow::WfExpression::IVisitor)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool WfLoadTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new WfTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}
