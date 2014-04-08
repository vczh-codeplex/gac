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
				SortedList<WfExpression*> all;
				for (vint i = 0; i < count; i++)
				{
					const auto& values = dependencies.GetByIndex(i);
					if (values.Contains(0) && values.Count()>1)
					{
						dependencies.Remove(dependencies.Keys()[i], 0);
					}
					FOREACH(WfExpression*, observe, values)
					{
						if (!all.Contains(observe))
						{
							all.Add(observe);
						}
					}
				}
				FOREACH(WfExpression*, observe, dependencies.Keys())
				{
					all.Remove(observe);
				}
				FOREACH(WfExpression*, observe, all)
				{
					dependencies.Add(0, observe);
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
ExpandObserveExpression
***********************************************************************/

			class ExpandObserveExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				Dictionary<WfExpression*, WString>&		cacheNames;
				Ptr<WfExpression>						result;

				ExpandObserveExpressionVisitor(Dictionary<WfExpression*, WString>& _cacheNames)
					:cacheNames(_cacheNames)
				{
				}

				void Visit(WfTopQualifiedExpression* node)override
				{
					auto expr = MakePtr<WfTopQualifiedExpression>();
					expr->name.value = node->name.value;
					result = expr;
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
				}

				void Visit(WfUnaryExpression* node)override
				{
				}

				void Visit(WfBinaryExpression* node)override
				{
				}

				void Visit(WfLetExpression* node)override
				{
				}

				void Visit(WfIfExpression* node)override
				{
				}

				void Visit(WfRangeExpression* node)override
				{
				}

				void Visit(WfSetTestingExpression* node)override
				{
				}

				void Visit(WfConstructorExpression* node)override
				{
				}

				void Visit(WfInferExpression* node)override
				{
				}

				void Visit(WfTypeCastingExpression* node)override
				{
				}

				void Visit(WfTypeTestingExpression* node)override
				{
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
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
				}

				void Visit(WfCallExpression* node)override
				{
				}

				void Visit(WfFunctionExpression* node)override
				{
				}

				void Visit(WfNewTypeExpression* node)override
				{
				}
			};

			Ptr<WfExpression> ExpandObserveExpression(Ptr<WfExpression> expression, collections::Dictionary<WfExpression*, WString>& cacheNames, bool useCache)
			{
				if (useCache)
				{
					vint index = cacheNames.Keys().IndexOf(expression.Obj());
					if (index != -1)
					{
						auto ref = MakePtr<WfReferenceExpression>();
						ref->name.value = cacheNames.Values()[index];
						return ref;
					}
				}

				ExpandObserveExpressionVisitor visitor(cacheNames);
				expression->Accept(&visitor);
				return visitor.result;
			}

