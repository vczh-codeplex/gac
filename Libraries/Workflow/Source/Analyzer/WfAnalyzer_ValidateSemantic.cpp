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
ValidateSemantic(Declaration)
***********************************************************************/

			class ValidateSemanticDeclarationVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfLexicalScopeManager*				manager;

				ValidateSemanticDeclarationVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					FOREACH(Ptr<WfDeclaration>, declaration, node->declarations)
					{
						declaration->Accept(this);
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					ValidateStatementSemantic(manager, node->statement);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					auto scope = manager->declarationScopes[node];
					auto symbol = scope->symbols[node->name.value][0];
					symbol->typeInfo = GetExpressionType(manager, node->expression, symbol->typeInfo);
					if (symbol->typeInfo && !symbol->type)
					{
						symbol->type = GetTypeFromTypeInfo(symbol->typeInfo.Obj());
					}
				}

				static void Execute(Ptr<WfDeclaration> declaration, WfLexicalScopeManager* manager)
				{
					ValidateSemanticDeclarationVisitor visitor(manager);
					declaration->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateSemantic(Statement)
***********************************************************************/

			class ValidateSemanticStatementVisitor : public Object, public WfStatement::IVisitor
			{
			public:
				WfLexicalScopeManager*				manager;

				ValidateSemanticStatementVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
				{
				}

				void Visit(WfBreakStatement* node)override
				{
				}

				void Visit(WfContinueStatement* node)override
				{
				}

				void Visit(WfReturnStatement* node)override
				{
				}

				void Visit(WfDeleteStatement* node)override
				{
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
				}

				void Visit(WfIfStatement* node)override
				{
				}

				void Visit(WfSwitchStatement* node)override
				{
				}

				void Visit(WfWhileStatement* node)override
				{
				}

				void Visit(WfForEachStatement* node)override
				{
				}

				void Visit(WfTryStatement* node)override
				{
				}

				void Visit(WfBlockStatement* node)override
				{
					FOREACH(Ptr<WfStatement>, statement, node->statements)
					{
						statement->Accept(this);
					}
				}

				void Visit(WfExpressionStatement* node)override
				{
					GetExpressionType(manager, node->expression, 0);
				}

				void Visit(WfVariableStatement* node)override
				{
					ValidateDeclarationSemantic(manager, node->variable);
				}

				static void Execute(Ptr<WfStatement> statement, WfLexicalScopeManager* manager)
				{
					ValidateSemanticStatementVisitor visitor(manager);
					statement->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateSemantic(Expression)
***********************************************************************/

			class ValidateSemanticExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*				manager;
				Ptr<ITypeInfo>						expectedType;
				List<ResolveExpressionResult>&		results;

				ValidateSemanticExpressionVisitor(WfLexicalScopeManager* _manager, Ptr<ITypeInfo> _expectedType, List<ResolveExpressionResult>& _results)
					:manager(_manager)
					, expectedType(_expectedType)
					, results(_results)
				{
				}

				void Visit(WfTopQualifiedExpression* node)override
				{
					if (manager->globalName)
					{
						vint index = manager->globalName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							results.Add(ResolveExpressionResult(manager->globalName->children.Values()[index]));
							return;
						}
					}
					manager->errors.Add(WfErrors::TopQualifiedSymbolNotExists(node, node->name.value));
				}

				void Visit(WfReferenceExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();

					List<Ptr<WfLexicalSymbol>> symbols;
					manager->ResolveSymbol(scope, node->name.value, symbols);
					if (symbols.Count() >= 1)
					{
						FOREACH(Ptr<WfLexicalSymbol>, symbol, symbols)
						{
							if (symbol->typeInfo)
							{
								results.Add(ResolveExpressionResult(symbol, symbol->typeInfo));
							}
						}

						if (results.Count() == 0)
						{
							FOREACH(Ptr<WfLexicalSymbol>, symbol, symbols)
							{
								manager->errors.Add(WfErrors::ExpressionCannotResolveType(node, symbol));
							}
						}
						return;
					}

					List<Ptr<WfLexicalScopeName>> scopeNames;
					manager->ResolveScopeName(scope, node->name.value, scopeNames);
					if (scopeNames.Count() >= 1)
					{
						FOREACH(Ptr<WfLexicalScopeName>, scopeName, scopeNames)
						{
							results.Add(ResolveExpressionResult(scopeName));
						}
					}
					else
					{
						manager->errors.Add(WfErrors::ReferenceNotExists(node, node->name.value));
					}
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();

					List<Ptr<WfLexicalSymbol>> symbols;
					manager->ResolveSymbol(scope, node->name.value, symbols);
					if (symbols.Count() >= 1)
					{
						FOREACH(Ptr<WfLexicalSymbol>, symbol, symbols)
						{
							if (symbol->typeInfo)
							{
								results.Add(ResolveExpressionResult(symbol, symbol->typeInfo));
							}
						}
					}
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
					if (!expectedType)
					{
						manager->errors.Add(WfErrors::OrderedLambdaCannotResolveType(node));
						return;
					}
					{
						ITypeInfo* type = expectedType.Obj();
						if (type->GetDecorator() != ITypeInfo::SharedPtr) goto ORDERED_FAILED;
						type = type->GetElementType();
						if (type->GetDecorator() != ITypeInfo::Generic) goto ORDERED_FAILED;
						{
							ITypeInfo* functionType = type->GetElementType();
							if (functionType->GetDecorator() != ITypeInfo::TypeDescriptor) goto ORDERED_FAILED;
							if (functionType->GetTypeDescriptor() != description::GetTypeDescriptor<IValueFunctionProxy>()) goto ORDERED_FAILED;
						}

						auto scope = manager->expressionScopes[node].Obj();
						List<Ptr<WfLexicalSymbol>> parameterSymbols;
						CopyFrom(
							parameterSymbols,
							Range<vint>(0, scope->symbols.Count())
								.Select([scope](vint index)->Ptr<WfLexicalSymbol>{return scope->symbols.GetByIndex(index)[0];})
								.OrderBy([](Ptr<WfLexicalSymbol> a, Ptr<WfLexicalSymbol> b)
								{
									vint aId = wtoi(a->name.Sub(1, a->name.Length() - 1));
									vint bId = wtoi(b->name.Sub(1, a->name.Length() - 1));
									return aId - bId;
								})
							);
						if (type->GetGenericArgumentCount() != parameterSymbols.Count() + 1)
						{
							goto ORDERED_FAILED;
						}

						Ptr<ITypeInfo> resultType = type->GetGenericArgument(0);
						FOREACH_INDEXER(Ptr<WfLexicalSymbol>, symbol, index, parameterSymbols)
						{
							symbol->typeInfo = type->GetGenericArgument(index + 1);
							symbol->type = GetTypeFromTypeInfo(symbol->typeInfo.Obj());
						}
						GetExpressionType(manager, node->body, resultType);
					}

					goto ORDERED_FINISHED;
				ORDERED_FAILED:
					manager->errors.Add(WfErrors::OrderedLambdaCannotImplicitlyConvertToType(node, expectedType.Obj()));
				ORDERED_FINISHED:
					results.Add(ResolveExpressionResult(expectedType));
				}

				void Visit(WfMemberExpression* node)override
				{
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->parent, 0);
					if (type)
					{
						ITypeDescriptor* typeDescriptor = type->GetTypeDescriptor();
						if (IPropertyInfo* info = typeDescriptor->GetPropertyByName(node->name.value, true))
						{
							ResolveExpressionResult result(info, CopyTypeInfo(info->GetReturn()));
							results.Add(result);
						}
						if (IEventInfo* info = typeDescriptor->GetEventByName(node->name.value, true))
						{
							ResolveExpressionResult result(info);
							results.Add(result);
						}
						if (IMethodGroupInfo* groupInfo = typeDescriptor->GetMethodGroupByName(node->name.value, true))
						{
							vint count = groupInfo->GetMethodCount();
							for (vint i = 0; i < count; i++)
							{
								IMethodInfo* info = groupInfo->GetMethod(i);
								Ptr<TypeInfoImpl> functionType = new TypeInfoImpl(ITypeInfo::SharedPtr);
								{
									Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
									functionType->SetElementType(genericType);
									{
										Ptr<TypeInfoImpl> elementType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
										elementType->SetTypeDescriptor(description::GetTypeDescriptor<IValueFunctionProxy>());
										genericType->SetElementType(elementType);
									}

									genericType->AddGenericArgument(CopyTypeInfo(info->GetReturn()));
									vint parameterCount = info->GetParameterCount();
									for (vint j = 0; j < parameterCount; j++)
									{
										genericType->AddGenericArgument(CopyTypeInfo(info->GetParameter(j)->GetType()));
									}
								}
								ResolveExpressionResult result(info, functionType);
								results.Add(result);
							}
						}

						if (results.Count() == 0)
						{
							manager->errors.Add(WfErrors::MemberNotExists(node, typeDescriptor, node->name.value));
						}
					}
				}

				void Visit(WfChildExpression* node)override
				{
					if (Ptr<WfLexicalScopeName> scopeName = GetExpressionScopeName(manager, node->parent))
					{
						vint index = scopeName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							results.Add(ResolveExpressionResult(scopeName->children.Values()[index]));
							return;
						}
						manager->errors.Add(WfErrors::ChildSymbolNotExists(node, scopeName, node->name.value));
					}
				}

				void Visit(WfLiteralExpression* node)override
				{
					if (node->value == WfLiteralValue::Null)
					{
						if (!expectedType)
						{
							manager->errors.Add(WfErrors::NullCannotResolveType(node));
							return;
						}

						switch (expectedType->GetDecorator())
						{
						case ITypeInfo::RawPtr:
						case ITypeInfo::SharedPtr:
						case ITypeInfo::Nullable:
							break;
						case ITypeInfo::TypeDescriptor:
							if (expectedType->GetTypeDescriptor() != description::GetTypeDescriptor<Value>())
							{
								goto NULL_FAILED;
							}
							break;
						case ITypeInfo::Generic:
							goto NULL_FAILED;
						}

						goto NULL_FINISHED;
					NULL_FAILED:
						manager->errors.Add(WfErrors::NullCannotImplicitlyConvertToType(node, expectedType.Obj()));
					NULL_FINISHED:
						results.Add(ResolveExpressionResult(expectedType));
					}
					else
					{
						results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
					}
				}

				void Visit(WfFloatingExpression* node)override
				{
					results.Add(ResolveExpressionResult(TypeInfoRetriver<double>::CreateTypeInfo()));
				}

				void Visit(WfIntegerExpression* node)override
				{
					ITypeDescriptor* typeDescriptor = 0;
#ifndef VCZH_64
					typeDescriptor = description::GetTypeDescriptor<vint32_t>();
					if (typeDescriptor->GetValueSerializer()->Validate(node->value.value))
					{
						goto TYPE_FINISHED;
					}
#endif
					typeDescriptor = description::GetTypeDescriptor<vint64_t>();
					if (!typeDescriptor->GetValueSerializer()->Validate(node->value.value))
					{
						manager->errors.Add(WfErrors::IntegerLiteralOutOfRange(node));
					}
#ifndef VCZH_64
				TYPE_FINISHED:
#endif
					Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					typeInfo->SetTypeDescriptor(typeDescriptor);
					results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)typeInfo));
				}

				void Visit(WfStringExpression* node)override
				{
					results.Add(ResolveExpressionResult(TypeInfoRetriver<WString>::CreateTypeInfo()));
				}

				void Visit(WfFormatExpression* node)override
				{
					Ptr<ITypeInfo> typeInfo = TypeInfoRetriver<WString>::CreateTypeInfo();
					results.Add(ResolveExpressionResult(typeInfo));
					GetExpressionType(manager, node->expandedExpression, typeInfo);
				}

				void Visit(WfUnaryExpression* node)override
				{
				}

				void Visit(WfBinaryExpression* node)override
				{
				}

				void Visit(WfLetExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();

					FOREACH(Ptr<WfLetVariable>, variable, node->variables)
					{
						auto symbol = scope->symbols[variable->name.value][0];
						symbol->typeInfo = GetExpressionType(manager, variable->value, 0);
						if (symbol->typeInfo)
						{
							symbol->type = GetTypeFromTypeInfo(symbol->typeInfo.Obj());
						}
					}
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->expression, expectedType);
					if (type)
					{
						results.Add(ResolveExpressionResult(type));
					}
				}

				void Visit(WfIfExpression* node)override
				{
					Ptr<ITypeInfo> boolType = TypeInfoRetriver<bool>::CreateTypeInfo();
					GetExpressionType(manager, node->condition, boolType);

					Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->trueBranch, expectedType);
					Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->falseBranch, expectedType);

					if (firstType && !secondType)
					{
						results.Add(ResolveExpressionResult(firstType));
					}
					else if (!firstType && secondType)
					{
						results.Add(ResolveExpressionResult(secondType));
					}
					else if (firstType && secondType)
					{
						if (CanConvertToType(secondType.Obj(), firstType.Obj(), false))
						{
							results.Add(ResolveExpressionResult(firstType));
						}
						else if (CanConvertToType(firstType.Obj(), secondType.Obj(), false))
						{
							results.Add(ResolveExpressionResult(secondType));
						}
						else
						{
							manager->errors.Add(WfErrors::CannotMergeTwoType(node, firstType.Obj(), secondType.Obj()));
						}
					}
				}

				void Visit(WfRangeExpression* node)override
				{
					Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->begin, 0);
					Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->end, 0);
					Ptr<ITypeInfo> elementType;

					if (firstType && !secondType)
					{
						elementType = firstType;
					}
					else if (!firstType && secondType)
					{
						elementType = secondType;
					}
					else if (firstType && secondType)
					{
						if (CanConvertToType(secondType.Obj(), firstType.Obj(), false))
						{
							elementType = firstType;
						}
						else if (CanConvertToType(firstType.Obj(), secondType.Obj(), false))
						{
							elementType = secondType;
						}
						else
						{
							manager->errors.Add(WfErrors::CannotMergeTwoType(node, firstType.Obj(), secondType.Obj()));
						}
					}

					if (elementType)
					{
						Ptr<TypeInfoImpl> enumerableType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						enumerableType->SetTypeDescriptor(description::GetTypeDescriptor<IValueEnumerable>());

						Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
						genericType->SetElementType(enumerableType);
						genericType->AddGenericArgument(elementType);

						Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::SharedPtr);
						pointerType->SetElementType(genericType);
						results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)pointerType));
					}
				}

				void Visit(WfSetTestingExpression* node)override
				{
				}

				void Visit(WfConstructorExpression* node)override
				{
				}

				void Visit(WfInferExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();
					Ptr<ITypeInfo> type = CreateTypeInfoFromType(scope, node->type);
					Ptr<ITypeInfo> expressionType = GetExpressionType(manager, node->expression, type);
					if (expressionType)
					{
						results.Add(ResolveExpressionResult(type));
					}
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();
					Ptr<ITypeInfo> type = CreateTypeInfoFromType(scope, node->type);
					Ptr<ITypeInfo> expressionType = GetExpressionType(manager, node->expression, 0);
					if (type)
					{
						if (expressionType)
						{
							if (!CanConvertToType(expressionType.Obj(), type.Obj(), true))
							{
								manager->errors.Add(WfErrors::ExpressionCannotExplicitlyConvertToType(node->expression.Obj(), expressionType.Obj(), type.Obj()));
							}
						}
						if (node->strategy == WfTypeCastingStrategy::Weak)
						{
							switch (type->GetDecorator())
							{
							case ITypeInfo::RawPtr:
							case ITypeInfo::SharedPtr:
							case ITypeInfo::Nullable:
								break;
							default:
								manager->errors.Add(WfErrors::CannotWeakCastToType(node->expression.Obj(), type.Obj()));
							}
						}
						results.Add(ResolveExpressionResult(type));
					}
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					GetExpressionType(manager, node->expression, 0);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
					results.Add(ResolveExpressionResult(TypeInfoRetriver<ITypeDescriptor*>::CreateTypeInfo()));
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					GetExpressionType(manager, node->expression, 0);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<ITypeDescriptor*>::CreateTypeInfo()));
				}

				void Visit(WfAttachEventExpression* node)override
				{
					IEventInfo* eventInfo = GetExpressionEventInfo(manager, node->event);
					Ptr<ITypeInfo> functionType = CopyTypeInfo(eventInfo->GetHandlerType());
					GetExpressionType(manager, node->function, functionType);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<Ptr<IEventHandler>>::CreateTypeInfo()));
				}

				void Visit(WfDetachEventExpression* node)override
				{
					Ptr<ITypeInfo> pointerType = TypeInfoRetriver<Ptr<IEventHandler>>::CreateTypeInfo();
					GetExpressionType(manager, node->handler, pointerType);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
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
					ValidateDeclarationSemantic(manager, node->function);
					auto scope = manager->declarationScopes[node->function.Obj()].Obj();

					Ptr<TypeInfoImpl> functionType = new TypeInfoImpl(ITypeInfo::SharedPtr);
					{
						Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
						functionType->SetElementType(genericType);
						{
							Ptr<TypeInfoImpl> elementType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
							elementType->SetTypeDescriptor(description::GetTypeDescriptor<IValueFunctionProxy>());
							genericType->SetElementType(elementType);
						}

						genericType->AddGenericArgument(CreateTypeInfoFromType(scope, node->function->returnType));
						FOREACH(Ptr<WfFunctionArgument>, argument, node->function->arguments)
						{
							genericType->AddGenericArgument(scope->symbols[argument->name.value][0]->typeInfo);
						}
					}
					results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)functionType));
				}

				void Visit(WfNewTypeExpression* node)override
				{
				}

				static void Execute(Ptr<WfExpression> expression, WfLexicalScopeManager* manager, Ptr<ITypeInfo> expectedType, List<ResolveExpressionResult>& results)
				{
					ValidateSemanticExpressionVisitor visitor(manager, expectedType, results);
					expression->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateSemantic
***********************************************************************/

			void ValidateModuleSemantic(WfLexicalScopeManager* manager, Ptr<WfModule> module)
			{
				FOREACH(Ptr<WfDeclaration>, declaration, module->declarations)
				{
					ValidateDeclarationSemantic(manager, declaration);
				}
			}

			void ValidateDeclarationSemantic(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration)
			{
				return ValidateSemanticDeclarationVisitor::Execute(declaration, manager);
			}

			void ValidateStatementSemantic(WfLexicalScopeManager* manager, Ptr<WfStatement> statement)
			{
				return ValidateSemanticStatementVisitor::Execute(statement, manager);
			}

			void ValidateExpressionSemantic(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, collections::List<ResolveExpressionResult>& results)
			{
				ValidateSemanticExpressionVisitor::Execute(expression, manager, expectedType, results);
				for (vint i = results.Count() - 1; i >= 0; i--)
				{
					auto result = results[i];
					if (result.scopeName && result.scopeName->declarations.Count() > 0)
					{
						List<ResolveExpressionResult> replaces;
						FOREACH(Ptr<WfDeclaration>, decl, result.scopeName->declarations)
						{
							vint index = manager->declarationScopes.Keys().IndexOf(decl);
							if (index == -1) continue;
							auto scope = manager->declarationScopes.Values()[index]->parentScope;

							index = scope->symbols.Keys().IndexOf(decl->name.value);
							if (index == -1) continue;
							FOREACH(Ptr<WfLexicalSymbol>, symbol, scope->symbols.GetByIndex(index))
							{
								if (symbol->ownerDeclaration == decl && symbol->typeInfo)
								{
									replaces.Add(ResolveExpressionResult(symbol, symbol->typeInfo));
								}
							}
						}

						if (replaces.Count() > 0)
						{
							results.RemoveAt(i);
							FOREACH_INDEXER(ResolveExpressionResult, replaceResult, index, replaces)
							{
								results.Insert(i + index, replaceResult);
							}
						}
					}
				}
			}

/***********************************************************************
GetExpressionScopeName
***********************************************************************/

			Ptr<WfLexicalScopeName> GetExpressionScopeName(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				List<ResolveExpressionResult> results;
				ValidateExpressionSemantic(manager, expression, 0, results);
				if (results.Count() == 0) return 0;

				for (vint i = results.Count() - 1; i >= 0; i--)
				{
					auto& result = results[i];
					if (!result.scopeName)
					{
						results.RemoveAt(i);
					}
				}
				if (results.Count() == 0)
				{
					manager->errors.Add(WfErrors::ExpressionIsNotScopeName(expression.Obj()));
					return 0;
				}
				else if (results.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManyTargets(expression.Obj(), results, GetExpressionName(expression)));
					return 0;
				}
				return results[0].scopeName;
			}

