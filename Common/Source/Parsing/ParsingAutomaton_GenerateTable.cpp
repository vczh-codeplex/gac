#include "ParsingAutomaton.h"
#include "..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		using namespace collections;
		using namespace definitions;
		using namespace tabling;

		namespace analyzing
		{
			Ptr<tabling::ParsingTable> GenerateTable(Ptr<Automaton> jointPDA, collections::List<Ptr<ParsingError>>& errors)
			{
				return 0;
			}
		}
	}
}