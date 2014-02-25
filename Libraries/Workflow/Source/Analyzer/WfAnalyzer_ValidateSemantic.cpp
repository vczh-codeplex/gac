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
IsExpressionDependOnExpectedType(Expression)
***********************************************************************/

			class IsExpressionDependOnExpectedTypeVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				bool								result;

				IsExpressionDependOnExpectedTypeVisitor()
					:result(false)
				{
				}

				static bool Execute(Ptr<WfExpression> expression)
				{
					IsExpressionDependOnExpectedTypeVisitor visitor;
					expression->Accept(&visitor);
					return visitor.result;
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
					result = true;
				}

				void Visit(WfMemberExpression* node)override
				{
				}

				void Visit(WfChildExpression* node)override
				{
				}

				void Visit(WfLiteralExpression* node)override
				{
					if (node->value == WfLiteralValue::Null)
					{
						result = true;
					}
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
					result = Execute(node->trueBranch) && Execute(node->falseBranch);
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
						ITypeDescriptor* typeDescriptor = description::GetTypeDescriptor<bool>();
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						typeInfo->SetTypeDescriptor(typeDescriptor);
						results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)typeInfo));
					}
				}

				void Visit(WfFloatingExpression* node)override
				{
					ITypeDescriptor* typeDescriptor = description::GetTypeDescriptor<double>();
					Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					typeInfo->SetTypeDescriptor(typeDescriptor);
					results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)typeInfo));
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
					ITypeDescriptor* typeDescriptor = description::GetTypeDescriptor<WString>();
					Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					typeInfo->SetTypeDescriptor(typeDescriptor);
					results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)typeInfo));
				}

				void Visit(WfFormatExpression* node)override
				{
					ITypeDescriptor* typeDescriptor = description::GetTypeDescriptor<WString>();
					Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
					typeInfo->SetTypeDescriptor(typeDescriptor);
					results.Add(ResolveExpressionResult((Ptr<ITypeInfo>)typeInfo));
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
						symbol->type = GetTypeFromTypeInfo(symbol->typeInfo.Obj());
					}
					Ptr<ITypeInfo> type = GetExpressionType(manager, node->expression, expectedType);
					if (type)
					{
						results.Add(ResolveExpressionResult(type));
					}
				}

				void Visit(WfIfExpression* node)override
				{
					Ptr<ITypeInfo> boolType;
					{
						ITypeDescriptor* typeDescriptor = description::GetTypeDescriptor<bool>();
						Ptr<TypeInfoImpl> typeInfo = new TypeInfoImpl(ITypeInfo::TypeDescriptor);
						typeInfo->SetTypeDescriptor(typeDescriptor);
						boolType = typeInfo;
					}
					GetExpressionType(manager, node->condition, boolType);

					Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->trueBranch, expectedType);
					Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->falseBranch, expectedType);
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

				void Visit(WfRangeExpression* node)override
				{
					Ptr<ITypeInfo> firstType = GetExpressionType(manager, node->begin, 0);
					Ptr<ITypeInfo> secondType = GetExpressionType(manager, node->end, 0);
					Ptr<ITypeInfo> elementType;
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
						return;
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
						if (expressionType)
						{
							if (!CanConvertToType(expressionType.Obj(), type.Obj(), true))
							{
								manager->errors.Add(WfErrors::ExpressionCannotExplicitlyConvertToType(node->expression.Obj(), expressionType.Obj(), type.Obj()));
							}
						}
						results.Add(ResolveExpressionResult(type));
					}
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

				static void Execute(Ptr<WfExpression> expression, WfLexicalScopeManager* manager, Ptr<ITypeInfo> expectedType, List<ResolveExpressionResult>& results)
				{
					ValidateSemanticExpressionVisitor visitor(manager, expectedType, results);
					expression->Accept(&visitor);
				}
			};

