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

			void WfObservingDependency::Prepare(WfExpression* observe)
			{
				AddInternal(observe, 0);

				if (!outputObserves.Contains(observe))
				{
					outputObserves.Add(observe);
				}
			}

			void WfObservingDependency::AddInternal(WfExpression* observe, WfExpression* dependedObserve)
			{
				auto index = dependencies.Keys().IndexOf(observe);
				if (index == -1)
				{
					dependencies.Add(observe, dependedObserve);
				}
				else if (!dependencies.GetByIndex(index).Contains(dependedObserve))
				{
					dependencies.Add(observe, dependedObserve);
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
						
				auto newSubscription = MakePtr<WfNewTypeExpression>();
				node->expandedExpression = newSubscription;
				{
					auto systemType = MakePtr<WfTopQualifiedType>();
					systemType->name.value = L"system";

					auto subType = MakePtr<WfChildType>();
					subType->parent = systemType;
					subType->name.value = L"Subscription";

					auto ptrType = MakePtr<WfSharedPointerType>();
					ptrType->element = subType;
					newSubscription->type = ptrType;
				}
				{
					auto func = MakePtr<WfFunctionDeclaration>();
					newSubscription->functions.Add(func);
					func->name.value = L"Subscribe";
					{
						auto systemType = MakePtr<WfTopQualifiedType>();
						systemType->name.value = L"system";

						auto subType = MakePtr<WfChildType>();
						subType->parent = systemType;
						subType->name.value = L"Listener";

						auto ptrType = MakePtr<WfSharedPointerType>();
						ptrType->element = subType;
						func->returnType = ptrType;
					}
					{
						auto funcType = MakePtr<WfFunctionType>();
						{
							auto type = MakePtr<WfPredefinedType>();
							type->name = WfPredefinedTypeName::Void;
							funcType->result = type;
						}
						{
							auto type = MakePtr<WfPredefinedType>();
							type->name = WfPredefinedTypeName::Object;
							funcType->arguments.Add(type);
						}

						auto argument = MakePtr<WfFunctionArgument>();
						argument->name.value = L"<subscription-callback>";
						argument->type = funcType;
						func->arguments.Add(argument);
					}
					{
						auto ex = MakePtr<WfStringExpression>();
						ex->value.value = L"This function is not implemented.";

						auto raise = MakePtr<WfRaiseExceptionStatement>();
						raise->expression = ex;
						func->statement = raise;
					}
				}
				{
					auto func = MakePtr<WfFunctionDeclaration>();
					newSubscription->functions.Add(func);
					func->name.value = L"Close";
					{
						auto type = MakePtr<WfPredefinedType>();
						type->name = WfPredefinedTypeName::Bool;
						func->returnType = type;
					}
					{
						auto ex = MakePtr<WfStringExpression>();
						ex->value.value = L"This function is not implemented.";

						auto raise = MakePtr<WfRaiseExceptionStatement>();
						raise->expression = ex;
						func->statement = raise;
					}
				}
			}
		}
	}
}