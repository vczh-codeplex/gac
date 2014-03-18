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

			typedef WfInstruction Ins;

#define INSTRUCTION(X) context.assembly->instructions.Add(X);

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
					throw 0;
				}

				void Visit(WfContinueStatement* node)override
				{
					throw 0;
				}

				void Visit(WfReturnStatement* node)override
				{
					// todo: inline try-finally
					if (node->expression)
					{
						GenerateExpressionInstructions(context, node->expression);
					}
					else
					{
						INSTRUCTION(Ins::LoadValue(Value()));
					}
					INSTRUCTION(Ins::Return());
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
					FOREACH(Ptr<WfStatement>, statement, node->statements)
					{
						GenerateStatementInstructions(context, statement);
					}
				}

				void Visit(WfExpressionStatement* node)override
				{
					throw 0;
				}

				void Visit(WfVariableStatement* node)override
				{
					throw 0;
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
					throw 0;
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
					INSTRUCTION(Ins::LoadValue(BoxValue(node->value.value)));
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

				for (vint i = 0; i < assembly->instructions.Count(); i++)
				{
					WfInstruction& ins = assembly->instructions[i];
					switch (ins.code)
					{
					case WfInsCode::LoadLambda:
						ins.countParameter = assembly->functions[ins.indexParameter]->capturedVariableNames.Count();
						break;
					}
				}
				return assembly;
			}
		}
	}
}