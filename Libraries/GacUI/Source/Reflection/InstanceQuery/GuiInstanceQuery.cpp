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
			List<Ptr<GuiConstructorRepr>>&		input;
			List<Ptr<GuiConstructorRepr>>&		output;

			ExecuteQueryVisitor(Ptr<GuiInstanceContext> _context, List<Ptr<GuiConstructorRepr>>& _input, List<Ptr<GuiConstructorRepr>>& _output)
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
							if (node->childOption == GuiIqChildOption::Indirect)
							{
								if (auto setter = value.Cast<GuiAttSetterRepr>())
								{
									Traverse(node, setter);
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
					if (node->childOption == GuiIqChildOption::Indirect)
					{
						Traverse(node, context->instance);
					}
				}
			}

			void Visit(GuiIqPrimaryQuery* node)override
			{
				if (&input)
				{
					FOREACH(Ptr<GuiConstructorRepr>, setter, input)
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
				List<Ptr<GuiConstructorRepr>> temp;
				ExecuteQuery(node->parent, context, input, temp);
				ExecuteQuery(node->child, context, temp, output);
			}

			void Visit(GuiIqSetQuery* node)override
			{
				List<Ptr<GuiConstructorRepr>> first, second;
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

		void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiConstructorRepr>>& input, collections::List<Ptr<GuiConstructorRepr>>& output)
		{
			ExecuteQueryVisitor visitor(context, input, output);
			query->Accept(&visitor);
		}

		void ExecuteQuery(Ptr<GuiIqQuery> query, Ptr<GuiInstanceContext> context, collections::List<Ptr<GuiConstructorRepr>>& output)
		{
			ExecuteQuery(query, context, *(List<Ptr<GuiConstructorRepr>>*)0, output);
		}

/***********************************************************************
ApplyStyle
***********************************************************************/

		void ApplyStyleInternal(Ptr<GuiAttSetterRepr> src, Ptr<GuiAttSetterRepr> dst)
		{
			FOREACH_INDEXER(WString, attribute, srcIndex, src->setters.Keys())
			{
				auto srcValue = src->setters.Values()[srcIndex];
				vint dstIndex = dst->setters.Keys().IndexOf(attribute);
				if (dstIndex == -1)
				{
					dst->setters.Add(attribute, srcValue);
				}
				else
				{
					auto dstValue = dst->setters.Values()[dstIndex];
					if (srcValue->binding == dstValue->binding)
					{
						if (srcValue->binding == L"set")
						{
							ApplyStyleInternal(srcValue->values[0].Cast<GuiAttSetterRepr>(), dstValue->values[0].Cast<GuiAttSetterRepr>());
						}
						else
						{
							CopyFrom(dstValue->values, srcValue->values, true);
						}
					}
				}
			}

			FOREACH_INDEXER(WString, eventName, srcIndex, src->eventHandlers.Keys())
			{
				if (!dst->eventHandlers.Keys().Contains(eventName))
				{
					auto srcValue = src->eventHandlers.Values()[srcIndex];
					dst->eventHandlers.Add(eventName, srcValue);
				}
			}
		}

		void ApplyStyle(Ptr<GuiInstanceStyle> style, Ptr<GuiConstructorRepr> ctor)
		{
			ApplyStyleInternal(style->setter->Clone().Cast<GuiAttSetterRepr>(), ctor);
		}
	}
}