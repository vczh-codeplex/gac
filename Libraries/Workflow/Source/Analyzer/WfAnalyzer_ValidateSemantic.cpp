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
					Ptr<ITypeInfo> typeInfo = GetExpressionType(manager, node->operand, expectedType);
					if (typeInfo)
					{
						TypeFlag flag = GetTypeFlag(typeInfo.Obj());
						switch (node->op)
						{
						case WfUnaryOperator::Not:
							if (flag != TypeFlag::Bool)
							{
								manager->errors.Add(WfErrors::UnaryOperatorOnWrongType(node, typeInfo.Obj()));
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
						Ptr<ITypeInfo> variableType = GetExpressionType(manager, node->first, 0);
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
							switch (containerType->GetDecorator())
							{
							case ITypeInfo::RawPtr:
							case ITypeInfo::SharedPtr:
								{
									ITypeInfo* genericType = containerType->GetElementType();
									Ptr<ITypeInfo> indexType;
									Ptr<ITypeInfo> resultType;

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
										}
										else
										{
											manager->errors.Add(WfErrors::IndexOperatorOnWrongType(node, containerType.Obj()));
										}
									}

									GetExpressionType(manager, node->second, indexType);
									if (resultType)
									{
										results.Add(ResolveExpressionResult(resultType));
									}
								}
							default:
								manager->errors.Add(WfErrors::IndexOperatorOnWrongType(node, containerType.Obj()));
							}
						}

						Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->second, 0);
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
						GetExpressionType(manager, node->second, firstType);
						if (firstType)
						{
							results.Add(ResolveExpressionResult(firstType));
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
									switch (firstType->GetDecorator())
									{
									case ITypeInfo::RawPtr:
									case ITypeInfo::SharedPtr:
										switch (secondType->GetDecorator())
										{
										case ITypeInfo::RawPtr:
										case ITypeInfo::SharedPtr:
											results.Add(ResolveExpressionResult(TypeInfoRetriver<bool>::CreateTypeInfo()));
											return;
										}
									}

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
							case WfBinaryOperator::Xor:
							case WfBinaryOperator::And:
							case WfBinaryOperator::Or:
								{
									static TypeFlag conversionTable[(vint)TypeFlag::Count] = {
										/*Bool		*/TypeFlag::Bool,
										/*I1		*/TypeFlag::Unknown,
										/*I2		*/TypeFlag::Unknown,
										/*I4		*/TypeFlag::Unknown,
										/*I8		*/TypeFlag::Unknown,
										/*U1		*/TypeFlag::Unknown,
										/*U2		*/TypeFlag::Unknown,
										/*U4		*/TypeFlag::Unknown,
										/*U8		*/TypeFlag::Unknown,
										/*F4		*/TypeFlag::Unknown,
										/*F8		*/TypeFlag::Unknown,
										/*String	*/TypeFlag::Unknown,
										/*Others	*/TypeFlag::Unknown,
									};
									selectedTable = conversionTable;
								}
								break;
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
						bool resolveFirst = !IsExpressionDependOnExpectedType(node->trueBranch);
						bool resolveSecond = !IsExpressionDependOnExpectedType(node->falseBranch);

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
							;
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
					GetExpressionType(manager, node->expression, 0);
					results.Add(ResolveExpressionResult(TypeInfoRetriver<Ptr<IValueSubscription>>::CreateTypeInfo()));
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
							{
								auto ref = node->expression.Cast<WfReferenceExpression>();
								IPropertyInfo* info = td->GetPropertyByName(ref->name.value, true);
								if (info)
								{
									observeeType = CopyTypeInfo(info->GetReturn());
								}
								else
								{
									manager->errors.Add(WfErrors::MemberNotExists(ref.Obj(), td, ref->name.value));
								}
							}

							if (node->events.Count() == 0)
							{
								auto ref = node->expression.Cast<WfReferenceExpression>();
								IEventInfo* info = td->GetEventByName(ref->name.value + L"Changed", true);
								if (!info)
								{
									manager->errors.Add(WfErrors::MemberNotExists(ref.Obj(), td, ref->name.value + L"Changed"));
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

				void Visit(WfCallExpression* node)override
				{
					List<ResolveExpressionResult> functions;
					GetExpressionTypes(manager, node->function, 0, functions);

					List<bool> resolvables;
					List<Ptr<ITypeInfo>> types;
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						if (IsExpressionDependOnExpectedType(argument))
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

					List<ResolveExpressionResult> removed;
					List<Ptr<parsing::ParsingError>> errors;
					for (vint i = functions.Count() - 1; i >= 0; i--)
					{
						auto result = functions[i];
					}

					if (functions.Count() == 0)
					{
						CopyFrom(manager->errors, errors, true);
						return;
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
GetExpressionType(s)
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
				else
				{
					return expectedType ? expectedType : results[0].type;
				}
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
						switch (collectionType->GetDecorator())
						{
						case ITypeInfo::RawPtr:
						case ITypeInfo::SharedPtr:
							{
								ITypeInfo* genericType = collectionType->GetElementType();
								if (genericType->GetDecorator() == ITypeInfo::Generic && genericType->GetGenericArgumentCount() == 1)
								{
									return CopyTypeInfo(genericType->GetGenericArgument(0));
								}
							}
							break;
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