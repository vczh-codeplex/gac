/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parser::GuiInstanceQuery_Parser

本文件使用Vczh Parsing Generator工具自动生成
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_INSTANCEQUERY_GUIINSTANCEQUERY_PARSER
#define VCZH_PRESENTATION_REFLECTION_INSTANCEQUERY_GUIINSTANCEQUERY_PARSER

#include "../../../../../Common/Source/Parsing/Parsing.h"
#include "../../../../../Common/Source/Parsing/ParsingDefinitions.h"
#include "../../../../../Common/Source/Parsing/ParsingAutomaton.h"

namespace vl
{
	namespace presentation
	{
		enum class GuiIqParserTokenIndex
		{
			INDIRECT = 0,
			DIRECT = 1,
			NAME = 2,
			WILDCARD = 3,
			OPEN = 4,
			CLOSE = 5,
			XOR = 6,
			INTERSECT = 7,
			UNION = 8,
			SUBSTRACT = 9,
			ATTRIBUTE = 10,
			COLON = 11,
			DOT = 12,
			SPACE = 13,
		};
		class GuiIqQuery;
		class GuiIqPrimaryQuery;
		class GuiIqCascadeQuery;
		class GuiIqSetQuery;

		class GuiIqQuery abstract : public vl::parsing::ParsingTreeCustomBase, vl::reflection::Description<GuiIqQuery>
		{
		public:
			class IVisitor : public vl::reflection::IDescriptable, vl::reflection::Description<IVisitor>
			{
			public:
				virtual void Visit(GuiIqPrimaryQuery* node)=0;
				virtual void Visit(GuiIqCascadeQuery* node)=0;
				virtual void Visit(GuiIqSetQuery* node)=0;
			};

			virtual void Accept(GuiIqQuery::IVisitor* visitor)=0;

		};

		enum class GuiIqNameOption
		{
			Specified,
			Any,
		};

		enum class GuiIqChildOption
		{
			Direct,
			Indirect,
		};

		class GuiIqPrimaryQuery : public GuiIqQuery, vl::reflection::Description<GuiIqPrimaryQuery>
		{
		public:
			GuiIqChildOption childOption;
			GuiIqNameOption attributeNameOption;
			vl::parsing::ParsingToken attributeName;
			GuiIqNameOption typeNameOption;
			vl::parsing::ParsingToken typeName;
			vl::parsing::ParsingToken referenceName;

			void Accept(GuiIqQuery::IVisitor* visitor)override;

			static vl::Ptr<GuiIqPrimaryQuery> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		class GuiIqCascadeQuery : public GuiIqQuery, vl::reflection::Description<GuiIqCascadeQuery>
		{
		public:
			vl::Ptr<GuiIqQuery> parent;
			vl::Ptr<GuiIqQuery> child;

			void Accept(GuiIqQuery::IVisitor* visitor)override;

			static vl::Ptr<GuiIqCascadeQuery> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		enum class GuiIqBinaryOperator
		{
			ExclusiveOr,
			Intersect,
			Union,
			Substract,
		};

		class GuiIqSetQuery : public GuiIqQuery, vl::reflection::Description<GuiIqSetQuery>
		{
		public:
			vl::Ptr<GuiIqQuery> first;
			vl::Ptr<GuiIqQuery> second;
			GuiIqBinaryOperator op;

			void Accept(GuiIqQuery::IVisitor* visitor)override;

			static vl::Ptr<GuiIqSetQuery> Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		};

		extern vl::WString GuiIqGetParserTextBuffer();
		extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> GuiIqConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);
		extern vl::Ptr<vl::parsing::tabling::ParsingTable> GuiIqLoadTable();

		extern vl::Ptr<vl::parsing::ParsingTreeNode> GuiIqParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
		extern vl::Ptr<vl::parsing::ParsingTreeNode> GuiIqParseAsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);
		extern vl::Ptr<GuiIqQuery> GuiIqParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors, vl::vint codeIndex = -1);
		extern vl::Ptr<GuiIqQuery> GuiIqParse(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::vint codeIndex = -1);
	}
}
namespace vl
{
	namespace reflection
	{
		namespace description
		{
#ifndef VCZH_DEBUG_NO_REFLECTION
			DECL_TYPE_INFO(vl::presentation::GuiIqQuery)
			DECL_TYPE_INFO(vl::presentation::GuiIqNameOption)
			DECL_TYPE_INFO(vl::presentation::GuiIqChildOption)
			DECL_TYPE_INFO(vl::presentation::GuiIqPrimaryQuery)
			DECL_TYPE_INFO(vl::presentation::GuiIqCascadeQuery)
			DECL_TYPE_INFO(vl::presentation::GuiIqBinaryOperator)
			DECL_TYPE_INFO(vl::presentation::GuiIqSetQuery)
			DECL_TYPE_INFO(vl::presentation::GuiIqQuery::IVisitor)

			namespace interface_proxy
			{
				class GuiIqQuery_IVisitor : public ValueInterfaceRoot, public virtual vl::presentation::GuiIqQuery::IVisitor
				{
				public:
					GuiIqQuery_IVisitor(Ptr<IValueInterfaceProxy> proxy)
						:ValueInterfaceRoot(proxy)
					{
					}

					static Ptr<vl::presentation::GuiIqQuery::IVisitor> Create(Ptr<IValueInterfaceProxy> proxy)
					{
						return new GuiIqQuery_IVisitor(proxy);
					}

					void Visit(vl::presentation::GuiIqPrimaryQuery* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::presentation::GuiIqCascadeQuery* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

					void Visit(vl::presentation::GuiIqSetQuery* node)override
					{
						INVOKE_INTERFACE_PROXY(Visit, node);
					}

				};

				}
#endif

			extern bool GuiIqLoadTypes();
		}
	}
}
#endif