/***********************************************************************
ExpandBindExpression
***********************************************************************/

			void DecodeObserveExpression(WfLexicalScopeManager* manager, WfExpression* observe, List<IEventInfo*>& events, WfExpression*& parent)
			{
				if (auto observeExpr = dynamic_cast<WfObserveExpression*>(observe))
				{
					parent = observeExpr->parent.Obj();
					FOREACH(Ptr<WfExpression>, eventExpr, observeExpr->events)
					{
						auto result = manager->expressionResolvings[eventExpr.Obj()];
						events.Add(result.eventInfo);
					}
				}
				else if (auto memberExpr = dynamic_cast<WfMemberExpression*>(observe))
				{
					parent = memberExpr->parent.Obj();
					auto result = manager->expressionResolvings[memberExpr];
					auto td = result.propertyInfo->GetOwnerTypeDescriptor();
					auto ev = td->GetEventByName(result.propertyInfo->GetName() + L"Changed", true);
					events.Add(ev);
				}
			}

			Ptr<WfExpression> CreateBindDefaultValue(ITypeInfo* elementType)
			{
				if (elementType->GetTypeDescriptor()->GetValueSerializer())
				{
					auto stringExpr = MakePtr<WfStringExpression>();
					stringExpr->value.value = elementType->GetTypeDescriptor()->GetValueSerializer()->GetDefaultText();

					auto castExpr = MakePtr<WfTypeCastingExpression>();
					castExpr->strategy = WfTypeCastingStrategy::Strong;
					castExpr->expression = stringExpr;
					castExpr->type = GetTypeFromTypeInfo(elementType);

					return castExpr;
				}
				else
				{
					auto nullExpr = MakePtr<WfLiteralExpression>();
					nullExpr->value = WfLiteralValue::Null;

					auto inferExpr = MakePtr<WfInferExpression>();
					inferExpr->expression = nullExpr;
					inferExpr->type = GetTypeFromTypeInfo(elementType);

					return inferExpr;
				}
			}

			Ptr<WfExpression> CreateBindVariableReference(const WString& name)
			{
				auto ref = MakePtr<WfReferenceExpression>();
				ref->name.value = name;

				auto zero = MakePtr<WfIntegerExpression>();
				zero->value.value = L"0";

				auto index = MakePtr<WfBinaryExpression>();
				index->op = WfBinaryOperator::Index;
				index->first = ref;
				index->second = zero;

				return index;
			}

			Ptr<WfVariableStatement> CreateBindWritableVariable(const WString& name, ITypeInfo* elementType)
			{
				auto ctorArg = MakePtr<WfConstructorArgument>();
				ctorArg->key = CreateBindDefaultValue(elementType);
				
				auto ctorExpr = MakePtr<WfConstructorExpression>();
				ctorExpr->arguments.Add(ctorArg);

				auto variable = MakePtr<WfVariableDeclaration>();
				variable->name.value = name;
				variable->expression = ctorExpr;
					
				auto variableStat = MakePtr<WfVariableStatement>();
				variableStat->variable = variable;
				return variableStat;
			}

			Ptr<WfFunctionDeclaration> CreateListenerGetSubscriptionFunction()
			{
				auto func = MakePtr<WfFunctionDeclaration>();
				func->name.value = L"GetSubscription";
				{
					auto typeInfo = TypeInfoRetriver<IValueSubscription*>::CreateTypeInfo();
					func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
				}

				auto block = MakePtr<WfBlockStatement>();
				func->statement = block;
				{
					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression =  CreateBindVariableReference(L"<subscription>");
					block->statements.Add(returnStat);
				}
				return func;
			}

			Ptr<WfFunctionDeclaration> CreateListenerGetStoppedFunction()
			{
				auto func = MakePtr<WfFunctionDeclaration>();
				func->name.value = L"GetStopped";
				{
					auto typeInfo = TypeInfoRetriver<bool>::CreateTypeInfo();
					func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
				}

				auto block = MakePtr<WfBlockStatement>();
				func->statement = block;
				{
					auto listenersRef = MakePtr<WfReferenceExpression>();
					listenersRef->name.value = L"<bind-listeners>";

					auto keysExpr = MakePtr<WfMemberExpression>();
					keysExpr->parent = listenersRef;
					keysExpr->name.value = L"Keys";

					auto containsExpr = MakePtr<WfMemberExpression>();
					containsExpr->parent = keysExpr;
					containsExpr->name.value = L"Contains";

					auto callExpr = MakePtr<WfCallExpression>();
					callExpr->function = containsExpr;
					callExpr->arguments.Add(CreateBindVariableReference(L"<listener>"));

					auto notExpr = MakePtr<WfUnaryExpression>();
					notExpr->op = WfUnaryOperator::Not;
					notExpr->operand = callExpr;

					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = notExpr;
					block->statements.Add(returnStat);
				}
				return func;
			}

			Ptr<WfFunctionDeclaration> CreateListenerStopListeningFunction()
			{
				auto func = MakePtr<WfFunctionDeclaration>();
				func->name.value = L"StopListening";
				{
					auto typeInfo = TypeInfoRetriver<bool>::CreateTypeInfo();
					func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
				}

				auto block = MakePtr<WfBlockStatement>();
				func->statement = block;
				{
					auto ifStat = MakePtr<WfIfStatement>();
					block->statements.Add(ifStat);
					{
						auto listenersRef = MakePtr<WfReferenceExpression>();
						listenersRef->name.value = L"<bind-listeners>";

						auto keysExpr = MakePtr<WfMemberExpression>();
						keysExpr->parent = listenersRef;
						keysExpr->name.value = L"Keys";

						auto containsExpr = MakePtr<WfMemberExpression>();
						containsExpr->parent = keysExpr;
						containsExpr->name.value = L"Contains";

						auto callExpr = MakePtr<WfCallExpression>();
						callExpr->function = containsExpr;
						callExpr->arguments.Add(CreateBindVariableReference(L"<listener>"));
						ifStat->expression = callExpr;
					}

					auto ifBlock = MakePtr<WfBlockStatement>();
					ifStat->trueBranch = ifBlock;
					{
						auto listenersRef = MakePtr<WfReferenceExpression>();
						listenersRef->name.value = L"<bind-listeners>";

						auto removeExpr = MakePtr<WfMemberExpression>();
						removeExpr->parent = listenersRef;
						removeExpr->name.value = L"Remove";

						auto callExpr = MakePtr<WfCallExpression>();
						callExpr->function = removeExpr;
						callExpr->arguments.Add(CreateBindVariableReference(L"<listener>"));

						auto stat = MakePtr<WfExpressionStatement>();
						stat->expression = callExpr;
						ifBlock->statements.Add(stat);
					}
					{
						auto literal = MakePtr<WfLiteralExpression>();
						literal->value = WfLiteralValue::True;

						auto returnStat = MakePtr<WfReturnStatement>();
						returnStat->expression = literal;
						ifBlock->statements.Add(returnStat);
					}
				}
				{
					auto literal = MakePtr<WfLiteralExpression>();
					literal->value = WfLiteralValue::False;

					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = literal;
					block->statements.Add(returnStat);
				}
				return func;
			}

			Ptr<WfFunctionDeclaration> CreateBindSubscribeFunction()
			{
				auto func = MakePtr<WfFunctionDeclaration>();
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

				auto block = MakePtr<WfBlockStatement>();
				func->statement = block;
				block->statements.Add(CreateBindWritableVariable(L"<listener>", TypeInfoRetriver<IValueListener*>::CreateTypeInfo().Obj()));
				{
					auto newListener = MakePtr<WfNewTypeExpression>();
					{
						auto typeInfo = TypeInfoRetriver<Ptr<IValueListener>>::CreateTypeInfo();
						newListener->type = GetTypeFromTypeInfo(typeInfo.Obj());
					}
					newListener->functions.Add(CreateListenerGetSubscriptionFunction());
					newListener->functions.Add(CreateListenerGetStoppedFunction());
					newListener->functions.Add(CreateListenerStopListeningFunction());

					auto variable = MakePtr<WfVariableDeclaration>();
					variable->name.value = L"<listener-shared>";
					variable->expression = newListener;
					
					auto variableStat = MakePtr<WfVariableStatement>();
					variableStat->variable = variable;
					block->statements.Add(variableStat);
				}
				{
					auto listenerRef = MakePtr<WfReferenceExpression>();
					listenerRef->name.value = L"<listener-shared>";

					auto castExpr = MakePtr<WfTypeCastingExpression>();
					castExpr->strategy = WfTypeCastingStrategy::Strong;
					castExpr->expression = listenerRef;
					castExpr->type = GetTypeFromTypeInfo(TypeInfoRetriver<IValueListener*>::CreateTypeInfo().Obj());

					auto assign = MakePtr<WfBinaryExpression>();
					assign->op = WfBinaryOperator::Assign;
					assign->first = CreateBindVariableReference(L"<listener>");
					assign->second = castExpr;

					auto stat = MakePtr<WfExpressionStatement>();
					stat->expression = assign;
					block->statements.Add(stat);
				}
				{
					auto callbackRef = MakePtr<WfReferenceExpression>();
					callbackRef->name.value = L"<bind-callback>";

					auto mapRef = MakePtr<WfReferenceExpression>();
					mapRef->name.value = L"<bind-listeners>";

					auto func = MakePtr<WfMemberExpression>();
					func->parent = mapRef;
					func->name.value = L"Set";

					auto call = MakePtr<WfCallExpression>();
					call->function = func;
					call->arguments.Add(CreateBindVariableReference(L"<listener>"));
					call->arguments.Add(callbackRef);

					auto stat = MakePtr<WfExpressionStatement>();
					stat->expression = call;
					block->statements.Add(stat);
				}
				{
					auto listenerRef = MakePtr<WfReferenceExpression>();
					listenerRef->name.value = L"<listener-shared>";

					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = listenerRef;
					block->statements.Add(returnStat);
				}
				return func;
			}

			Ptr<WfFunctionDeclaration> CreateBindCloseFunction(const Dictionary<WString, Ptr<ITypeInfo>>& variableTypes, const Group<WfExpression*, WString>& handlerNames)
			{
				auto func = MakePtr<WfFunctionDeclaration>();
				func->name.value = L"Close";
				{
					auto typeInfo = TypeInfoRetriver<bool>::CreateTypeInfo();
					func->returnType = GetTypeFromTypeInfo(typeInfo.Obj());
				}

				auto block = MakePtr<WfBlockStatement>();
				func->statement = block;
				{
					auto ifStat = MakePtr<WfIfStatement>();
					block->statements.Add(ifStat);
					{
						auto notExpr = MakePtr<WfUnaryExpression>();
						notExpr->op = WfUnaryOperator::Not;
						notExpr->operand = CreateBindVariableReference(L"<bind-closed>");
						ifStat->expression = notExpr;
					}

					auto ifBlock = MakePtr<WfBlockStatement>();
					ifStat->trueBranch = ifBlock;
					{
						auto literal = MakePtr<WfLiteralExpression>();
						literal->value = WfLiteralValue::True;

						auto assign = MakePtr<WfBinaryExpression>();
						assign->op = WfBinaryOperator::Assign;
						assign->first = CreateBindVariableReference(L"<bind-closed>");
						assign->second = literal;

						auto stat = MakePtr<WfExpressionStatement>();
						stat->expression = assign;
						ifBlock->statements.Add(stat);
					}
					for (vint i = 0; i < handlerNames.Keys().Count(); i++)
					{
						FOREACH(WString, name, handlerNames.GetByIndex(i))
						{
							auto detach = MakePtr<WfDetachEventExpression>();
							detach->handler = CreateBindVariableReference(name);

							auto stat = MakePtr<WfExpressionStatement>();
							stat->expression = detach;
							//ifBlock->statements.Add(stat);
						}
					}
					FOREACH_INDEXER(WString, name, index, variableTypes.Keys())
					{
						auto assign = MakePtr<WfBinaryExpression>();
						assign->op = WfBinaryOperator::Assign;
						assign->first = CreateBindVariableReference(name);
						assign->second = CreateBindDefaultValue(variableTypes.Values()[index].Obj());

						auto stat = MakePtr<WfExpressionStatement>();
						stat->expression = assign;
						ifBlock->statements.Add(stat);
					}
					{
						auto ref = MakePtr<WfReferenceExpression>();
						ref->name.value = L"<bind-listeners>";

						auto func = MakePtr<WfMemberExpression>();
						func->parent = ref;
						func->name.value = L"Clear";

						auto call = MakePtr<WfCallExpression>();
						call->function = func;

						auto stat = MakePtr<WfExpressionStatement>();
						stat->expression = call;
						ifBlock->statements.Add(stat);
					}
					{
						auto literal = MakePtr<WfLiteralExpression>();
						literal->value = WfLiteralValue::True;

						auto returnStat = MakePtr<WfReturnStatement>();
						returnStat->expression = literal;
						ifBlock->statements.Add(returnStat);
					}
				}
				{
					auto literal = MakePtr<WfLiteralExpression>();
					literal->value = WfLiteralValue::False;

					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = literal;
					block->statements.Add(returnStat);
				}

				return func;
			}

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

				Dictionary<WfExpression*, WString> cacheNames;
				Group<WfExpression*, WString> handlerNames, callbackNames;
				Dictionary<WString, Ptr<ITypeInfo>> variableTypes;

				FOREACH_INDEXER(WfExpression*, observe, observeIndex, dependency.dependencies.Keys())
				{
					if (!observe) continue;
					List<IEventInfo*> events;
					WfExpression* parent = 0;
					DecodeObserveExpression(manager, observe, events, parent);

					WString cacheName = L"<bind-cache>" + itow(observeIndex);
					cacheNames.Add(observe, cacheName);
					{
						auto elementType = manager->expressionResolvings[parent].type;
						variableTypes.Add(cacheName, elementType);
						lambdaBlock->statements.Add(CreateBindWritableVariable(cacheName, elementType.Obj()));
					}

					FOREACH_INDEXER(IEventInfo*, ev, eventIndex, events)
					{
						WString handlerName = L"<bind-observe>" + itow(observeIndex) + L"_" + itow(eventIndex);
						{
							handlerNames.Add(observe, handlerName);
							auto elementType = TypeInfoRetriver<Ptr<IEventHandler>>::CreateTypeInfo();
							variableTypes.Add(handlerName, elementType);
							lambdaBlock->statements.Add(CreateBindWritableVariable(handlerName, elementType.Obj()));
						}
						
						WString callbackName = L"<bind-callback>" + itow(observeIndex) + L"_" + itow(eventIndex);
						{
							callbackNames.Add(observe, callbackName);
							auto elementType = CopyTypeInfo(ev->GetHandlerType());
							variableTypes.Add(callbackName, elementType);
							lambdaBlock->statements.Add(CreateBindWritableVariable(callbackName, elementType.Obj()));
						}
					}
				}

				lambdaBlock->statements.Add(CreateBindWritableVariable(L"<bind-closed>", TypeInfoRetriver<bool>::CreateTypeInfo().Obj()));
				{
					auto typeInfo = TypeInfoRetriver<Dictionary<IValueListener*, Func<void(Value)>>>::CreateTypeInfo();
					auto variable = MakePtr<WfVariableDeclaration>();
					variable->name.value = L"<bind-listeners>";
					variable->type = GetTypeFromTypeInfo(typeInfo.Obj());
					variable->expression = MakePtr<WfConstructorExpression>();

					auto variableStat = MakePtr<WfVariableStatement>();
					variableStat->variable = variable;
					lambdaBlock->statements.Add(variableStat);
				}
				lambdaBlock->statements.Add(CreateBindWritableVariable(L"<subscription>", TypeInfoRetriver<IValueSubscription*>::CreateTypeInfo().Obj()));
						
				auto newSubscription = MakePtr<WfNewTypeExpression>();
				{
					auto typeInfo = TypeInfoRetriver<Ptr<IValueSubscription>>::CreateTypeInfo();
					newSubscription->type = GetTypeFromTypeInfo(typeInfo.Obj());
				}
				newSubscription->functions.Add(CreateBindSubscribeFunction());
				newSubscription->functions.Add(CreateBindCloseFunction(variableTypes, handlerNames));

				{
					auto variable = MakePtr<WfVariableDeclaration>();
					variable->name.value = L"<subscription-shared>";
					variable->expression = newSubscription;
					
					auto variableStat = MakePtr<WfVariableStatement>();
					variableStat->variable = variable;
					lambdaBlock->statements.Add(variableStat);
				}
				{
					auto subscriptionRef = MakePtr<WfReferenceExpression>();
					subscriptionRef->name.value = L"<subscription-shared>";

					auto castExpr = MakePtr<WfTypeCastingExpression>();
					castExpr->strategy = WfTypeCastingStrategy::Strong;
					castExpr->expression = subscriptionRef;
					castExpr->type = GetTypeFromTypeInfo(TypeInfoRetriver<IValueSubscription*>::CreateTypeInfo().Obj());

					auto assign = MakePtr<WfBinaryExpression>();
					assign->op = WfBinaryOperator::Assign;
					assign->first = CreateBindVariableReference(L"<subscription>");
					assign->second = castExpr;

					auto stat = MakePtr<WfExpressionStatement>();
					stat->expression = assign;
					lambdaBlock->statements.Add(stat);
				}
				{
					auto subscriptionRef = MakePtr<WfReferenceExpression>();
					subscriptionRef->name.value = L"<subscription-shared>";

					auto returnStat = MakePtr<WfReturnStatement>();
					returnStat->expression = subscriptionRef;
					lambdaBlock->statements.Add(returnStat);
				}
			}
		}
	}
}