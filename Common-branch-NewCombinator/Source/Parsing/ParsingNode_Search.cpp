#include "ParsingNode.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		namespace parsing_internal
		{
			class RuleCollector : public Object, public IParsingNodeVisitor
			{
			protected:
				SortedList<const RuleNode*>		rules;

			public:
				void Copy(collections::ICollection<const RuleNode*>& outputRules)
				{
					CopyFrom(outputRules, rules.Wrap());
				}

				void Collect(const RuleNode* rootRule)
				{
					vint index=rules.IndexOf(rootRule);
					if(index==-1)
					{
						rules.Add(rootRule);
						rootRule->node->Accept(this);
					}
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					node->first->Accept(this);
					node->second->Accept(this);
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					node->first->Accept(this);
					node->second->Accept(this);
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					node->node->Accept(this);
				}

				void Visit(parsing_internal::_Token* node)override
				{
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					Collect(node->rule);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					node->node->Accept(this);
				}
			};

			void SearchRulesFromRule(const RuleNode* rootRule, collections::List<const RuleNode*>& rules)
			{
				RuleCollector ruleCollector;
				ruleCollector.Collect(rootRule);
				ruleCollector.Copy(rules.Wrap());
			}
		}
	}
}