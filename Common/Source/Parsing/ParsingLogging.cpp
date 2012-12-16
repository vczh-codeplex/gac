#include "ParsingDefinitions.h"
#include "ParsingAutomaton.h"
#include "..\Regex\RegexExpression.h"
#include "..\Stream\MemoryStream.h"
#include "..\Collections\OperationForEach.h"

namespace vl
{
	using namespace stream;
	using namespace collections;

	namespace parsing
	{
		namespace definitions
		{
			void LogString(const WString& input, TextWriter& writer)
			{
				writer.WriteChar(L'\"');
				for(int i=0;i<input.Length();i++)
				{
					if(input[i]==L'\"')
					{
						writer.WriteString(L"\"\"");
					}
					else
					{
						writer.WriteChar(input[i]);
					}
				}
				writer.WriteChar(L'\"');
			}

/***********************************************************************
Logger (ParsingDefinitionType)
***********************************************************************/

			class ParsingDefinitionTypeLogger : public Object, public ParsingDefinitionType::IVisitor
			{
			public:
				TextWriter&		writer;

				ParsingDefinitionTypeLogger(TextWriter& _writer)
					:writer(_writer)
				{
				}

				static void LogInternal(ParsingDefinitionType* type, TextWriter& writer)
				{
					ParsingDefinitionTypeLogger visitor(writer);
					type->Accept(&visitor);
				}

				void Visit(ParsingDefinitionPrimitiveType* node)override
				{
					writer.WriteString(node->name);
				}

				void Visit(ParsingDefinitionTokenType* node)override
				{
					writer.WriteString(L"token");
				}

				void Visit(ParsingDefinitionSubType* node)override
				{
					LogInternal(node->parentType.Obj(), writer);
					writer.WriteString(L".");
					writer.WriteString(node->subTypeName);
				}

				void Visit(ParsingDefinitionArrayType* node)override
				{
					LogInternal(node->elementType.Obj(), writer);
					writer.WriteString(L"[]");
				}
			};

			void Log(ParsingDefinitionType* type, TextWriter& writer)
			{
				ParsingDefinitionTypeLogger::LogInternal(type, writer);
			}

/***********************************************************************
Logger (ParsingDefinitionTypeDefinition)
***********************************************************************/

			class ParsingDefinitionTypeDefinitionLogger : public Object, public ParsingDefinitionTypeDefinition::IVisitor
			{
			public:
				WString			prefix;
				TextWriter&		writer;

				static void LogInternal(ParsingDefinitionTypeDefinition* definition, const WString& prefix, TextWriter& writer)
				{
					ParsingDefinitionTypeDefinitionLogger visitor(prefix, writer);
					definition->Accept(&visitor);
				}

				ParsingDefinitionTypeDefinitionLogger(const WString& _prefix, TextWriter& _writer)
					:prefix(_prefix)
					,writer(_writer)
				{
				}

				void Visit(ParsingDefinitionClassMemberDefinition* node)override
				{
					writer.WriteString(prefix);
					Log(node->type.Obj(), writer);
					writer.WriteString(L" ");
					writer.WriteString(node->name);
					writer.WriteLine(L";");
				}

				void Visit(ParsingDefinitionClassDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(L"class ");
					if(node->parentType)
					{
						writer.WriteString(node->name);
						writer.WriteString(L" : ");
						Log(node->parentType.Obj(), writer);
						writer.WriteLine(L"");
					}
					else
					{
						writer.WriteLine(node->name);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"{");

					for(int i=0;i<node->subTypes.Count();i++)
					{
						LogInternal(node->subTypes[i].Obj(), prefix+L"    ", writer);
						writer.WriteLine(L"");
					}

					for(int i=0;i<node->members.Count();i++)
					{
						LogInternal(node->members[i].Obj(), prefix+L"    ", writer);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"}");
				}

				void Visit(ParsingDefinitionEnumMemberDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(node->name);
					writer.WriteLine(L",");
				}

				void Visit(ParsingDefinitionEnumDefinition* node)override
				{
					writer.WriteString(prefix);
					writer.WriteString(L"enum ");
					writer.WriteLine(node->name);

					writer.WriteString(prefix);
					writer.WriteLine(L"{");

					for(int i=0;i<node->members.Count();i++)
					{
						LogInternal(node->members[i].Obj(), prefix+L"    ", writer);
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"}");
				}
			};

