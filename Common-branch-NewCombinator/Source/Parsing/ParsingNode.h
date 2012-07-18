/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Interface

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGNODE
#define VCZH_PARSING_PARSINGNODE

#include "..\String.h"
#include "..\Pointer.h"
#include "..\Function.h"
#include "..\Collections\List.h"
#include "..\Stream\Accessor.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
语法分析器基础结构
***********************************************************************/

		namespace parsing_internal
		{
			class _Seq;
			class _Alt;
			class _Loop;
			class _Token;
			class _Rule;
			class _Action;
		}

		class IParsingNodeVisitor : public Interface
		{
		public:
			virtual void					Visit(parsing_internal::_Seq* node)=0;
			virtual void					Visit(parsing_internal::_Alt* node)=0;
			virtual void					Visit(parsing_internal::_Loop* node)=0;
			virtual void					Visit(parsing_internal::_Token* node)=0;
			virtual void					Visit(parsing_internal::_Rule* node)=0;
			virtual void					Visit(parsing_internal::_Action* node)=0;
		};

		class ParsingNode : public Object
		{
		public:
			virtual void					Accept(IParsingNodeVisitor* visitor)=0;
		};

		class IParsingNodeAction : public Interface
		{
		public:
			virtual WString					GetName()=0;
		};

		class RuleNode : public Object
		{
		public:
			WString							name;
			Ptr<ParsingNode>				node;
		};

/***********************************************************************
语法分析器扩展结构
***********************************************************************/

		namespace parsing_internal
		{
			class _Seq : public ParsingNode
			{
			public:
				Ptr<ParsingNode>			first;
				Ptr<ParsingNode>			second;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};
			
			class _Alt : public ParsingNode
			{
			public:
				Ptr<ParsingNode>			first;
				Ptr<ParsingNode>			second;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};

			class _Loop : public ParsingNode
			{
			public:
				Ptr<ParsingNode>			node;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};

			class _Token : public ParsingNode
			{
			public:
				vint						token;
				WString						name;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};

			class _Rule : public ParsingNode
			{
			public:
				RuleNode*					rule;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};

			class _Action : public ParsingNode
			{
			public:
				Ptr<ParsingNode>			node;
				Ptr<IParsingNodeAction>		action;

				void						Accept(IParsingNodeVisitor* visitor)override;
			};
		}

/***********************************************************************
语法分析器行为
***********************************************************************/

		namespace parsing_internal
		{
			template<typename TSource, typename TCast>
			class _CreateAction : public Object, public IParsingNodeAction
			{
			public:
				WString GetName()
				{
					return L"Create";
				}
			};

			template<typename TSource, typename TMember>
			class _AssignAction : public Object, public IParsingNodeAction
			{
			public:
				TMember TSource::*		member;

				_AssignAction(TMember TSource::* _member)
					:member(_member)
				{
				}

				WString GetName()
				{
					return L"Assign";
				}
			};

			template<typename TSource, typename TMember>
			class _AssignAction<TSource, collections::List<TMember>> : public Object, public IParsingNodeAction
			{
			public:
				collections::List<TMember> TSource::*		member;

				_AssignAction(collections::List<TMember> TSource::* _member)
					:member(_member)
				{
				}

				WString GetName()
				{
					return L"Push";
				}
			};
			
			template<typename TSource, typename TCast>
			class _CastAction : public Object, public IParsingNodeAction
			{
			public:
				WString GetName()
				{
					return L"Cast";
				}
			};

			template<typename TSource>
			class _UseAction : public Object, public IParsingNodeAction
			{
			public:
				WString GetName()
				{
					return L"Use";
				}
			};

			template<typename TSource, typename TDestination>
			class _TransformAction : public Object, public IParsingNodeAction
			{
			public:
				Func<TDestination(const TSource&)>	transformation;

				_TransformAction(const Func<TDestination(const TSource&)>& _transformation)
					:transformation(_transformation)
				{
				}

				WString GetName()
				{
					return L"Transform";
				}
			};
		}

/***********************************************************************
辅助函数
***********************************************************************/

		// ParsingNode_Search.cpp
		extern void SearchRulesFromRule(const RuleNode* rootRule, collections::List<const RuleNode*>& rules);
		// ParsingNode_Log.cpp
		extern void LogGrammarFromRule(const RuleNode* rootRule, bool logAction, stream::TextWriter& writer);
		// ParsingNode_FirstSet.cpp
		extern void CalculateFirstSet(const RuleNode* rootRule, collections::List<vint>& tokens);
	}
}

#endif