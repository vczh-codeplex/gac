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

			void Visit(GuiIqPrimaryQuery* node)override
			{
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