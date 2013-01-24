/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parser::ParsingXml

***********************************************************************/

#ifndef VCZH_PARSING_XML_PARSINGXML
#define VCZH_PARSING_XML_PARSINGXML

#include "ParsingXml_Parser.h"
#include "..\..\Collections\Operation.h"

namespace vl
{
	namespace parsing
	{
		namespace xml
		{
			extern WString							XmlEscapeValue(const WString& value);
			extern WString							XmlUnescapeValue(const WString& value);
			extern WString							XmlEscapeCData(const WString& value);
			extern WString							XmlUnescapeCData(const WString& value);
			extern WString							XmlEscapeComment(const WString& value);
			extern WString							XmlUnescapeComment(const WString& value);
			extern void								XmlPrint(Ptr<XmlNode> node, stream::TextWriter& writer);
			extern WString							XmlToString(Ptr<XmlNode> node);

			extern Ptr<XmlAttribute>							XmlGetAttribute(Ptr<XmlElement> element, const WString& name);
			extern Ptr<XmlElement>								XmlGetElement(Ptr<XmlElement> element, const WString& name);
			extern collections::LazyList<Ptr<XmlElement>>		XmlGetElements(Ptr<XmlElement> element);
			extern collections::LazyList<Ptr<XmlElement>>		XmlGetElements(Ptr<XmlElement> element, const WString& name);
			extern WString										XmlGetValue(Ptr<XmlElement> element);
		}
	}
}

#endif