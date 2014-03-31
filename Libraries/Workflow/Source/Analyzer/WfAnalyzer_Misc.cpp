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
IsExpressionDependOnExpectedType(Expression)
***********************************************************************/

			class IsExpressionDependOnExpectedTypeVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WfLexicalScopeManager*				manager;
				bool								result;

				IsExpressionDependOnExpectedTypeVisitor(WfLexicalScopeManager* _manager)
					:manager(_manager)
					, result(false)
				{
				}

				bool Execute(Ptr<WfExpression> expression)
				{
					result = false;
					expression->Accept(this);
					return result;
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
					auto scope = manager->expressionScopes[node].Obj();
					result = scope->symbols.Count() > 0;
				}

				void Visit(WfMemberExpression* node)override
				{
				}

				void Visit(WfChildExpression* node)override
				{
				}

				void Visit(WfLiteralExpression* node)override
				{
					if (node->value == WfLiteralValue::Null)
					{
						result = true;
					}
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
					result = Execute(node->trueBranch) && Execute(node->falseBranch);
				}

				void Visit(WfRangeExpression* node)override
				{
				}

				void Visit(WfSetTestingExpression* node)override
				{
				}

				void Visit(WfConstructorExpression* node)override
				{
					result = node->arguments.Count() == 0;
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

			bool IsExpressionDependOnExpectedType(WfLexicalScopeManager* manager, Ptr<WfExpression> expression)
			{
				IsExpressionDependOnExpectedTypeVisitor visitor(manager);
				expression->Accept(&visitor);
				return visitor.result;
			}

/***********************************************************************
GetExpressionName(Expression)
***********************************************************************/

			class GetExpressionNameVisitor : public Object, public WfExpression::IVisitor
			{
			public:
				WString								result;

				void Visit(WfTopQualifiedExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfReferenceExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfOrderedNameExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfOrderedLambdaExpression* node)override
				{
				}

				void Visit(WfMemberExpression* node)override
				{
					result = node->name.value;
				}

				void Visit(WfChildExpression* node)override
				{
					result = node->name.value;
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

			WString GetExpressionName(Ptr<WfExpression> expression)
			{
				GetExpressionNameVisitor visitor;
				expression->Accept(&visitor);
				return visitor.result;
			}
		}
	}
}