#include "ParsingDefinitions.h"
#include "..\Collections\Operation.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		namespace definitions
		{

/***********************************************************************
ParsingDefinitionType(Visitor)
***********************************************************************/

			void ParsingDefinitionPrimitiveType::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionTokenType::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionSubType::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionArrayType::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
ParsingDefinitionTypeDefinition(Visitor)
***********************************************************************/

			void ParsingDefinitionClassMemberDefinition::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionClassDefinition::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionEnumMemberDefinition::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionEnumDefinition::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
ParsingDefinitionGrammar(Visitor)
***********************************************************************/

			void ParsingDefinitionPrimitiveGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionTextGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionSequenceGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionAlternativeGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionLoopGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionOptionalGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionCreateGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionAssignGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionUseGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

			void ParsingDefinitionSetterGrammar::Accept(IVisitor* visitor)
			{
				visitor->Visit(this);
			}

/***********************************************************************
ParsingDefinitionTypeWriter
***********************************************************************/

			ParsingDefinitionTypeWriter::ParsingDefinitionTypeWriter(Ptr<ParsingDefinitionType> internalType)
			{
				type=internalType;
			}

			ParsingDefinitionTypeWriter::ParsingDefinitionTypeWriter(const ParsingDefinitionTypeWriter& typeWriter)
			{
				type=typeWriter.type;
			}

			ParsingDefinitionTypeWriter::ParsingDefinitionTypeWriter(const WString& name)
			{
				Ptr<ParsingDefinitionPrimitiveType> primitiveType=new ParsingDefinitionPrimitiveType;
				primitiveType->name=name;
				type=primitiveType;
			}

			ParsingDefinitionTypeWriter ParsingDefinitionTypeWriter::Sub(const WString& subTypeName)const
			{
				Ptr<ParsingDefinitionSubType> subType=new ParsingDefinitionSubType;
				subType->parentType=type;
				subType->subTypeName=subTypeName;
				return subType;
			}

			ParsingDefinitionTypeWriter ParsingDefinitionTypeWriter::Array()const
			{
				Ptr<ParsingDefinitionArrayType> arrayType=new ParsingDefinitionArrayType;
				arrayType->elementType=type;
				return arrayType;
			}

			Ptr<ParsingDefinitionType> ParsingDefinitionTypeWriter::Type()const
			{
				return type;
			}

			ParsingDefinitionTypeWriter Type(const WString& name)
			{
				return ParsingDefinitionTypeWriter(name);
			}

			ParsingDefinitionTypeWriter TokenType()
			{
				Ptr<ParsingDefinitionTokenType> type=new ParsingDefinitionTokenType;
				return type;
			}

/***********************************************************************
ParsingDefinitionClassDefinitionWriter
***********************************************************************/

			ParsingDefinitionClassDefinitionWriter::ParsingDefinitionClassDefinitionWriter(const WString& name, const WString& parentType)
			{
				definition=new ParsingDefinitionClassDefinition;
				definition->name=name;
				definition->parentType=parentType;
			}

			ParsingDefinitionClassDefinitionWriter& ParsingDefinitionClassDefinitionWriter::Member(const WString& name, const ParsingDefinitionTypeWriter& type)
			{
				Ptr<ParsingDefinitionClassMemberDefinition> member=new ParsingDefinitionClassMemberDefinition;
				member->name=name;
				member->type=type.Type();
				definition->members.Add(member);
				return *this;
			}

			ParsingDefinitionClassDefinitionWriter& ParsingDefinitionClassDefinitionWriter::SubType(const ParsingDefinitionTypeDefinitionWriter& type)
			{
				definition->subTypes.Add(type.Definition());
				return *this;
			}

			Ptr<ParsingDefinitionTypeDefinition> ParsingDefinitionClassDefinitionWriter::Definition()const
			{
				return definition;
			}

			ParsingDefinitionClassDefinitionWriter Class(const WString& name, const WString& parentType)
			{
				return ParsingDefinitionClassDefinitionWriter(name, parentType);
			}

/***********************************************************************
ParsingDefinitionEnumDefinitionWriter
***********************************************************************/

			ParsingDefinitionEnumDefinitionWriter::ParsingDefinitionEnumDefinitionWriter(const WString& name)
			{
				definition=new ParsingDefinitionEnumDefinition;
				definition->name=name;
			}

			ParsingDefinitionEnumDefinitionWriter& ParsingDefinitionEnumDefinitionWriter::Member(const WString& name)
			{
				Ptr<ParsingDefinitionEnumMemberDefinition> member=new ParsingDefinitionEnumMemberDefinition;
				member->name=name;
				definition->members.Add(member);
				return *this;
			}

			Ptr<ParsingDefinitionTypeDefinition> ParsingDefinitionEnumDefinitionWriter::Definition()const
			{
				return definition;
			}

			ParsingDefinitionEnumDefinitionWriter Enum(const WString& name)
			{
				return ParsingDefinitionEnumDefinitionWriter(name);
			}

/***********************************************************************
ParsingDefinitionGrammarWriter
***********************************************************************/

			ParsingDefinitionGrammarWriter::ParsingDefinitionGrammarWriter(Ptr<ParsingDefinitionGrammar> internalGrammar)
			{
				grammar=internalGrammar;
			}

			ParsingDefinitionGrammarWriter::ParsingDefinitionGrammarWriter(const ParsingDefinitionGrammarWriter& grammarWriter)
			{
				grammar=grammarWriter.grammar;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::operator+(const ParsingDefinitionGrammarWriter& next)const
			{
				Ptr<ParsingDefinitionSequenceGrammar> sequence=new ParsingDefinitionSequenceGrammar;
				sequence->first=grammar;
				sequence->second=next.Grammar();
				return sequence;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::operator|(const ParsingDefinitionGrammarWriter& next)const
			{
				Ptr<ParsingDefinitionAlternativeGrammar> alternative=new ParsingDefinitionAlternativeGrammar;
				alternative->first=grammar;
				alternative->second=next.Grammar();
				return alternative;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::operator*()const
			{
				Ptr<ParsingDefinitionLoopGrammar> loop=new ParsingDefinitionLoopGrammar;
				loop->grammar=grammar;
				return loop;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::As(const ParsingDefinitionTypeWriter& type)const
			{
				Ptr<ParsingDefinitionCreateGrammar> create=new ParsingDefinitionCreateGrammar;
				create->grammar=grammar;
				create->type=type.Type();
				return create;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::operator[](const WString& memberName)const
			{
				Ptr<ParsingDefinitionAssignGrammar> assign=new ParsingDefinitionAssignGrammar;
				assign->grammar=grammar;
				assign->memberName=memberName;
				return assign;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::operator!()const
			{
				Ptr<ParsingDefinitionUseGrammar> use=new ParsingDefinitionUseGrammar;
				use->grammar=grammar;
				return use;
			}

			ParsingDefinitionGrammarWriter ParsingDefinitionGrammarWriter::Set(const WString& memberName, const WString& value)const
			{
				Ptr<ParsingDefinitionSetterGrammar> setter=new ParsingDefinitionSetterGrammar;
				setter->grammar=grammar;
				setter->memberName=memberName;
				setter->value=value;
				return setter;
			}

			Ptr<ParsingDefinitionGrammar> ParsingDefinitionGrammarWriter::Grammar()const
			{
				return grammar;
			}

			ParsingDefinitionGrammarWriter Rule(const WString& name)
			{
				Ptr<ParsingDefinitionPrimitiveGrammar> grammar=new ParsingDefinitionPrimitiveGrammar;
				grammar->name=name;
				return grammar;
			}

			ParsingDefinitionGrammarWriter Text(const WString& text)
			{
				Ptr<ParsingDefinitionTextGrammar> grammar=new ParsingDefinitionTextGrammar;
				grammar->text=text;
				return grammar;
			}

			ParsingDefinitionGrammarWriter Opt(const ParsingDefinitionGrammarWriter& writer)
			{
				Ptr<ParsingDefinitionOptionalGrammar> grammar=new ParsingDefinitionOptionalGrammar;
				grammar->grammar=writer.Grammar();
				return grammar;
			}

/***********************************************************************
ParsingDefinitionRuleDefinitionWriter
***********************************************************************/

			ParsingDefinitionRuleDefinitionWriter::ParsingDefinitionRuleDefinitionWriter(ParsingDefinitionWriter& _owner, Ptr<ParsingDefinitionRuleDefinition> _rule)
				:owner(_owner)
				,rule(_rule)
			{
			}

			ParsingDefinitionRuleDefinitionWriter& ParsingDefinitionRuleDefinitionWriter::Imply(const ParsingDefinitionGrammarWriter& grammar)
			{
				rule->grammars.Add(grammar.Grammar());
				return *this;
			}

			ParsingDefinitionWriter& ParsingDefinitionRuleDefinitionWriter::EndRule()
			{
				return owner;
			}

/***********************************************************************
ParsingDefinitionWriter
***********************************************************************/

			ParsingDefinitionWriter::ParsingDefinitionWriter()
			{
				definition=new ParsingDefinition;
			}

			ParsingDefinitionWriter& ParsingDefinitionWriter::Type(const ParsingDefinitionTypeDefinitionWriter& type)
			{
				definition->types.Add(type.Definition());
				return *this;
			}

			ParsingDefinitionWriter& ParsingDefinitionWriter::Token(const WString& name, const WString& regex)
			{
				Ptr<ParsingDefinitionTokenDefinition> token=new ParsingDefinitionTokenDefinition;
				token->name=name;
				token->regex=regex;
				definition->tokens.Add(token);
				return *this;
			}

			ParsingDefinitionRuleDefinitionWriter ParsingDefinitionWriter::Rule(const WString& name, const ParsingDefinitionTypeWriter& type)
			{
				Ptr<ParsingDefinitionRuleDefinition> rule=new ParsingDefinitionRuleDefinition;
				rule->name=name;
				rule->type=type.Type();
				definition->rules.Add(rule);
				return ParsingDefinitionRuleDefinitionWriter(*this, rule);
			}

			Ptr<ParsingDefinition> ParsingDefinitionWriter::Definition()const
			{
				return definition;
			}
		}
	}
}