#include "Parsing.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		void LogGrammarFromRule(RuleNode* rootRule, stream::TextWriter& writer)
		{
			List<RuleNode*> rules;
			SearchRulesFromRule(rootRule, rules);
		}
	}
}