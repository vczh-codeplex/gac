#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace runtime;

/***********************************************************************
GenerateGlobalDeclarationMetadata
***********************************************************************/

			class GenerateGlobalDeclarationMetadataVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfCodegenContext&						context;
				WString									namePrefix;

				GenerateGlobalDeclarationMetadataVisitor(WfCodegenContext& _context, const WString& _namePrefix)
					:context(_context)
					, namePrefix(_namePrefix)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
					FOREACH(Ptr<WfDeclaration>, decl, node->declarations)
					{
						GenerateGlobalDeclarationMetadata(context, decl, namePrefix + node->name.value + L"::");
					}
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					auto meta = MakePtr<WfAssemblyFunction>();
					meta->name = namePrefix + node->name.value;
					FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
					{
						meta->argumentNames.Add(argument->name.value);
					}

					vint index = context.assembly->functions.Add(meta);
					context.globalFunctions.Add(node, index);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					vint index = context.assembly->variableNames.Add(namePrefix + node->name.value);
					context.globalVariables.Add(node, index);
				}
			};

			void GenerateGlobalDeclarationMetadata(WfCodegenContext& context, Ptr<WfDeclaration> declaration, const WString& namePrefix)
			{
				GenerateGlobalDeclarationMetadataVisitor visitor(context, namePrefix);
				declaration->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Declaration)
***********************************************************************/

			class GenerateInstructionsVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfCodegenContext&						context;

				GenerateInstructionsVisitor(WfCodegenContext& _context)
					:context(_context)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					auto functionContext = MakePtr<WfCodegenFunctionContext>();
					functionContext->function = context.assembly->functions[context.globalFunctions[node]];
					context.functionContext = functionContext;
					GenerateStatementInstructions(context, node->statement);
					context.functionContext = 0;
				}

				void Visit(WfVariableDeclaration* node)override
				{
				}
			};

			void GenerateDeclarationInstructions(WfCodegenContext& context, Ptr<WfDeclaration> declaration)
			{
				GenerateInstructionsVisitor visitor(context);
				declaration->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Statement)
***********************************************************************/

			class GenerateStatementInstructionsVisitor : public Object, public WfStatement::IVisitor
			{
			public:
				WfCodegenContext&						context;

				GenerateStatementInstructionsVisitor(WfCodegenContext& _context)
					:context(_context)
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
				}

				void Visit(WfExpressionStatement* node)override
				{
				}

				void Visit(WfVariableStatement* node)override
				{
				}
			};

			void GenerateStatementInstructions(WfCodegenContext& context, Ptr<WfStatement> statement)
			{
				GenerateStatementInstructionsVisitor visitor(context);
				statement->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Expression)
***********************************************************************/

			class GenerateExpressionInstructionsVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfCodegenContext&						context;

				GenerateExpressionInstructionsVisitor(WfCodegenContext& _context)
					:context(_context)
				{
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
				}

				void Visit(WfMemberExpression* node)override
				{
				}

				void Visit(WfChildExpression* node)override
				{
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
			};

			void GenerateExpressionInstructions(WfCodegenContext& context, Ptr<WfExpression> expression)
			{
				GenerateExpressionInstructionsVisitor visitor(context);
				expression->Accept(&visitor);
			}

/***********************************************************************
GenerateAssembly
***********************************************************************/

			Ptr<runtime::WfAssembly> GenerateAssembly(WfLexicalScopeManager* manager)
			{
				auto assembly = MakePtr<WfAssembly>();
				WfCodegenContext context(assembly, manager);
				FOREACH(Ptr<WfModule>, module, manager->modules)
				{
					FOREACH(Ptr<WfDeclaration>, decl, module->declarations)
					{
						GenerateGlobalDeclarationMetadata(context, decl);
					}
				}
				
				FOREACH(Ptr<WfModule>, module, manager->modules)
				{
					FOREACH(Ptr<WfDeclaration>, decl, module->declarations)
					{
						GenerateDeclarationInstructions(context, decl);
					}
				}
				return assembly;
			}
		}
	}
}