			void Log(ParsingDefinitionTypeDefinition* definition, const WString& prefix, TextWriter& writer)
			{
				ParsingDefinitionTypeDefinitionLogger::LogInternal(definition, prefix, writer);
			}

/***********************************************************************
Logger (ParsingDefinitionGrammar)
***********************************************************************/

#define PRIORITY_NONE			0
#define PRIORITY_CREATE			1
#define PRIORITY_SET			1
#define PRIORITY_ALTERNATIVE	2
#define PRIORITY_SEQUENCE		3
#define PRIORITY_USE			4
#define PRIORITY_ASSIGN			4

			class ParsingDefinitionGrammarLogger : public Object, public ParsingDefinitionGrammar::IVisitor
			{
			public:
				TextWriter&					writer;
				int							parentPriority;
				ParsingDefinitionGrammar*	stateNode;
				bool						beforeNode;

				ParsingDefinitionGrammarLogger(TextWriter& _writer, int _parentPriority, ParsingDefinitionGrammar* _stateNode, bool _beforeNode)
					:writer(_writer)
					,parentPriority(_parentPriority)
					,stateNode(_stateNode)
					,beforeNode(_beforeNode)
				{
				}

				static void LogInternal(ParsingDefinitionGrammar* grammar, int parentPriority, ParsingDefinitionGrammar* stateNode, bool beforeNode, TextWriter& writer)
				{
					if(grammar==stateNode && beforeNode)
					{
						writer.WriteString(L"¡ñ");
					}
					ParsingDefinitionGrammarLogger visitor(writer, parentPriority, stateNode, beforeNode);
					grammar->Accept(&visitor);
					if(grammar==stateNode && !beforeNode)
					{
						writer.WriteString(L"¡ñ");
					}
				}

				void LogInternal(ParsingDefinitionGrammar* grammar, int parentPriority, TextWriter& writer)
				{
					LogInternal(grammar, parentPriority, stateNode, beforeNode, writer);
				}

				void Visit(ParsingDefinitionPrimitiveGrammar* node)override
				{
					writer.WriteString(node->name);
				}

				void Visit(ParsingDefinitionTextGrammar* node)override
				{
					LogString(node->text, writer);
				}

				void Visit(ParsingDefinitionSequenceGrammar* node)override
				{
					int priority=PRIORITY_SEQUENCE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->first.Obj(), priority, writer);
					writer.WriteString(L" ");
					LogInternal(node->second.Obj(), priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionAlternativeGrammar* node)override
				{
					int priority=PRIORITY_ALTERNATIVE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->first.Obj(), priority, writer);
					writer.WriteString(L" | ");
					LogInternal(node->second.Obj(), priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionLoopGrammar* node)override
				{
					writer.WriteString(L"{ ");
					LogInternal(node->grammar.Obj(), PRIORITY_NONE, writer);
					writer.WriteString(L" }");
				}

				void Visit(ParsingDefinitionOptionalGrammar* node)override
				{
					writer.WriteString(L"[ ");
					LogInternal(node->grammar.Obj(), PRIORITY_NONE, writer);
					writer.WriteString(L" ]");
				}

				void Visit(ParsingDefinitionCreateGrammar* node)override
				{
					int priority=PRIORITY_CREATE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar.Obj(), priority, writer);
					writer.WriteString(L" as ");
					Log(node->type.Obj(), writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionAssignGrammar* node)override
				{
					int priority=PRIORITY_ASSIGN;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar.Obj(), priority, writer);
					writer.WriteString(L" : ");
					writer.WriteString(node->memberName);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionUseGrammar* node)override
				{
					int priority=PRIORITY_USE;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					writer.WriteString(L"!");
					LogInternal(node->grammar.Obj(), priority, writer);
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}

				void Visit(ParsingDefinitionSetterGrammar* node)override
				{
					int priority=PRIORITY_SET;
					if(parentPriority>priority)
					{
						writer.WriteString(L"( ");
					}
					LogInternal(node->grammar.Obj(), priority, writer);
					writer.WriteString(L" with { ");
					writer.WriteString(node->memberName);
					writer.WriteString(L" = ");
					LogString(node->value, writer);
					writer.WriteString(L" }");
					if(parentPriority>priority)
					{
						writer.WriteString(L" )");
					}
				}
			};

			void Log(ParsingDefinitionGrammar* grammar, TextWriter& writer)
			{
				ParsingDefinitionGrammarLogger::LogInternal(grammar, PRIORITY_NONE, 0, true, writer);
			}

