#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace reflection;
			using namespace reflection::description;

/***********************************************************************
WfObservingDependency
***********************************************************************/

			WfObservingDependency::WfObservingDependency(WfObservingDependency& dependency)
				:dependencies(dependency.dependencies)
			{
				CopyFrom(inputObserves, dependency.inputObserves);
			}

			WfObservingDependency::WfObservingDependency(DependencyGroup& _dependencies)
				:dependencies(_dependencies)
			{
			}

			WfObservingDependency::WfObservingDependency(DependencyGroup& _dependencies, ObserveList& _inputObserves)
				:dependencies(_dependencies)
			{
				CopyFrom(inputObserves, _inputObserves);
			}

			void WfObservingDependency::AddInternal(WfExpression* observe, WfExpression* dependedObserve)
			{
				auto index = dependencies.Keys().IndexOf(dependedObserve);
				if (index == -1)
				{
					dependencies.Add(dependedObserve, observe);
				}
				else if (!dependencies.GetByIndex(index).Contains(observe))
				{
					dependencies.Add(dependedObserve, observe);
				}
			}

			void WfObservingDependency::Prepare(WfExpression* observe)
			{
				AddInternal(0, observe);

				if (!outputObserves.Contains(observe))
				{
					outputObserves.Add(observe);
				}
			}

			void WfObservingDependency::Add(WfExpression* observe)
			{
				Add(observe, *this);
			}

			void WfObservingDependency::Add(WfExpression* observe, WfObservingDependency& dependency)
			{
				Prepare(observe);
				FOREACH(WfExpression*, dependedObserve, dependency.inputObserves)
				{
					AddInternal(observe, dependedObserve);
				}
			}

			void WfObservingDependency::TurnToInput()
			{
				if (outputObserves.Count() > 0)
				{
					CopyFrom(inputObserves, outputObserves);
					outputObserves.Clear();
				}
			}

			void WfObservingDependency::Cleanup()
			{
				vint count = dependencies.Keys().Count();
				for (vint i = 0; i < count; i++)
				{
					const auto& values = dependencies.GetByIndex(i);
					if (values.Contains(0) && values.Count()>1)
					{
						dependencies.Remove(dependencies.Keys()[i], 0);
					}
				}
			}

