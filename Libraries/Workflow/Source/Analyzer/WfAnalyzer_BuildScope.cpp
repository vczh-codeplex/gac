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
BuildScopeForModule
***********************************************************************/

			void BuildScopeForModule(WfLexicalScopeManager* manager, Ptr<WfModule> module)
			{
				Ptr<WfLexicalScope> scope = new WfLexicalScope(manager);
				scope->ownerModule = module;
				manager->moduleScopes.Add(module, scope);

				FOREACH(Ptr<WfDeclaration>, declaration, module->declarations)
				{
					BuildScopeForDeclaration(manager, scope, declaration);
				}
			}

/***********************************************************************
BuildScopeForDeclaration
***********************************************************************/

			class BuildScopeForDeclarationVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				Ptr<WfLexicalScope>						parentScope;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForDeclarationVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
					symbol->name = node->name.value;
					symbol->ownerDeclaration = node;
					{
						Ptr<WfPredefinedType> type = new WfPredefinedType;
						type->name = WfPredefinedTypeName::Namespace;
						symbol->type = type;
					}
					parentScope->symbols.Add(symbol->name, symbol);

					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfDeclaration>, declaration, node->declarations)
					{
						BuildScopeForDeclaration(manager, resultScope, declaration);
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					if (node->anonymity == WfFunctionAnonymity::Named)
					{
						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = node->name.value;
						symbol->ownerDeclaration = node;
						{
							Ptr<WfFunctionType> type = new WfFunctionType;
							type->result = node->returnType;
							FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
							{
								type->arguments.Add(argument->type);
							}
							symbol->type = type;
						}
						parentScope->symbols.Add(symbol->name, symbol);
					}
					
					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
					{
						Ptr<WfLexicalSymbol> argumentSymbol = new WfLexicalSymbol;
						argumentSymbol->name = argument->name.value;
						argumentSymbol->type = argument->type;
						argumentSymbol->ownerDeclaration = node;
						resultScope->symbols.Add(argumentSymbol->name, argumentSymbol);
					}

					BuildScopeForStatement(manager, resultScope, node->statement);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
					symbol->name = node->name.value;
					symbol->ownerDeclaration = node;
					{
						symbol->type = node->type;
					}
					parentScope->symbols.Add(symbol->name, symbol);

					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration)
				{
					BuildScopeForDeclarationVisitor visitor(manager, parentScope);
					declaration->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->declarationScopes.Add(declaration, visitor.resultScope);
						visitor.resultScope->ownerDeclaration = declaration;
					}
					else
					{
						manager->declarationScopes.Add(declaration, parentScope);
					}
					return visitor.resultScope;
				}
			};

			void BuildScopeForDeclaration(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration)
			{
				BuildScopeForDeclarationVisitor::Execute(manager, parentScope, declaration);
			}

/***********************************************************************
BuildScopeForStatement
***********************************************************************/

			class BuildScopeForStatementVisitor : public Object, public WfStatement::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				Ptr<WfLexicalScope>						parentScope;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForStatementVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
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
					if (node->expression)
					{
						BuildScopeForExpression(manager, parentScope, node->expression);
					}
				}

				void Visit(WfDeleteStatement* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfIfStatement* node)override
				{
					resultScope = new WfLexicalScope(parentScope);
					if (node->type)
					{
						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = node->name.value;
						symbol->type = node->type;
						symbol->ownerStatement = node;
						resultScope->symbols.Add(symbol->name, symbol);
					}

					BuildScopeForExpression(manager, resultScope, node->expression);
					BuildScopeForStatement(manager, resultScope, node->trueBranch);
					if (node->falseBranch)
					{
						BuildScopeForStatement(manager, resultScope, node->falseBranch);
					}
				}

				void Visit(WfSwitchStatement* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
					FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
					{
						BuildScopeForExpression(manager, parentScope, switchCase->expression);
						BuildScopeForStatement(manager, parentScope, switchCase->statement);
					}
					if (node->defaultBranch)
					{
						BuildScopeForStatement(manager, parentScope, node->defaultBranch);
					}
				}

				void Visit(WfWhileStatement* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->condition);
					BuildScopeForStatement(manager, parentScope, node->statement);
				}

				void Visit(WfForEachStatement* node)override
				{
					resultScope = new WfLexicalScope(parentScope);

					Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
					symbol->name = node->name.value;
					symbol->ownerStatement = node;
					resultScope->symbols.Add(symbol->name, symbol);

					BuildScopeForExpression(manager, parentScope, node->collection);
					BuildScopeForStatement(manager, resultScope, node->statement);
				}

				void Visit(WfTryStatement* node)override
				{
					BuildScopeForStatement(manager, parentScope, node->protectedStatement);
					if (node->catchStatement)
					{
						resultScope = new WfLexicalScope(parentScope);

						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = node->name.value;
						symbol->ownerStatement = node;
						{
							Ptr<WfPredefinedType> type = new WfPredefinedType;
							type->name = WfPredefinedTypeName::Object;
							symbol->type = type;
						}
						resultScope->symbols.Add(symbol->name, symbol);

						BuildScopeForStatement(manager, resultScope, node->catchStatement);
					}
					if (node->finallyStatement)
					{
						BuildScopeForStatement(manager, parentScope, node->finallyStatement);
					}
				}

				void Visit(WfBlockStatement* node)override
				{
					resultScope = new WfLexicalScope(parentScope);

					FOREACH(Ptr<WfStatement>, statement, node->statements)
					{
						BuildScopeForStatement(manager, resultScope, statement);
					}
				}

				void Visit(WfExpressionStatement* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfVariableStatement* node)override
				{
					BuildScopeForDeclaration(manager, parentScope, node->variable);
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement)
				{
					BuildScopeForStatementVisitor visitor(manager, parentScope);
					statement->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->statementScopes.Add(statement, visitor.resultScope);
						visitor.resultScope->ownerStatement = statement;
					}
					else
					{
						manager->statementScopes.Add(statement, parentScope);
					}
					return visitor.resultScope;
				}
			};

			void BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement)
			{
				BuildScopeForStatementVisitor::Execute(manager, parentScope, statement);
			}