			void Log(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode, TextWriter& writer)
			{
				ParsingDefinitionGrammarLogger::LogInternal(grammar, PRIORITY_NONE, stateNode, beforeNode, writer);
			}

#undef PRIORITY_NONE
#undef PRIORITY_CREATE
#undef PRIORITY_SET
#undef PRIORITY_ALTERNATIVE
#undef PRIORITY_SEQUENCE
#undef PRIORITY_USE
#undef PRIORITY_ASSIGN

/***********************************************************************
FindAppropriateGrammarState
***********************************************************************/

			class FindAppropriateGrammarStateVisitor : public Object, public ParsingDefinitionGrammar::IVisitor
			{
			public:
				ParsingDefinitionGrammar*	stateNode;
				bool						beforeNode;
				ParsingDefinitionGrammar*	beforeReference;
				ParsingDefinitionGrammar*	afterReference;
				ParsingDefinitionGrammar*	result;

				FindAppropriateGrammarStateVisitor(ParsingDefinitionGrammar* _stateNode, bool _beforeNode, ParsingDefinitionGrammar* _beforeReference, ParsingDefinitionGrammar* _afterReference)
					:stateNode(_stateNode)
					,beforeNode(_beforeNode)
					,beforeReference(_beforeReference)
					,afterReference(_afterReference)
					,result(0)
				{
				}

				static ParsingDefinitionGrammar* Find(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode, ParsingDefinitionGrammar* beforeReference, ParsingDefinitionGrammar* afterReference)
				{
					if(grammar==stateNode)
					{
						return
							beforeNode
							?(beforeReference?beforeReference:stateNode)
							:(afterReference?afterReference:stateNode)
							;
					}
					else
					{
						FindAppropriateGrammarStateVisitor visitor(stateNode, beforeNode, beforeReference, afterReference);
						grammar->Accept(&visitor);
						return visitor.result;
					}
				}

				void Visit(ParsingDefinitionPrimitiveGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionTextGrammar* node)override
				{
				}

				void Visit(ParsingDefinitionSequenceGrammar* node)override
				{
					result=Find(node->first.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), 0);
					if(!result)
					{
						result=Find(node->second.Obj(), stateNode, beforeNode, 0, (afterReference?afterReference:node));
					}
				}

				void Visit(ParsingDefinitionAlternativeGrammar* node)override
				{
					result=Find(node->first.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
					if(!result)
					{
						result=Find(node->second.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
					}
				}

				void Visit(ParsingDefinitionLoopGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
				}

				void Visit(ParsingDefinitionOptionalGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
				}

				void Visit(ParsingDefinitionCreateGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
				}

				void Visit(ParsingDefinitionAssignGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
				}

				void Visit(ParsingDefinitionUseGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, (beforeReference?beforeReference:node), (afterReference?afterReference:node));
				}

				void Visit(ParsingDefinitionSetterGrammar* node)override
				{
					result=Find(node->grammar.Obj(), stateNode, beforeNode, beforeReference, afterReference);
				}
			};

/***********************************************************************
Logger (ParsingDefinitionGrammar)
***********************************************************************/

			WString TypeToString(ParsingDefinitionType* type)
			{
				MemoryStream stream(64);
				{
					StreamWriter writer(stream);
					Log(type, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					return reader.ReadToEnd();
				}
			}

			WString GrammarToString(ParsingDefinitionGrammar* grammar)
			{
				return GrammarStateToString(grammar, 0, true);
			}

			WString GrammarStateToString(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode)
			{
				MemoryStream stream(64);
				{
					StreamWriter writer(stream);
					Log(grammar, stateNode, beforeNode, writer);
				}
				stream.SeekFromBegin(0);
				{
					StreamReader reader(stream);
					return reader.ReadToEnd();
				}
			}

			ParsingDefinitionGrammar* FindAppropriateGrammarState(ParsingDefinitionGrammar* grammar, ParsingDefinitionGrammar* stateNode, bool beforeNode)
			{
				return FindAppropriateGrammarStateVisitor::Find(grammar, stateNode, beforeNode, 0, 0);
			}

			void Log(Ptr<ParsingDefinition> definition, TextWriter& writer)
			{
				FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, definition->types)
				{
					Log(type.Obj(), L"", writer);
					writer.WriteLine(L"");
				}
				
				FOREACH(Ptr<ParsingDefinitionTokenDefinition>, token, definition->tokens)
				{
					writer.WriteString(L"token ");
					writer.WriteString(token->name);
					writer.WriteString(L" = ");
					LogString(token->regex, writer);
					writer.WriteLine(L";");
				}
				writer.WriteLine(L"");
				
				FOREACH(Ptr<ParsingDefinitionRuleDefinition>, rule, definition->rules)
				{
					writer.WriteString(L"rule ");
					Log(rule->type.Obj(), writer);
					writer.WriteString(L" ");
					writer.WriteLine(rule->name);
					FOREACH(Ptr<ParsingDefinitionGrammar>, grammar, rule->grammars)
					{
						writer.WriteString(L"        = ");
						Log(grammar.Obj(), writer);
						writer.WriteLine(L";");
					}
					writer.WriteLine(L"");
				}
			}
		}

