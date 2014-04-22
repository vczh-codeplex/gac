#include "GuiInstanceQuery_Parser.h"

namespace vl
{
	namespace presentation
	{
/***********************************************************************
ParserText
***********************************************************************/

const wchar_t parserTextBuffer[] = 
L"\r\n" L""
L"\r\n" L"class Query"
L"\r\n" L"{"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"enum NameOption"
L"\r\n" L"{"
L"\r\n" L"\tSpecified,"
L"\r\n" L"\tAny,"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"enum ChildOption"
L"\r\n" L"{"
L"\r\n" L"\tDirect,"
L"\r\n" L"\tIndirect,"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"class PrimaryQuery : Query"
L"\r\n" L"{"
L"\r\n" L"\tChildOption\t\tchildOption;"
L"\r\n" L"\tNameOption\t\tattributeNameOption;"
L"\r\n" L"\ttoken\t\t\tattributeName;"
L"\r\n" L"\tNameOption\t\ttypeNameOption;"
L"\r\n" L"\ttoken\t\t\ttypeName;"
L"\r\n" L"\ttoken\t\t\treferenceName;"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"class CascadeQuery : Query"
L"\r\n" L"{"
L"\r\n" L"\tQuery\t\t\tparent;"
L"\r\n" L"\tQuery\t\t\tchild;"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"enum BinaryOperator"
L"\r\n" L"{"
L"\r\n" L"\tExclusiveOr,"
L"\r\n" L"\tIntersect,"
L"\r\n" L"\tUnion,"
L"\r\n" L"\tSubstract,"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"class SetQuery : Query"
L"\r\n" L"{"
L"\r\n" L"\tQuery\t\t\tfirst;"
L"\r\n" L"\tQuery\t\t\tsecond;"
L"\r\n" L"\tBinaryOperator\top;"
L"\r\n" L"}"
L"\r\n" L""
L"\r\n" L"token INDIRECT = \"////\";"
L"\r\n" L"token DIRECT = \"//\";"
L"\r\n" L"token NAME = \"[a-zA-Z_][a-zA-Z0-9]*\";"
L"\r\n" L"token WILDCARD = \"/*\";"
L"\r\n" L"token OPEN = \"/(\";"
L"\r\n" L"token CLOSE = \"/)\";"
L"\r\n" L"token XOR = \"/^\";"
L"\r\n" L"token INTERSECT = \"/*\";"
L"\r\n" L"token UNION = \"/+\";"
L"\r\n" L"token SUBSTRACT = \"-\";"
L"\r\n" L"token ATTRIBUTE = \"@\";"
L"\r\n" L"token COLON = \":\";"
L"\r\n" L"token DOT = \".\";"
L"\r\n" L""
L"\r\n" L"discardtoken SPACE = \"/s+\";"
L"\r\n" L""
L"\r\n" L"rule PrimaryQuery QPrimaryFragment"
L"\r\n" L"\t=\t("
L"\r\n" L"\t\t\t(NAME : typeName with {typeNameOption=\"Specified\"})"
L"\r\n" L"\t\t\t| (\"*\" with {typeNameOption=\"Any\"})"
L"\r\n" L"\t\t)"
L"\r\n" L"\t\t[\".\" NAME : referenceName] as PrimaryQuery"
L"\r\n" L"\t;"
L"\r\n" L""
L"\r\n" L"rule PrimaryQuery QPrimary"
L"\r\n" L"\t= !QPrimaryFragment"
L"\r\n" L"\t= \"@\" ("
L"\r\n" L"\t\t\t(NAME : attributeName with {attributeNameOption=\"Specified\"})"
L"\r\n" L"\t\t\t| (\"*\" with {attributeNameOption=\"Any\"})"
L"\r\n" L"\t\t) \":\" !QPrimaryFragment"
L"\r\n" L"\t= \"(\" !QueryRoot \")\""
L"\r\n" L"\t;"
L"\r\n" L""
L"\r\n" L"rule Query Query0"
L"\r\n" L"\t= !QPrimary"
L"\r\n" L"\t= Query0 : parent QPrimary : child as CascadeQuery;"
L"\r\n" L""
L"\r\n" L"rule Query Query1"
L"\r\n" L"\t= !Query0"
L"\r\n" L"\t= Query1 : first \"^\" Query0 : second as SetQuery with {op=\"ExclusiveOr\"}"
L"\r\n" L"\t= Query1 : first \"*\" Query0 : second as SetQuery with {op=\"Intersect\"}"
L"\r\n" L"\t;"
L"\r\n" L"\t"
L"\r\n" L"rule Query Query2"
L"\r\n" L"\t= !Query1"
L"\r\n" L"\t= Query2 : first \"+\" Query1 : second as SetQuery with {op=\"Union\"}"
L"\r\n" L"\t= Query2 : first \"-\" Query1 : second as SetQuery with {op=\"Substract\"}"
L"\r\n" L"\t;"
L"\r\n" L""
L"\r\n" L"rule Query QueryRoot"
L"\r\n" L"\t= !Query2"
L"\r\n" L"\t;"
;

