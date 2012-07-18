#include "ParsingNode.h"
#include "..\Collections\OperationCopyFrom.h"

namespace vl
{
	using namespace collections;

	namespace parsing
	{
		namespace parsing_internal
		{
			class FirstSetCalculator : public Object, public IParsingNodeVisitor
			{
			protected:
				SortedList<vint>					tokens;
				SortedList<const RuleNode*>			visitedRules;

				void AddToken(vint token)
				{
					vint index=tokens.IndexOf(token);
					if(index==-1)
					{
						tokens.Add(token);
					}
				}
			public:
				void Copy(ICollection<vint>& outputTokens)
				{
					CopyFrom(outputTokens, tokens.Wrap());
				}

				void CalculateFirstSet(const RuleNode* rootRule)
				{
					vint index=visitedRules.IndexOf(rootRule);
					if(index==-1)
					{
						visitedRules.Add(rootRule);
						rootRule->node->Accept(this);
					}
				}

				void Visit(parsing_internal::_Seq* node)override
				{
					node->first->Accept(this);
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
					AddToken(node->token);
				}

				void Visit(parsing_internal::_Rule* node)override
				{
					CalculateFirstSet(node->rule);
				}

				void Visit(parsing_internal::_Action* node)override
				{
					node->node->Accept(this);
				}
			};

			void CalculateFirstSet(const RuleNode* rootRule, collections::List<vint>& tokens)
			{
				FirstSetCalculator firstSetCalculator;
				firstSetCalculator.CalculateFirstSet(rootRule);
				firstSetCalculator.Copy(tokens.Wrap());
			}
		}
	}
}