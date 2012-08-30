/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Interface

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSING
#define VCZH_PARSING_PARSING

#include "ParsingNode.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
语法分析器类型规则
***********************************************************************/

		namespace parsing_internal
		{
			struct NullParsingType
			{
			};

			template<typename T, typename U>
			struct ParsingNodeTypeMergerInternal
			{
			};

			template<typename T>
			struct ParsingNodeTypeDowngrader
			{
				typedef T				Type;
			};

			template<>
			struct ParsingNodeTypeDowngrader<NullParsingType>
			{
				typedef NullParsingType	Type;
			};

			template<>
			struct ParsingNodeTypeMergerInternal<NullParsingType, NullParsingType>
			{
				typedef NullParsingType	Type;
			};

			template<typename T>
			struct ParsingNodeTypeMergerInternal<NullParsingType, T>
			{
				typedef T				Type;
			};

			template<typename T>
			struct ParsingNodeTypeMergerInternal<T, NullParsingType>
			{
				typedef T				Type;
			};

			template<typename T>
			struct ParsingNodeTypeMergerInternal<T, T>
			{
				typedef T				Type;
			};

			template<typename T, typename U>
			struct ParsingNodeTypeMerger
			{
				typedef typename ParsingNodeTypeMergerInternal<
					typename ParsingNodeTypeDowngrader<T>::Type,
					typename ParsingNodeTypeDowngrader<U>::Type
					>::Type				Type;
			};

			template<typename T>
			struct ParsingNodeTypeMerger<T, T>
			{
				typedef T				Type;
			};

			template<typename T>
			struct ParsingNodeGetPtrElement
			{
			};

			template<typename T>
			struct ParsingNodeGetPtrElement<Ptr<T>>
			{
				typedef T				Type;
			};
		}

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

			virtual Ptr<ParsingNode> GetNode()const
			{
				return node;
			}
		};

/***********************************************************************
语法分析器构造函数(+)
***********************************************************************/

		template<typename T, typename U>
		Node<typename parsing_internal::ParsingNodeTypeMerger<T, U>::Type> operator+(const Node<T>& t, const Node<U>& u)
		{
			Ptr<parsing_internal::_Seq> result=new parsing_internal::_Seq;
			result->first=t.GetNode();
			result->second=u.GetNode();
			return result;
		}

/***********************************************************************
语法分析器构造函数(|)
***********************************************************************/

		template<typename T, typename U>
		Node<typename parsing_internal::ParsingNodeTypeMerger<T, U>::Type> operator|(const Node<T>& t, const Node<U>& u)
		{
			Ptr<parsing_internal::_Alt> result=new parsing_internal::_Alt;
			result->first=t.GetNode();
			result->second=u.GetNode();
			return result;
		}

/***********************************************************************
语法分析器构造函数(*)
***********************************************************************/

		template<typename T>
		Node<T> operator*(const Node<T>& t)
		{
			Ptr<parsing_internal::_Loop> result=new parsing_internal::_Loop;
			result->node=t.GetNode();
			return result;
		}

/***********************************************************************
语法分析器构造函数(~)
***********************************************************************/

		template<typename T>
		Node<parsing_internal::NullParsingType> operator~(const Node<T>& t)
		{
			return t.GetNode();
		}

/***********************************************************************
语法分析器构造函数(create)
***********************************************************************/

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

/***********************************************************************
语法分析器构造函数(assign)
***********************************************************************/

		template<typename TSource, typename TMember>
		Node<Ptr<TSource>> assign(const Node<TMember>& t, TMember TSource::* member)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_AssignAction<TSource, TMember>(member);
			return result;
		}

		template<typename TSource, typename TMember>
		Node<Ptr<TSource>> assign(const Node<TMember>& t, collections::List<TMember> TSource::* member)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_AssignAction<TSource, collections::List<TMember>>(member);
			return result;
		}

/***********************************************************************
语法分析器构造函数(cast)
***********************************************************************/

		template<typename TSource, typename TCast>
		Node<Ptr<TCast>> cast(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_CastAction<TSource, TCast>;
			return result;
		}

/***********************************************************************
语法分析器构造函数(use)
***********************************************************************/

		template<typename TSource>
		Node<Ptr<TSource>> use(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Action> result=new parsing_internal::_Action;
			result->node=t.GetNode();
			result->action=new parsing_internal::_UseAction<TSource>;
			return result;
		}

/***********************************************************************
语法分析器构造函数(transform)
***********************************************************************/

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
		private:
			void operator=(const Rule<T>& rule);
		protected:
			Ptr<RuleNode>					ruleNode;
		public:
			Rule(const WString& name=L"")
			{
				ruleNode=new RuleNode;
				ruleNode->name=name;
			}

			Ptr<ParsingNode> GetNode()const override
			{
				Ptr<parsing_internal::_Rule> ruleObject=new parsing_internal::_Rule;
				ruleObject->rule=ruleNode.Obj();
				return ruleObject;
			}

			Ptr<RuleNode> GetRuleNode()const
			{
				return ruleNode;
			}

			void operator=(const Node<T>& ruleDefinition)
			{
				if(ruleNode->node)
				{
					ruleNode->node=(Node<T>(ruleNode->node) | ruleDefinition).GetNode();
				}
				else
				{
					ruleNode->node=ruleDefinition.GetNode();
				}
			}

			template<typename U>
			void operator=(const Node<U>& ruleDefinition)
			{
				typedef typename parsing_internal::ParsingNodeGetPtrElement<T>::Type TElement;
				typedef typename parsing_internal::ParsingNodeGetPtrElement<U>::Type UElement;
				if(ruleNode->node)
				{
					ruleNode->node=(Node<T>(ruleNode->node) | cast<UElement, TElement>(ruleDefinition)).GetNode();
				}
				else
				{
					ruleNode->node=cast<UElement, TElement>(ruleDefinition).GetNode();
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

		class GrammarBase : public NotCopyable
		{
		protected:
			virtual WString					GetTokenName(vint token)=0;
			void							BuildParser(const RuleNode* rootRule, stream::TextWriter* logWriter=0);
		};

		template<typename TTokenElement, typename TTokenHelper>
		class Grammar : public GrammarBase
		{
		public:
		};
	}
}

#endif