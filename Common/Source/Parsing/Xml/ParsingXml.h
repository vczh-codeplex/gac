/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Parser::ParsingXml

***********************************************************************/

#ifndef VCZH_PARSING_XML_PARSINGXML
#define VCZH_PARSING_XML_PARSINGXML

#include "ParsingXml_Parser.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
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

#endif