/***********************************************************************
BuildScopeForExpression
***********************************************************************/

			class BuildScopeForExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				Ptr<WfLexicalScope>						parentScope;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForExpressionVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
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
					SortedList<vint> names;
					SearchOrderedName(node->body, names);

					resultScope = new WfLexicalScope(parentScope);
					FOREACH(vint, name, names)
					{
						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = L"$" + itow(name);
						symbol->ownerExpression = node;
						resultScope->symbols.Add(symbol->name, symbol);
					}

					BuildScopeForExpression(manager, resultScope, node->body);
				}

				void Visit(WfMemberExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->parent);
				}

				void Visit(WfChildExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->parent);
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
					BuildScopeForExpression(manager, parentScope, node->expandedExpression);
				}

				void Visit(WfUnaryExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->operand);
				}

				void Visit(WfBinaryExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->first);
					BuildScopeForExpression(manager, parentScope, node->second);
				}

				void Visit(WfLetExpression* node)override
				{
					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfLetVariable>, variable, node->variables)
					{
						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = variable->name.value;
						symbol->ownerExpression = node;
						resultScope->symbols.Add(symbol->name, symbol);
					}

					BuildScopeForExpression(manager, resultScope, node->expression);
				}

				void Visit(WfIfExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->condition);
					BuildScopeForExpression(manager, parentScope, node->trueBranch);
					BuildScopeForExpression(manager, parentScope, node->falseBranch);
				}

				void Visit(WfRangeExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->begin);
					BuildScopeForExpression(manager, parentScope, node->end);
				}

				void Visit(WfSetTestingExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->element);
					BuildScopeForExpression(manager, parentScope, node->collection);
				}

				void Visit(WfConstructorExpression* node)override
				{
					FOREACH(Ptr<WfConstructorArgument>, argument, node->arguments)
					{
						BuildScopeForExpression(manager, parentScope, argument->key);
						if (argument->value)
						{
							BuildScopeForExpression(manager, parentScope, argument->value);
						}
					}
				}

				void Visit(WfInferExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					if (node->expression)
					{
						BuildScopeForExpression(manager, parentScope, node->expression);
					}
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfAttachEventExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->event);
					BuildScopeForExpression(manager, parentScope, node->function);
				}

				void Visit(WfDetachEventExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->handler);
				}

				void Visit(WfBindExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				void Visit(WfObserveExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->parent);
					if (node->observeType == WfObserveType::SimpleObserve)
					{
						BuildScopeForExpression(manager, parentScope, node->expression);
						FOREACH(Ptr<WfExpression>, event, node->events)
						{
							BuildScopeForExpression(manager, parentScope, event);
						}
					}
					else
					{
						resultScope = new WfLexicalScope(parentScope);
						{
							Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
							symbol->name = node->name.value;
							symbol->ownerExpression = node;
							resultScope->symbols.Add(symbol->name, symbol);
						}

						BuildScopeForExpression(manager, resultScope, node->expression);
						FOREACH(Ptr<WfExpression>, event, node->events)
						{
							BuildScopeForExpression(manager, resultScope, event);
						}
					}
				}

				void Visit(WfCallExpression* node)override
				{
					BuildScopeForExpression(manager, parentScope, node->function);
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						BuildScopeForExpression(manager, parentScope, argument);
					}
				}

				void Visit(WfFunctionExpression* node)override
				{
					BuildScopeForDeclaration(manager, parentScope, node->function);
				}

				void Visit(WfNewTypeExpression* node)override
				{
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						BuildScopeForExpression(manager, parentScope, argument);
					}

					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfFunctionDeclaration>, function, node->functions)
					{
						BuildScopeForDeclaration(manager, resultScope, function);
					}
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression)
				{
					BuildScopeForExpressionVisitor visitor(manager, parentScope);
					expression->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->expressionScopes.Add(expression, visitor.resultScope);
						visitor.resultScope->ownerExpression = expression;
					}
					else
					{
						manager->expressionScopes.Add(expression, parentScope);
					}
					return visitor.resultScope;
				}
			};

			void BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression)
			{
				BuildScopeForExpressionVisitor::Execute(manager, parentScope, expression);
			}
		}
	}
}