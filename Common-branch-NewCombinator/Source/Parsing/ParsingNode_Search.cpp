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
			
			class ContaineredNodeChecker : public Object, public IParsingNodeVisitor
			{
			protected:
				ParsingNode*					referenceNode;
				bool							found;

				void Search(ParsingNode* node)
				{
					found|=node==referenceNode;
					node->Accept(this);
				}
			public:
				ContaineredNodeChecker(ParsingNode* _referenceNode)
					:referenceNode(_referenceNode)
					,found(false)
				{
				}

				bool Check(ParsingNode* containerNode)
				{
					found=false;
					Search(containerNode);
					return found;
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					Search(node->first.Obj());
					Search(node->second.Obj());
				}

				void Visit(parsing_internal::_Alt* node)override
				{
					Search(node->first.Obj());
					Search(node->second.Obj());
				}

				void Visit(parsing_internal::_Loop* node)override
				{
					Search(node->node.Obj());
				}

				void Visit(parsing_internal::_Token* node)override
				{
				}

				void Visit(parsing_internal::_Rule* node)override
				{
				}

				void Visit(parsing_internal::_Action* node)override
				{
					Search(node->node.Obj());
				}
			};

			bool IsNodeContained(ParsingNode* containerNode, ParsingNode* referenceNode)
			{
				ContaineredNodeChecker checker(referenceNode);
				return checker.Check(containerNode);
			}

			void SearchRulesFromRule(const RuleNode* rootRule, collections::List<const RuleNode*>& rules)
			{
				RuleCollector ruleCollector;
				ruleCollector.Collect(rootRule);
				ruleCollector.Copy(rules.Wrap());
			}
		}
	}
}