/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parser::ParsingJson_Parser

***********************************************************************/

#include "ParsingJson_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace json
		{
			extern void						JsonEscapeString(const WString& text, stream::TextWriter& writer);
			extern void						JsonUnescapeString(const WString& text, stream::TextWriter& writer);
			extern void						JsonPrint(Ptr<JsonNode> node, stream::TextWriter& writer);
			extern WString					JsonToString(Ptr<JsonNode> node);
		}
	}
}