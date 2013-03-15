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
			WString GetTypeNameForCreateInstruction(ParsingSymbol* type)
			{
				ParsingSymbol* parent=type->GetParentSymbol();
				if(parent->GetType()==ParsingSymbol::ClassType)
				{
					return GetTypeNameForCreateInstruction(parent)+L"."+type->GetName();
				}
				else
				{
					return type->GetName();
				}
			}

			Ptr<tabling::ParsingTable> GenerateTable(Ptr<definitions::ParsingDefinition> definition, Ptr<Automaton> jointPDA, collections::List<Ptr<ParsingError>>& errors)
			{
				Dictionary<ParsingSymbol*, vint> tokenIds;
				List<WString> discardTokens;
				List<State*> stateIds;
				vint availableStateCount=0;
				{
					vint currentState=0;
					List<State*> scanningStates;
					FOREACH(Ptr<RuleInfo>, ruleInfo, jointPDA->ruleInfos.Values())
					{
						if(!scanningStates.Contains(ruleInfo->rootRuleStartState))
						{
							scanningStates.Add(ruleInfo->rootRuleStartState);
						}
						
						while(currentState<scanningStates.Count())
						{
							State* state=scanningStates[currentState++];
							stateIds.Add(state);

							FOREACH(Transition*, transition, state->transitions)
							{
								if(!scanningStates.Contains(transition->target))
								{
									scanningStates.Add(transition->target);
								}
							}
						}
					}
					availableStateCount=scanningStates.Count();
				}

				// there will be some states that is used in shift and reduce but it is not a reachable state
				// so the state table will record all state
				FOREACH(Ptr<State>, state, jointPDA->states)
				{
					if(!stateIds.Contains(state.Obj()))
					{
						stateIds.Add(state.Obj());
					}
				}

				vint tokenCount=0;
				vint discardTokenCount=0;
				vint stateCount=stateIds.Count();

				FOREACH(Ptr<ParsingDefinitionTokenDefinition>, token, definition->tokens)
				{
					if(token->discard)
					{
						discardTokens.Add(token->name);
						discardTokenCount++;
					}
					else
					{
						ParsingSymbol* tokenSymbol=jointPDA->symbolManager->GetGlobal()->GetSubSymbolByName(token->name);
						tokenIds.Add(tokenSymbol, tokenIds.Count()+ParsingTable::UserTokenStart);
						tokenCount++;
					}
				}
				Ptr<ParsingTable> table=new ParsingTable(tokenCount, discardTokenCount, stateCount, definition->rules.Count());

				FOREACH(ParsingSymbol*, symbol, tokenIds.Keys())
				{
					ParsingTable::TokenInfo info;
					info.name=symbol->GetName();
					info.regex=symbol->GetDescriptorString();

					vint id=tokenIds[symbol];
					table->SetTokenInfo(id, info);
				}

				FOREACH_INDEXER(WString, name, i, discardTokens)
				{
					ParsingSymbol* symbol=jointPDA->symbolManager->GetGlobal()->GetSubSymbolByName(name);

					ParsingTable::TokenInfo info;
					info.name=symbol->GetName();
					info.regex=symbol->GetDescriptorString();
					table->SetDiscardTokenInfo(i, info);
				}

				FOREACH_INDEXER(ParsingDefinitionRuleDefinition*, rule, i, jointPDA->ruleInfos.Keys())
				{
					Ptr<RuleInfo> pdaRuleInfo=jointPDA->ruleInfos[rule];
					ParsingTable::RuleInfo info;
					info.name=rule->name;
					info.type=TypeToString(rule->type.Obj());
					info.rootStartState=stateIds.IndexOf(pdaRuleInfo->rootRuleStartState);
					table->SetRuleInfo(i, info);
				}

				FOREACH_INDEXER(State*, state, i, stateIds)
				{
					ParsingTable::StateInfo info;
					info.ruleName=state->ownerRule->name;
					info.stateName=state->stateName;
					info.stateExpression=state->stateExpression;
					table->SetStateInfo(i, info);
				}

				FOREACH_INDEXER(State*, state, stateIndex, stateIds)
				{
					// if this state is not necessary, stop building the table
					if(stateIndex>=availableStateCount) break;

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
						case Transition::TryReduce:
							tokenIndex=ParsingTable::TryReduce;
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
						item->token=tokenIndex;
						item->targetState=stateIds.IndexOf(transition->target);
						bag->transitionItems.Add(item);

						FOREACH(Ptr<Action>, action, transition->actions)
						{
							ParsingTable::Instruction ins;
							switch(action->actionType)
							{
							case Action::Create:
								{
									ins.instructionType=ParsingTable::Instruction::Create;
									ins.nameParameter=GetTypeNameForCreateInstruction(action->actionSource);
								}
								break;
							case Action::Using:
								{
									ins.instructionType=ParsingTable::Instruction::Using;
								}
								break;
							case Action::Assign:
								{
									if(action->actionSource->GetDescriptorSymbol()->GetType()==ParsingSymbol::ArrayType)
									{
										ins.instructionType=ParsingTable::Instruction::Item;
									}
									else
									{
										ins.instructionType=ParsingTable::Instruction::Assign;
									}
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
									ins.stateParameter=stateIds.IndexOf(action->shiftReduceSource);
								}
								break;
							case Action::Reduce:
								{
									ins.instructionType=ParsingTable::Instruction::Reduce;
									ins.stateParameter=stateIds.IndexOf(action->shiftReduceSource);
									item->stackPattern.Add(ins.stateParameter);
								}
								break;
							case Action::LeftRecursiveReduce:
								{
									ins.instructionType=ParsingTable::Instruction::LeftRecursiveReduce;
									ins.stateParameter=stateIds.IndexOf(action->shiftReduceSource);
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
							CopyFrom(bag->transitionItems, From(bag->transitionItems).OrderBy(ParsingTable::TransitionItem::Compare));
							for(vint k=0;k<bag->transitionItems.Count()-1;k++)
							{
								Ptr<ParsingTable::TransitionItem> t1=bag->transitionItems[k];
								Ptr<ParsingTable::TransitionItem> t2=bag->transitionItems[k+1];
								if(ParsingTable::TransitionItem::CheckOrder(t1, t2, false)==ParsingTable::TransitionItem::UnknownOrder)
								{
									WString stateName=itow(i)+L"["+table->GetStateInfo(i).stateName+L"]";
									WString tokenName=
										j==ParsingTable::TokenBegin?WString(L"$TokenBegin"):
										j==ParsingTable::TokenFinish?WString(L"$TokenFinish"):
										j==ParsingTable::TryReduce?WString(L"$TryReduce"):
										table->GetTokenInfo(j).name;
									errors.Add(new ParsingError(stateIds[i]->ownerRule, L"Conflict happened in transition of \""+tokenName+L"\" of state \""+stateName+L"\"."));
								}
							}
						}
					}
				}

				table->Initialize();
				return table;
			}

			Ptr<tabling::ParsingTable> GenerateTable(Ptr<definitions::ParsingDefinition> definition, collections::List<Ptr<ParsingError>>& errors)
			{
				errors.Clear();
				ParsingSymbolManager symbolManager;
				ValidateDefinition(definition, &symbolManager, errors);
				if(errors.Count()==0)
				{
					Ptr<Automaton> epsilonPDA=CreateEpsilonPDA(definition, &symbolManager);
					Ptr<Automaton> nondeterministicPDA=CreateNondeterministicPDAFromEpsilonPDA(epsilonPDA);
					Ptr<Automaton> jointPDA=CreateJointPDAFromNondeterministicPDA(nondeterministicPDA);
					CompactJointPDA(jointPDA);
					MergeJointPDAStates(jointPDA);
					MarkLeftRecursiveInJointPDA(jointPDA, errors);
					if(errors.Count()==0)
					{
						Ptr<ParsingTable> table=GenerateTable(definition, jointPDA, errors);
						if(errors.Count()==0)
						{
							return table;
						}
					}
				}
				return 0;
			}
		}
	}
}