		vl::WString GuiIqGetParserTextBuffer()
		{
			return parserTextBuffer;
		}

/***********************************************************************
Unescaping Function Foward Declarations
***********************************************************************/

/***********************************************************************
Parsing Tree Conversion Driver Implementation
***********************************************************************/

		class GuiIqTreeConverter : public vl::parsing::ParsingTreeConverter
		{
		public:
			using vl::parsing::ParsingTreeConverter::SetMember;

			bool SetMember(GuiIqNameOption& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Specified") { member=GuiIqNameOption::Specified; return true; }
					else if(token->GetValue()==L"Any") { member=GuiIqNameOption::Any; return true; }
					else { member=GuiIqNameOption::Specified; return false; }
				}
				member=GuiIqNameOption::Specified;
				return false;
			}

			bool SetMember(GuiIqChildOption& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"Direct") { member=GuiIqChildOption::Direct; return true; }
					else if(token->GetValue()==L"Indirect") { member=GuiIqChildOption::Indirect; return true; }
					else { member=GuiIqChildOption::Direct; return false; }
				}
				member=GuiIqChildOption::Direct;
				return false;
			}

			bool SetMember(GuiIqBinaryOperator& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)
			{
				vl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();
				if(token)
				{
					if(token->GetValue()==L"ExclusiveOr") { member=GuiIqBinaryOperator::ExclusiveOr; return true; }
					else if(token->GetValue()==L"Intersect") { member=GuiIqBinaryOperator::Intersect; return true; }
					else if(token->GetValue()==L"Union") { member=GuiIqBinaryOperator::Union; return true; }
					else if(token->GetValue()==L"Substract") { member=GuiIqBinaryOperator::Substract; return true; }
					else { member=GuiIqBinaryOperator::ExclusiveOr; return false; }
				}
				member=GuiIqBinaryOperator::ExclusiveOr;
				return false;
			}

			void Fill(vl::Ptr<GuiIqQuery> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
			}

			void Fill(vl::Ptr<GuiIqPrimaryQuery> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->childOption, obj->GetMember(L"childOption"), tokens);
				SetMember(tree->attributeNameOption, obj->GetMember(L"attributeNameOption"), tokens);
				SetMember(tree->attributeName, obj->GetMember(L"attributeName"), tokens);
				SetMember(tree->typeNameOption, obj->GetMember(L"typeNameOption"), tokens);
				SetMember(tree->typeName, obj->GetMember(L"typeName"), tokens);
				SetMember(tree->referenceName, obj->GetMember(L"referenceName"), tokens);
			}

			void Fill(vl::Ptr<GuiIqCascadeQuery> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->parent, obj->GetMember(L"parent"), tokens);
				SetMember(tree->child, obj->GetMember(L"child"), tokens);
			}

			void Fill(vl::Ptr<GuiIqSetQuery> tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)
			{
				SetMember(tree->first, obj->GetMember(L"first"), tokens);
				SetMember(tree->second, obj->GetMember(L"second"), tokens);
				SetMember(tree->op, obj->GetMember(L"op"), tokens);
			}

			vl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override
			{
				if(obj->GetType()==L"PrimaryQuery")
				{
					vl::Ptr<GuiIqPrimaryQuery> tree = new GuiIqPrimaryQuery;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<GuiIqQuery>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"CascadeQuery")
				{
					vl::Ptr<GuiIqCascadeQuery> tree = new GuiIqCascadeQuery;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<GuiIqQuery>(), obj, tokens);
					return tree;
				}
				else if(obj->GetType()==L"SetQuery")
				{
					vl::Ptr<GuiIqSetQuery> tree = new GuiIqSetQuery;
					vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());
					Fill(tree, obj, tokens);
					Fill(tree.Cast<GuiIqQuery>(), obj, tokens);
					return tree;
				}
				else 
					return 0;
			}
		};

		vl::Ptr<vl::parsing::ParsingTreeCustomBase> GuiIqConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			GuiIqTreeConverter converter;
			vl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;
			converter.SetMember(tree, node, tokens);
			return tree;
		}

