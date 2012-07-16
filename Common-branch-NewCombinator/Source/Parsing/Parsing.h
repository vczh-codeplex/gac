/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Interface

Classes:
	a + b
	a | b
	*a
	token
	rule_reference
	create<source, cast_as>
	assign<source, member>
	use<source, cast_as>
	transform<source, destination>
***********************************************************************/

#ifndef VCZH_PARSING_PARSING
#define VCZH_PARSING_PARSING

#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\List.h"
#include "..\Collections\Dictionary.h"

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
			virtual void				Visit(parsing_internal::_Use* node)=0;
			virtual void				Visit(parsing_internal::_Transform* node)=0;
		};

		class ParserNode : public Object
		{
		public:
			virtual void				Accept(IParserNodeVisitor* visitor)=0;
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

				void					Accept(IParserNodeVisitor* visitor)override;
			};

			class _Rule : public ParserNode
			{
			public:
				vint					rule;

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
		struct ParserNodeTypeMerger
		{
		};

		template<>
		struct ParserNodeTypeMerger<parsing_internal::NullParserType, parsing_internal::NullParserType>
		{
			typedef parsing_internal::NullParserType	Type;
		};

		template<typename T>
		struct ParserNodeTypeMerger<parsing_internal::NullParserType, T>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParserNodeTypeMerger<T, parsing_internal::NullParserType>
		{
			typedef T									Type;
		};

		template<typename T>
		struct ParserNodeTypeMerger<T, T>
		{
			typedef T									Type;
		};

/***********************************************************************
语法分析器构造器
***********************************************************************/

		template<typename T>
		class ParserBuilder : public Object
		{
		public:
		};
	}
}

#endif