/***********************************************************************
GetObservingDependency
***********************************************************************/

			class GetObservingDependencyVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*				manager;
				WfObservingDependency&				dependency;

				GetObservingDependencyVisitor(WfLexicalScopeManager* _manager, WfObservingDependency& _dependency)
					:manager(_manager)
					, dependency(_dependency)
				{
				}

				void Visit(WfTopQualifiedExpression* node)override
				{
				}

				void Visit(WfReferenceExpression* node)override
				{
				}

				void Visit(WfOrderedNameExpression* node)override
				{
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
				}

				void Visit(WfMemberExpression* node)override
				{
					WfObservingDependency parent(dependency);
					GetObservingDependency(manager, node->parent, parent);
					parent.TurnToInput();

					auto memberResult = manager->expressionResolvings[node];
					if (memberResult.propertyInfo)
					{
						auto td = memberResult.propertyInfo->GetOwnerTypeDescriptor();
						auto ev = td->GetEventByName(memberResult.propertyInfo->GetName() + L"Changed", true);
						if (ev)
						{
							dependency.Add(node, parent);
						}
					}
				}

				void Visit(WfChildExpression* node)override
				{
				}

				void Visit(WfLiteralExpression* node)override
				{
				}

				void Visit(WfFloatingExpression* node)override
				{
				}

				void Visit(WfIntegerExpression* node)override
				{
				}

				void Visit(WfStringExpression* node)override
				{
				}

				void Visit(WfFormatExpression* node)override
				{
					GetObservingDependency(manager, node->expandedExpression, dependency);
				}

				void Visit(WfUnaryExpression* node)override
				{
					GetObservingDependency(manager, node->operand, dependency);
				}

				void Visit(WfBinaryExpression* node)override
				{
					GetObservingDependency(manager, node->first, dependency);
					GetObservingDependency(manager, node->second, dependency);
				}

				void Visit(WfLetExpression* node)override
				{
					auto result = MakePtr<WfObservingDependency>(dependency);
					FOREACH(Ptr<WfLetVariable>, var, node->variables)
					{
						GetObservingDependency(manager, var->value, *result.Obj());
						result->TurnToInput();
						result = MakePtr<WfObservingDependency>(*result.Obj());
					}
					GetObservingDependency(manager, node->expression, *result.Obj());
					CopyFrom(dependency.outputObserves, result->outputObserves);
				}

				void Visit(WfIfExpression* node)override
				{
					WfObservingDependency condition(dependency);
					GetObservingDependency(manager, node->condition, condition);
					condition.TurnToInput();

					WfObservingDependency trueBranch(condition), falseBranch(condition);
					GetObservingDependency(manager, node->trueBranch, trueBranch);
					GetObservingDependency(manager, node->falseBranch, falseBranch);
					CopyFrom(dependency.outputObserves, From(trueBranch.outputObserves).Concat(falseBranch.outputObserves));
				}

				void Visit(WfRangeExpression* node)override
				{
					GetObservingDependency(manager, node->begin, dependency);
					GetObservingDependency(manager, node->end, dependency);
				}

				void Visit(WfSetTestingExpression* node)override
				{
					GetObservingDependency(manager, node->element, dependency);
					GetObservingDependency(manager, node->collection, dependency);
				}

				void Visit(WfConstructorExpression* node)override
				{
					FOREACH(Ptr<WfConstructorArgument>, argument, node->arguments)
					{
						GetObservingDependency(manager, argument->key, dependency);
						if (argument->value)
						{
							GetObservingDependency(manager, argument->value, dependency);
						}
					}
				}

				void Visit(WfInferExpression* node)override
				{
					GetObservingDependency(manager, node->expression, dependency);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					GetObservingDependency(manager, node->expression, dependency);
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					GetObservingDependency(manager, node->expression, dependency);
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					GetObservingDependency(manager, node->expression, dependency);
				}

				void Visit(WfAttachEventExpression* node)override
				{
				}

				void Visit(WfDetachEventExpression* node)override
				{
				}

				void Visit(WfBindExpression* node)override
				{
				}

				void Visit(WfObserveExpression* node)override
				{
					WfObservingDependency parent(dependency);
					GetObservingDependency(manager, node->parent, parent);
					parent.TurnToInput();

					dependency.Add(node, parent);
					dependency.TurnToInput();
					GetObservingDependency(manager, node->expression, dependency);
				}

				void Visit(WfCallExpression* node)override
				{
					GetObservingDependency(manager, node->function, dependency);
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						GetObservingDependency(manager, argument, dependency);
					}
				}

				void Visit(WfFunctionExpression* node)override
				{
				}

				void Visit(WfNewTypeExpression* node)override
				{
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						GetObservingDependency(manager, argument, dependency);
					}
				}
			};

			void GetObservingDependency(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, WfObservingDependency& dependency)
			{
				GetObservingDependencyVisitor visitor(manager, dependency);
				expression->Accept(&visitor);
			}

