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
				Ptr<ITypeInfo>						resultType;
				Ptr<WfLexicalScopeName>				resultScopeName;

				ValidateSemanticExpressionVisitor(WfLexicalScopeManager* _manager, Ptr<ITypeInfo> _expectedType)
					:manager(_manager)
					, expectedType(_expectedType)
				{
				}

				void Visit(WfTopQualifiedExpression* node)override
				{
					if (manager->globalName)
					{
						vint index = manager->globalName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							resultScopeName = manager->globalName->children.Values()[index];
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
					if (symbols.Count() > 1)
					{
						manager->errors.Add(WfErrors::TooManySymbol(node, symbols, node->name.value));
						return;
					}
					else if (symbols.Count() == 1)
					{
						auto symbol = symbols[0];
						if (symbol->typeInfo)
						{
							resultType = symbol->typeInfo;
						}
						else
						{
							manager->errors.Add(WfErrors::ExpressionCannotResolveType(node, symbol));
						}
						return;
					}

					List<Ptr<WfLexicalScopeName>> scopeNames;
					manager->ResolveScopeName(scope, node->name.value, scopeNames);
					if (scopeNames.Count() > 1)
					{
						manager->errors.Add(WfErrors::TooManyScopeName(node, scopeNames, node->name.value));
					}
					else if (scopeNames.Count() == 1)
					{
						resultScopeName = scopeNames[0];
					}
					else
					{
						manager->errors.Add(WfErrors::ReferenceNotExists(node, node->name.value));
					}
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
					if (Ptr<WfLexicalScopeName> scopeName = GetExpressionScopeName(manager, node->parent))
					{
						vint index = scopeName->children.Keys().IndexOf(node->name.value);
						if (index != -1)
						{
							resultScopeName = scopeName->children.Values()[index];
							return;
						}
						manager->errors.Add(WfErrors::ChildSymbolNotExists(node, scopeName, node->name.value));
					}
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

				static void Execute(Ptr<WfExpression> expression, WfLexicalScopeManager* manager, Ptr<ITypeInfo> expectedType, Ptr<ITypeInfo>& resultType, Ptr<WfLexicalScopeName>& resultScopeName)
				{
					ValidateSemanticExpressionVisitor visitor(manager, expectedType);
					expression->Accept(&visitor);
					resultType = visitor.resultType;
					resultScopeName = visitor.resultScopeName;
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

			void ValidateExpressionSemantic(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType, Ptr<reflection::description::ITypeInfo>& resultType, Ptr<WfLexicalScopeName>& resultScopeName)
			{
				ValidateSemanticExpressionVisitor::Execute(expression, manager, expectedType, resultType, resultScopeName);
			}

			Ptr<WfLexicalScopeName> GetExpressionScopeName(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				Ptr<ITypeInfo> resultType;
				Ptr<WfLexicalScopeName> resultScopeName;
				ValidateExpressionSemantic(manager, expression, 0, resultType, resultScopeName);
				if (resultType && !resultScopeName)
				{
					manager->errors.Add(WfErrors::ExpressionIsNotScopeName(expression.Obj()));
				}
				return resultScopeName;
			}

			Ptr<reflection::description::ITypeInfo> GetExpressionType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				Ptr<ITypeInfo> resultType;
				Ptr<WfLexicalScopeName> resultScopeName;
				ValidateExpressionSemantic(manager, expression, expectedType, resultType, resultScopeName);
				if (!resultType && resultScopeName)
				{
					manager->errors.Add(WfErrors::ScopeNameIsNotExpression(expression.Obj(), resultScopeName));
				}
				if (expectedType && resultType)
				{
					if (!CanConvertToType(resultType.Obj(), expectedType.Obj(), false))
					{
						manager->errors.Add(WfErrors::ExpressionCannotImplicitlyConvertToType(expression.Obj(), resultType.Obj(), expectedType.Obj()));
					}
				}
				return expectedType ? expectedType : resultType;
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