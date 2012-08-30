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
			
			class RuleReferenceNodeChecker : public Object, public IParsingNodeVisitor
			{
			protected:
				ParsingNode*					referenceNode;
				bool							found;

				void Search(Ptr<ParsingNode> node)
				{
					found|=node==referenceNode;
					node->Accept(this);
				}
			public:
				RuleReferenceNodeChecker(ParsingNode* _referenceNode)
					:referenceNode(_referenceNode)
					,found(false)
				{
				}

				bool Check(const RuleNode* rootRule)
				{
					found=false;
					Search(rootRule->node);
					return found;
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					Search(node->first);
					Search(node->second);
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					Search(node->first);
					Search(node->second);
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					Search(node->node);
				}

				void Visit(parsing_internal::_Token* node)override
				{
				}

				void Visit(parsing_internal::_Rule* node)override
				{
				}

				void Visit(parsing_internal::_Action* node)override
				{
					Search(node->node);
				}
			};

			void SearchRulesFromRule(const RuleNode* rootRule, collections::List<const RuleNode*>& rules, ParsingNode* referenceNode)
			{
				RuleCollector ruleCollector;
				ruleCollector.Collect(rootRule);
				ruleCollector.Copy(rules.Wrap());
				if(referenceNode)
				{
					RuleReferenceNodeChecker checker(referenceNode);
					for(vint i=rules.Count()-1;i>=0;i--)
					{
						if(!checker.Check(rules[i]))
						{
							rules.RemoveAt(i);
						}
					}
				}
			}
		}
	}
}