/***********************************************************************
ExpandBindExpression
***********************************************************************/

			void ExpandBindExpression(WfLexicalScopeManager* manager, WfBindExpression* node)
			{
				Group<WfExpression*, WfExpression*> group;
				WfObservingDependency dependency(group);
				GetObservingDependency(manager, node->expression, dependency);
				dependency.Cleanup();

				auto lambdaBlock = MakePtr<WfBlockStatement>();
				{
					auto lambda = MakePtr<WfFunctionDeclaration>();
					auto typeInfo = TypeInfoRetriver<Ptr<IValueSubscription>>::CreateTypeInfo();
					lambda->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
					lambda->statement = lambdaBlock;

					auto callLambda = MakePtr<WfCallExpression>();
					node->expandedExpression = callLambda;

					auto lambdaExpr = MakePtr<WfFunctionExpression>();
					lambdaExpr->function = lambda;
					callLambda->function = lambdaExpr;
				}

				Group<WfExpression*, WString> observeNames;
				FOREACH_INDEXER(WfExpression*, observe, observeIndex, dependency.dependencies.Keys())
				{
					List<IEventInfo*> events;
					if (auto observeExpr = dynamic_cast<WfObserveExpression*>(observe))
					{
						FOREACH(Ptr<WfExpression>, eventExpr, observeExpr->events)
						{
							auto result = manager->expressionResolvings[eventExpr.Obj()];
							events.Add(result.eventInfo);
						}
					}
					else if (auto memberExpr = dynamic_cast<WfMemberExpression*>(observe))
					{
						auto result = manager->expressionResolvings[memberExpr];
						auto td = result.propertyInfo->GetOwnerTypeDescriptor();
						auto ev = td->GetEventByName(result.propertyInfo->GetName() + L"Changed", true);
						events.Add(ev);
					}

					FOREACH_INDEXER(IEventInfo*, ev, eventIndex, events)
					{
						WString handlerName = L"<bind-observe>" + itow(observeIndex) + L"_" + itow(eventIndex);
						WString callbackName = L"<bind-callback>" + itow(observeIndex) + L"_" + itow(eventIndex);
						observeNames.Add(observe, handlerName);
						{
							auto variable = MakePtr<WfVariableDeclaration>();
							variable->name.value = handlerName;
					
							auto variableStat = MakePtr<WfVariableStatement>();
							variableStat->variable = variable;
							lambdaBlock->statements.Add(variableStat);

							auto nullExpr = MakePtr<WfLiteralExpression>();
							nullExpr->value = WfLiteralValue::Null;

							auto inferExpr = MakePtr<WfInferExpression>();
							inferExpr->expression = nullExpr;
							inferExpr->type = GetTypeFromTypeInfo(TypeInfoRetriver<Ptr<IEventHandler>>::CreateTypeInfo().Obj());

							auto ctorArg = MakePtr<WfConstructorArgument>();
							ctorArg->key = inferExpr;

							auto ctorExpr = MakePtr<WfConstructorExpression>();
							ctorExpr->arguments.Add(ctorArg);

							variable->expression = ctorExpr;
						}
						{
							auto variable = MakePtr<WfVariableDeclaration>();
							variable->name.value = callbackName;
							variable->type = GetTypeFromTypeInfo(ev->GetHandlerType());
					
							auto variableStat = MakePtr<WfVariableStatement>();
							variableStat->variable = variable;
							lambdaBlock->statements.Add(variableStat);

							auto nullExpr = MakePtr<WfLiteralExpression>();
							nullExpr->value = WfLiteralValue::Null;
							variable->expression = nullExpr;
						}
					}
				}
						
				auto newSubscription = MakePtr<WfNewTypeExpression>();
				{
					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = newSubscription;
					lambdaBlock->statements.Add(returnStat);
				}
				{
					auto typeInfo = TypeInfoRetriver<Ptr<IValueSubscription>>::CreateTypeInfo();
					newSubscription->type = GetTypeFromTypeInfo(typeInfo.Obj());
				}
				{
					auto func = MakePtr<WfFunctionDeclaration>();
					newSubscription->functions.Add(func);
					func->name.value = L"Subscribe";
					{
						auto typeInfo = TypeInfoRetriver<Ptr<IValueListener>>::CreateTypeInfo();
						func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
					}
					{
						auto typeInfo = TypeInfoRetriver<Func<void(Value)>>::CreateTypeInfo();
						auto argument = MakePtr<WfFunctionArgument>();
						argument->name.value = L"<bind-callback>";
						argument->type = GetTypeFromTypeInfo(typeInfo.Obj());
						func->arguments.Add(argument);
					}
					{
						auto literal = MakePtr<WfLiteralExpression>();
						literal->value = WfLiteralValue::Null;

						auto returnStat = MakePtr<WfReturnStatement>();
						returnStat->expression = literal;

						auto block = MakePtr<WfBlockStatement>();
						block->statements.Add(returnStat);
						func->statement = block;
					}
				}
				{
					auto func = MakePtr<WfFunctionDeclaration>();
					newSubscription->functions.Add(func);
					func->name.value = L"Close";
					{
						auto typeInfo = TypeInfoRetriver<bool>::CreateTypeInfo();
						func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
					}
					{
						auto literal = MakePtr<WfLiteralExpression>();
						literal->value = WfLiteralValue::False;

						auto returnStat = MakePtr<WfReturnStatement>();
						returnStat->expression = literal;

						auto block = MakePtr<WfBlockStatement>();
						block->statements.Add(returnStat);
						func->statement = block;
					}
				}
			}
		}
	}
}