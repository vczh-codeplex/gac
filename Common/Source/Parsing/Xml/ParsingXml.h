/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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

			extern Ptr<XmlAttribute>		XmlGetAttribute(Ptr<XmlElement> element, const WString& name);
			extern Ptr<XmlElement>			XmlGetElement(Ptr<XmlElement> element, const WString& name);
			extern void						XmlGetElements(Ptr<XmlElement> element, collections::List<Ptr<XmlElement>>& elements);
			extern void						XmlGetElements(Ptr<XmlElement> element, const WString& name, collections::List<Ptr<XmlElement>>& elements);
			extern WString					XmlGetValue(Ptr<XmlElement> element);
		}
	}
}

#endif