#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace regex;
			using namespace parsing;
			using namespace reflection;
			using namespace reflection::description;
			using namespace runtime;

			typedef WfInstruction Ins;

#define INSTRUCTION(X) context.assembly->instructions.Add(X)

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
					context.assembly->functionByName.Add(meta->name, index);

					auto scope = context.manager->declarationScopes[node]->parentScope.Obj();
					auto symbol = From(scope->symbols[node->name.value])
						.Where([=](Ptr<WfLexicalSymbol> symbol)
						{
							return symbol->creatorDeclaration == node;
						})
						.First();
					context.globalFunctions.Add(symbol.Obj(), index);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					vint index = context.assembly->variableNames.Add(namePrefix + node->name.value);

					auto scope = context.manager->declarationScopes[node].Obj();
					auto symbol = scope->symbols[node->name.value][0];
					context.globalVariables.Add(symbol.Obj(), index);
				}
			};

			void GenerateGlobalDeclarationMetadata(WfCodegenContext& context, Ptr<WfDeclaration> declaration, const WString& namePrefix)
			{
				GenerateGlobalDeclarationMetadataVisitor visitor(context, namePrefix);
				declaration->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Initialize)
***********************************************************************/

			class GenerateInitializeInstructionsVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfCodegenContext&						context;

				GenerateInitializeInstructionsVisitor(WfCodegenContext& _context)
					:context(_context)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
				}

				void Visit(WfFunctionDeclaration* node)override
				{
				}

				void Visit(WfVariableDeclaration* node)override
				{
				}
			};

			void GenerateInitializeInstructions(WfCodegenContext& context, Ptr<WfDeclaration> declaration)
			{
				GenerateInitializeInstructionsVisitor visitor(context);
				declaration->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Declaration)
***********************************************************************/

			class GenerateDeclarationInstructionsVisitor : public Object, public WfDeclaration::IVisitor
			{
			public:
				WfCodegenContext&						context;

				GenerateDeclarationInstructionsVisitor(WfCodegenContext& _context)
					:context(_context)
				{
				}

				void Visit(WfNamespaceDeclaration* node)override
				{
				}

				void Visit(WfFunctionDeclaration* node)override
				{
					auto scope = context.manager->declarationScopes[node];
					auto symbol = From(scope->parentScope->symbols[node->name.value])
						.Where([=](Ptr<WfLexicalSymbol> symbol)
						{
							return symbol->creatorDeclaration == node;
						})
						.First();
					auto meta = context.assembly->functions[context.globalFunctions[symbol.Obj()]];

					auto functionContext = MakePtr<WfCodegenFunctionContext>();
					functionContext->function = meta;
					context.functionContext = functionContext;
					FOREACH_INDEXER(Ptr<WfFunctionArgument>, argument, index, node->arguments)
					{
						auto argumentSymbol = scope->symbols[argument->name.value][0];
						functionContext->arguments.Add(argumentSymbol.Obj(), index);
					}
					
					meta->firstInstruction = context.assembly->instructions.Count();
					GenerateStatementInstructions(context, node->statement);
					auto returnType = symbol->typeInfo->GetElementType()->GetGenericArgument(0);
					if (returnType->GetDecorator() == ITypeInfo::TypeDescriptor && returnType->GetTypeDescriptor()->GetValueSerializer())
					{
						auto serializer = returnType->GetTypeDescriptor()->GetValueSerializer();
						auto defaultText = serializer->GetDefaultText();
						Value result;
						serializer->Parse(defaultText, result);
						INSTRUCTION(Ins::LoadValue(result));
					}
					else
					{
						INSTRUCTION(Ins::LoadValue(Value()));
					}
					INSTRUCTION(Ins::Return());
					meta->lastInstruction = context.assembly->instructions.Count() - 1;
					context.functionContext = 0;

					GenerateClosureInstructions(context, functionContext);
				}

				void Visit(WfVariableDeclaration* node)override
				{
				}
			};

			void GenerateDeclarationInstructions(WfCodegenContext& context, Ptr<WfDeclaration> declaration)
			{
				GenerateDeclarationInstructionsVisitor visitor(context);
				declaration->Accept(&visitor);
			}

