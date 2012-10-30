/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Definitions

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGDEFINITIONS
#define VCZH_PARSING_PARSINGDEFINITIONS

#include "ParsingTree.h"

namespace vl
{
	namespace parsing
	{
		namespace definitions
		{

/***********************************************************************
基础结构
***********************************************************************/

			class ParsingDefinitionNode : public Object
			{
			public:
				ParsingTextRange				codeRange;
			};

/***********************************************************************
类型结构
***********************************************************************/

			class ParsingDefinitionPrimitiveType;
			class ParsingDefinitionSubType;
			class ParsingDefinitionArrayType;

			class ParsingDefinitionType : public ParsingDefinitionNode
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void				Visit(ParsingDefinitionPrimitiveType* node)=0;
					virtual void				Visit(ParsingDefinitionSubType* node)=0;
					virtual void				Visit(ParsingDefinitionArrayType* node)=0;
				};

				virtual void					Accept(IVisitor* visitor)=0;
			};

			class ParsingDefinitionPrimitiveType : public ParsingDefinitionType
			{
			public:
				WString							name;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionSubType : public ParsingDefinitionType
			{
			public:
				Ptr<ParsingDefinitionType>		parentType;
				WString							subTypeName;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionArrayType : public ParsingDefinitionType
			{
			public:
				Ptr<ParsingDefinitionType>		elementType;

				void							Accept(IVisitor* visitor)override;
			};

/***********************************************************************
数据结构
***********************************************************************/

			class ParsingDefinitionClassMemberDefinition;
			class ParsingDefinitionClassDefinition;
			class ParsingDefinitionEnumMemberDefinition;
			class ParsingDefinitionEnumDefinition;

			class ParsingDefinitionTypeDefinition : public ParsingDefinitionNode
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void				Visit(ParsingDefinitionClassMemberDefinition* node)=0;
					virtual void				Visit(ParsingDefinitionClassDefinition* node)=0;
					virtual void				Visit(ParsingDefinitionEnumMemberDefinition* node)=0;
					virtual void				Visit(ParsingDefinitionEnumDefinition* node)=0;
				};

				virtual void					Accept(IVisitor* visitor)=0;
			public:
				WString							name;
			};

			class ParsingDefinitionClassMemberDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				Ptr<ParsingDefinitionType>		type;
				WString							name;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionClassDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				typedef collections::List<Ptr<ParsingDefinitionClassMemberDefinition>>	MemberList;
				typedef collections::List<Ptr<ParsingDefinitionClassDefinition>>		TypeList;

				MemberList						members;
				TypeList						subTypes;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionEnumMemberDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				WString							name;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionEnumDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				typedef collections::List<Ptr<ParsingDefinitionEnumMemberDefinition>>	MemberList;

				MemberList						members;

				void							Accept(IVisitor* visitor)override;
			};

/***********************************************************************
文法结构
***********************************************************************/

			class ParsingDefinitionPrimitiveGrammar;
			class ParsingDefinitionSequenceGrammar;
			class ParsingDefinitionAlternativeGrammar;
			class ParsingDefinitionLoopGrammar;
			class ParsingDefinitionCreateGrammar;
			class ParsingDefinitionAssignGrammar;
			class ParsingDefinitionUseGrammar;

			class ParsingDefinitionGrammar : public ParsingDefinitionNode
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void				Visit(ParsingDefinitionPrimitiveGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionSequenceGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionAlternativeGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionLoopGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionCreateGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionAssignGrammar* node)=0;
					virtual void				Visit(ParsingDefinitionUseGrammar* node)=0;
				};

				virtual void					Accept(IVisitor* visitor)=0;
			};

			class ParsingDefinitionTokenDefinition : public ParsingDefinitionNode
			{
			public:
				WString							name;
				WString							regex;
			};

			class ParsingDefinitionRuleDefinition : public ParsingDefinitionNode
			{
			public:
				WString							name;
				Ptr<ParsingDefinitionType>		type;
				Ptr<ParsingDefinitionGrammar>	grammar;
			};

/***********************************************************************
文法规则
***********************************************************************/

			class ParsingDefinitionPrimitiveGrammar : public ParsingDefinitionGrammar
			{
			public:
				WString							name;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionSequenceGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	first;
				Ptr<ParsingDefinitionGrammar>	second;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionAlternativeGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	first;
				Ptr<ParsingDefinitionGrammar>	second;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionLoopGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	grammar;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionCreateGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	grammar;
				Ptr<ParsingDefinitionType>		type;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionAssignGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	grammar;
				WString							memberName;

				void							Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionUseGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>	grammar;

				void							Accept(IVisitor* visitor)override;
			};

/***********************************************************************
语法描述
***********************************************************************/

			class ParsingDefinition : public Object
			{
			public:
				collections::List<Ptr<ParsingDefinitionTypeDefinition>>			types;
				collections::List<Ptr<ParsingDefinitionTokenDefinition>>		tokens;
				collections::List<Ptr<ParsingDefinitionRuleDefinition>>			rules;
			};
		}
	}
}

#endif