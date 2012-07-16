#include "Parsing.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		class RuleCollector : public Object, public IParsingNodeVisitor
		{
		protected:
			SortedList<RuleNode*>		rules;

		public:
			void Copy(collections::ICollection<RuleNode*>& outputRules)
			{
				CopyFrom(outputRules, rules.Wrap());
			}

			void Collect(RuleNode* rootRule)
			{
				vint index=rules.IndexOf(rootRule);
				if(index==-1)
				{
					rules.Add(rootRule);
					rootRule->node->Accept(this);
				}
			}

			void Visit(parsing_internal::_Seq* node)
			{
				node->first->Accept(this);
				node->second->Accept(this);
			}

			void Visit(parsing_internal::_Alt* node)
			{
				node->first->Accept(this);
				node->second->Accept(this);
			}

			void Visit(parsing_internal::_Loop* node)
			{
				node->node->Accept(this);
			}

			void Visit(parsing_internal::_Token* node)
			{
			}

			void Visit(parsing_internal::_Rule* node)
			{
				Collect(node->rule);
			}

			void Visit(parsing_internal::_Action* node)
			{
				node->node->Accept(this);
			}
		};

		void SearchRulesFromRule(RuleNode* rootRule, collections::List<RuleNode*>& rules)
		{
			RuleCollector ruleCollector;
			ruleCollector.Collect(rootRule);
			ruleCollector.Copy(rules.Wrap());
		}
	}
}