/***********************************************************************
GenerateInstructions(Closure)
***********************************************************************/

			void GenerateClosureInstructions_StaticMethod(WfCodegenContext& context, vint functionIndex, WfExpression* expression)
			{
				auto result = context.manager->expressionResolvings[expression];
				auto methodInfo = result.methodInfo;
				auto meta = context.assembly->functions[functionIndex];

				for (vint j = 0; j < methodInfo->GetParameterCount(); j++)
				{
					meta->argumentNames.Add(methodInfo->GetParameter(j)->GetName());
				}

				meta->firstInstruction = context.assembly->instructions.Count();
				for (vint j = 0; j < methodInfo->GetParameterCount(); j++)
				{
					INSTRUCTION(Ins::LoadLocalVar(j));
				}
				INSTRUCTION(Ins::LoadValue(Value()));
				INSTRUCTION(Ins::InvokeMethod(methodInfo, methodInfo->GetParameterCount()));
				INSTRUCTION(Ins::Return());
				meta->lastInstruction = context.assembly->instructions.Count() - 1;
			}

			void GenerateClosureInstructions_Method(WfCodegenContext& context, vint functionIndex, WfMemberExpression* expression)
			{
				auto result = context.manager->expressionResolvings[expression];
				auto methodInfo = result.methodInfo;
				auto meta = context.assembly->functions[functionIndex];

				for (vint j = 0; j < methodInfo->GetParameterCount(); j++)
				{
					meta->argumentNames.Add(methodInfo->GetParameter(j)->GetName());
				}
				meta->capturedVariableNames.Add(L"<this>");

				meta->firstInstruction = context.assembly->instructions.Count();
				for (vint j = 0; j < methodInfo->GetParameterCount(); j++)
				{
					INSTRUCTION(Ins::LoadLocalVar(j));
				}
				INSTRUCTION(Ins::LoadCapturedVar(0));
				INSTRUCTION(Ins::InvokeMethod(methodInfo, methodInfo->GetParameterCount()));
				INSTRUCTION(Ins::Return());
				meta->lastInstruction = context.assembly->instructions.Count() - 1;
			}

			void GenerateClosureInstructions_Function(WfCodegenContext& context, vint functionIndex, WfFunctionExpression* expression)
			{
				// next version
				throw 0;
			}

			void GenerateClosureInstructions_Ordered(WfCodegenContext& context, vint functionIndex, WfOrderedLambdaExpression* expression)
			{
				// next version
				throw 0;
			}

			void GenerateClosureInstructions(WfCodegenContext& context, Ptr<WfCodegenFunctionContext> functionContext)
			{
				for (vint i = 0; i < functionContext->closuresToCodegen.Count(); i++)
				{
					vint functionIndex = functionContext->closuresToCodegen.Keys()[i];
					auto closure = functionContext->closuresToCodegen.Values()[i];
					
					if (closure.staticMethodReferenceExpression)
					{
						GenerateClosureInstructions_StaticMethod(context, functionIndex, closure.staticMethodReferenceExpression);
					}
					else if (closure.methodReferenceExpression)
					{
						GenerateClosureInstructions_Method(context, functionIndex, closure.methodReferenceExpression);
					}
					else if (closure.functionExpression)
					{
						GenerateClosureInstructions_Function(context, functionIndex, closure.functionExpression);
					}
					else if (closure.orderedLambdaExpression)
					{
						GenerateClosureInstructions_Ordered(context, functionIndex, closure.orderedLambdaExpression);
					}
				}
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
					// next version: inline try-finally
					context.functionContext->GetCurrentLoopContext()->breakInstructions.Add(INSTRUCTION(Ins::Jump(-1)));
				}

				void Visit(WfContinueStatement* node)override
				{
					// next version: inline try-finally
					context.functionContext->GetCurrentLoopContext()->continueInstructions.Add(INSTRUCTION(Ins::Jump(-1)));
				}

				void Visit(WfReturnStatement* node)override
				{
					// next version: inline try-finally
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
					// next version
					throw 0;
				}

				void Visit(WfRaiseExceptionStatement* node)override
				{
					// next version: inline exit code
					// next version
					throw 0;
				}

				void Visit(WfIfStatement* node)override
				{
					vint variableIndex = -1;

					GenerateExpressionInstructions(context, node->expression);
					if (node->name.value != L"")
					{
						// TODO: test
						auto scope = context.manager->statementScopes[node];
						auto symbol = scope->symbols[node->name.value][0];
						auto function = context.functionContext->function;
						variableIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<if>" + node->name.value);
						context.functionContext->localVariables.Add(symbol.Obj(), variableIndex);
						INSTRUCTION(Ins::StoreLocalVar(variableIndex));
						INSTRUCTION(Ins::LoadLocalVar(variableIndex));
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::CompareReference());
					}
					INSTRUCTION(Ins::OpNot(WfInsType::Bool));
					vint fillElseIndex = INSTRUCTION(Ins::JumpIf(-1));

					GenerateStatementInstructions(context, node->trueBranch);
					if (variableIndex != -1)
					{
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::StoreLocalVar(variableIndex));
					}
					vint fillEndIndex = INSTRUCTION(Ins::Jump(-1));
					context.assembly->instructions[fillElseIndex].indexParameter = context.assembly->instructions.Count();

					if (node->falseBranch)
					{
						GenerateStatementInstructions(context, node->falseBranch);
					}
					context.assembly->instructions[fillEndIndex].indexParameter = context.assembly->instructions.Count();
				}

				void Visit(WfSwitchStatement* node)override
				{
					auto function = context.functionContext->function;
					vint variableIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<switch>");
					GenerateExpressionInstructions(context, node->expression);
					INSTRUCTION(Ins::StoreLocalVar(variableIndex));

					List<vint> caseInstructions, caseLabelIndices, breakInstructions;
					auto expressionResult = context.manager->expressionResolvings[node->expression.Obj()];
					auto expressionType = expressionResult.expectedType ? expressionResult.expectedType : expressionResult.type;
					FOREACH(Ptr<WfSwitchCase>, switchCase, node->caseBranches)
					{
						auto caseResult = context.manager->expressionResolvings[switchCase->expression.Obj()];
						auto caseType = caseResult.expectedType ? caseResult.expectedType : caseResult.type;
						auto mergedType = GetMergedType(expressionType, caseType);

						INSTRUCTION(Ins::LoadLocalVar(variableIndex));
						if (!IsSameType(expressionType.Obj(), mergedType.Obj()))
						{
							GenerateTypeCastInstructions(context, mergedType, true);
						}
						GenerateExpressionInstructions(context, switchCase->expression);
						if (!IsSameType(caseType.Obj(), mergedType.Obj()))
						{
							GenerateTypeCastInstructions(context, mergedType, true);
						}
						if (mergedType->GetDecorator() == ITypeInfo::RawPtr || mergedType->GetDecorator() == ITypeInfo::SharedPtr)
						{
							INSTRUCTION(Ins::CompareReference());
							INSTRUCTION(Ins::OpNot(WfInsType::Bool));
						}
						else
						{
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpNE());
						}
						caseInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));

						GenerateStatementInstructions(context, switchCase->statement);
						breakInstructions.Add(INSTRUCTION(Ins::Jump(-1)));
						caseLabelIndices.Add(context.assembly->instructions.Count());
					}

					if (node->defaultBranch)
					{
						GenerateStatementInstructions(context, node->defaultBranch);
					}
					
					vint breakLabelIndex = context.assembly->instructions.Count();
					for (vint i = 0; i < caseInstructions.Count(); i++)
					{
						context.assembly->instructions[caseInstructions[i]].indexParameter = caseLabelIndices[i];
					}
					FOREACH(vint, index, breakInstructions)
					{
						context.assembly->instructions[index].indexParameter = breakLabelIndex;
					}
					// next version: mark exit code
					INSTRUCTION(Ins::LoadValue(Value()));
					INSTRUCTION(Ins::StoreLocalVar(variableIndex));
				}

				void Visit(WfWhileStatement* node)override
				{
					vint continueLabelIndex = -1;
					vint breakLabelIndex = -1;
					vint loopLabelIndex = -1;
					auto loopContext = MakePtr<WfCodegenLoopContext>();
					context.functionContext->loopContextStack.Add(loopContext);

					loopLabelIndex = context.assembly->instructions.Count();
					continueLabelIndex = context.assembly->instructions.Count();
					GenerateExpressionInstructions(context, node->condition);
					INSTRUCTION(Ins::OpNot(WfInsType::Bool));
					loopContext->breakInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));
					GenerateStatementInstructions(context, node->statement);
					INSTRUCTION(Ins::Jump(loopLabelIndex));
					breakLabelIndex = context.assembly->instructions.Count();

					FOREACH(vint, index, loopContext->continueInstructions)
					{
						context.assembly->instructions[index].indexParameter = continueLabelIndex;
					}
					FOREACH(vint, index, loopContext->breakInstructions)
					{
						context.assembly->instructions[index].indexParameter = breakLabelIndex;
					}
					context.functionContext->loopContextStack.RemoveAt(context.functionContext->loopContextStack.Count() - 1);
				}

				void Visit(WfForEachStatement* node)override
				{
					vint continueLabelIndex = -1;
					vint breakLabelIndex = -1;
					vint loopLabelIndex = -1;
					auto loopContext = MakePtr<WfCodegenLoopContext>();
					context.functionContext->loopContextStack.Add(loopContext);

					auto scope = context.manager->statementScopes[node].Obj();
					auto symbol = scope->symbols[node->name.value][0];
					auto function = context.functionContext->function;
					vint elementIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<for>" + node->name.value);
					context.functionContext->localVariables.Add(symbol.Obj(), elementIndex);

					if (auto range = node->collection.Cast<WfRangeExpression>())
					{
						auto typeArgument = GetInstructionTypeArgument(symbol->typeInfo);
						vint beginIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<for-begin>" + node->name.value);
						vint endIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<for-end>" + node->name.value);
						GenerateExpressionInstructions(context, range->begin, symbol->typeInfo);
						if (range->beginBoundary == WfRangeBoundary::Exclusive)
						{
							INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
							INSTRUCTION(Ins::OpAdd(typeArgument));
						}
						INSTRUCTION(Ins::StoreLocalVar(beginIndex));
						GenerateExpressionInstructions(context, range->end, symbol->typeInfo);
						if (range->endBoundary == WfRangeBoundary::Exclusive)
						{
							INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
							INSTRUCTION(Ins::OpSub(typeArgument));
						}
						INSTRUCTION(Ins::StoreLocalVar(endIndex));

						if (node->direction == WfForEachDirection::Normal)
						{
							INSTRUCTION(Ins::LoadLocalVar(beginIndex));
							INSTRUCTION(Ins::StoreLocalVar(elementIndex));
							loopLabelIndex = INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadLocalVar(endIndex));
							INSTRUCTION(Ins::CompareLiteral(typeArgument));
							INSTRUCTION(Ins::OpGT());
							loopContext->breakInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));
						}
						else
						{
							INSTRUCTION(Ins::LoadLocalVar(endIndex));
							INSTRUCTION(Ins::StoreLocalVar(elementIndex));
							loopLabelIndex = INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadLocalVar(beginIndex));
							INSTRUCTION(Ins::CompareLiteral(typeArgument));
							INSTRUCTION(Ins::OpLT());
							loopContext->breakInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));
						}
						GenerateStatementInstructions(context, node->statement);
						continueLabelIndex = context.assembly->instructions.Count();
						if (node->direction == WfForEachDirection::Normal)
						{
							INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadLocalVar(endIndex));
							INSTRUCTION(Ins::CompareLiteral(typeArgument));
							INSTRUCTION(Ins::OpEQ());
							loopContext->breakInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));
							INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
							INSTRUCTION(Ins::OpAdd(typeArgument));
							INSTRUCTION(Ins::StoreLocalVar(elementIndex));
						}
						else
						{
							INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadLocalVar(beginIndex));
							INSTRUCTION(Ins::CompareLiteral(typeArgument));
							INSTRUCTION(Ins::OpEQ());
							loopContext->breakInstructions.Add(INSTRUCTION(Ins::JumpIf(-1)));
							INSTRUCTION(Ins::LoadLocalVar(elementIndex));
							INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
							INSTRUCTION(Ins::OpSub(typeArgument));
							INSTRUCTION(Ins::StoreLocalVar(elementIndex));
						}
						INSTRUCTION(Ins::Jump(loopLabelIndex));

						breakLabelIndex = context.assembly->instructions.Count();
						// next version: mark exit code
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::StoreLocalVar(beginIndex));
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::StoreLocalVar(endIndex));
					}
					else
					{
						// TODO: implement enumerable and reverse enumerable
						throw 0;
					}
					// next version: mark exit code
					INSTRUCTION(Ins::LoadValue(Value()));
					INSTRUCTION(Ins::StoreLocalVar(elementIndex));

					FOREACH(vint, index, loopContext->continueInstructions)
					{
						context.assembly->instructions[index].indexParameter = continueLabelIndex;
					}
					FOREACH(vint, index, loopContext->breakInstructions)
					{
						context.assembly->instructions[index].indexParameter = breakLabelIndex;
					}
					context.functionContext->loopContextStack.RemoveAt(context.functionContext->loopContextStack.Count() - 1);
				}

				void Visit(WfTryStatement* node)override
				{
					// next version
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
					GenerateExpressionInstructions(context, node->expression);
					INSTRUCTION(Ins::Pop());
				}

				void Visit(WfVariableStatement* node)override
				{
					auto manager = context.manager;
					auto scope = manager->declarationScopes[node->variable.Obj()];
					auto symbol = scope->symbols[node->variable->name.value][0].Obj();
					auto function = context.functionContext->function;
					vint index = function->argumentNames.Count() + function->localVariableNames.Add(node->variable->name.value);
					context.functionContext->localVariables.Add(symbol, index);

					GenerateExpressionInstructions(context, node->variable->expression);
					INSTRUCTION(Ins::StoreLocalVar(index));
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

				void VisitReferenceExpression(WfExpression* node)
				{
					auto result = context.manager->expressionResolvings[node];
					vint index = -1;
					if ((index = context.globalFunctions.Keys().IndexOf(result.symbol.Obj())) != -1)
					{
						vint functionIndex = context.globalFunctions.Values()[index];
						INSTRUCTION(Ins::LoadFunction(functionIndex));
					}
					else if ((index = context.globalVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
					{
						vint variableIndex = context.globalVariables.Values()[index];
						INSTRUCTION(Ins::LoadGlobalVar(variableIndex));
					}
					else if ((index = context.functionContext->capturedVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
					{
						vint variableIndex = context.functionContext->capturedVariables.Values()[index];
						INSTRUCTION(Ins::LoadCapturedVar(variableIndex));
					}
					else if ((index = context.functionContext->localVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
					{
						vint variableIndex = context.functionContext->localVariables.Values()[index];
						INSTRUCTION(Ins::LoadLocalVar(variableIndex));
					}
					else if ((index = context.functionContext->arguments.Keys().IndexOf(result.symbol.Obj())) != -1)
					{
						vint variableIndex = context.functionContext->arguments.Values()[index];
						INSTRUCTION(Ins::LoadLocalVar(variableIndex));
					}
					else if (result.methodInfo && result.methodInfo->IsStatic())
					{
						auto meta = MakePtr<WfAssemblyFunction>();
						meta->name = result.methodInfo->GetName() + L"<" + result.methodInfo->GetOwnerTypeDescriptor()->GetTypeName() + L">";
						vint functionIndex = context.assembly->functions.Add(meta);
						context.assembly->functionByName.Add(meta->name, functionIndex);
						
						WfCodegenLambdaContext lc;
						lc.staticMethodReferenceExpression = node;
						context.functionContext->closuresToCodegen.Add(functionIndex, lc);

						INSTRUCTION(Ins::LoadFunction(functionIndex));
					}
					else
					{
						// next version: named lambda reference
						throw 0;
					}
				}

				void Visit(WfTopQualifiedExpression* node)override
				{
					VisitReferenceExpression(node);
				}

				void Visit(WfReferenceExpression* node)override
				{
					VisitReferenceExpression(node);
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					// next version
					throw 0;
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
					// next version
					throw 0;
				}

				void Visit(WfMemberExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node];
					if (result.propertyInfo)
					{
						GenerateExpressionInstructions(context, node->parent);
						if (result.propertyInfo->GetGetter())
						{
							INSTRUCTION(Ins::InvokeMethod(result.propertyInfo->GetGetter(), 0));
						}
						else
						{
							INSTRUCTION(Ins::GetProperty(result.propertyInfo));
						}
					}
					else
					{
						auto meta = MakePtr<WfAssemblyFunction>();
						meta->name = result.methodInfo->GetName() + L"<" + result.methodInfo->GetOwnerTypeDescriptor()->GetTypeName() + L">";
						vint functionIndex = context.assembly->functions.Add(meta);
						context.assembly->functionByName.Add(meta->name, functionIndex);
						
						WfCodegenLambdaContext lc;
						lc.methodReferenceExpression = node;
						context.functionContext->closuresToCodegen.Add(functionIndex, lc);

						GenerateExpressionInstructions(context, node->parent);
						INSTRUCTION(Ins::LoadLambda(functionIndex, 1));
					}
				}

				void Visit(WfChildExpression* node)override
				{
					VisitReferenceExpression(node);
				}

				void Visit(WfLiteralExpression* node)override
				{
					switch (node->value)
					{
					case WfLiteralValue::Null:
						INSTRUCTION(Ins::LoadValue(Value()));
						break;
					case WfLiteralValue::True:
						INSTRUCTION(Ins::LoadValue(BoxValue(true)));
						break;
					case WfLiteralValue::False:
						INSTRUCTION(Ins::LoadValue(BoxValue(false)));
						break;
					}
				}

				void Visit(WfFloatingExpression* node)override
				{
					INSTRUCTION(Ins::LoadValue(BoxValue(wtof(node->value.value))));
				}

				void Visit(WfIntegerExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node];
					auto td = result.type->GetTypeDescriptor();
					if (td == description::GetTypeDescriptor<vint32_t>())
					{
						INSTRUCTION(Ins::LoadValue(BoxValue((vint32_t)wtoi(node->value.value))));
					}
					else if (td == description::GetTypeDescriptor<vint64_t>())
					{
						INSTRUCTION(Ins::LoadValue(BoxValue((vint64_t)wtoi64(node->value.value))));
					}
					else if (td == description::GetTypeDescriptor<vuint64_t>())
					{
						INSTRUCTION(Ins::LoadValue(BoxValue((vuint64_t)wtou64(node->value.value))));
					}
				}

				void Visit(WfStringExpression* node)override
				{
					INSTRUCTION(Ins::LoadValue(BoxValue(node->value.value)));
				}

				void Visit(WfFormatExpression* node)override
				{
					GenerateExpressionInstructions(context, node->expandedExpression);
				}

				void Visit(WfUnaryExpression* node)override
				{
					auto type = GenerateExpressionInstructions(context, node->operand);
					switch (node->op)
					{
					case WfUnaryOperator::Not:
						INSTRUCTION(Ins::OpNot(GetInstructionTypeArgument(type)));
						break;
					case WfUnaryOperator::Positive:
						INSTRUCTION(Ins::OpPositive(GetInstructionTypeArgument(type)));
						break;
					case WfUnaryOperator::Negative:
						INSTRUCTION(Ins::OpNegative(GetInstructionTypeArgument(type)));
						break;
					}
				}

				void Visit(WfBinaryExpression* node)override
				{
					if (node->op == WfBinaryOperator::Assign)
					{
						if (auto binary = node->first.Cast<WfBinaryExpression>())
						{
							auto result = context.manager->expressionResolvings[binary->first.Obj()];
							auto containerType = result.expectedType ? result.expectedType : result.type;
							auto methodInfo = containerType->GetTypeDescriptor()->GetMethodGroupByName(L"Set", true)->GetMethod(0);
							GenerateExpressionInstructions(context, binary->second);
							INSTRUCTION(Ins::Duplicate(0));
							GenerateExpressionInstructions(context, node->second);
							GenerateExpressionInstructions(context, binary->first);
							INSTRUCTION(Ins::InvokeMethod(methodInfo, 2));
							INSTRUCTION(Ins::Pop());
						}
						else if (auto member = node->first.Cast<WfMemberExpression>())
						{
							auto result = context.manager->expressionResolvings[member.Obj()];
							auto methodInfo = result.propertyInfo->GetSetter();
							GenerateExpressionInstructions(context, node->second);
							INSTRUCTION(Ins::Duplicate(0));
							GenerateExpressionInstructions(context, member->parent);
							INSTRUCTION(Ins::InvokeMethod(methodInfo, 1));
							INSTRUCTION(Ins::Pop());
						}
						else
						{
							GenerateExpressionInstructions(context, node->second);
							INSTRUCTION(Ins::Duplicate(0));
							auto result = context.manager->expressionResolvings[node->first.Obj()];
							vint index = -1;
							if ((index = context.globalVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
							{
								vint variableIndex = context.globalVariables.Values()[index];
								INSTRUCTION(Ins::StoreGlobalVar(variableIndex));
							}
							else if ((index = context.functionContext->localVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
							{
								vint variableIndex = context.functionContext->localVariables.Values()[index];
								INSTRUCTION(Ins::StoreLocalVar(variableIndex));
							}
						}
					}
					else if (node->op == WfBinaryOperator::Index)
					{
						auto result = context.manager->expressionResolvings[node->first.Obj()];
						auto containerType = result.expectedType ? result.expectedType : result.type;
						auto methodInfo = containerType->GetTypeDescriptor()->GetMethodGroupByName(L"Get", true)->GetMethod(0);
						GenerateExpressionInstructions(context, node->second);
						GenerateExpressionInstructions(context, node->first);
						INSTRUCTION(Ins::InvokeMethod(methodInfo, 1));
					}
					else if (node->op == WfBinaryOperator::Concat)
					{
						auto type = TypeInfoRetriver<WString>::CreateTypeInfo();
						GenerateExpressionInstructions(context, node->first, type);
						GenerateExpressionInstructions(context, node->second, type);
						INSTRUCTION(Ins::OpConcat());
					}
					else if (node->op == WfBinaryOperator::FailedThen)
					{
						// next version: FailedThen
						throw 0;
					}
					else
					{
						Ptr<ITypeInfo> mergedType;
						switch (node->op)
						{
						case WfBinaryOperator::Exp:
						case WfBinaryOperator::Add:
						case WfBinaryOperator::Sub:
						case WfBinaryOperator::Mul:
						case WfBinaryOperator::Div:
						case WfBinaryOperator::Shl:
						case WfBinaryOperator::Shr:
							{
								auto result = context.manager->expressionResolvings[node];
								mergedType = result.type;
							}
							break;
						default:
							{
								auto firstResult = context.manager->expressionResolvings[node->first.Obj()];
								auto secondResult = context.manager->expressionResolvings[node->second.Obj()];
								auto firstType = firstResult.expectedType ? firstResult.expectedType : firstResult.type;
								auto secondType = secondResult.expectedType ? secondResult.expectedType : secondResult.type;
								if (node->op == WfBinaryOperator::EQ || node->op == WfBinaryOperator::NE)
								{
									if (firstType->GetDecorator() == ITypeInfo::RawPtr || firstType->GetDecorator() == ITypeInfo::SharedPtr)
									{
										GenerateExpressionInstructions(context, node->first);
										GenerateExpressionInstructions(context, node->second);
										INSTRUCTION(Ins::CompareReference());
										if (node->op == WfBinaryOperator::NE)
										{
											INSTRUCTION(Ins::OpNot(WfInsType::Bool));
										}
										return;
									}
								}
								mergedType = GetMergedType(firstType, secondType);
							}
						}

						GenerateExpressionInstructions(context, node->first, mergedType);
						GenerateExpressionInstructions(context, node->second, mergedType);

						switch (node->op)
						{
						case WfBinaryOperator::Exp:
							INSTRUCTION(Ins::OpExp(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Add:
							INSTRUCTION(Ins::OpAdd(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Sub:
							INSTRUCTION(Ins::OpSub(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Mul:
							INSTRUCTION(Ins::OpMul(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Div:
							INSTRUCTION(Ins::OpDiv(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Shl:
							INSTRUCTION(Ins::OpShl(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Shr:
							INSTRUCTION(Ins::OpShr(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::LT:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpLT());
							break;
						case WfBinaryOperator::GT:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpGT());
							break;
						case WfBinaryOperator::LE:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpLE());
							break;
						case WfBinaryOperator::GE:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpGE());
							break;
						case WfBinaryOperator::EQ:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpEQ());
							break;
						case WfBinaryOperator::NE:
							INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(mergedType)));
							INSTRUCTION(Ins::OpNE());
							break;
						case WfBinaryOperator::Xor:
							INSTRUCTION(Ins::OpXor(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::And:
							INSTRUCTION(Ins::OpAnd(GetInstructionTypeArgument(mergedType)));
							break;
						case WfBinaryOperator::Or:
							INSTRUCTION(Ins::OpOr(GetInstructionTypeArgument(mergedType)));
							break;
						}
					}
				}

				void Visit(WfLetExpression* node)override
				{
					auto scope = context.manager->expressionScopes[node].Obj();
					Array<vint> variableIndices(node->variables.Count());
					auto function = context.functionContext->function;
					FOREACH_INDEXER(Ptr<WfLetVariable>, var, index, node->variables)
					{
						auto symbol = scope->symbols[var->name.value][0];
						vint variableIndex = function->argumentNames.Count() + function->localVariableNames.Add(L"<let>" + var->name.value);
						context.functionContext->localVariables.Add(symbol.Obj(), variableIndex);
						variableIndices[index] = variableIndex;

						GenerateExpressionInstructions(context, var->value);
						INSTRUCTION(Ins::StoreLocalVar(variableIndex));
					}
					GenerateExpressionInstructions(context, node->expression);
					FOREACH_INDEXER(Ptr<WfLetVariable>, var, index, node->variables)
					{
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::StoreLocalVar(variableIndices[index]));
					}
				}

				void Visit(WfIfExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node];
					GenerateExpressionInstructions(context, node->condition);
					vint fillTrueIndex = INSTRUCTION(Ins::JumpIf(-1));
					GenerateExpressionInstructions(context, node->falseBranch, result.type);
					vint fillEndIndex = INSTRUCTION(Ins::Jump(-1));
					context.assembly->instructions[fillTrueIndex].indexParameter = context.assembly->instructions.Count();
					GenerateExpressionInstructions(context, node->trueBranch, result.type);
					context.assembly->instructions[fillEndIndex].indexParameter = context.assembly->instructions.Count();
				}

				void Visit(WfRangeExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node];
					auto elementType = result.type->GetElementType()->GetGenericArgument(0);
					auto type = GetInstructionTypeArgument(elementType);
					
					GenerateExpressionInstructions(context, node->begin, elementType);
					if (node->beginBoundary == WfRangeBoundary::Exclusive)
					{
						INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
						INSTRUCTION(Ins::OpAdd(type));
					}
					
					GenerateExpressionInstructions(context, node->end, elementType);
					if (node->endBoundary == WfRangeBoundary::Exclusive)
					{
						INSTRUCTION(Ins::LoadValue(BoxValue<vint>(1)));
						INSTRUCTION(Ins::OpSub(type));
					}

					INSTRUCTION(Ins::CreateRange(type));
				}

				void Visit(WfSetTestingExpression* node)override
				{
					if (auto range = node->collection.Cast<WfRangeExpression>())
					{
						auto resultElement = context.manager->expressionResolvings[node->element.Obj()];
						auto resultBegin = context.manager->expressionResolvings[range->begin.Obj()];
						auto resultEnd = context.manager->expressionResolvings[range->end.Obj()];
						auto typeElement = resultElement.expectedType ? resultElement.expectedType : resultElement.type;
						auto typeBegin = resultBegin.expectedType ? resultBegin.expectedType : resultBegin.type;
						auto typeEnd = resultEnd.expectedType ? resultEnd.expectedType : resultEnd.type;
						auto typeLeft = GetMergedType(typeElement, typeBegin);
						auto typeRight = GetMergedType(typeElement, typeEnd);

						auto function = context.functionContext->function;
						vint index = function->argumentNames.Count() + function->localVariableNames.Add(L"<anonymous-range-test>");
						GenerateExpressionInstructions(context, node->element);
						INSTRUCTION(Ins::StoreLocalVar(index));
						
						INSTRUCTION(Ins::LoadLocalVar(index));
						if (!IsSameType(typeElement.Obj(), typeLeft.Obj()))
						{
							GenerateTypeCastInstructions(context, typeLeft, true);
						}
						GenerateExpressionInstructions(context, range->begin, typeLeft);
						INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(typeLeft)));
						if (range->beginBoundary == WfRangeBoundary::Exclusive)
						{
							INSTRUCTION(Ins::OpGT());
						}
						else
						{
							INSTRUCTION(Ins::OpGE());
						}

						INSTRUCTION(Ins::LoadLocalVar(index));
						if (!IsSameType(typeElement.Obj(), typeRight.Obj()))
						{
							GenerateTypeCastInstructions(context, typeRight, true);
						}
						GenerateExpressionInstructions(context, range->end, typeRight);
						INSTRUCTION(Ins::CompareLiteral(GetInstructionTypeArgument(typeRight)));
						if (range->endBoundary == WfRangeBoundary::Exclusive)
						{
							INSTRUCTION(Ins::OpLT());
						}
						else
						{
							INSTRUCTION(Ins::OpLE());
						}

						INSTRUCTION(Ins::OpAnd(WfInsType::Bool));
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::StoreLocalVar(index));
					}
					else
					{
						auto result = context.manager->expressionResolvings[node->collection.Obj()];
						auto type = result.expectedType ? result.expectedType : result.type;

						GenerateExpressionInstructions(context, node->element);
						GenerateExpressionInstructions(context, node->collection);

						auto tdList = description::GetTypeDescriptor<IValueReadonlyList>();
						if (result.type->GetTypeDescriptor()->CanConvertTo(tdList))
						{
							auto method = tdList->GetMethodGroupByName(L"Contains", true)->GetMethod(0);
							INSTRUCTION(Ins::InvokeMethod(method, 1));
						}
						else
						{
							INSTRUCTION(Ins::TestElementInSet());
						}
					}
				}

				void Visit(WfConstructorExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node];

					if (result.type->GetTypeDescriptor() == description::GetTypeDescriptor<IValueEnumerable>()
						|| result.type->GetTypeDescriptor() == description::GetTypeDescriptor<IValueReadonlyList>()
						|| result.type->GetTypeDescriptor() == description::GetTypeDescriptor<IValueList>())
					{
						Ptr<ITypeInfo> keyType = CopyTypeInfo(result.type->GetElementType()->GetGenericArgument(0));
						FOREACH(Ptr<WfConstructorArgument>, argument, From(node->arguments).Reverse())
						{
							GenerateExpressionInstructions(context, argument->key, keyType);
						}
						INSTRUCTION(Ins::CreateArray(node->arguments.Count()));
					}
					else
					{
						Ptr<ITypeInfo> keyType = CopyTypeInfo(result.type->GetElementType()->GetGenericArgument(0));
						Ptr<ITypeInfo> valueType = CopyTypeInfo(result.type->GetElementType()->GetGenericArgument(1));
						FOREACH(Ptr<WfConstructorArgument>, argument, From(node->arguments).Reverse())
						{
							GenerateExpressionInstructions(context, argument->key, keyType);
							GenerateExpressionInstructions(context, argument->value, valueType);
						}
						INSTRUCTION(Ins::CreateMap(node->arguments.Count() * 2));
					}
				}

				void Visit(WfInferExpression* node)override
				{
					GenerateExpressionInstructions(context, node->expression);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					if (node->strategy == WfTypeCastingStrategy::Strong)
					{
						auto result = context.manager->expressionResolvings[node];
						GenerateExpressionInstructions(context, node->expression, result.type);
					}
					else
					{
						auto scope = context.manager->expressionScopes[node].Obj();
						auto type = CreateTypeInfoFromType(scope, node->type);
						GenerateExpressionInstructions(context, node->expression);
						GenerateTypeCastInstructions(context, type, false);
					}
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					switch (node->test)
					{
					case WfTypeTesting::IsNull:
						GenerateExpressionInstructions(context, node->expression);
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::CompareReference());
						break;
					case WfTypeTesting::IsNotNull:
						GenerateExpressionInstructions(context, node->expression);
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::CompareReference());
						INSTRUCTION(Ins::OpNot(WfInsType::Bool));
						break;
					case WfTypeTesting::IsType:
						{
							auto scope = context.manager->expressionScopes[node].Obj();
							auto type = CreateTypeInfoFromType(scope, node->type);
							GenerateExpressionInstructions(context, node->expression);
							GenerateTypeTestingInstructions(context, type);
						}
						break;
					case WfTypeTesting::IsNotType:
						{
							auto scope = context.manager->expressionScopes[node].Obj();
							auto type = CreateTypeInfoFromType(scope, node->type);
							GenerateExpressionInstructions(context, node->expression);
							GenerateTypeTestingInstructions(context, type);
							INSTRUCTION(Ins::OpNot(WfInsType::Bool));
						}
						break;
					}
				}

				void Visit(WfTypeOfTypeExpression* node)override
				{
					auto scope = context.manager->expressionScopes[node].Obj();
					auto type = CreateTypeInfoFromType(scope, node->type);
					auto value = Value::From(type->GetTypeDescriptor());
					INSTRUCTION(Ins::LoadValue(value));
				}

				void Visit(WfTypeOfExpressionExpression* node)override
				{
					GenerateExpressionInstructions(context, node->expression);
					INSTRUCTION(Ins::GetType());
				}

				void Visit(WfAttachEventExpression* node)override
				{
					auto result = context.manager->expressionResolvings[node->event.Obj()];
					auto parent = node->event.Cast<WfMemberExpression>()->parent;
					GenerateExpressionInstructions(context, parent);
					GenerateExpressionInstructions(context, node->function);
					INSTRUCTION(Ins::AttachEvent(result.eventInfo));
				}

				void Visit(WfDetachEventExpression* node)override
				{
					GenerateExpressionInstructions(context, node->handler);
					INSTRUCTION(Ins::DetachEvent());
				}

				void Visit(WfBindExpression* node)override
				{
					// next version
					throw 0;
				}

				void Visit(WfObserveExpression* node)override
				{
					// next version
					throw 0;
				}

				void Visit(WfCallExpression* node)override
				{
					FOREACH(Ptr<WfExpression>, argument, node->arguments)
					{
						GenerateExpressionInstructions(context, argument);
					}

					auto result = context.manager->expressionResolvings[node->function.Obj()];
					if (result.methodInfo)
					{
						if (result.methodInfo->IsStatic())
						{
							INSTRUCTION(Ins::LoadValue(Value()));
						}
						else
						{
							auto member = node->function.Cast<WfMemberExpression>();
							GenerateExpressionInstructions(context, member->parent);
						}

						INSTRUCTION(Ins::InvokeMethod(result.methodInfo, node->arguments.Count()));
						return;
					}
					else if (result.symbol)
					{
						vint index = context.globalFunctions.Keys().IndexOf(result.symbol.Obj());
						if (index != -1)
						{
							vint functionIndex = context.globalFunctions.Values()[index];
							INSTRUCTION(Ins::Invoke(functionIndex, node->arguments.Count()));
							return;
						}
					}

					GenerateExpressionInstructions(context, node->function);
					INSTRUCTION(Ins::InvokeProxy(node->arguments.Count()));
				}

				void Visit(WfFunctionExpression* node)override
				{
					// next version
					throw 0;
				}

				void Visit(WfNewTypeExpression* node)override
				{
					if (node->functions.Count() == 0)
					{
						auto result = context.manager->expressionResolvings[node];
						FOREACH(Ptr<WfExpression>, argument, node->arguments)
						{
							GenerateExpressionInstructions(context, argument);
						}
						INSTRUCTION(Ins::LoadValue(Value()));
						INSTRUCTION(Ins::InvokeMethod(result.methodInfo, node->arguments.Count()));
					}
					else
					{
						// next version
						throw 0;
					}
				}
			};

			Ptr<reflection::description::ITypeInfo> GenerateExpressionInstructions(WfCodegenContext& context, Ptr<WfExpression> expression, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				GenerateExpressionInstructionsVisitor visitor(context);
				expression->Accept(&visitor);

				auto result = context.manager->expressionResolvings[expression.Obj()];
				auto type = result.type;

				if (result.expectedType && !IsSameType(type.Obj(), result.expectedType.Obj()))
				{
					type = result.expectedType;
					GenerateTypeCastInstructions(context, type, true);
				}

				if (expectedType && !IsSameType(type.Obj(), expectedType.Obj()))
				{
					type = expectedType;
					GenerateTypeCastInstructions(context, type, true);
				}

				return type;
			}

/***********************************************************************
GenerateTypeCastInstructions
***********************************************************************/

			void GenerateTypeCastInstructions(WfCodegenContext& context, Ptr<reflection::description::ITypeInfo> expectedType, bool strongCast)
			{
				if (strongCast)
				{
					switch (expectedType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
						INSTRUCTION(Ins::ConvertToType(Value::RawPtr, expectedType->GetTypeDescriptor()));
						break;
					case ITypeInfo::SharedPtr:
						INSTRUCTION(Ins::ConvertToType(Value::SharedPtr, expectedType->GetTypeDescriptor()));
						break;
					case ITypeInfo::Nullable:
					case ITypeInfo::TypeDescriptor:
					case ITypeInfo::Generic:
						INSTRUCTION(Ins::ConvertToType(Value::Text, expectedType->GetTypeDescriptor()));
						break;
					}
				}
				else
				{
					switch (expectedType->GetDecorator())
					{
					case ITypeInfo::RawPtr:
						INSTRUCTION(Ins::TryConvertToType(Value::RawPtr, expectedType->GetTypeDescriptor()));
						break;
					case ITypeInfo::SharedPtr:
						INSTRUCTION(Ins::TryConvertToType(Value::SharedPtr, expectedType->GetTypeDescriptor()));
						break;
					case ITypeInfo::Nullable:
					case ITypeInfo::TypeDescriptor:
					case ITypeInfo::Generic:
						INSTRUCTION(Ins::TryConvertToType(Value::Text, expectedType->GetTypeDescriptor()));
						break;
					}
				}
			}

/***********************************************************************
GetInstructionTypeArgument
***********************************************************************/

			void GenerateTypeTestingInstructions(WfCodegenContext& context, Ptr<reflection::description::ITypeInfo> expectedType)
			{
				switch (expectedType->GetDecorator())
				{
				case ITypeInfo::RawPtr:
					INSTRUCTION(Ins::TestType(Value::RawPtr, expectedType->GetTypeDescriptor()));
					break;
				case ITypeInfo::SharedPtr:
					INSTRUCTION(Ins::TestType(Value::SharedPtr, expectedType->GetTypeDescriptor()));
					break;
				case ITypeInfo::Nullable:
				case ITypeInfo::TypeDescriptor:
				case ITypeInfo::Generic:
					INSTRUCTION(Ins::TestType(Value::Text, expectedType->GetTypeDescriptor()));
					break;
				}
			}

/***********************************************************************
GenerateTypeCastInstructions
***********************************************************************/

			runtime::WfInsType GetInstructionTypeArgument(Ptr<reflection::description::ITypeInfo> expectedType)
			{
				auto td = expectedType->GetTypeDescriptor();
				if (td == GetTypeDescriptor<bool>()) return WfInsType::Bool;
				if (td == GetTypeDescriptor<vint8_t>()) return WfInsType::I1;
				if (td == GetTypeDescriptor<vint16_t>()) return WfInsType::I2;
				if (td == GetTypeDescriptor<vint32_t>()) return WfInsType::I4;
				if (td == GetTypeDescriptor<vint64_t>()) return WfInsType::I8;
				if (td == GetTypeDescriptor<vuint8_t>()) return WfInsType::U1;
				if (td == GetTypeDescriptor<vuint16_t>()) return WfInsType::U2;
				if (td == GetTypeDescriptor<vuint32_t>()) return WfInsType::U4;
				if (td == GetTypeDescriptor<vuint64_t>()) return WfInsType::U8;
				if (td == GetTypeDescriptor<float>()) return WfInsType::F4;
				if (td == GetTypeDescriptor<double>()) return WfInsType::F8;
				if (td == GetTypeDescriptor<WString>()) return WfInsType::String;
				return WfInsType::Unknown;
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

				{
					auto meta = MakePtr<WfAssemblyFunction>();
					meta->name = L"<initialize>";
					vint functionIndex = assembly->functions.Add(meta);
					assembly->functionByName.Add(meta->name, functionIndex);

					auto functionContext = MakePtr<WfCodegenFunctionContext>();
					functionContext->function = meta;
					context.functionContext = functionContext;
					
					meta->firstInstruction = assembly->instructions.Count();
					FOREACH(Ptr<WfModule>, module, manager->modules)
					{
						FOREACH(Ptr<WfDeclaration>, decl, module->declarations)
						{
							GenerateInitializeInstructions(context, decl);
						}
					}
					INSTRUCTION(Ins::LoadValue(Value()));
					INSTRUCTION(Ins::Return());
					meta->lastInstruction = assembly->instructions.Count() - 1;

					context.functionContext = 0;
					GenerateClosureInstructions(context, functionContext);
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

#undef INSTRUCTION

/***********************************************************************
Compile
***********************************************************************/

			Ptr<runtime::WfAssembly> Compile(Ptr<parsing::tabling::ParsingTable> table, WfLexicalScopeManager* manager, collections::List<WString>& moduleCodes, collections::List<Ptr<parsing::ParsingError>>& errors)
			{
				manager->Clear(true, true);
				FOREACH_INDEXER(WString, code, index, moduleCodes)
				{
					Ptr<ParsingTreeNode> node = WfParseModuleAsParsingTreeNode(code, table, errors);
					if (node)
					{
						List<RegexToken> tokens;
						Ptr<WfModule> module = WfConvertParsingTreeNode(node, tokens).Cast<WfModule>();
						if (module)
						{
							manager->modules.Add(module);
						}
					}
				}
				if (errors.Count() > 0) return 0;

				manager->Rebuild(true);
				if (manager->errors.Count() > 0)
				{
					CopyFrom(errors, manager->errors);
					return 0;
				}

				return GenerateAssembly(manager);
			}

			Ptr<runtime::WfAssembly> Compile(Ptr<parsing::tabling::ParsingTable> table, collections::List<WString>& moduleCodes, collections::List<Ptr<parsing::ParsingError>>& errors)
			{
				WfLexicalScopeManager manager(table);
				return Compile(table, &manager, moduleCodes, errors);
			}
		}
	}
}