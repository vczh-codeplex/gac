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
			Ptr<tabling::ParsingTable> GenerateTable(Ptr<definitions::ParsingDefinition> definition, Ptr<Automaton> jointPDA, collections::List<Ptr<ParsingError>>& errors)
			{
				vint tokenCount=definition->tokens.Count();
				vint stateCount=jointPDA->states.Count();
				List<WString> tokenRegex;
				Dictionary<ParsingSymbol*, vint> tokenIds;
				FOREACH(Ptr<ParsingDefinitionTokenDefinition>, token, definition->tokens)
				{
					ParsingSymbol* tokenSymbol=jointPDA->symbolManager->GetGlobal()->GetSubSymbolByName(token->name);
					tokenIds.Add(tokenSymbol, tokenRegex.Count()+ParsingTable::UserTokenStart);
					tokenRegex.Add(tokenSymbol->GetDescriptorString());
				}
				Ptr<ParsingTable> table=new ParsingTable(tokenRegex, tokenCount, stateCount, definition->rules.Count());

				FOREACH(ParsingSymbol*, symbol, tokenIds.Keys())
				{
					ParsingTable::TokenInfo info;
					info.name=symbol->GetName();
					info.regex=symbol->GetDescriptorString();

					vint id=tokenIds[symbol];
					table->SetTokenInfo(id, info);
				}

				FOREACH_INDEXER(ParsingDefinitionRuleDefinition*, rule, i, jointPDA->ruleInfos.Keys())
				{
					Ptr<RuleInfo> pdaRuleInfo=jointPDA->ruleInfos[rule];
					ParsingTable::RuleInfo info;
					info.name=rule->name;
					info.type=TypeToString(rule->type.Obj());
					info.rootStartState=jointPDA->states.IndexOf(pdaRuleInfo->rootRuleStartState);
					table->SetRuleInfo(i, info);
				}

				FOREACH_INDEXER(Ptr<State>, state, i, jointPDA->states)
				{
					ParsingTable::StateInfo info;
					info.ruleName=state->ownerRule->name;
					info.stateName=state->stateName;
					info.stateExpression=state->stateExpression;
					table->SetStateInfo(i, info);
				}

				FOREACH_INDEXER(Ptr<State>, state, stateIndex, jointPDA->states)
				{
					FOREACH(Transition*, transition, state->transitions)
					{
						vint tokenIndex=-1;
						switch(transition->transitionType)
						{
						case Transition::TokenBegin:
							tokenIndex=ParsingTable::TokenBegin;
							break;
						case Transition::TokenFinish:
							tokenIndex=ParsingTable::TokenFinish;
							break;
						case Transition::Symbol:
							tokenIndex=tokenIds[transition->transitionSymbol];
							break;
						}

						Ptr<ParsingTable::TransitionBag> bag=table->GetTransitionBag(stateIndex, tokenIndex);
						if(!bag)
						{
							bag=new ParsingTable::TransitionBag;
							table->SetTransitionBag(stateIndex, tokenIndex, bag);
						}

						Ptr<ParsingTable::TransitionItem> item=new ParsingTable::TransitionItem;
						bag->transitionItems.Add(item);

						FOREACH(Ptr<Action>, action, transition->actions)
						{
							ParsingTable::Instruction ins;
							switch(action->actionType)
							{
							case Action::Create:
								{
									ins.instructionType=ParsingTable::Instruction::Create;
									ins.nameParameter=action->actionSource->GetName();
								}
								break;
							case Action::Using:
								{
									ins.instructionType=ParsingTable::Instruction::Using;
								}
								break;
							case Action::Assign:
								{
									ins.instructionType=ParsingTable::Instruction::Assign;
									ins.nameParameter=action->actionSource->GetName();
								}
								break;
							case Action::Setter:
								{
									ins.instructionType=ParsingTable::Instruction::Setter;
									ins.nameParameter=action->actionSource->GetName();
									ins.value=action->actionTarget->GetName();
								}
								break;
							case Action::Shift:
								{
									ins.instructionType=ParsingTable::Instruction::Shift;
									ins.stateParameter=jointPDA->states.IndexOf(action->shiftReduceSource);
								}
								break;
							case Action::Reduce:
								{
									ins.instructionType=ParsingTable::Instruction::Reduce;
									ins.stateParameter=jointPDA->states.IndexOf(action->shiftReduceSource);
								}
								break;
							case Action::LeftRecursiveReduce:
								{
									ins.instructionType=ParsingTable::Instruction::LeftRecursiveReduce;
									ins.stateParameter=jointPDA->states.IndexOf(action->shiftReduceSource);
								}
								break;
							}
							item->instructions.Add(ins);
						}
					}
				}

				for(vint i=0;i<table->GetStateCount();i++)
				{
					for(vint j=0;j<table->GetTokenCount();j++)
					{
						Ptr<ParsingTable::TransitionBag> bag=table->GetTransitionBag(i, j);
						if(bag)
						{
							CopyFrom(bag->transitionItems, bag->transitionItems>>OrderBy(ParsingTable::TransitionItem::Compare));
							for(vint k=0;k<bag->transitionItems.Count()-1;k++)
							{
								Ptr<ParsingTable::TransitionItem> t1=bag->transitionItems[k];
								Ptr<ParsingTable::TransitionItem> t2=bag->transitionItems[k+1];
								if(ParsingTable::TransitionItem::CheckOrder(t1, t2, false)==ParsingTable::TransitionItem::UnknownOrder)
								{
									// print error
								}
							}
						}
					}
				}

				return table;
			}
		}
	}
}