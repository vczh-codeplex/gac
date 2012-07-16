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

namespace vl
{
	namespace parsing
	{

/***********************************************************************
语法分析器基础结构
***********************************************************************/

		namespace parsing_internal
		{
			struct NullParserType
			{
			};

			class _Seq;
			class _Alt;
			class _Loop;
			class _Token;
			class _Rule;
			class _Create;
			class _Assign;
			class _Cast;
			class _Use;
			class _Transform;
		}

		class IParserNodeVisitor : public Interface
		{
		public:
			virtual void				Visit(parsing_internal::_Seq* node)=0;
			virtual void				Visit(parsing_internal::_Alt* node)=0;
			virtual void				Visit(parsing_internal::_Loop* node)=0;
			virtual void				Visit(parsing_internal::_Token* node)=0;
			virtual void				Visit(parsing_internal::_Rule* node)=0;
			virtual void				Visit(parsing_internal::_Create* node)=0;
			virtual void				Visit(parsing_internal::_Assign* node)=0;
			virtual void				Visit(parsing_internal::_Cast* node)=0;
			virtual void				Visit(parsing_internal::_Use* node)=0;
			virtual void				Visit(parsing_internal::_Transform* node)=0;
		};

		class ParserNode : public Object
		{
		public:
			virtual void				Accept(IParserNodeVisitor* visitor)=0;
		};

		class RuleNode : public Object
		{
		public:
			WString						name;
			Ptr<ParserNode>				node;
		};

/***********************************************************************
语法分析器扩展结构
***********************************************************************/

		namespace parsing_internal
		{
			class _Seq : public ParserNode
			{
			public:
				Ptr<ParserNode>			first;
				Ptr<ParserNode>			second;

				void					Accept(IParserNodeVisitor* visitor)override;
			};
			
			class _Alt : public ParserNode
			{
			public:
				Ptr<ParserNode>			first;
				Ptr<ParserNode>			second;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Loop : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Token : public ParserNode
			{
			public:
				vint					token;
				WString					name;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Rule : public ParserNode
			{
			public:
				RuleNode*				rule;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Create : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Assign : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Cast : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Use : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Transform : public ParserNode
			{
			public:
				Ptr<ParserNode>			node;

				void					Accept(IParserNodeVisitor* visitor)override;
			};
		}

/***********************************************************************
语法分析器类型规则
***********************************************************************/

		template<typename T, typename U>
		struct ParserNodeTypeMergerInternal
		{
		};

		template<typename T>
		struct ParserNodeTypeDowngrader
		{
			typedef T									Type;
		};

		template<>
		struct ParserNodeTypeDowngrader<parsing_internal::NullParserType>
		{
			typedef parsing_internal::NullParserType	Type;
		};

		template<>
		struct ParserNodeTypeMergerInternal<parsing_internal::NullParserType, parsing_internal::NullParserType>
		{
			typedef parsing_internal::NullParserType	Type;
		};

		template<typename T>
		struct ParserNodeTypeMergerInternal<parsing_internal::NullParserType, T>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParserNodeTypeMergerInternal<T, parsing_internal::NullParserType>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParserNodeTypeMergerInternal<T, T>
		{
			typedef T									Type;
		};

		template<typename T, typename U>
		struct ParserNodeTypeMerger
		{
			typedef typename ParserNodeTypeMergerInternal<
				typename ParserNodeTypeDowngrader<T>::Type,
				typename ParserNodeTypeDowngrader<U>::Type
				>::Type									Type;
		};

		template<typename T>
		struct ParserNodeTypeMerger<T, T>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParserNodeGetPtrElement
		{
		};

		template<typename T>
		struct ParserNodeGetPtrElement<Ptr<T>>
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
			Ptr<ParserNode>				node;

		public:
			Node()
			{
			}

			Node(Ptr<ParserNode> _node)
				:node(_node)
			{
			}

			Ptr<ParserNode> GetNode()const
			{
				return node;
			}
		};

/***********************************************************************
语法分析器构造函数
***********************************************************************/

		template<typename T, typename U>
		Node<typename ParserNodeTypeMerger<T, U>::Type> operator+(const Node<T>& t, const Node<U>& u)
		{
			Ptr<parsing_internal::_Seq> result=new parsing_internal::_Seq;
			result->first=t.GetNode();
			result->second=u.GetNode();
			return result;
		}

		template<typename T, typename U>
		Node<typename ParserNodeTypeMerger<T, U>::Type> operator|(const Node<T>& t, const Node<U>& u)
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
		Node<parsing_internal::NullParserType> operator~(const Node<T>& t)
		{
			return t.GetNode();
		}

		template<typename TSource>
		Node<Ptr<TSource>> create(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Create> result=new parsing_internal::_Create;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TCast>
		Node<Ptr<TCast>> create(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Create> result=new parsing_internal::_Create;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TMember>
		Node<Ptr<TSource>> assign(const Node<TMember>& t, TMember TSource::* member)
		{
			Ptr<parsing_internal::_Assign> result=new parsing_internal::_Assign;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TCast>
		Node<Ptr<TCast>> cast(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Cast> result=new parsing_internal::_Cast;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource>
		Node<Ptr<TSource>> use(const Node<Ptr<TSource>>& t)
		{
			Ptr<parsing_internal::_Use> result=new parsing_internal::_Use;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, TDestination(*transformation)(const TSource&))
		{
			Ptr<parsing_internal::_Transform> result=new parsing_internal::_Transform;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, TDestination(*transformation)(TSource))
		{
			Ptr<parsing_internal::_Transform> result=new parsing_internal::_Transform;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, const Func<TDestination(const TSource&)>& transformation)
		{
			Ptr<parsing_internal::_Transform> result=new parsing_internal::_Transform;
			result->node=t.GetNode();
			return result;
		}

		template<typename TSource, typename TDestination>
		Node<TDestination> transform(const Node<TSource>& t, const Func<TDestination(TSource)>& transformation)
		{
			Ptr<parsing_internal::_Transform> result=new parsing_internal::_Transform;
			result->node=t.GetNode();
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
					ruleNode.node=(Node<T>(ruleNode.node) | cast<typename ParserNodeGetPtrElement<U>::Type, typename ParserNodeGetPtrElement<T>::Type>(ruleDefinition)).GetNode();
				}
				else
				{
					ruleNode.node=cast<typename ParserNodeGetPtrElement<U>::Type, typename ParserNodeGetPtrElement<T>::Type>(ruleDefinition).GetNode();
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
	}
}

#endif