/***********************************************************************
GetExpressionEventInfo
***********************************************************************/

			reflection::description::IEventInfo* GetExpressionEventInfo(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				List<ResolveExpressionResult> results;
				ValidateExpressionSemantic(manager, expression, 0, results);
				if (results.Count() == 0) return 0;

				for (vint i = results.Count() - 1; i >= 0; i--)
				{
					auto& result = results[i];
					if (!result.eventInfo)
					{
						results.RemoveAt(i);
					}
				}
				if (results.Count() == 0)
				{
					manager->errors.Add(WfErrors::ExpressionIsNotEvent(expression.Obj()));
					return 0;
				}
				else if (results.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManyTargets(expression.Obj(), results, GetExpressionName(expression)));
					return 0;
				}
				return results[0].eventInfo;
			}

/***********************************************************************
GetExpressionType
***********************************************************************/

			Ptr<reflection::description::ITypeInfo> GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				List<ResolveExpressionResult> results;
				ValidateExpressionSemantic(manager, expression, expectedType, results);
				if (results.Count() == 0) return expectedType;

				Ptr<WfLexicalScopeName> scopeName;
				IEventInfo* eventInfo = 0;
				for (vint i = results.Count() - 1; i >= 0; i--)
				{
					auto& result = results[i];
					if (result.scopeName && !scopeName)
					{
						scopeName = result.scopeName;
					}
					if (result.eventInfo && !eventInfo)
					{
						eventInfo = result.eventInfo;
					}
					if (!result.type)
					{
						results.RemoveAt(i);
					}
				}
				if (results.Count() == 0)
				{
					if (scopeName)
					{
						manager->errors.Add(WfErrors::ScopeNameIsNotExpression(expression.Obj(), scopeName));
					}
					if (eventInfo)
					{
						manager->errors.Add(WfErrors::EventIsNotExpression(expression.Obj(), eventInfo));
					}
					return expectedType;
				}

				if (expectedType)
				{
					List<Ptr<ITypeInfo>> failedTypes;
					for (vint i = results.Count() - 1; i >= 0; i--)
					{
						auto& result = results[i];
						if (!CanConvertToType(result.type.Obj(), expectedType.Obj(), false))
						{
							failedTypes.Add(result.type);
							results.RemoveAt(i);
						}
					}

					if (results.Count() == 0)
					{
						FOREACH(Ptr<ITypeInfo>, type, failedTypes)
						{
							manager->errors.Add(WfErrors::ExpressionCannotImplicitlyConvertToType(expression.Obj(), type.Obj(), expectedType.Obj()));
						}
						return expectedType;
					}
				}

				if (results.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManyTargets(expression.Obj(), results, GetExpressionName(expression)));
					return expectedType;
				}
				else
				{
					return expectedType ? expectedType : results[0].type;
				}
			}
		}
	}
}