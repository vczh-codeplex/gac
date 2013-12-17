#include "WfAnalyzer.h"

namespace vl
{
	namespace workflow
	{
		namespace analyzer
		{
			using namespace collections;
			using namespace parsing;

/***********************************************************************
ValidateStructureContext
***********************************************************************/

			ValidateStructureContext::ValidateStructureContext()
				:currentBindExpression(0)
			{
			}

/***********************************************************************
ValidateStructure(Type)
***********************************************************************/

			class ValidateReferenceTypeVisitor : public Object, public WfType::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;
				bool									result;

				ValidateReferenceTypeVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
					, result(true)
				{
				}
				
				void Visit(WfPredefinedType* node)override
				{
					if (node->name != WfPredefinedTypeName::Interface)
					{
						result = false;
					}
				}

				void Visit(WfTopQualifiedType* node)override
				{
				}

				void Visit(WfReferenceType* node)override
				{
				}

				void Visit(WfRawPointerType* node)override
				{
					result = false;
				}

				void Visit(WfSharedPointerType* node)override
				{
					result = false;
				}

				void Visit(WfNullableType* node)override
				{
					result = false;
				}

				void Visit(WfEnumerableType* node)override
				{
					result = false;
				}

				void Visit(WfMapType* node)override
				{
					result = false;
				}

				void Visit(WfFunctionType* node)override
				{
					result = false;
				}

				void Visit(WfChildType* node)override
				{
					node->parent->Accept(this);
				}

				static bool Execute(Ptr<WfType> type, WfLexicalScopeManager* manager)
				{
					ValidateReferenceTypeVisitor visitor(manager);
					type->Accept(&visitor);
					return visitor.result;
				}
			};

			class ValidateStructureTypeVisitor : public Object, public WfType::IVisitor
			{
			public:
				WfLexicalScopeManager*					manager;

				ValidateStructureTypeVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
				{
				}
				
				void Visit(WfPredefinedType* node)override
				{
					switch (node->name)
					{
					case WfPredefinedTypeName::Void:
						manager->errors.Add(WfErrors::WrongVoidType(node));
						break;
					case WfPredefinedTypeName::Interface:
						manager->errors.Add(WfErrors::WrongInterfaceType(node));
						break;
					case WfPredefinedTypeName::Namespace:
						manager->errors.Add(WfErrors::WrongNamespaceType(node));
						break;
					}
				}

				void Visit(WfTopQualifiedType* node)override
				{
				}

				void Visit(WfReferenceType* node)override
				{
				}

				void Visit(WfRawPointerType* node)override
				{
					if (!ValidateReferenceTypeVisitor::Execute(node->element, manager))
					{
						manager->errors.Add(WfErrors::RawPointerToNonReferenceType(node));
					}
				}

				void Visit(WfSharedPointerType* node)override
				{
					if (!ValidateReferenceTypeVisitor::Execute(node->element, manager))
					{
						manager->errors.Add(WfErrors::SharedPointerToNonReferenceType(node));
					}
				}

				void Visit(WfNullableType* node)override
				{
					if (!ValidateReferenceTypeVisitor::Execute(node->element, manager))
					{
						manager->errors.Add(WfErrors::NullableToNonReferenceType(node));
					}
				}

				void Visit(WfEnumerableType* node)override
				{
					ValidateTypeStructure(manager, node->element);
				}

				void Visit(WfMapType* node)override
				{
					if (node->key)
					{
						ValidateTypeStructure(manager, node->key);
					}
					ValidateTypeStructure(manager, node->value);
				}

				void Visit(WfFunctionType* node)override
				{
					ValidateTypeStructure(manager, node->result);
					FOREACH(Ptr<WfType>, argument, node->arguments)
					{
						ValidateTypeStructure(manager, argument);
					}
				}

				void Visit(WfChildType* node)override
				{
					if (!ValidateReferenceTypeVisitor::Execute(node->parent, manager))
					{
						manager->errors.Add(WfErrors::ChildOfNonReferenceType(node));
					}
				}

				static void Execute(Ptr<WfType> type, WfLexicalScopeManager* manager)
				{
					ValidateStructureTypeVisitor visitor(manager);
					type->Accept(&visitor);
				}
			};

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
					ValidateTypeStructure(manager, node->returnType, true);
					FOREACH(Ptr<WfFunctionArgument>, argument, node->arguments)
					{
						ValidateTypeStructure(manager, argument->type);
					}
					ValidateStructureContext context;
					ValidateStatementStructure(manager, &context, node->statement);
				}

