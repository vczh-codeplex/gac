#include "ParsingAutomaton.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		using namespace collections;
		using namespace definitions;

		namespace analyzing
		{
/***********************************************************************
RemoveEpsilonTransitions
***********************************************************************/

			Ptr<Automaton> CreateJointPDAFromNondeterministicPDA(Ptr<Automaton> nondeterministicPDA)
			{
				Ptr<Automaton> automaton=new Automaton(nondeterministicPDA->symbolManager);
				return automaton;
			}
		}
	}
}