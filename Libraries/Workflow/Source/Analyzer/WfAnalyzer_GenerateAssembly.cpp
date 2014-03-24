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
					auto scope = context.manager->declarationScopes[node]->parentScope.Obj();
					auto symbol = From(scope->symbols[node->name.value])
						.Where([=](Ptr<WfLexicalSymbol> symbol)
						{
							return symbol->creatorDeclaration == node;
						})
						.First();
					auto meta = context.assembly->functions[context.globalFunctions[symbol.Obj()]];

					auto functionContext = MakePtr<WfCodegenFunctionContext>();
					functionContext->function = meta;
					context.functionContext = functionContext;
					
					meta->firstInstruction = context.assembly->instructions.Count();
					GenerateStatementInstructions(context, node->statement);
					meta->lastInstruction = context.assembly->instructions.Count() - 1;
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
						vint argumentCount = context.functionContext->function->argumentNames.Count();
						vint variableIndex = context.functionContext->localVariables.Values()[index];
						INSTRUCTION(Ins::LoadLocalVar(argumentCount + variableIndex));
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
						throw 0;
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
							GenerateExpressionInstructions(context, node->second);
							GenerateExpressionInstructions(context, binary->first);
							INSTRUCTION(Ins::InvokeMethod(methodInfo, 2));
						}
						else if (auto member = node->first.Cast<WfMemberExpression>())
						{
							throw 0;
						}
						else
						{
							GenerateExpressionInstructions(context, node->second);
							auto result = context.manager->expressionResolvings[node->first.Obj()];
							vint index = -1;
							if ((index = context.globalVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
							{
								vint variableIndex = context.globalVariables.Values()[index];
								INSTRUCTION(Ins::StoreGlobalVar(variableIndex));
							}
							else if ((index = context.functionContext->localVariables.Keys().IndexOf(result.symbol.Obj())) != -1)
							{
								vint argumentCount = context.functionContext->function->argumentNames.Count();
								vint variableIndex = context.functionContext->localVariables.Values()[index];
								INSTRUCTION(Ins::StoreLocalVar(argumentCount + variableIndex));
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
					throw 0;
				}

				void Visit(WfIfExpression* node)override
				{
					throw 0;
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
						throw 0;
					case WfTypeTesting::IsNotType:
						throw 0;
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
					auto result = context.manager->expressionResolvings[node->function.Obj()];
					if (result.methodInfo)
					{
						FOREACH(Ptr<WfExpression>, argument, node->arguments)
						{
							GenerateExpressionInstructions(context, argument);
						}

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
					}
					else if (result.symbol)
					{
						throw 0;
					}
					else
					{
						throw 0;
					}
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