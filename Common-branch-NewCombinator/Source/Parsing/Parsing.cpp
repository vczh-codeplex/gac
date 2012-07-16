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
			void _Seq::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Alt::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Loop::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Token::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Rule::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Create::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Assign::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Cast::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Use::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
			
			void _Transform::Accept(IParserNodeVisitor* visitor)
			{
				visitor->Visit(this);
			}
		}
	}
}