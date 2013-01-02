/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
���ͽṹ
***********************************************************************/

			class ParsingDefinitionPrimitiveType;
			class ParsingDefinitionTokenType;
			class ParsingDefinitionSubType;
			class ParsingDefinitionArrayType;

			class ParsingDefinitionType : public ParsingTreeCustomBase
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void								Visit(ParsingDefinitionPrimitiveType* node)=0;
					virtual void								Visit(ParsingDefinitionTokenType* node)=0;
					virtual void								Visit(ParsingDefinitionSubType* node)=0;
					virtual void								Visit(ParsingDefinitionArrayType* node)=0;
				};

				virtual void									Accept(IVisitor* visitor)=0;
			};

			class ParsingDefinitionPrimitiveType : public ParsingDefinitionType
			{
			public:
				WString											name;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionTokenType : public ParsingDefinitionType
			{
			public:

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionSubType : public ParsingDefinitionType
			{
			public:
				Ptr<ParsingDefinitionType>						parentType;
				WString											subTypeName;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionArrayType : public ParsingDefinitionType
			{
			public:
				Ptr<ParsingDefinitionType>						elementType;

				void											Accept(IVisitor* visitor)override;
			};

/***********************************************************************
���ݽṹ
***********************************************************************/

			class ParsingDefinitionClassMemberDefinition;
			class ParsingDefinitionClassDefinition;
			class ParsingDefinitionEnumMemberDefinition;
			class ParsingDefinitionEnumDefinition;

			class ParsingDefinitionTypeDefinition : public ParsingTreeCustomBase
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void								Visit(ParsingDefinitionClassMemberDefinition* node)=0;
					virtual void								Visit(ParsingDefinitionClassDefinition* node)=0;
					virtual void								Visit(ParsingDefinitionEnumMemberDefinition* node)=0;
					virtual void								Visit(ParsingDefinitionEnumDefinition* node)=0;
				};

				virtual void									Accept(IVisitor* visitor)=0;
			public:
				WString											name;
			};

			class ParsingDefinitionClassMemberDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				Ptr<ParsingDefinitionType>						type;
				WString											name;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionClassDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				typedef collections::List<Ptr<ParsingDefinitionClassMemberDefinition>>	MemberList;
				typedef collections::List<Ptr<ParsingDefinitionTypeDefinition>>			TypeList;

				Ptr<ParsingDefinitionType>						parentType;
				MemberList										members;
				TypeList										subTypes;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionEnumMemberDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				WString											name;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionEnumDefinition : public ParsingDefinitionTypeDefinition
			{
			public:
				typedef collections::List<Ptr<ParsingDefinitionEnumMemberDefinition>>	MemberList;

				MemberList										members;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionPrimitiveGrammar;
			class ParsingDefinitionTextGrammar;
			class ParsingDefinitionSequenceGrammar;
			class ParsingDefinitionAlternativeGrammar;
			class ParsingDefinitionLoopGrammar;
			class ParsingDefinitionOptionalGrammar;
			class ParsingDefinitionCreateGrammar;
			class ParsingDefinitionAssignGrammar;
			class ParsingDefinitionUseGrammar;
			class ParsingDefinitionSetterGrammar;

			class ParsingDefinitionGrammar : public ParsingTreeCustomBase
			{
			public:
				class IVisitor : public Interface
				{
				public:
					virtual void								Visit(ParsingDefinitionPrimitiveGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionTextGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionSequenceGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionAlternativeGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionLoopGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionOptionalGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionCreateGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionAssignGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionUseGrammar* node)=0;
					virtual void								Visit(ParsingDefinitionSetterGrammar* node)=0;
				};

				virtual void									Accept(IVisitor* visitor)=0;
			};

/***********************************************************************
�ķ�����
***********************************************************************/

			class ParsingDefinitionPrimitiveGrammar : public ParsingDefinitionGrammar
			{
			public:
				WString											name;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionTextGrammar : public ParsingDefinitionGrammar
			{
			public:
				WString											text;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionSequenceGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					first;
				Ptr<ParsingDefinitionGrammar>					second;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionAlternativeGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					first;
				Ptr<ParsingDefinitionGrammar>					second;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionLoopGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionOptionalGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionCreateGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;
				Ptr<ParsingDefinitionType>						type;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionAssignGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;
				WString											memberName;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionUseGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;

				void											Accept(IVisitor* visitor)override;
			};

			class ParsingDefinitionSetterGrammar : public ParsingDefinitionGrammar
			{
			public:
				Ptr<ParsingDefinitionGrammar>					grammar;
				WString											memberName;
				WString											value;

				void											Accept(IVisitor* visitor)override;
			};

/***********************************************************************
�ķ��ṹ
***********************************************************************/

			class ParsingDefinitionTokenDefinition : public ParsingTreeCustomBase
			{
			public:
				WString											name;
				WString											regex;
				bool											discard;
			};

			class ParsingDefinitionRuleDefinition : public ParsingTreeCustomBase
			{
			public:
				WString															name;
				Ptr<ParsingDefinitionType>										type;
				collections::List<Ptr<ParsingDefinitionGrammar>>				grammars;
			};

			class ParsingDefinition : public ParsingTreeCustomBase
			{
			public:
				collections::List<Ptr<ParsingDefinitionTypeDefinition>>			types;
				collections::List<Ptr<ParsingDefinitionTokenDefinition>>		tokens;
				collections::List<Ptr<ParsingDefinitionRuleDefinition>>			rules;
			};

/***********************************************************************
�����������ͣ�
***********************************************************************/

			class ParsingDefinitionTypeWriter : public Object
			{
				friend ParsingDefinitionTypeWriter				Type(const WString& name);
				friend ParsingDefinitionTypeWriter				TokenType();
			protected:
				Ptr<ParsingDefinitionType>						type;

				ParsingDefinitionTypeWriter(Ptr<ParsingDefinitionType> internalType);
				ParsingDefinitionTypeWriter(const WString& name);
			public:
				ParsingDefinitionTypeWriter(const ParsingDefinitionTypeWriter& typeWriter);

				ParsingDefinitionTypeWriter						Sub(const WString& subTypeName)const;
				ParsingDefinitionTypeWriter						Array()const;
				Ptr<ParsingDefinitionType>						Type()const;
			};

			extern ParsingDefinitionTypeWriter					Type(const WString& name);
			extern ParsingDefinitionTypeWriter					TokenType();

/***********************************************************************
�����������Ͷ��壩
***********************************************************************/

			class ParsingDefinitionTypeDefinitionWriter : public Object
			{
			public:
				virtual Ptr<ParsingDefinitionTypeDefinition>	Definition()const=0;
			};

			class ParsingDefinitionClassDefinitionWriter : public ParsingDefinitionTypeDefinitionWriter
			{
			protected:
				Ptr<ParsingDefinitionClassDefinition>			definition;

			public:
				ParsingDefinitionClassDefinitionWriter(const WString& name);
				ParsingDefinitionClassDefinitionWriter(const WString& name, const ParsingDefinitionTypeWriter& parentType);

				ParsingDefinitionClassDefinitionWriter&			Member(const WString& name, const ParsingDefinitionTypeWriter& type);
				ParsingDefinitionClassDefinitionWriter&			SubType(const ParsingDefinitionTypeDefinitionWriter& type);

				Ptr<ParsingDefinitionTypeDefinition>			Definition()const override;
			};
			
			extern ParsingDefinitionClassDefinitionWriter		Class(const WString& name);
			extern ParsingDefinitionClassDefinitionWriter		Class(const WString& name, const ParsingDefinitionTypeWriter& parentType);

			class ParsingDefinitionEnumDefinitionWriter : public ParsingDefinitionTypeDefinitionWriter
			{
			protected:
				Ptr<ParsingDefinitionEnumDefinition>			definition;

			public:
				ParsingDefinitionEnumDefinitionWriter(const WString& name);

				ParsingDefinitionEnumDefinitionWriter&			Member(const WString& name);

				Ptr<ParsingDefinitionTypeDefinition>			Definition()const override;
			};

			extern ParsingDefinitionEnumDefinitionWriter		Enum(const WString& name);

/***********************************************************************
���������ķ�����
***********************************************************************/

			class ParsingDefinitionGrammarWriter : public Object
			{
				friend ParsingDefinitionGrammarWriter			Rule(const WString& name);
				friend ParsingDefinitionGrammarWriter			Text(const WString& name);
				friend ParsingDefinitionGrammarWriter			Opt(const ParsingDefinitionGrammarWriter& writer);
			protected:
				Ptr<ParsingDefinitionGrammar>					grammar;

				ParsingDefinitionGrammarWriter(Ptr<ParsingDefinitionGrammar> internalGrammar);
			public:
				ParsingDefinitionGrammarWriter(const ParsingDefinitionGrammarWriter& grammarWriter);

				ParsingDefinitionGrammarWriter					operator+(const ParsingDefinitionGrammarWriter& next)const;
				ParsingDefinitionGrammarWriter					operator|(const ParsingDefinitionGrammarWriter& next)const;
				ParsingDefinitionGrammarWriter					operator*()const;
				ParsingDefinitionGrammarWriter					As(const ParsingDefinitionTypeWriter& type)const;
				ParsingDefinitionGrammarWriter					operator[](const WString& memberName)const;
				ParsingDefinitionGrammarWriter					operator!()const;
				ParsingDefinitionGrammarWriter					Set(const WString& memberName, const WString& value)const;

				Ptr<ParsingDefinitionGrammar>					Grammar()const;
			};

			extern ParsingDefinitionGrammarWriter				Rule(const WString& name);
			extern ParsingDefinitionGrammarWriter				Text(const WString& text);
			extern ParsingDefinitionGrammarWriter				Opt(const ParsingDefinitionGrammarWriter& writer);

/***********************************************************************
���������ķ��ṹ��
***********************************************************************/

			class ParsingDefinitionWriter;

			class ParsingDefinitionRuleDefinitionWriter : public Object
			{
			protected:
				Ptr<ParsingDefinitionRuleDefinition>			rule;
				ParsingDefinitionWriter&						owner;
			public:
				ParsingDefinitionRuleDefinitionWriter(ParsingDefinitionWriter& _owner, Ptr<ParsingDefinitionRuleDefinition> _rule);

				ParsingDefinitionRuleDefinitionWriter&			Imply(const ParsingDefinitionGrammarWriter& grammar);
				ParsingDefinitionWriter&						EndRule();
			};

			class ParsingDefinitionWriter : public Object
			{
			protected:
				Ptr<ParsingDefinition>							definition;

			public:
				ParsingDefinitionWriter();

				ParsingDefinitionWriter&						Type(const ParsingDefinitionTypeDefinitionWriter& type);
				ParsingDefinitionWriter&						Token(const WString& name, const WString& regex);
				ParsingDefinitionWriter&						Discard(const WString& name, const WString& regex);
				ParsingDefinitionRuleDefinitionWriter			Rule(const WString& name, const ParsingDefinitionTypeWriter& type);

				Ptr<ParsingDefinition>							Definition()const;
			};

/***********************************************************************
��������
***********************************************************************/

			extern WString										TypeToString(ParsingDefinitionType* type);
			extern WString										GrammarToString(ParsingDefinitionGrammar* grammar);
			extern WString										GrammarStateToString(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode);
			extern ParsingDefinitionGrammar*					FindAppropriateGrammarState(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode);
			extern void											Log(Ptr<ParsingDefinition> definition, stream::TextWriter& writer);

/***********************************************************************
�Ծ�
***********************************************************************/
			
			extern Ptr<ParsingDefinition>						CreateParserDefinition();
			extern Ptr<ParsingDefinition>						DeserializeDefinition(Ptr<ParsingTreeNode> node);
		}
	}
}

#endif