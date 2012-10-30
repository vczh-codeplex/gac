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
		}
	}
}