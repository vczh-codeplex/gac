#include "Parsing.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
语法分析器扩展结构
***********************************************************************/
		namespace parsing_internal
		{
			void _Seq::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Alt::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Loop::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Token::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Rule::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Action::Accept(IParsingNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
		}
	}
}