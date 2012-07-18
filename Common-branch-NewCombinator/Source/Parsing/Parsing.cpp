#include "Parsing.h"
#include "ParsingNodeState.h"
#include "..\Collections\OperationForEach.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		using namespace parsing_internal;

/***********************************************************************
GrammarBase
***********************************************************************/

		void GrammarBase::BuildParser(const RuleNode* rootRule, stream::TextWriter* logWriter)
		{
			if(logWriter)
			{
				logWriter->WriteLine(L"==================================[SIMPLIFIED GRAMMAR]");
				LogGrammarFromRule(rootRule, false, *logWriter);

				logWriter->WriteLine(L"==================================[FULL GRAMMAR]");
				LogGrammarFromRule(rootRule, true, *logWriter);

				logWriter->WriteLine(L"==================================[FIRST SET]");
				List<const RuleNode*> rules;
				SearchRulesFromRule(rootRule, rules);
				FOREACH(const RuleNode*, rule, rules.Wrap())
				{
					List<vint> tokens;
					CalculateFirstSet(rule, tokens);
					logWriter->WriteString(rule->name);
					logWriter->WriteString(L": ");
					FOREACH(vint, token, tokens.Wrap())
					{
						logWriter->WriteString(GetTokenName(token));
						logWriter->WriteString(L" ");
					}
					logWriter->WriteLine(L"");
				}

				logWriter->WriteLine(L"==================================[NODE NON-DETERMINISTIC AUTOMATON]");
				ParsingNodeAutomaton na;
				CreateAutomaton(rootRule, na);

				logWriter->WriteLine(L"==================================[NODE DETERMINISTIC AUTOMATON]");
			}
		}
	}
}