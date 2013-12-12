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
				Ptr<WfLexicalSymbol>					resultSymbol;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForDeclarationVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					resultSymbol = new WfLexicalSymbol;
					resultSymbol->name = node->name.value;
					resultSymbol->ownerDeclaration = node;
					{
						Ptr<WfPredefinedType> type = new WfPredefinedType;
						type->name = WfPredefinedTypeName::Namespace;
						resultSymbol->type = type;
					}
					parentScope->symbols.Add(resultSymbol->name, resultSymbol);

					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfDeclaration>, declaration, node->declarations)
					{
						BuildScopeForDeclaration(manager, resultScope, declaration);
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					resultSymbol = new WfLexicalSymbol;
					resultSymbol->name = node->name.value;
					resultSymbol->ownerDeclaration = node;
					{
						Ptr<WfFunctionType> type = new WfFunctionType;
						type->result = node->returnType;
						FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
						{
							type->arguments.Add(argument->type);
						}
						resultSymbol->type = type;
					}
					parentScope->symbols.Add(resultSymbol->name, resultSymbol);
					
					resultScope = new WfLexicalScope(parentScope);
					FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
					{
						Ptr<WfLexicalSymbol> symbol = new WfLexicalSymbol;
						symbol->name = argument->name.value;
						symbol->type = argument->type;
						symbol->ownerDeclaration = node;
						resultScope->symbols.Add(symbol->name, symbol);
					}

					BuildScopeForStatement(manager, resultScope, node->statement);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					resultSymbol = new WfLexicalSymbol;
					resultSymbol->name = node->name.value;
					resultSymbol->ownerDeclaration = node;
					{
						resultSymbol->type = node->type;
					}
					parentScope->symbols.Add(resultSymbol->name, resultSymbol);

					BuildScopeForExpression(manager, parentScope, node->expression);
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfDeclaration> declaration)
				{
					BuildScopeForDeclarationVisitor visitor(manager, parentScope);
					declaration->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->declarationScopes.Add(declaration, visitor.resultScope);
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
				Ptr<WfLexicalSymbol>					resultSymbol;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForStatementVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
				{
				}

				void Visit(WfBreakStatement* node)override
				{
					throw 0;
				}

				void Visit(WfContinueStatement* node)override
				{
					throw 0;
				}

				void Visit(WfReturnStatement* node)override
				{
					throw 0;
				}

				void Visit(WfDeleteStatement* node)override
				{
					throw 0;
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					throw 0;
				}

				void Visit(WfIfStatement* node)override
				{
					throw 0;
				}

				void Visit(WfSwitchStatement* node)override
				{
					throw 0;
				}

				void Visit(WfWhileStatement* node)override
				{
					throw 0;
				}

				void Visit(WfForEachStatement* node)override
				{
					throw 0;
				}

				void Visit(WfTryStatement* node)override
				{
					throw 0;
				}

				void Visit(WfBlockStatement* node)override
				{
					throw 0;
				}

				void Visit(WfExpressionStatement* node)override
				{
					throw 0;
				}

				void Visit(WfVariableStatement* node)override
				{
					throw 0;
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement)
				{
					BuildScopeForStatementVisitor visitor(manager, parentScope);
					statement->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->statementScopes.Add(statement, visitor.resultScope);
					}
					return visitor.resultScope;
				}
			};

			void BuildScopeForStatement(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfStatement> statement)
			{
				//BuildScopeForStatementVisitor::Execute(manager, parentScope, statement);
			}

/***********************************************************************
BuildScopeForExpression
***********************************************************************/

			class BuildScopeForExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				Ptr<WfLexicalScope>						parentScope;
				Ptr<WfLexicalSymbol>					resultSymbol;
				Ptr<WfLexicalScope>						resultScope;

				BuildScopeForExpressionVisitor(WfLexicalScopeManager* _manager, Ptr<WfLexicalScope> _parentScope)
					:manager(_manager)
					, parentScope(_parentScope)
				{
				}

				void Visit(WfReferenceExpression* node)override
				{
					throw 0;
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					throw 0;
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
					throw 0;
				}

				void Visit(WfMemberExpression* node)override
				{
					throw 0;
				}

				void Visit(WfChildExpression* node)override
				{
					throw 0;
				}

				void Visit(WfLiteralExpression* node)override
				{
					throw 0;
				}

				void Visit(WfFloatingExpression* node)override
				{
					throw 0;
				}

				void Visit(WfIntegerExpression* node)override
				{
					throw 0;
				}

				void Visit(WfStringExpression* node)override
				{
					throw 0;
				}

				void Visit(WfFormatExpression* node)override
				{
					throw 0;
				}

				void Visit(WfUnaryExpression* node)override
				{
					throw 0;
				}

				void Visit(WfBinaryExpression* node)override
				{
					throw 0;
				}

				void Visit(WfLetExpression* node)override
				{
					throw 0;
				}

				void Visit(WfIfExpression* node)override
				{
					throw 0;
				}

				void Visit(WfRangeExpression* node)override
				{
					throw 0;
				}

				void Visit(WfSetTestingExpression* node)override
				{
					throw 0;
				}

				void Visit(WfConstructorExpression* node)override
				{
					throw 0;
				}

				void Visit(WfInferExpression* node)override
				{
					throw 0;
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					throw 0;
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					throw 0;
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
					throw 0;
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					throw 0;
				}

				void Visit(WfAttachEventExpression* node)override
				{
					throw 0;
				}

				void Visit(WfDetachEventExpression* node)override
				{
					throw 0;
				}

				void Visit(WfBindExpression* node)override
				{
					throw 0;
				}

				void Visit(WfObserveExpression* node)override
				{
					throw 0;
				}

				void Visit(WfCallExpression* node)override
				{
					throw 0;
				}

				void Visit(WfFunctionExpression* node)override
				{
					throw 0;
				}

				void Visit(WfNewTypeExpression* node)override
				{
					throw 0;
				}

				static Ptr<WfLexicalScope> Execute(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression)
				{
					BuildScopeForExpressionVisitor visitor(manager, parentScope);
					expression->Accept(&visitor);
					if (visitor.resultScope)
					{
						manager->expressionScopes.Add(expression, visitor.resultScope);
					}
					return visitor.resultScope;
				}
			};

			void BuildScopeForExpression(WfLexicalScopeManager* manager, Ptr<WfLexicalScope> parentScope, Ptr<WfExpression> expression)
			{
				//BuildScopeForExpressionVisitor::Execute(manager, parentScope, expression);
			}
		}
	}
}