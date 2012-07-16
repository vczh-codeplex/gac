/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Interface

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSING
#define VCZH_PARSING_PARSING

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
			struct NullParsingType
			{
			};

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
语法分析器类型规则
***********************************************************************/

		template<typename T, typename U>
		struct ParsingNodeTypeMergerInternal
		{
		};

		template<typename T>
		struct ParsingNodeTypeDowngrader
		{
			typedef T									Type;
		};

		template<>
		struct ParsingNodeTypeDowngrader<parsing_internal::NullParsingType>
		{
			typedef parsing_internal::NullParsingType	Type;
		};

		template<>
		struct ParsingNodeTypeMergerInternal<parsing_internal::NullParsingType, parsing_internal::NullParsingType>
		{
			typedef parsing_internal::NullParsingType	Type;
		};

		template<typename T>
		struct ParsingNodeTypeMergerInternal<parsing_internal::NullParsingType, T>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParsingNodeTypeMergerInternal<T, parsing_internal::NullParsingType>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParsingNodeTypeMergerInternal<T, T>
		{
			typedef T									Type;
		};

		template<typename T, typename U>
		struct ParsingNodeTypeMerger
		{
			typedef typename ParsingNodeTypeMergerInternal<
				typename ParsingNodeTypeDowngrader<T>::Type,
				typename ParsingNodeTypeDowngrader<U>::Type
				>::Type									Type;
		};

		template<typename T>
		struct ParsingNodeTypeMerger<T, T>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParsingNodeGetPtrElement
		{
		};

		template<typename T>
		struct ParsingNodeGetPtrElement<Ptr<T>>
		{
			typedef T									Type;
		};

/***********************************************************************
语法分析器基础节点构造器
***********************************************************************/

		template<typename T>
		class Node : public Object
		{
		protected:
			Ptr<ParsingNode>				node;

		public:
			Node()
			{
			}

			Node(Ptr<ParsingNode> _node)
				:node(_node)
			{
			}

			Ptr<ParsingNode> GetNode()const
			{
				return node;
			}
		};

/***********************************************************************
语法分析器构造函数
***********************************************************************/

		template<typename T, typename U>
		Node<typename ParsingNodeTypeMerger<T, U>::Type> operator+(const Node<T>& t, const Node<U>& u)
		{
			Ptr<parsing_internal::_Seq> result=new parsing_internal::_Seq;
			result->first=t.GetNode();
			result->second=u.GetNode();
			return result;
		}

		template<typename T, typename U>
		Node<typename ParsingNodeTypeMerger<T, U>::Type> operator|(const Node<T>& t, const Node<U>& u)
		{
			Ptr<parsing_internal::_Alt> result=new parsing_internal::_Alt;
			result->first=t.GetNode();
			result->second=u.GetNode();
			return result;
		}

		template<typename T>
		Node<T> operator*(const Node<T>& t)
		{
			Ptr<parsing_internal::_Loop> result=new parsing_internal::_Loop;
			result->node=t.GetNode();
			return result;
		}

		template<typename T>
		Node<parsing_internal::NullParsingType> operator~(const Node<T>& t)
		{
			return t.GetNode();
		}

		template<typename TSource>
		Node<Ptr<TSource>> create(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_CreateAction<TSource, TSource>;
			return result;
		}

		template<typename TSource, typename TCast>
		Node<Ptr<TCast>> create(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_CreateAction<TSource, TCast>;
			return result;
		}

		template<typename TSource, typename TMember>
		Node<Ptr<TSource>> assign(const Node<TMember>& t, TMember TSource::* member)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_AssignAction<TSource, TMember>(member);
			return result;
		}

		template<typename TSource, typename TCast>
		Node<Ptr<TCast>> cast(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_CastAction<TSource, TCast>;
			return result;
		}

		template<typename TSource>
		Node<Ptr<TSource>> use(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_UseAction<TSource>;
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, TDestination(*transformation)(const TSource&))
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_TransformAction<TSource, TDestination>(transformation);
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, TDestination(*transformation)(TSource))
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_TransformAction<TSource, TDestination>(Func<TDestination(TSource)>(transformation));
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, const Func<TDestination(const TSource&)>& transformation)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_TransformAction<TSource, TDestination>(transformation);
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, const Func<TDestination(TSource)>& transformation)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_TransformAction<TSource, TDestination>(transformation);
			return result;
		}

/***********************************************************************
语法分析器规则构造器
***********************************************************************/

		template<typename T>
		class Rule : public Node<T>
		{
		protected:
			RuleNode						ruleNode;
			Ptr<parsing_internal::_Rule>	ruleObject;

		public:
			Rule(const WString& name=L"")
			{
				ruleNode.name=name;
				ruleObject=new parsing_internal::_Rule;
				ruleObject->rule=&ruleNode;
				node=ruleObject;
			}

			void operator=(const Node<T>& ruleDefinition)
			{
				if(ruleNode.node)
				{
					ruleNode.node=(Node<T>(ruleNode.node) | ruleDefinition).GetNode();
				}
				else
				{
					ruleNode.node=ruleDefinition.GetNode();
				}
			}

			template<typename U>
			void operator=(const Node<U>& ruleDefinition)
			{
				if(ruleNode.node)
				{
					ruleNode.node=(Node<T>(ruleNode.node) | cast<typename ParsingNodeGetPtrElement<U>::Type, typename ParsingNodeGetPtrElement<T>::Type>(ruleDefinition)).GetNode();
				}
				else
				{
					ruleNode.node=cast<typename ParsingNodeGetPtrElement<U>::Type, typename ParsingNodeGetPtrElement<T>::Type>(ruleDefinition).GetNode();
				}
			}
		};

/***********************************************************************
语法分析器

class TokenHelper
{
public:
		static vint Id(const TokenElement& element);
};
***********************************************************************/

		template<typename TTokenElement, typename TTokenHelper>
		class Grammar : public NotCopyable
		{
		public:
		};

/***********************************************************************
辅助函数
***********************************************************************/

		extern void SearchRulesFromRule(RuleNode* rootRule, collections::List<RuleNode*>& rules);
		extern void LogGrammarFromRule(RuleNode* rootRule, stream::TextWriter& writer);
	}
}

#endif