/***********************************************************************
GetExpressionName(Expression)
***********************************************************************/

			class GetExpressionNameVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WString								result;

				void Visit(WfTopQualifiedExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfReferenceExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
				}

				void Visit(WfMemberExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfChildExpression* node)override
				{
					result = node->name.value;
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

				static WString Execute(Ptr<WfExpression> expression)
				{
					GetExpressionNameVisitor visitor;
					expression->Accept(&visitor);
					return visitor.result;
				}
			};

/***********************************************************************
ResolveExpressionResult
***********************************************************************/

			ResolveExpressionResult::ResolveExpressionResult()
			{
			}

			ResolveExpressionResult::ResolveExpressionResult(const ResolveExpressionResult& result)
				:scopeName(result.scopeName)
				, symbol(result.symbol)
				, type(result.type)
			{
			}

			ResolveExpressionResult::ResolveExpressionResult(Ptr<WfLexicalScopeName> _scopeName)
				:scopeName(_scopeName)
			{
			}

			ResolveExpressionResult::ResolveExpressionResult(Ptr<reflection::description::ITypeInfo> _type)
				:type(_type)
			{
			}

			ResolveExpressionResult::ResolveExpressionResult(Ptr<WfLexicalSymbol> _symbol, Ptr<reflection::description::ITypeInfo> _type)
				:symbol(_symbol)
				, type(_type)
			{
			}

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
					List<Ptr<WfLexicalScopeName>> scopeNames;
					FOREACH(ResolveExpressionResult, result, results)
					{
						scopeNames.Add(result.scopeName);
					}
					if (scopeNames.Count() > 0)
					{
						manager->errors.Add(WfErrors::TooManyScopeName(expression.Obj(), scopeNames, GetExpressionNameVisitor::Execute(expression)));
					}
					return 0;
				}
				return results[0].scopeName;
			}

			Ptr<reflection::description::ITypeInfo> GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				List<ResolveExpressionResult> results;
				ValidateExpressionSemantic(manager, expression, expectedType, results);
				if (results.Count() == 0) return expectedType;

				Ptr<WfLexicalScopeName> scopeName;
				for (vint i = results.Count() - 1; i >= 0; i--)
				{
					auto& result = results[i];
					if (result.scopeName)
					{
						scopeName = result.scopeName;
					}
					if (!result.type)
					{
						results.RemoveAt(i);
					}
				}
				if (results.Count() == 0)
				{
					manager->errors.Add(WfErrors::ScopeNameIsNotExpression(expression.Obj(), scopeName));
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

				List<Ptr<WfLexicalSymbol>> symbols;
				FOREACH(ResolveExpressionResult, result, results)
				{
					if (result.symbol)
					{
						symbols.Add(result.symbol);
					}
				}
				if (symbols.Count() > 1)
				{
					manager->errors.Add(WfErrors::TooManySymbol(expression.Obj(), symbols, GetExpressionNameVisitor::Execute(expression)));
				}
				
				return expectedType ? expectedType : results[0].type;
			}

			bool CanConvertToType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType, bool explicitly)
			{
				ITypeDescriptor* objectType = GetTypeDescriptor<Value>();
				bool fromObject = fromType->GetDecorator() == ITypeInfo::TypeDescriptor && fromType->GetTypeDescriptor() == objectType;
				bool toObject = toType->GetDecorator() == ITypeInfo::TypeDescriptor && toType->GetTypeDescriptor() == objectType;

				if (fromObject && toObject)
				{
					return true;
				}
				else if (fromObject)
				{
					return explicitly;
				}
				else if (toObject)
				{
					return true;
				}

				switch (fromType->GetDecorator())
				{
				case ITypeInfo::RawPtr:
					switch (toType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
						return CanConvertToType(fromType->GetElementType(), toType->GetElementType(), explicitly);
					case ITypeInfo::SharedPtr:
						return explicitly && CanConvertToType(fromType->GetElementType(), toType->GetElementType(), explicitly);
					case ITypeInfo::Nullable:
					case ITypeInfo::TypeDescriptor:
					case ITypeInfo::Generic:
						return false;
					}
					break;
				case ITypeInfo::SharedPtr:
					switch (toType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
					case ITypeInfo::SharedPtr:
						return CanConvertToType(fromType->GetElementType(), toType->GetElementType(), explicitly);
					case ITypeInfo::Nullable:
					case ITypeInfo::TypeDescriptor:
					case ITypeInfo::Generic:
						return false;
					}
					break;
				case ITypeInfo::Nullable:
					switch (toType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
					case ITypeInfo::SharedPtr:
						return false;
					case ITypeInfo::Nullable:
						return CanConvertToType(fromType->GetElementType(), toType->GetElementType(), explicitly);
					case ITypeInfo::TypeDescriptor:
						return explicitly && CanConvertToType(fromType->GetElementType(), toType, explicitly);
					case ITypeInfo::Generic:
						return false;
					}
					break;
				case ITypeInfo::TypeDescriptor:
					switch (toType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
					case ITypeInfo::SharedPtr:
						return false;
					case ITypeInfo::Nullable:
						return CanConvertToType(fromType, toType->GetElementType(), explicitly);
					case ITypeInfo::TypeDescriptor:
						{
							ITypeDescriptor* fromTd = fromType->GetTypeDescriptor();
							ITypeDescriptor* toTd = toType->GetTypeDescriptor();
							if ((fromTd->GetValueSerializer() != 0) != (toTd->GetValueSerializer() != 0))
							{
								return false;
							}

							if (fromTd->GetValueSerializer())
							{
								if (fromTd == toTd)
								{
									return true;
								}
								ITypeDescriptor* stringType = GetTypeDescriptor<WString>();
								return (explicitly && fromTd == stringType) || toTd == stringType;
							}
							else
							{
								if (fromTd->CanConvertTo(toTd))
								{
									return true;
								}
								if (explicitly && toTd->CanConvertTo(fromTd))
								{
									return true;
								}
							}
						}
						break;
					case ITypeInfo::Generic:
						return explicitly && CanConvertToType(fromType, toType->GetElementType(), explicitly);
					}
					break;
				case ITypeInfo::Generic:
					switch (toType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
					case ITypeInfo::SharedPtr:
					case ITypeInfo::Nullable:
						return false;
					case ITypeInfo::TypeDescriptor:
						return CanConvertToType(fromType->GetElementType(), toType, explicitly);
					case ITypeInfo::Generic:
						if (explicitly) return true;
						if (fromType->GetGenericArgumentCount() != toType->GetGenericArgumentCount())
						{
							return false;
						}
						if (!CanConvertToType(fromType->GetElementType(), toType->GetElementType(), explicitly)) return false;
						for (vint i = 0; i < fromType->GetGenericArgumentCount(); i++)
						{
							if (!IsSameType(fromType->GetGenericArgument(i), toType->GetGenericArgument(i)))
							{
								return false;
							}
						}
						return true;
					}
					break;
				}
				return false;
			}

			bool IsSameType(reflection::description::ITypeInfo* fromType, reflection::description::ITypeInfo* toType)
			{
				if (fromType->GetDecorator() != toType->GetDecorator())
				{
					return false;
				}
				switch (fromType->GetDecorator())
				{
				case ITypeInfo::RawPtr:
				case ITypeInfo::SharedPtr:
				case ITypeInfo::Nullable:
					return IsSameType(fromType->GetElementType(), toType->GetElementType());
				case ITypeInfo::TypeDescriptor:
					return fromType->GetTypeDescriptor() == toType->GetTypeDescriptor();
				case ITypeInfo::Generic:
					if (fromType->GetGenericArgumentCount() != toType->GetGenericArgumentCount())
					{
						return false;
					}
					if (!IsSameType(fromType->GetElementType(), toType->GetElementType())) return false;
					for (vint i = 0; i < fromType->GetGenericArgumentCount(); i++)
					{
						if (!IsSameType(fromType->GetGenericArgument(i), toType->GetGenericArgument(i)))
						{
							return false;
						}
					}
					return true;
				}
				return false;
			}
		}
	}
}