#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;

/***********************************************************************
SearchOrderedName(Declaration)
***********************************************************************/

			class SearchOrderedNameDeclarationVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				SortedList<vint>&						names;

				SearchOrderedNameDeclarationVisitor(SortedList<vint>& _names)
					:names(_names)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					FOREACH(Ptr<WfDeclaration>, declaration, node->declarations)
					{
						SearchOrderedName(declaration, names);
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					SearchOrderedName(node->statement, names);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					SearchOrderedName(node->expression, names);
				}

				static void Execute(Ptr<WfDeclaration> declaration, SortedList<vint>& names)
				{
					SearchOrderedNameDeclarationVisitor visitor(names);
					declaration->Accept(&visitor);
				}
			};

/***********************************************************************
SearchOrderedName(Statement)
***********************************************************************/

			class SearchOrderedNameStatementVisitor : public Object, public WfStatement::IVisitor
			{
			public:
				SortedList<vint>&						names;

				SearchOrderedNameStatementVisitor(SortedList<vint>& _names)
					:names(_names)
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
						SearchOrderedName(node->expression, names);
					}
				}

				void Visit(WfDeleteStatement* node)override
				{
					SearchOrderedName(node->expression, names);
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					if (node->expression)
					{
						SearchOrderedName(node->expression, names);
					}
				}

				void Visit(WfIfStatement* node)override
				{
					SearchOrderedName(node->expression, names);
					node->trueBranch->Accept(this);
					if (node->falseBranch)
					{
						node->falseBranch->Accept(this);
					}
				}

				void Visit(WfSwitchStatement* node)override
				{
					SearchOrderedName(node->expression, names);
					FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
					{
						SearchOrderedName(switchCase->expression, names);
						switchCase->statement->Accept(this);
					}
					if (node->defaultBranch)
					{
						node->defaultBranch->Accept(this);
					}
				}

				void Visit(WfWhileStatement* node)override
				{
					SearchOrderedName(node->condition, names);
					node->statement->Accept(this);
				}

				void Visit(WfForEachStatement* node)override
				{
					SearchOrderedName(node->collection, names);
					node->statement->Accept(this);
				}

				void Visit(WfTryStatement* node)override
				{
					node->protectedStatement->Accept(this);
					if (node->catchStatement)
					{
						node->catchStatement->Accept(this);
					}
					if (node->finallyStatement)
					{
						node->finallyStatement->Accept(this);
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
					SearchOrderedName(node->expression, names);
				}

				void Visit(WfVariableStatement* node)override
				{
					SearchOrderedName(node->variable.Cast<WfDeclaration>(), names);
				}

				static void Execute(Ptr<WfStatement> statement, SortedList<vint>& names)
				{
					SearchOrderedNameStatementVisitor visitor(names);
					statement->Accept(&visitor);
				}
			};

/***********************************************************************
SearchOrderedName(Expression)
***********************************************************************/

			class SearchOrderedNameExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				SortedList<vint>&						names;

				SearchOrderedNameExpressionVisitor(SortedList<vint>& _names)
					:names(_names)
				{
				}

				void Visit(WfReferenceExpression* node)override
				{
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					vint name = wtoi(node->name.value.Sub(1, node->name.value.Length()));
					if (!names.Contains(name))
					{
						names.Add(name);
					}
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
					// names in nested ordered lambda expression is not counted
				}

				void Visit(WfMemberExpression* node)override
				{
					node->parent->Accept(this);
				}

				void Visit(WfChildExpression* node)override
				{
					node->parent->Accept(this);
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
						node->expandedExpression->Accept(this);
					}
				}

				void Visit(WfUnaryExpression* node)override
				{
					node->operand->Accept(this);
				}

				void Visit(WfBinaryExpression* node)override
				{
					node->first->Accept(this);
					node->second->Accept(this);
				}

				void Visit(WfLetExpression* node)override
				{
					FOREACH(Ptr<WfLetVariable>, variable, node->variables)
					{
						variable->value->Accept(this);
					}
					
					node->expression->Accept(this);
				}

				void Visit(WfIfExpression* node)override
				{
					node->condition->Accept(this);
					node->trueBranch->Accept(this);
					node->falseBranch->Accept(this);
				}

				void Visit(WfRangeExpression* node)override
				{
					node->begin->Accept(this);
					node->end->Accept(this);
				}

				void Visit(WfSetTestingExpression* node)override
				{
					node->element->Accept(this);
					node->collection->Accept(this);
				}

				void Visit(WfConstructorExpression* node)override
				{
					FOREACH(Ptr<WfConstructorArgument>, argument, node->arguments)
					{
						argument->value->Accept(this);
					}
				}

				void Visit(WfInferExpression* node)override
				{
					node->expression->Accept(this);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					node->expression->Accept(this);
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					if (node->expression)
					{
						node->expression->Accept(this);
					}
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					node->expression->Accept(this);
				}

				void Visit(WfAttachEventExpression* node)override
				{
					node->event->Accept(this);
					node->function->Accept(this);
				}

				void Visit(WfDetachEventExpression* node)override
				{
					node->handler->Accept(this);
				}

				void Visit(WfBindExpression* node)override
				{
					node->expression->Accept(this);
				}

				void Visit(WfObserveExpression* node)override
				{
					node->parent->Accept(this);
					node->expression->Accept(this);
					FOREACH(Ptr<WfExpression>, event, node->events)
					{
						event->Accept(this);
					}
				}

				void Visit(WfCallExpression* node)override
				{
					node->function->Accept(this);
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						argument->Accept(this);
					}
				}

				void Visit(WfFunctionExpression* node)override
				{
					SearchOrderedName(node->function.Cast<WfDeclaration>(), names);
				}

				void Visit(WfNewTypeExpression* node)override
				{
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						argument->Accept(this);
					}

					FOREACH(Ptr<WfFunctionDeclaration>, function, node->functions)
					{
						SearchOrderedName(function.Cast<WfDeclaration>(), names);
					}
				}

				static void Execute(Ptr<WfExpression> expression, SortedList<vint>& names)
				{
					SearchOrderedNameExpressionVisitor visitor(names);
					expression->Accept(&visitor);
				}
			};

/***********************************************************************
SearchOrderedName
***********************************************************************/

			void SearchOrderedName(Ptr<WfDeclaration> declaration, collections::SortedList<vint>& names)
			{
				SearchOrderedNameDeclarationVisitor::Execute(declaration, names);
			}

			void SearchOrderedName(Ptr<WfStatement> statement, collections::SortedList<vint>& names)
			{
				SearchOrderedNameStatementVisitor::Execute(statement, names);
			}

			void SearchOrderedName(Ptr<WfExpression> expression, collections::SortedList<vint>& names)
			{
				SearchOrderedNameExpressionVisitor::Execute(expression, names);
			}
		}
	}
}