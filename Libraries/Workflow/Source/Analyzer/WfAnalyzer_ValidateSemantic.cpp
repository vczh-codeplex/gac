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
					auto scope = manager->statementScopes[node].Obj();
					auto decl = scope->FindDeclaration().Cast<WfFunctionDeclaration>();
					auto returnType = CreateTypeInfoFromType(scope, decl->returnType);
					if (node->expression)
					{
						GetExpressionType(manager, node->expression, returnType);
					}
					else if (returnType->GetDecorator() != ITypeInfo::TypeDescriptor || returnType->GetTypeDescriptor() != description::GetTypeDescriptor<void>())
					{
						manager->errors.Add(WfErrors::ReturnMissExpression(node, returnType.Obj()));
					}
				}

				void Visit(WfDeleteStatement* node)override
				{
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->expression, 0);
					if (type)
					{
						if (type->GetDecorator() != ITypeInfo::RawPtr)
						{
							manager->errors.Add(WfErrors::DeleteNonRawPointer(node, type.Obj()));
						}
					}
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					if (node->expression)
					{
						Ptr<ITypeInfo> stringType = TypeInfoRetriver<WString>::CreateTypeInfo();
						GetExpressionType(manager, node->expression, stringType);
					}
				}

				void Visit(WfIfStatement* node)override
				{
					if (node->type)
					{
						auto scope = manager->statementScopes[node].Obj();
						auto symbol = scope->symbols[node->name.value][0];
						if (!IsNullAcceptableType(symbol->typeInfo.Obj()))
						{
							manager->errors.Add(WfErrors::NullCannotImplicitlyConvertToType(node->expression.Obj(), symbol->typeInfo.Obj()));
						}
						GetExpressionType(manager, node->expression, 0);
					}
					else
					{
						Ptr<ITypeInfo> boolType = TypeInfoRetriver<bool>::CreateTypeInfo();
						GetExpressionType(manager, node->expression, boolType);
					}
					ValidateStatementSemantic(manager, node->trueBranch);
					if (node->falseBranch)
					{
						ValidateStatementSemantic(manager, node->falseBranch);
					}
				}

				void Visit(WfSwitchStatement* node)override
				{
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->expression, 0);
					FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
					{
						Ptr<ITypeInfo> caseType;
						if (IsExpressionDependOnExpectedType(manager, switchCase->expression))
						{
							caseType = GetExpressionType(manager, switchCase->expression, type);
						}
						else
						{
							caseType = GetExpressionType(manager, switchCase->expression, 0);
						}

						if (type && caseType)
						{
							if (!GetMergedType(type, caseType))
							{
								manager->errors.Add(WfErrors::CannotMergeTwoType(switchCase->expression.Obj(), type.Obj(), caseType.Obj()));
							}
						}
						ValidateStatementSemantic(manager, switchCase->statement);
					}
					if (node->defaultBranch)
					{
						ValidateStatementSemantic(manager, node->defaultBranch);
					}
				}

				void Visit(WfWhileStatement* node)override
				{
					Ptr<ITypeInfo> boolType = TypeInfoRetriver<bool>::CreateTypeInfo();
					GetExpressionType(manager, node->condition, boolType);
					ValidateStatementSemantic(manager, node->statement);
				}

				void Visit(WfForEachStatement* node)override
				{
					Ptr<ITypeInfo> elementType = GetEnumerableExpressionItemType(manager, node->collection, 0);
					if (elementType)
					{
						auto scope = manager->statementScopes[node].Obj();
						auto symbol = scope->symbols[node->name.value][0];
						symbol->typeInfo = elementType;
						symbol->type = GetTypeFromTypeInfo(elementType.Obj());
					}
					ValidateStatementSemantic(manager, node->statement);
				}

				void Visit(WfTryStatement* node)override
				{
					ValidateStatementSemantic(manager, node->protectedStatement);
					if (node->catchStatement)
					{
						ValidateStatementSemantic(manager, node->catchStatement);
					}
					if (node->finallyStatement)
					{
						ValidateStatementSemantic(manager, node->finallyStatement);
					}
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

				void VisitSymbol(WfLexicalScope* scope, Ptr<WfLexicalSymbol> symbol)
				{
					if (symbol->typeInfo)
					{
						bool captured = false;
						if (!symbol->ownerScope->ownerDeclaration.Cast<WfNamespaceDeclaration>())
						{
							auto currentScope = scope;
							while (currentScope)
							{
								vint index = currentScope->symbols.Keys().IndexOf(symbol->name);
								if (index != -1 && currentScope->symbols.GetByIndex(index).Contains(symbol.Obj()))
								{
									break;
								}
								if (auto node = currentScope->ownerDeclaration.Cast<WfFunctionDeclaration>())
								{
									captured = true;
									vint index = manager->functionLambdaCaptures.Keys().IndexOf(node.Obj());
									if (index == -1 || !manager->functionLambdaCaptures.GetByIndex(index).Contains(symbol.Obj()))
									{
										manager->functionLambdaCaptures.Add(node.Obj(), symbol);
									}
								}
								if (auto node = currentScope->ownerExpression.Cast<WfOrderedLambdaExpression>())
								{
									captured = true;
									vint index = manager->orderedLambdaCaptures.Keys().IndexOf(node.Obj());
									if (index == -1 || !manager->orderedLambdaCaptures.GetByIndex(index).Contains(symbol.Obj()))
									{
										manager->orderedLambdaCaptures.Add(node.Obj(), symbol);
									}
								}
								currentScope = currentScope->parentScope.Obj();
							}
						}

						bool writable = false;
						if (symbol->creatorDeclaration.Cast<WfVariableDeclaration>())
						{
							writable = !captured;
						}

						if (writable)
						{
							results.Add(ResolveExpressionResult(symbol, symbol->typeInfo, symbol->typeInfo));
						}
						else
						{
							results.Add(ResolveExpressionResult(symbol, symbol->typeInfo));
						}
					}
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
							VisitSymbol(scope, symbol);
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
					FOREACH(Ptr<WfLexicalSymbol>, symbol, symbols)
					{
						VisitSymbol(scope, symbol);
					}
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
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
					Ptr<ITypeInfo> resultType = expectedType;

					if (!expectedType && parameterSymbols.Count() > 0)
					{
						manager->errors.Add(WfErrors::OrderedLambdaCannotResolveType(node));
						return;
					}
					else if (expectedType)
					{
						ITypeInfo* type = expectedType.Obj();
						if (type->GetDecorator() != ITypeInfo::SharedPtr)
						{
							goto ORDERED_FAILED;
						}
						type = type->GetElementType();
						if (type->GetDecorator() != ITypeInfo::Generic)
						{
							goto ORDERED_FAILED;
						}
						{
							ITypeInfo* functionType = type->GetElementType();
							if (functionType->GetDecorator() != ITypeInfo::TypeDescriptor)
							{
								goto ORDERED_FAILED;
							}
							if (functionType->GetTypeDescriptor() != description::GetTypeDescriptor<IValueFunctionProxy>())
							{
								goto ORDERED_FAILED;
							}
						}

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
					else
					{
						auto bodyType = GetExpressionType(manager, node->body, 0);
						if (bodyType)
						{
							Ptr<TypeInfoImpl> funcType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
							funcType->SetTypeDescriptor(description::GetTypeDescriptor<IValueFunctionProxy>());

							Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
							genericType->SetElementType(funcType);
							genericType->AddGenericArgument(bodyType);

							Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::SharedPtr);
							pointerType->SetElementType(genericType);
							resultType = pointerType;
						}
					}

					goto ORDERED_FINISHED;
				ORDERED_FAILED:
					manager->errors.Add(WfErrors::OrderedLambdaCannotImplicitlyConvertToType(node, expectedType.Obj()));
				ORDERED_FINISHED:
					if (resultType)
					{
						results.Add(ResolveExpressionResult(resultType));
					}
				}

				void Visit(WfMemberExpression* node)override
				{
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->parent, 0);
					if (type)
					{
						ITypeDescriptor* typeDescriptor = type->GetTypeDescriptor();
						if (IPropertyInfo* info = typeDescriptor->GetPropertyByName(node->name.value, true))
						{
							Ptr<ITypeInfo> getterType = CopyTypeInfo(info->GetReturn());
							Ptr<ITypeInfo> setterType;
							if (IMethodInfo* setter = info->GetSetter())
							{
								setterType = getterType;
								if (setter->GetParameterCount() == 1 && !IsSameType(getterType.Obj(), setter->GetParameter(0)->GetType()))
								{
									setterType = CopyTypeInfo(setter->GetParameter(0)->GetType());
								}
							}
							ResolveExpressionResult result(info, getterType, setterType);
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
								ResolveExpressionResult result(info, CreateTypeInfoFromMethodInfo(info));
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

						if (scopeName->typeDescriptor)
						{
							if (auto group = scopeName->typeDescriptor->GetMethodGroupByName(node->name.value, true))
							{
								bool found = false;
								for (vint i = 0; i < group->GetMethodCount(); i++)
								{
									auto info = group->GetMethod(i);
									if (info->IsStatic())
									{
										found = true;
										results.Add(ResolveExpressionResult(info, CreateTypeInfoFromMethodInfo(info)));
									}
								}

								if (found)
								{
									return;
								}
							}
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
						}
						else if (!IsNullAcceptableType(expectedType.Obj()))
						{
							manager->errors.Add(WfErrors::NullCannotImplicitlyConvertToType(node, expectedType.Obj()));
						}

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

					typeDescriptor = description::GetTypeDescriptor<vuint32_t>();
					if (typeDescriptor->GetValueSerializer()->Validate(node->value.value))
					{
						goto TYPE_FINISHED;
					}
#endif
					typeDescriptor = description::GetTypeDescriptor<vint64_t>();
					if (typeDescriptor->GetValueSerializer()->Validate(node->value.value))
					{
						goto TYPE_FINISHED;
					}

					typeDescriptor = description::GetTypeDescriptor<vuint64_t>();
					if (typeDescriptor->GetValueSerializer()->Validate(node->value.value))
					{
						goto TYPE_FINISHED;
					}

					manager->errors.Add(WfErrors::IntegerLiteralOutOfRange(node));

				TYPE_FINISHED:
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
					Ptr<ITypeInfo> typeInfo = GetExpressionType(manager, node->operand, 0);
					if (typeInfo)
					{
						TypeFlag flag = GetTypeFlag(typeInfo.Obj());
						switch (node->op)
						{
						case WfUnaryOperator::Not:
							switch (flag)
							{
							case TypeFlag::F4:
							case TypeFlag::F8:
							case TypeFlag::String:
							case TypeFlag::Others:
								manager->errors.Add(WfErrors::UnaryOperatorOnWrongType(node, typeInfo.Obj()));
								break;
							default:;
							}
							break;
						case WfUnaryOperator::Positive:
							switch (flag)
							{
							case TypeFlag::Bool:
							case TypeFlag::String:
							case TypeFlag::Others:
								manager->errors.Add(WfErrors::UnaryOperatorOnWrongType(node, typeInfo.Obj()));
								break;
							default:;
							}
							break;
						case WfUnaryOperator::Negative:
							switch (flag)
							{
							case TypeFlag::Bool:
							case TypeFlag::U1:
							case TypeFlag::U2:
							case TypeFlag::U4:
							case TypeFlag::U8:
							case TypeFlag::String:
							case TypeFlag::Others:
								manager->errors.Add(WfErrors::UnaryOperatorOnWrongType(node, typeInfo.Obj()));
								break;
							default:;
							}
							break;
						}

						results.Add(ResolveExpressionResult(typeInfo));
					}
				}

				void Visit(WfBinaryExpression* node)override
				{

					if (node->op == WfBinaryOperator::Assign)
					{
						Ptr<ITypeInfo> variableType = GetLeftValueExpressionType(manager, node->first);
						GetExpressionType(manager, node->second, variableType);
						if (variableType)
						{
							results.Add(ResolveExpressionResult(variableType));
						}
					}
					else if (node->op == WfBinaryOperator::Index)
					{
						Ptr<ITypeInfo> containerType = GetExpressionType(manager, node->first, 0);
						if (containerType)
						{
							if (containerType->GetDecorator() == ITypeInfo::SharedPtr)
							{
								ITypeInfo* genericType = containerType->GetElementType();
								Ptr<ITypeInfo> indexType;
								Ptr<ITypeInfo> resultType;
								bool leftValue = false;

								if (genericType->GetDecorator() == ITypeInfo::Generic)
								{
									ITypeInfo* classType = genericType->GetElementType();
									if (classType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyList>())
									{
										indexType = TypeInfoRetriver<vint>::CreateTypeInfo();
										resultType = CopyTypeInfo(genericType->GetGenericArgument(0));
									}
									else if (classType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueList>())
									{
										indexType = TypeInfoRetriver<vint>::CreateTypeInfo();
										resultType = CopyTypeInfo(genericType->GetGenericArgument(0));
										leftValue = true;
									}
									else if (classType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyDictionary>())
									{
										indexType = CopyTypeInfo(genericType->GetGenericArgument(0));
										resultType = CopyTypeInfo(genericType->GetGenericArgument(1));
									}
									else if (classType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueDictionary>())
									{
										indexType = CopyTypeInfo(genericType->GetGenericArgument(0));
										resultType = CopyTypeInfo(genericType->GetGenericArgument(1));
										leftValue = true;
									}
									else
									{
										manager->errors.Add(WfErrors::IndexOperatorOnWrongType(node, containerType.Obj()));
									}
								}
								else
								{
									if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyList>())
									{
										indexType = TypeInfoRetriver<vint>::CreateTypeInfo();
										resultType = TypeInfoRetriver<Value>::CreateTypeInfo();
									}
									else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueList>())
									{
										indexType = TypeInfoRetriver<vint>::CreateTypeInfo();
										resultType = TypeInfoRetriver<Value>::CreateTypeInfo();
										leftValue = true;
									}
									else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyDictionary>())
									{
										indexType = TypeInfoRetriver<Value>::CreateTypeInfo();
										resultType = TypeInfoRetriver<Value>::CreateTypeInfo();
									}
									else if (genericType->GetTypeDescriptor() == description::GetTypeDescriptor<IValueDictionary>())
									{
										indexType = TypeInfoRetriver<Value>::CreateTypeInfo();
										resultType = TypeInfoRetriver<Value>::CreateTypeInfo();
										leftValue = true;
									}
									else
									{
										manager->errors.Add(WfErrors::IndexOperatorOnWrongType(node, containerType.Obj()));
									}
								}

								GetExpressionType(manager, node->second, indexType);
								if (resultType)
								{
									if (leftValue)
									{
										results.Add(ResolveExpressionResult(resultType, resultType));
									}
									else
									{
										results.Add(ResolveExpressionResult(resultType));
									}
								}
							}
							else
							{
								manager->errors.Add(WfErrors::IndexOperatorOnWrongType(node, containerType.Obj()));
							}
						}
					}
					else if (node->op == WfBinaryOperator::Concat)
					{
						Ptr<ITypeInfo> stringType = TypeInfoRetriver<WString>::CreateTypeInfo();
						GetExpressionType(manager, node->first, stringType);
						GetExpressionType(manager, node->second, stringType);
						results.Add(ResolveExpressionResult(stringType));
					}
					else if (node->op == WfBinaryOperator::FailedThen)
					{
						Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->first, 0);
						bool depend = IsExpressionDependOnExpectedType(manager, node->second);
						Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->second, (depend ? firstType : nullptr));

						if (firstType && secondType)
						{
							auto mergedType = GetMergedType(firstType, secondType);
							results.Add(ResolveExpressionResult(mergedType ? mergedType : firstType));
						}
					}
					else
					{
						Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->first, 0);
						Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->second, 0);
						Ptr<ITypeInfo> elementType;
						if (firstType && secondType)
						{
							if (!(elementType = GetMergedType(firstType, secondType)))
							{
								manager->errors.Add(WfErrors::CannotMergeTwoType(node, firstType.Obj(), secondType.Obj()));
							}
						}

						if (elementType)
						{
							TypeFlag flag = GetTypeFlag(elementType.Obj());
							TypeFlag* selectedTable = 0;
							switch (node->op)
							{
							case WfBinaryOperator::Exp:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Unknown,
										/*I1		*/TypeFlag::F4,
										/*I2		*/TypeFlag::F4,
										/*I4		*/TypeFlag::F8,
										/*I8		*/TypeFlag::F8,
										/*U1		*/TypeFlag::F4,
										/*U2		*/TypeFlag::F4,
										/*U4		*/TypeFlag::F8,
										/*U8		*/TypeFlag::F8,
										/*F4		*/TypeFlag::F4,
										/*F8		*/TypeFlag::F8,
										/*String	*/TypeFlag::Unknown,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
							case WfBinaryOperator::Add:
							case WfBinaryOperator::Sub:
							case WfBinaryOperator::Mul:
							case WfBinaryOperator::Div:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Unknown,
										/*I1		*/TypeFlag::I,
										/*I2		*/TypeFlag::I,
										/*I4		*/TypeFlag::I,
										/*I8		*/TypeFlag::I8,
										/*U1		*/TypeFlag::U,
										/*U2		*/TypeFlag::U,
										/*U4		*/TypeFlag::U,
										/*U8		*/TypeFlag::U8,
										/*F4		*/TypeFlag::F4,
										/*F8		*/TypeFlag::F8,
										/*String	*/TypeFlag::Unknown,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
							case WfBinaryOperator::Mod:
							case WfBinaryOperator::Shl:
							case WfBinaryOperator::Shr:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Unknown,
										/*I1		*/TypeFlag::I,
										/*I2		*/TypeFlag::I,
										/*I4		*/TypeFlag::I,
										/*I8		*/TypeFlag::I8,
										/*U1		*/TypeFlag::U,
										/*U2		*/TypeFlag::U,
										/*U4		*/TypeFlag::U,
										/*U8		*/TypeFlag::U8,
										/*F4		*/TypeFlag::Unknown,
										/*F8		*/TypeFlag::Unknown,
										/*String	*/TypeFlag::Unknown,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
							case WfBinaryOperator::LT:
							case WfBinaryOperator::GT:
							case WfBinaryOperator::LE:
							case WfBinaryOperator::GE:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Unknown,
										/*I1		*/TypeFlag::Bool,
										/*I2		*/TypeFlag::Bool,
										/*I4		*/TypeFlag::Bool,
										/*I8		*/TypeFlag::Bool,
										/*U1		*/TypeFlag::Bool,
										/*U2		*/TypeFlag::Bool,
										/*U4		*/TypeFlag::Bool,
										/*U8		*/TypeFlag::Bool,
										/*F4		*/TypeFlag::Bool,
										/*F8		*/TypeFlag::Bool,
										/*String	*/TypeFlag::Bool,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
							case WfBinaryOperator::EQ:
							case WfBinaryOperator::NE:
								{
									results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
									return;
								}
								break;
							case WfBinaryOperator::Xor:
							case WfBinaryOperator::And:
							case WfBinaryOperator::Or:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Bool,
										/*I1		*/TypeFlag::I1,
										/*I2		*/TypeFlag::I2,
										/*I4		*/TypeFlag::I4,
										/*I8		*/TypeFlag::I8,
										/*U1		*/TypeFlag::U1,
										/*U2		*/TypeFlag::U2,
										/*U4		*/TypeFlag::U4,
										/*U8		*/TypeFlag::U8,
										/*F4		*/TypeFlag::Unknown,
										/*F8		*/TypeFlag::Unknown,
										/*String	*/TypeFlag::Unknown,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
							default:;
							}

							TypeFlag resultFlag = selectedTable[(vint)flag];
							if (resultFlag == TypeFlag::Unknown)
							{
								manager->errors.Add(WfErrors::BinaryOperatorOnWrongType(node, elementType.Obj()));
							}
							else
							{
								results.Add(ResolveExpressionResult(CreateTypeInfoFromTypeFlag(resultFlag)));
							}
						}
					}
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

					Ptr<ITypeInfo> firstType, secondType;

					if (expectedType)
					{
						firstType = GetExpressionType(manager, node->trueBranch, expectedType);
						secondType = GetExpressionType(manager, node->falseBranch, expectedType);
					}
					else
					{
						bool resolveFirst = !IsExpressionDependOnExpectedType(manager, node->trueBranch);
						bool resolveSecond = !IsExpressionDependOnExpectedType(manager, node->falseBranch);

						if (resolveFirst == resolveSecond)
						{
							firstType = GetExpressionType(manager, node->trueBranch, 0);
							secondType = GetExpressionType(manager, node->falseBranch, 0);
						}
						else if (resolveFirst)
						{
							firstType = GetExpressionType(manager, node->trueBranch, 0);
							secondType = GetExpressionType(manager, node->falseBranch, firstType);
						}
						else if (resolveSecond)
						{
							secondType = GetExpressionType(manager, node->falseBranch, 0);
							firstType = GetExpressionType(manager, node->trueBranch, secondType);
						}
					}
					
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
						if (auto mergedType = GetMergedType(firstType, secondType))
						{
							results.Add(ResolveExpressionResult(mergedType));
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
						if (!(elementType = GetMergedType(firstType, secondType)))
						{
							manager->errors.Add(WfErrors::CannotMergeTwoType(node, firstType.Obj(), secondType.Obj()));
						}
					}

					if (elementType)
					{
						TypeFlag flag = GetTypeFlag(elementType.Obj());
						switch (flag)
						{
						case TypeFlag::I1:
						case TypeFlag::I2:
						case TypeFlag::I4:
						case TypeFlag::I8:
						case TypeFlag::U1:
						case TypeFlag::U2:
						case TypeFlag::U4:
						case TypeFlag::U8:
							break;
						default:
							manager->errors.Add(WfErrors::RangeShouldBeInteger(node, elementType.Obj()));
						}

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
					Ptr<ITypeInfo> elementType = GetExpressionType(manager, node->element, 0);

					if (auto range = node->collection.Cast<WfRangeExpression>())
					{
						Ptr<ITypeInfo> beginType = GetExpressionType(manager, range->begin, 0);
						Ptr<ITypeInfo> endType = GetExpressionType(manager, range->end, 0);

						if (elementType && beginType)
						{
							if (!GetMergedType(elementType, beginType))
							{
								manager->errors.Add(WfErrors::CannotMergeTwoType(node, elementType.Obj(), beginType.Obj()));
							}
						}
						if (elementType && endType)
						{
							if (!GetMergedType(elementType, endType))
							{
								manager->errors.Add(WfErrors::CannotMergeTwoType(node, elementType.Obj(), endType.Obj()));
							}
						}
					}
					else
					{
						Ptr<ITypeInfo> itemType = GetEnumerableExpressionItemType(manager, node->collection, 0);
						if (elementType && itemType)
						{
							if (!GetMergedType(elementType, itemType))
							{
								manager->errors.Add(WfErrors::CannotMergeTwoType(node, elementType.Obj(), itemType.Obj()));
							}
						}
					}

					results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
				}

				void Visit(WfConstructorExpression* node)override
				{
					if (node->arguments.Count() == 0)
					{
						if (expectedType)
						{
							ITypeDescriptor* td = expectedType->GetTypeDescriptor();
							if (!td->CanConvertTo(description::GetTypeDescriptor<IValueEnumerable>()) && !td->CanConvertTo(description::GetTypeDescriptor<IValueReadonlyDictionary>()))
							{
								manager->errors.Add(WfErrors::ConstructorCannotImplicitlyConvertToType(node, expectedType.Obj()));
							}
							results.Add(ResolveExpressionResult(expectedType));
						}
						else
						{
							manager->errors.Add(WfErrors::ConstructorCannotResolveType(node));
						}
					}
					else
					{
						bool map = node->arguments[0]->value;
						Ptr<ITypeInfo> keyType, valueType;
						FOREACH(Ptr<WfConstructorArgument>, argument, node->arguments)
						{
							{
								Ptr<ITypeInfo> newKeyType = GetExpressionType(manager, argument->key, 0);
								if (!keyType)
								{
									keyType = newKeyType;
								}
								else if (auto mergedType = GetMergedType(keyType, newKeyType))
								{
									keyType = mergedType;
								}
								else
								{
									manager->errors.Add(WfErrors::CannotMergeTwoType(node, keyType.Obj(), newKeyType.Obj()));
								}
							}
							if (map)
							{
								Ptr<ITypeInfo> newValueType = GetExpressionType(manager, argument->value, 0);
								if (!valueType)
								{
									valueType = newValueType;
								}
								else if (auto mergedType = GetMergedType(valueType, newValueType))
								{
									valueType = mergedType;
								}
								else
								{
									manager->errors.Add(WfErrors::CannotMergeTwoType(node, valueType.Obj(), newValueType.Obj()));
								}
							}
						}

						if (map)
						{
							if (keyType && valueType)
							{
								Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::SharedPtr);
								{
									Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
									pointerType->SetElementType(genericType);
									{
										Ptr<TypeInfoImpl> classType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
										classType->SetTypeDescriptor(description::GetTypeDescriptor<IValueDictionary>());
										genericType->SetElementType(classType);
									}
									genericType->AddGenericArgument(keyType);
									genericType->AddGenericArgument(valueType);
								}
								results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)pointerType));
							}
						}
						else
						{
							if (keyType)
							{
								Ptr<TypeInfoImpl> pointerType = new TypeInfoImpl(ITypeInfo::SharedPtr);
								{
									Ptr<TypeInfoImpl> genericType = new TypeInfoImpl(ITypeInfo::Generic);
									pointerType->SetElementType(genericType);
									{
										Ptr<TypeInfoImpl> classType = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
										classType->SetTypeDescriptor(description::GetTypeDescriptor<IValueList>());
										genericType->SetElementType(classType);
									}
									genericType->AddGenericArgument(keyType);
								}
								results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)pointerType));
							}
						}
					}
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
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->expression, 0);
					if (type)
					{
						switch (node->test)
						{
						case WfTypeTesting::IsNull:
						case WfTypeTesting::IsNotNull:
							if (!IsNullAcceptableType(type.Obj()))
							{
								manager->errors.Add(WfErrors::NullCannotImplicitlyConvertToType(node->expression.Obj(), type.Obj()));
							}
							break;
						default:;
						}
					}
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
					Ptr<ITypeInfo> functionType;
					if (eventInfo)
					{
						functionType = CopyTypeInfo(eventInfo->GetHandlerType());
					}
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
					vint errorCount = manager->errors.Count();
					GetExpressionType(manager, node->expression, 0);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<Ptr<IValueSubscription>>::CreateTypeInfo()));

					if (manager->errors.Count() == errorCount)
					{
						ExpandBindExpression(manager, node);
						auto parentScope = manager->expressionScopes[node];
						BuildScopeForExpression(manager, parentScope, node->expandedExpression);
						if (manager->CheckScopes())
						{
							GetExpressionType(manager, node->expandedExpression, 0);
						}
					}
				}

				void Visit(WfObserveExpression* node)override
				{
					Ptr<ITypeInfo> parentType = GetExpressionType(manager, node->parent, 0);
					Ptr<ITypeInfo> observeeType;
					if (parentType)
					{
						if (node->observeType == WfObserveType::SimpleObserve)
						{
							ITypeDescriptor* td = parentType->GetTypeDescriptor();
							IPropertyInfo* propertyInfo = 0;
							{
								auto ref = node->expression.Cast<WfReferenceExpression>();
								propertyInfo = td->GetPropertyByName(ref->name.value, true);
								if (propertyInfo)
								{
									observeeType = CopyTypeInfo(propertyInfo->GetReturn());
								}
								else
								{
									manager->errors.Add(WfErrors::MemberNotExists(ref.Obj(), td, ref->name.value));
								}
							}

							if (node->events.Count() == 0)
							{
								if (propertyInfo)
								{
									IEventInfo* eventInfo = propertyInfo->GetValueChangedEvent();
									if (!eventInfo)
									{
										eventInfo = td->GetEventByName(propertyInfo->GetName() + L"Changed", true);
									}
									if (!eventInfo)
									{
										manager->errors.Add(WfErrors::MemberNotExists(node->expression.Obj(), td, propertyInfo->GetName() + L"Changed"));
									}
								}
							}
							else
							{
								FOREACH(Ptr<WfExpression>, eventExpr, node->events)
								{
									auto ref = eventExpr.Cast<WfReferenceExpression>();
									IEventInfo* info = td->GetEventByName(ref->name.value, true);
									if (!info)
									{
										manager->errors.Add(WfErrors::MemberNotExists(ref.Obj(), td, ref->name.value));
									}
								}
							}
						}
						else
						{
							auto scope = manager->expressionScopes[node].Obj();
							auto symbol = scope->symbols[node->name.value][0];
							symbol->typeInfo = parentType;
							symbol->type = GetTypeFromTypeInfo(parentType.Obj());

							observeeType = GetExpressionType(manager, node->expression, 0);
							FOREACH(Ptr<WfExpression>, eventExpr, node->events)
							{
								GetExpressionEventInfo(manager, eventExpr);
							}
						}
					}

					if (observeeType)
					{
						results.Add(ResolveExpressionResult(observeeType));
					}
				}

				Ptr<ITypeInfo> SelectFunction(WfExpression* node, Ptr<WfExpression> functionExpression, List<ResolveExpressionResult>& functions, List<Ptr<WfExpression>>& arguments)
				{
					List<bool> resolvables;
					List<Ptr<ITypeInfo>> types;
					FOREACH(Ptr<WfExpression>, argument, arguments)
					{
						if (IsExpressionDependOnExpectedType(manager, argument))
						{
							resolvables.Add(false);
							types.Add(0);
						}
						else
						{
							resolvables.Add(true);
							types.Add(GetExpressionType(manager, argument, 0));
						}
					}

					List<Ptr<parsing::ParsingError>> functionErrors, nonFunctionErrors;
					ITypeDescriptor* functionFd = description::GetTypeDescriptor<IValueFunctionProxy>();
					for (vint i = functions.Count() - 1; i >= 0; i--)
					{
						bool failed = false;
						auto result = functions[i];
						if (result.type->GetDecorator() == ITypeInfo::SharedPtr)
						{
							ITypeInfo* genericType = result.type->GetElementType();
							if (genericType->GetDecorator() != ITypeInfo::Generic) goto FUNCTION_TYPE_FAILED;
							ITypeInfo* functionType = genericType->GetElementType();
							if (functionType->GetDecorator() != ITypeInfo::TypeDescriptor || functionType->GetTypeDescriptor() != functionFd) goto FUNCTION_TYPE_FAILED;
								
							if (genericType->GetGenericArgumentCount() != types.Count() + 1)
							{
								functionErrors.Add(WfErrors::FunctionArgumentCountMismatched(node, arguments.Count(), result));
								failed = true;
							}
							else
							{
								for (vint j = 0; j < types.Count(); j++)
								{
									if (resolvables[j] && types[j])
									{
										ITypeInfo* argumentType = genericType->GetGenericArgument(j + 1);
										if (!CanConvertToType(types[j].Obj(), argumentType, false))
										{
											functionErrors.Add(WfErrors::FunctionArgumentTypeMismatched(node, result, i + 1, types[j].Obj(), argumentType));
											failed = true;
										}
									}
								}
							}
						}
						else
						{
							goto FUNCTION_TYPE_FAILED;
						}

						goto FUNCTION_TYPE_FINISHED;
					FUNCTION_TYPE_FAILED:
						nonFunctionErrors.Add(WfErrors::ExpressionIsNotFunction(functionExpression.Obj(), result.type.Obj()));
						failed = true;
					FUNCTION_TYPE_FINISHED:
						if (failed)
						{
							functions.RemoveAt(i);
						}
					}

					if (functions.Count() > 1)
					{
						manager->errors.Add(WfErrors::CannotPickOverloadedFunctions(node, functions));
					}

					if (functions.Count() == 1)
					{
						Ptr<ITypeInfo> functionType = functions[0].type;
						ITypeInfo* genericType = functionType->GetElementType();
						for (vint i = 0; i < types.Count(); i++)
						{
							if (!resolvables[i])
							{
								ITypeInfo* argumentType = genericType->GetGenericArgument(i + 1);
								GetExpressionType(manager, arguments[i], CopyTypeInfo(argumentType));
							}
						}

						return CopyTypeInfo(genericType->GetGenericArgument(0));
					}
					else
					{
						CopyFrom(manager->errors, (functionErrors.Count() > 0 ? functionErrors : nonFunctionErrors), true);
					}
					return 0;
				}

				void Visit(WfCallExpression* node)override
				{
					List<ResolveExpressionResult> functions;
					GetExpressionTypes(manager, node->function, 0, functions);

					Ptr<ITypeInfo> resultType = SelectFunction(node, node->function, functions, node->arguments);
					if (resultType)
					{
						manager->expressionResolvings.Add(node->function, functions[0]);
						results.Add(ResolveExpressionResult(resultType));
					}
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

				Ptr<ITypeInfo> GetFunctionDeclarationType(WfLexicalScope* scope,Ptr<WfFunctionDeclaration> decl)
				{
					Ptr<WfLexicalSymbol> symbol = From(manager->declarationScopes[decl.Obj()]->parentScope->symbols[decl->name.value])
						.Where([decl](Ptr<WfLexicalSymbol> symbol)
						{
							return symbol->creatorDeclaration == decl;
						})
						.First();
					return symbol->typeInfo;
				}

				void Visit(WfNewTypeExpression* node)override
				{
					auto scope = manager->expressionScopes[node].Obj();
					Ptr<ITypeInfo> type = CreateTypeInfoFromType(scope, node->type);
					if (type)
					{
						ITypeDescriptor* td = type->GetTypeDescriptor();
						ITypeDescriptor* proxyTd = description::GetTypeDescriptor<IValueInterfaceProxy>();
						IMethodGroupInfo* ctors = td->GetConstructorGroup();
						Ptr<ITypeInfo> selectedType;
						ResolveExpressionResult selectedFunction;
						if (!ctors || ctors->GetMethodCount() == 0)
						{
							manager->errors.Add(WfErrors::ClassContainsNoConstructor(node, type.Obj()));
						}
						else
						{
							if (node->functions.Count() == 0)
							{
								List<ResolveExpressionResult> functions;
								for (vint i = 0; i < ctors->GetMethodCount(); i++)
								{
									IMethodInfo* info = ctors->GetMethod(i);
									functions.Add(ResolveExpressionResult(info, CreateTypeInfoFromMethodInfo(info)));
								}

								selectedType = SelectFunction(node, 0, functions, node->arguments);
								if (selectedType)
								{
									selectedFunction = functions[0];
								}
							}
							else
							{
								for (vint i = 0; i < ctors->GetMethodCount(); i++)
								{
									IMethodInfo* info = ctors->GetMethod(i);
									if (info->GetParameterCount() == 1)
									{
										ITypeInfo* parameterType = info->GetParameter(0)->GetType();
										if (parameterType->GetDecorator() == ITypeInfo::SharedPtr)
										{
											parameterType = parameterType->GetElementType();
											if (parameterType->GetDecorator() == ITypeInfo::TypeDescriptor && parameterType->GetTypeDescriptor() == proxyTd)
											{
												selectedType = CopyTypeInfo(info->GetReturn());
												selectedFunction = ResolveExpressionResult(info, CreateTypeInfoFromMethodInfo(info));
												break;
											}
										}
									}
								}
								if (!selectedType)
								{
									manager->errors.Add(WfErrors::InterfaceContainsNoConstructor(node, type.Obj()));
								}
								
								Group<WString, IMethodInfo*> interfaceMethods;
								Group<WString, Ptr<WfFunctionDeclaration>> implementMethods;

								FOREACH(Ptr<WfFunctionDeclaration>, function, node->functions)
								{
									ValidateDeclarationSemantic(manager, function);
									implementMethods.Add(function->name.value, function);
								}

								{
									List<ITypeDescriptor*> types;
									types.Add(td);
									vint begin = 0;

									while (begin < types.Count())
									{
										ITypeDescriptor* currentType = types[begin++];
										vint count = currentType->GetBaseTypeDescriptorCount();
										for (vint i = 0; i < count; i++)
										{
											types.Add(currentType->GetBaseTypeDescriptor(i));
										}

										count = currentType->GetMethodGroupCount();
										for (vint i = 0; i < count; i++)
										{
											IMethodGroupInfo* group = currentType->GetMethodGroup(i);
											vint methodCount = group->GetMethodCount();
											for (vint j = 0; j < methodCount; j++)
											{
												interfaceMethods.Add(group->GetName(), group->GetMethod(j));
											}
										}
									}
								}

								auto discardFirst = [=](const WString& key, const List<IMethodInfo*>& methods)
									{
										FOREACH(IMethodInfo*, method, methods)
										{
											manager->errors.Add(WfErrors::InterfaceMethodNotImplemented(node, method));
										}
									};
								auto discardSecond = [=](const WString& key, const List<Ptr<WfFunctionDeclaration>>& methods)
									{
										FOREACH(Ptr<WfFunctionDeclaration>, decl, methods)
										{
											Ptr<ITypeInfo> declType = GetFunctionDeclarationType(scope, decl);
											manager->errors.Add(WfErrors::InterfaceMethodNotFound(decl.Obj(), type.Obj(), declType.Obj()));
										}
									};

								GroupInnerJoin(
									interfaceMethods,
									implementMethods,
									discardFirst,
									discardSecond,
									[=](const WString& key, const List<IMethodInfo*>& interfaces, const List<Ptr<WfFunctionDeclaration>>& implements)
									{
										Group<WString, IMethodInfo*> typedInterfaceMethods;
										Group<WString, Ptr<WfFunctionDeclaration>> typedImplementMethods;

										FOREACH(IMethodInfo*, method, interfaces)
										{
											Ptr<ITypeInfo> methodType = CreateTypeInfoFromMethodInfo(method);
											typedInterfaceMethods.Add(methodType->GetTypeFriendlyName(), method);
										}

										FOREACH(Ptr<WfFunctionDeclaration>, decl, implements)
										{
											Ptr<ITypeInfo> methodType = GetFunctionDeclarationType(scope, decl);
											typedImplementMethods.Add(methodType->GetTypeFriendlyName(), decl);
										}

										GroupInnerJoin(
											typedInterfaceMethods,
											typedImplementMethods,
											discardFirst,
											discardSecond,
											[=](const WString& key, const List<IMethodInfo*>& interfaces, const List<Ptr<WfFunctionDeclaration>>& implements)
											{
												if (interfaces.Count() > 1)
												{
													List<ResolveExpressionResult> functions;
													FOREACH(IMethodInfo*, method, interfaces)
													{
														functions.Add(ResolveExpressionResult(method, CreateTypeInfoFromMethodInfo(method)));
														manager->errors.Add(WfErrors::CannotPickOverloadedInterfaceMethods(node, functions));
													}
												}
												if (implements.Count() > 1)
												{
													auto decl = implements[0];
													Ptr<ITypeInfo> methodType = GetFunctionDeclarationType(scope, decl);
													manager->errors.Add(WfErrors::CannotPickOverloadedImplementMethods(decl.Obj(), methodType.Obj()));
												}
											});
									});
							}
						}
						if (selectedType)
						{
							if (!CanConvertToType(selectedType.Obj(), type.Obj(), false))
							{
								manager->errors.Add(WfErrors::ConstructorReturnTypeMismatched(node, selectedFunction, selectedType.Obj(), type.Obj()));
							}
						}
						results.Add(ResolveExpressionResult(selectedFunction.methodInfo, type));
					}
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
							auto scope = manager->declarationScopes.Values()[index];
							bool isVariable = decl.Cast<WfVariableDeclaration>();
							if (!isVariable)
							{
								scope = scope->parentScope;
							}

							index = scope->symbols.Keys().IndexOf(decl->name.value);
							if (index == -1) continue;
							FOREACH(Ptr<WfLexicalSymbol>, symbol, scope->symbols.GetByIndex(index))
							{
								if (symbol->creatorDeclaration == decl && symbol->typeInfo)
								{
									if (isVariable)
									{
										replaces.Add(ResolveExpressionResult(symbol, symbol->typeInfo, symbol->typeInfo));
									}
									else
									{
										replaces.Add(ResolveExpressionResult(symbol, symbol->typeInfo));
									}
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

				auto result = results[0];
				manager->expressionResolvings.Add(expression, result);
				return result.scopeName;
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

				auto result = results[0];
				manager->expressionResolvings.Add(expression, result);
				return result.eventInfo;
			}

/***********************************************************************
GetExpressionTypes/GetExpressionType/GetLeftValueExpressionType
***********************************************************************/

			void GetExpressionTypes(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, collections::List<ResolveExpressionResult>& results)
			{
				ValidateExpressionSemantic(manager, expression, expectedType, results);
				if (results.Count() == 0) return;

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
					}
				}
			}

			Ptr<reflection::description::ITypeInfo> GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				List<ResolveExpressionResult> results;
				GetExpressionTypes(manager, expression, expectedType, results);

				if (results.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManyTargets(expression.Obj(), results, GetExpressionName(expression)));
					return expectedType;
				}
				else if (results.Count() == 1)
				{
					auto result = results[0];
					result.expectedType = expectedType;
					manager->expressionResolvings.Add(expression, result);
					return expectedType ? expectedType : result.type;
				}
				else
				{
					return expectedType;
				}
			}

			Ptr<reflection::description::ITypeInfo>	GetLeftValueExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				List<ResolveExpressionResult> results;
				GetExpressionTypes(manager, expression, 0, results);

				if (results.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManyTargets(expression.Obj(), results, GetExpressionName(expression)));
				}
				else if (results.Count() == 1)
				{
					if (results[0].leftValueType)
					{
						auto result = results[0];
						manager->expressionResolvings.Add(expression, result);
						return result.leftValueType;
					}
					else
					{
						manager->errors.Add(WfErrors::ExpressionIsNotLeftValue(expression.Obj(), results[0]));
					}
				}
				return 0;
			}

/***********************************************************************
GetEnumerableExpressionItemType
***********************************************************************/

			Ptr<reflection::description::ITypeInfo>	GetEnumerableExpressionItemType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				Ptr<ITypeInfo> collectionType = GetExpressionType(manager, expression, expectedType);
				if (collectionType)
				{
					if (collectionType->GetTypeDescriptor()->CanConvertTo(description::GetTypeDescriptor<IValueEnumerable>()))
					{
						if (collectionType->GetDecorator() == ITypeInfo::SharedPtr)
						{
							ITypeInfo* genericType = collectionType->GetElementType();
							if (genericType->GetDecorator() == ITypeInfo::Generic && genericType->GetGenericArgumentCount() == 1)
							{
								return CopyTypeInfo(genericType->GetGenericArgument(0));
							}
						}
						return TypeInfoRetriver<Value>::CreateTypeInfo();
					}

					manager->errors.Add(WfErrors::ExpressionIsNotCollection(expression.Obj(), collectionType.Obj()));
				}
				return 0;
			}
		}
	}
}