		namespace analyzing
		{
/***********************************************************************
Logger (Automaton)
***********************************************************************/

			void LogTransitionSymbol(ParsingSymbol* symbol, stream::TextWriter& writer)
			{
				if(symbol->GetType()==ParsingSymbol::TokenDef)
				{
					writer.WriteString(L"[");
					writer.WriteString(symbol->GetName());

					WString regex=symbol->GetDescriptorString();
					if(regex_internal::IsRegexEscapedListeralString(regex))
					{
						writer.WriteString(L" ");
						definitions::LogString(regex_internal::UnescapeTextForRegex(regex), writer);
					}
					writer.WriteString(L"]");
				}
				else
				{
					writer.WriteString(L"<");
					writer.WriteString(symbol->GetName());
					writer.WriteString(L">");
				}
			}

			void Log(Ptr<Automaton> automaton, stream::TextWriter& writer)
			{
				FOREACH(Ptr<RuleInfo>, ruleInfo, automaton->ruleInfos.Values())
				{
					List<State*> states;
					states.Add(ruleInfo->rootRuleStartState);
					vint currentState=0;

					while(currentState<states.Count())
					{
						State* state=states[currentState++];
						writer.WriteLine(state->stateExpression);
						if(state->endState)
						{
							writer.WriteString(L"END STATE ");
						}
						else
						{
							writer.WriteString(L"STATE ");
						}
						writer.WriteLine(state->stateName);

						FOREACH(Transition*, transition, state->transitions)
						{
							if(!states.Contains(transition->target))
							{
								states.Add(transition->target);
							}
							switch(transition->transitionType)
							{
							case Transition::Epsilon:
								writer.WriteString(L"    EPSILON");
								break;
							case Transition::TokenBegin:
								writer.WriteString(L"    TOKEN-BEGIN");
								break;
							case Transition::TokenFinish:
								writer.WriteString(L"    TOKEN-FINISH");
								break;
							case Transition::Symbol:
								{
									writer.WriteString(L"    ");
									if(transition->transitionSymbol)
									{
										LogTransitionSymbol(transition->transitionSymbol, writer);
									}
								}
								break;
							}
							writer.WriteString(L" => ");
							writer.WriteLine(transition->target->stateName);

							if(transition->stackPattern.Count()>0)
							{
								writer.WriteString(L"        STACK PATTERNS");
								FOREACH(State*, stackPattern, transition->stackPattern)
								{
									writer.WriteString(L" <");
									writer.WriteString(stackPattern->stateName);
									writer.WriteString(L">");
								}
								writer.WriteLine(L"");
							}

							if(transition->lookAheadSymbol!=0)
							{
								writer.WriteString(L"        LOOK AHEAD");
								LogTransitionSymbol(transition->lookAheadSymbol, writer);
								writer.WriteLine(L"");
							}

							FOREACH(Ptr<Action>, action, transition->actions)
							{
								switch(action->actionType)
								{
								case Action::Create:
									writer.WriteString(L"        CREATE ");
									break;
								case Action::Assign:
									writer.WriteString(L"        ASSIGN ");
									break;
								case Action::Using:
									writer.WriteString(L"        USING ");
									break;
								case Action::Setter:
									writer.WriteString(L"        SET ");
									break;
								}
								if(action->actionSource)
								{
									writer.WriteString(action->actionSource->GetName());
								}
								if(action->actionTarget)
								{
									writer.WriteString(L" => ");
									writer.WriteString(action->actionTarget->GetName());
								}
								writer.WriteLine(L"");
							}
						}
						writer.WriteLine(L"");
					}
					writer.WriteLine(L"--------------------------------");
				}
			}
		}
	}
}