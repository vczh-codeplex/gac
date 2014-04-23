#include "GuiInstanceQuery.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
ExecuteQueryVisitor
***********************************************************************/

		class ExecuteQueryVisitor : public Object, public GuiIqQuery::IVisitor
		{
		public:
			Ptr<GuiInstanceContext>				context;
			List<Ptr<GuiAttSetterRepr>>&		input;
			List<Ptr<GuiAttSetterRepr>>&		output;

			ExecuteQueryVisitor(Ptr<GuiInstanceContext> _context, List<Ptr<GuiAttSetterRepr>>& _input, List<Ptr<GuiAttSetterRepr>>& _output)
				:context(_context), input(_input), output(_output)
			{
			}

			static bool TestCtor(GuiIqPrimaryQuery* node, const WString& attribute, Ptr<GuiConstructorRepr> ctor)
			{
				if (node->attributeNameOption == GuiIqNameOption::Specified && node->attributeName.value != attribute)
				{
					return false;
				}
				if (node->typeNameOption == GuiIqNameOption::Specified && node->typeName.value != ctor->typeName)
				{
					return false;
				}
				if (node->referenceName.value != L"" && (!ctor->instanceName || node->referenceName.value != ctor->instanceName.Value()))
				{
					return false;
				}
				return true;
			}

			void Traverse(GuiIqPrimaryQuery* node, Ptr<GuiAttSetterRepr> setter)
			{
				if (setter)
				{
					FOREACH_INDEXER(WString, attribute, index, setter->setters.Keys())
					{
						auto setterValue = setter->setters.Values()[index];
						FOREACH(Ptr<GuiValueRepr>, value, setterValue->values)
						{
							if (auto ctor = value.Cast<GuiConstructorRepr>())
							{
								if (TestCtor(node, attribute, ctor))
								{
									output.Add(ctor);
								}
							}
						}
					}
				}
				else
				{
					if (TestCtor(node, L"", context->instance))
					{
						output.Add(context->instance);
					}
					setter = context->instance;
				}

				if (node->childOption == GuiIqChildOption::Indirect)
				{
					FOREACH(Ptr<GuiAttSetterRepr::SetterValue>, setterValue, setter->setters.Values())
					{
						FOREACH(Ptr<GuiValueRepr>, value, setterValue->values)
						{
							if (auto setter = value.Cast<GuiAttSetterRepr>())
							{
								Traverse(node, setter);
							}
						}
					}
				}
			}

			void Visit(GuiIqPrimaryQuery* node)override
			{
				if (&input)
				{
					FOREACH(Ptr<GuiAttSetterRepr>, setter, input)
					{
						Traverse(node, setter);
					}
				}
				else
				{
					Traverse(node, 0);
				}
			}

			void Visit(GuiIqCascadeQuery* node)override
			{
				List<Ptr<GuiAttSetterRepr>> temp;
				ExecuteQuery(node->parent, context, input, temp);
				ExecuteQuery(node->child, context, temp, output);
			}

			void Visit(GuiIqSetQuery* node)override
			{
				List<Ptr<GuiAttSetterRepr>> first, second;
				ExecuteQuery(node->first, context, input, first);
				ExecuteQuery(node->second, context, input, second);

				switch (node->op)
				{
				case GuiIqBinaryOperator::ExclusiveOr:
					CopyFrom(output, From(first).Except(second).Union(From(second).Except(second)));
					break;
				case GuiIqBinaryOperator::Intersect:
					CopyFrom(output, From(first).Intersect(second));
					break;
				case GuiIqBinaryOperator::Union:
					CopyFrom(output, From(first).Union(second));
					break;
				case GuiIqBinaryOperator::Substract:
					CopyFrom(output, From(first).Except(second));
					break;
				}
			}
		};

/***********************************************************************
ExecuteQuery
***********************************************************************/

		void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiAttSetterRepr>>& input, collections::List<Ptr<GuiAttSetterRepr>>& output)
		{
			ExecuteQueryVisitor visitor(context, input, output);
			query->Accept(&visitor);
		}

		void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiAttSetterRepr>>& output)
		{
			ExecuteQuery(query, context, *(List<Ptr<GuiAttSetterRepr>>*)0, output);
		}
	}
}