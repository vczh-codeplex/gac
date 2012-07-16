#include "Parsing.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
�﷨��������չ�ṹ
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