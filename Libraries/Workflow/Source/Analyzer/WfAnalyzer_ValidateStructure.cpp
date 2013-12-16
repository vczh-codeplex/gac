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
					throw 0;
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					throw 0;
				}

				void Visit(WfVariableDeclaration* node)override
				{
					throw 0;
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

				ValidateStructureStatementVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
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

				static void Execute(Ptr<WfStatement> statement, WfLexicalScopeManager* manager)
				{
					ValidateStructureStatementVisitor visitor(manager);
					statement->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateStructure(Expression)
***********************************************************************/

			class ValidateStructureExpressionVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;

				ValidateStructureExpressionVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
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

				static void Execute(Ptr<WfExpression> expression, WfLexicalScopeManager* manager)
				{
					ValidateStructureExpressionVisitor visitor(manager);
					expression->Accept(&visitor);
				}
			};

/***********************************************************************
ValidateStructure
***********************************************************************/

			void ValidateModuleStructure(WfLexicalScopeManager* manager, Ptr<WfModule> module)
			{
				FOREACH(Ptr<WfDeclaration>, declaration, module->declarations)
				{
					ValidateDeclarationStructure(manager, declaration);
				}
			}

			void ValidateDeclarationStructure(WfLexicalScopeManager* manager, Ptr<WfDeclaration> declaration)
			{
				ValidateStructureDeclarationVisitor::Execute(declaration, manager);
			}

			void ValidateStatementStructure(WfLexicalScopeManager* manager, Ptr<WfStatement> statement)
			{
				ValidateStructureStatementVisitor::Execute(statement, manager);
			}

			void ValidateExpressionStructure(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				ValidateStructureExpressionVisitor::Execute(expression, manager);
			}
		}
	}
}