/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parser::ParsingXml

***********************************************************************/

#include "ParsingXml_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			extern void						XmlEscapeValue(const WString& value, stream::TextWriter& writer);
			extern void						XmlUnescapeValue(const WString& value, stream::TextWriter& writer);
			extern WString					XmlEscapeValue(const WString& value);
			extern WString					XmlUnescapeValue(const WString& value);
			extern WString					XmlEscapeCData(const WString& value);
			extern WString					XmlUnescapeCData(const WString& value);
			extern WString					XmlEscapeComment(const WString& value);
			extern WString					XmlUnescapeComment(const WString& value);
			extern void						XmlPrint(Ptr<XmlNode> node, stream::TextWriter& writer);
			extern WString					XmlToString(Ptr<XmlNode> node);
		}
	}
}