				void Visit(WfVariableDeclaration* node)override
				{
					if (node->type)
					{
						ValidateTypeStructure(manager, node->type);
					}
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
					if (node->type)
					{
						ValidateTypeStructure(manager, node->type);
					}
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
					auto bind = context->currentBindExpression;
					context->currentBindExpression = 0;
					ValidateExpressionStructure(manager, context, node->body);
					context->currentBindExpression = bind;
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
					if (!node->expandedExpression)
					{
						List<Ptr<WfExpression>> expressions;
						const wchar_t* reading = node->value.value.Buffer();

						while (*reading)
						{
							const wchar_t* begin = wcsstr(reading, L"$(");
							if (begin)
							{
								Ptr<WfStringExpression> expression = new WfStringExpression;
								expression->codeRange = node->codeRange;
								expression->value.value = WString(reading, begin - reading);
								expressions.Add(expression);
							}
							else
							{
								break;
							}

							const wchar_t* end = begin + 2;
							vint counter = 1;
							while (wchar_t c = *end++)
							{
								switch (c)
								{
								case L'(':
									counter++;
									break;
								case L')':
									counter--;
									break;
								}
								if (counter == 0)
								{
									break;
								}
							}

							if (counter != 0)
							{
								auto error = WfErrors::WrongFormatStringSyntax(node);
								error->errorMessage += L" (Does not find matched close bracket.)";
								manager->errors.Add(error);
								return;
							}
							else
							{
								WString input(begin + 2, end - begin - 3);
								List<Ptr<ParsingError>> errors;
								if (auto expression = WfParseExpression(input, manager->parsingTable, errors))
								{
									expressions.Add(expression);
								}
								FOREACH(Ptr<ParsingError>, originalError, errors)
								{
									auto error = WfErrors::WrongFormatStringSyntax(node);
									error->errorMessage += L" (" + originalError->errorMessage + L")";
									manager->errors.Add(error);
								}
								reading = end;
							}
						}
						if (*reading || expressions.Count() == 0)
						{
							Ptr<WfStringExpression> expression = new WfStringExpression;
							expression->codeRange = node->codeRange;
							expression->value.value = reading;
							expressions.Add(expression);
						}

						if (expressions.Count() > 0)
						{
							Ptr<WfExpression> current = expressions[0];
							FOREACH(Ptr<WfExpression>, expression, From(expressions).Skip(1))
							{
								Ptr<WfBinaryExpression> binary = new WfBinaryExpression;
								binary->codeRange = node->codeRange;
								binary->first = current;
								binary->second = expression;
								binary->op = WfBinaryOperator::Concat;
							}

							node->expandedExpression = current;
						}
					}

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
					ValidateTypeStructure(manager, node->type);
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfTypeCastingExpression* node)override
				{
					ValidateTypeStructure(manager, node->type);
					ValidateExpressionStructure(manager, context, node->expression);
				}

				void Visit(WfTypeTestingExpression* node)override
				{
					if (node->type)
					{
						ValidateTypeStructure(manager, node->type);
					}
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
					if (context->currentBindExpression)
					{
						manager->errors.Add(WfErrors::BindInBind(node));
					}

					auto bind = context->currentBindExpression;
					context->currentBindExpression = node;
					ValidateExpressionStructure(manager, context, node->expression);
					context->currentBindExpression = bind;
				}

				void Visit(WfObserveExpression* node)override
				{
					if (!context->currentBindExpression)
					{
						manager->errors.Add(WfErrors::ObserveNotInBind(node));
					}

					if (node->observeType == WfObserveType::SimpleObserve)
					{
						if (!node->expression.Cast<WfReferenceExpression>())
						{
							manager->errors.Add(WfErrors::WrongSimpleObserveExpression(node->expression.Obj()));
						}
						FOREACH(Ptr<WfExpression>, event, node->events)
						{
							if (!event.Cast<WfReferenceExpression>())
							{
								manager->errors.Add(WfErrors::WrongSimpleObserveEvent(event.Obj()));
							}
						}
					}

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
					ValidateTypeStructure(manager, node->type);
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

			void ValidateTypeStructure(WfLexicalScopeManager* manager, Ptr<WfType> type, bool returnType)
			{
				if (returnType)
				{
					if (auto predefinedType = type.Cast<WfPredefinedType>())
					{
						if (predefinedType->name == WfPredefinedTypeName::Void)
						{
							return;
						}
					}
				}
				ValidateStructureTypeVisitor::Execute(type, manager);
			}

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