/***********************************************************************
Parsing Tree Conversion Implementation
***********************************************************************/

		vl::Ptr<GuiIqPrimaryQuery> GuiIqPrimaryQuery::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return GuiIqConvertParsingTreeNode(node, tokens).Cast<GuiIqPrimaryQuery>();
		}

		vl::Ptr<GuiIqCascadeQuery> GuiIqCascadeQuery::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return GuiIqConvertParsingTreeNode(node, tokens).Cast<GuiIqCascadeQuery>();
		}

		vl::Ptr<GuiIqSetQuery> GuiIqSetQuery::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)
		{
			return GuiIqConvertParsingTreeNode(node, tokens).Cast<GuiIqSetQuery>();
		}

/***********************************************************************
Visitor Pattern Implementation
***********************************************************************/

		void GuiIqPrimaryQuery::Accept(GuiIqQuery::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void GuiIqCascadeQuery::Accept(GuiIqQuery::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

		void GuiIqSetQuery::Accept(GuiIqQuery::IVisitor* visitor)
		{
			visitor->Visit(this);
		}

/***********************************************************************
Parser Function
***********************************************************************/

		vl::Ptr<vl::parsing::ParsingTreeNode> GuiIqParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex)
		{
			vl::parsing::tabling::ParsingState state(input, table, codeIndex);
			state.Reset(L"QueryRoot");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			return node;
		}

		vl::Ptr<vl::parsing::ParsingTreeNode> GuiIqParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return GuiIqParseAsParsingTreeNode(input, table, errors, codeIndex);
		}

		vl::Ptr<GuiIqQuery> GuiIqParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex)
		{
			vl::parsing::tabling::ParsingState state(input, table, codeIndex);
			state.Reset(L"QueryRoot");
			vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);
			vl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);
			if(node && errors.Count()==0)
			{
				return GuiIqConvertParsingTreeNode(node, state.GetTokens()).Cast<GuiIqQuery>();
			}
			return 0;
		}

		vl::Ptr<GuiIqQuery> GuiIqParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex)
		{
			vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;
			return GuiIqParse(input, table, errors, codeIndex);
		}

