#include "Parsing.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
GrammarBase
***********************************************************************/

		void GrammarBase::BuildParser(const RuleNode* rootRule, stream::TextWriter* logWriter)
		{
			if(logWriter)
			{
				logWriter->WriteLine(L"[SIMPLIFIED GRAMMAR]");
				LogGrammarFromRule(rootRule, false, *logWriter);
				logWriter->WriteLine(L"[FULL GRAMMAR]");
				LogGrammarFromRule(rootRule, true, *logWriter);
			}
		}
	}
}