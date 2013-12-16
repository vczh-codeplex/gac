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
ValidateStructure(Declaration)
***********************************************************************/

			class ValidateStructureDeclarationVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;

				ValidateStructureDeclarationVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					for (vint i = 0; i < node->declarations.Count(); i++)
					{
						ValidateDeclarationStructure(manager, node->declarations[i]);
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					ValidateStructureContext context;
					ValidateStatementStructure(manager, &context, node->statement);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					ValidateStructureContext context;
					ValidateExpressionStructure(manager, &context, node->expression);
				}

				static void Execute(Ptr<WfDeclaration> declaration, WfLexicalScopeManager* manager)
				{
					ValidateStructureDeclarationVisitor visitor(manager);
					declaration->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateStructure(Statement)
***********************************************************************/

			class ValidateStructureStatementVisitor : public Object, public WfStatement::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				ValidateStructureContext*				context;
				Ptr<WfStatement>						result;

				ValidateStructureStatementVisitor(WfLexicalScopeManager* _manager, ValidateStructureContext* _context)
					:manager(_manager)
					, context(_context)
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
						ValidateExpressionStructure(manager, context, node->expression);
					}
				}

				void Visit(WfDeleteStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					if (node->expression)
					{
						ValidateExpressionStructure(manager, context, node->expression);
					}
				}

				void Visit(WfIfStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
					ValidateStatementStructure(manager, context, node->trueBranch);
					if (node->falseBranch)
					{
						ValidateStatementStructure(manager, context, node->falseBranch);
					}
				}

				void Visit(WfSwitchStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
					FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
					{
						ValidateExpressionStructure(manager, context, switchCase->expression);
						ValidateStatementStructure(manager, context, switchCase->statement);
					}
					if (node->defaultBranch)
					{
						ValidateStatementStructure(manager, context, node->defaultBranch);
					}
				}

				void Visit(WfWhileStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->condition);
					ValidateStatementStructure(manager, context, node->statement);
				}

				void Visit(WfForEachStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->collection);
					ValidateStatementStructure(manager, context, node->statement);
				}

				void Visit(WfTryStatement* node)override
				{
					ValidateStatementStructure(manager, context, node->protectedStatement);
					if (node->catchStatement)
					{
						ValidateStatementStructure(manager, context, node->catchStatement);
					}
					if (node->finallyStatement)
					{
						ValidateStatementStructure(manager, context, node->finallyStatement);
					}
				}

				void Visit(WfBlockStatement* node)override
				{
					for (vint i = 0; i < node->statements.Count(); i++)
					{
						ValidateStatementStructure(manager, context, node->statements[i]);
					}
				}

				void Visit(WfExpressionStatement* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfVariableStatement* node)override
				{
					ValidateDeclarationStructure(manager, node->variable);
				}

				static void Execute(Ptr<WfStatement>& statement, WfLexicalScopeManager* manager, ValidateStructureContext* context)
				{
					ValidateStructureStatementVisitor visitor(manager, context);
					statement->Accept(&visitor);
					if (visitor.result)
					{
						statement = visitor.result;
					}
				}
			};

/***********************************************************************
ValidateStructure(Expression)
***********************************************************************/

			class ValidateStructureExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				ValidateStructureContext*				context;
				Ptr<WfExpression>						result;

				ValidateStructureExpressionVisitor(WfLexicalScopeManager* _manager, ValidateStructureContext* _context)
					:manager(_manager)
					, context(_context)
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
					ValidateExpressionStructure(manager, context, node->body);
				}

				void Visit(WfMemberExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->parent);
				}

				void Visit(WfChildExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->parent);
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
					if (node->expandedExpression)
					{
						ValidateExpressionStructure(manager, context, node->expandedExpression);
					}
				}

				void Visit(WfUnaryExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->operand);
				}

				void Visit(WfBinaryExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->first);
					ValidateExpressionStructure(manager, context, node->second);
				}

				void Visit(WfLetExpression* node)override
				{
					FOREACH(Ptr<WfLetVariable>, variable, node->variables)
					{
						ValidateExpressionStructure(manager, context, variable->value);
					}
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfIfExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->condition);
					ValidateExpressionStructure(manager, context, node->trueBranch);
					ValidateExpressionStructure(manager, context, node->falseBranch);
				}

				void Visit(WfRangeExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->begin);
					ValidateExpressionStructure(manager, context, node->end);
				}

				void Visit(WfSetTestingExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->element);
					ValidateExpressionStructure(manager, context, node->collection);
				}

				void Visit(WfConstructorExpression* node)override
				{
					FOREACH(Ptr<WfConstructorArgument>, argument, node->arguments)
					{
						ValidateExpressionStructure(manager, context, argument->key);
						if (argument->value)
						{
							ValidateExpressionStructure(manager, context, argument->value);
						}
					}
				}

				void Visit(WfInferExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfAttachEventExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->event);
					ValidateExpressionStructure(manager, context, node->function);
				}

				void Visit(WfDetachEventExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->handler);
				}

				void Visit(WfBindExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfObserveExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->parent);
					ValidateExpressionStructure(manager, context, node->expression);
					for (vint i = 0; i < node->events.Count(); i++)
					{
						ValidateExpressionStructure(manager, context, node->events[i]);
					}
				}

				void Visit(WfCallExpression* node)override
				{
					ValidateExpressionStructure(manager, context, node->function);
					for (vint i = 0; i < node->arguments.Count(); i++)
					{
						ValidateExpressionStructure(manager, context, node->arguments[i]);
					}
				}

				void Visit(WfFunctionExpression* node)override
				{
					ValidateDeclarationStructure(manager, node->function);
				}

				void Visit(WfNewTypeExpression* node)override
				{
					for (vint i = 0; i < node->arguments.Count(); i++)
					{
						ValidateExpressionStructure(manager, context, node->arguments[i]);
					}
					FOREACH(Ptr<WfFunctionDeclaration>, function, node->functions)
					{
						ValidateDeclarationStructure(manager, function);
					}
				}

				static void Execute(Ptr<WfExpression>& expression, WfLexicalScopeManager* manager, ValidateStructureContext* context)
				{
					ValidateStructureExpressionVisitor visitor(manager, context);
					expression->Accept(&visitor);
					if (visitor.result)
					{
						expression = visitor.result;
					}
				}
			};

/***********************************************************************
ValidateStructure
***********************************************************************/

			void ValidateModuleStructure(WfLexicalScopeManager* manager, Ptr<WfModule> module)
			{
				for (vint i = 0; i < module->declarations.Count(); i++)
				{
					ValidateDeclarationStructure(manager, module->declarations[i]);
				}
			}

			void ValidateDeclarationStructure(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration)
			{
				ValidateStructureDeclarationVisitor::Execute(declaration, manager);
			}

			void ValidateStatementStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfStatement>& statement)
			{
				ValidateStructureStatementVisitor::Execute(statement, manager, context);
			}

			void ValidateExpressionStructure(WfLexicalScopeManager* manager, ValidateStructureContext* context, Ptr<WfExpression>& expression)
			{
				ValidateStructureExpressionVisitor::Execute(expression, manager, context);
			}
		}
	}
}