/***********************************************************************
Table Generation
***********************************************************************/

		vl::Ptr<vl::parsing::tabling::ParsingTable> GuiIqLoadTable()
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
			using namespace vl::presentation;

			IMPL_TYPE_INFO_RENAME(GuiIqQuery, presentation::GuiIqQuery)
			IMPL_TYPE_INFO_RENAME(GuiIqNameOption, presentation::GuiIqNameOption)
			IMPL_TYPE_INFO_RENAME(GuiIqChildOption, presentation::GuiIqChildOption)
			IMPL_TYPE_INFO_RENAME(GuiIqPrimaryQuery, presentation::GuiIqPrimaryQuery)
			IMPL_TYPE_INFO_RENAME(GuiIqCascadeQuery, presentation::GuiIqCascadeQuery)
			IMPL_TYPE_INFO_RENAME(GuiIqBinaryOperator, presentation::GuiIqBinaryOperator)
			IMPL_TYPE_INFO_RENAME(GuiIqSetQuery, presentation::GuiIqSetQuery)
			IMPL_TYPE_INFO_RENAME(GuiIqQuery::IVisitor, presentation::GuiIqQuery::IVisitor)

			BEGIN_CLASS_MEMBER(GuiIqQuery)
				CLASS_MEMBER_METHOD(Accept, {L"visitor"})

			END_CLASS_MEMBER(GuiIqQuery)

			BEGIN_ENUM_ITEM(GuiIqNameOption)
				ENUM_ITEM_NAMESPACE(GuiIqNameOption)
				ENUM_NAMESPACE_ITEM(Specified)
				ENUM_NAMESPACE_ITEM(Any)
			END_ENUM_ITEM(GuiIqNameOption)

			BEGIN_ENUM_ITEM(GuiIqChildOption)
				ENUM_ITEM_NAMESPACE(GuiIqChildOption)
				ENUM_NAMESPACE_ITEM(Direct)
				ENUM_NAMESPACE_ITEM(Indirect)
			END_ENUM_ITEM(GuiIqChildOption)

			BEGIN_CLASS_MEMBER(GuiIqPrimaryQuery)
				CLASS_MEMBER_BASE(GuiIqQuery)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<GuiIqPrimaryQuery>(), NO_PARAMETER)

				CLASS_MEMBER_EXTERNALMETHOD(get_attributeName, NO_PARAMETER, vl::WString(GuiIqPrimaryQuery::*)(), [](GuiIqPrimaryQuery* node){ return node->attributeName.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_attributeName, {L"value"}, void(GuiIqPrimaryQuery::*)(const vl::WString&), [](GuiIqPrimaryQuery* node, const vl::WString& value){ node->attributeName.value = value; })
				CLASS_MEMBER_EXTERNALMETHOD(get_typeName, NO_PARAMETER, vl::WString(GuiIqPrimaryQuery::*)(), [](GuiIqPrimaryQuery* node){ return node->typeName.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_typeName, {L"value"}, void(GuiIqPrimaryQuery::*)(const vl::WString&), [](GuiIqPrimaryQuery* node, const vl::WString& value){ node->typeName.value = value; })
				CLASS_MEMBER_EXTERNALMETHOD(get_referenceName, NO_PARAMETER, vl::WString(GuiIqPrimaryQuery::*)(), [](GuiIqPrimaryQuery* node){ return node->referenceName.value; })
				CLASS_MEMBER_EXTERNALMETHOD(set_referenceName, {L"value"}, void(GuiIqPrimaryQuery::*)(const vl::WString&), [](GuiIqPrimaryQuery* node, const vl::WString& value){ node->referenceName.value = value; })

				CLASS_MEMBER_FIELD(childOption)
				CLASS_MEMBER_FIELD(attributeNameOption)
				CLASS_MEMBER_PROPERTY(attributeName, get_attributeName, set_attributeName)
				CLASS_MEMBER_FIELD(typeNameOption)
				CLASS_MEMBER_PROPERTY(typeName, get_typeName, set_typeName)
				CLASS_MEMBER_PROPERTY(referenceName, get_referenceName, set_referenceName)
			END_CLASS_MEMBER(GuiIqPrimaryQuery)

			BEGIN_CLASS_MEMBER(GuiIqCascadeQuery)
				CLASS_MEMBER_BASE(GuiIqQuery)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<GuiIqCascadeQuery>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(parent)
				CLASS_MEMBER_FIELD(child)
			END_CLASS_MEMBER(GuiIqCascadeQuery)

			BEGIN_ENUM_ITEM(GuiIqBinaryOperator)
				ENUM_ITEM_NAMESPACE(GuiIqBinaryOperator)
				ENUM_NAMESPACE_ITEM(ExclusiveOr)
				ENUM_NAMESPACE_ITEM(Intersect)
				ENUM_NAMESPACE_ITEM(Union)
				ENUM_NAMESPACE_ITEM(Substract)
			END_ENUM_ITEM(GuiIqBinaryOperator)

			BEGIN_CLASS_MEMBER(GuiIqSetQuery)
				CLASS_MEMBER_BASE(GuiIqQuery)

				CLASS_MEMBER_CONSTRUCTOR(vl::Ptr<GuiIqSetQuery>(), NO_PARAMETER)


				CLASS_MEMBER_FIELD(first)
				CLASS_MEMBER_FIELD(second)
				CLASS_MEMBER_FIELD(op)
			END_CLASS_MEMBER(GuiIqSetQuery)

			BEGIN_CLASS_MEMBER(GuiIqQuery::IVisitor)
				CLASS_MEMBER_BASE(vl::reflection::IDescriptable)
				CLASS_MEMBER_EXTERNALCTOR(Ptr<GuiIqQuery::IVisitor>(Ptr<IValueInterfaceProxy>), {L"proxy"}, &interface_proxy::GuiIqQuery_IVisitor::Create)

				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(GuiIqQuery::IVisitor::*)(GuiIqPrimaryQuery* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(GuiIqQuery::IVisitor::*)(GuiIqCascadeQuery* node))
				CLASS_MEMBER_METHOD_OVERLOAD(Visit, {L"node"}, void(GuiIqQuery::IVisitor::*)(GuiIqSetQuery* node))
			END_CLASS_MEMBER(GuiIqQuery)

			class GuiIqTypeLoader : public vl::Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					ADD_TYPE_INFO(vl::presentation::GuiIqQuery)
					ADD_TYPE_INFO(vl::presentation::GuiIqNameOption)
					ADD_TYPE_INFO(vl::presentation::GuiIqChildOption)
					ADD_TYPE_INFO(vl::presentation::GuiIqPrimaryQuery)
					ADD_TYPE_INFO(vl::presentation::GuiIqCascadeQuery)
					ADD_TYPE_INFO(vl::presentation::GuiIqBinaryOperator)
					ADD_TYPE_INFO(vl::presentation::GuiIqSetQuery)
					ADD_TYPE_INFO(vl::presentation::GuiIqQuery::IVisitor)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};
#endif

			bool GuiIqLoadTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiIqTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}
