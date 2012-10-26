#ifndef VCZH_NESTLESDK
#define VCZH_NESTLESDK

#include "..\..\..\Source\HttpUtility.h"
#include "..\..\..\Source\Collections\OperationForEach.h"

#include <Windows.h>
#include <msxml6.h>

namespace vl
{
	using namespace collections;

	namespace nestle
	{

/***********************************************************************
XML Utility
***********************************************************************/

		extern void					XmlSetProperty(IXMLDOMDocument2* pDom, const wchar_t* name, const wchar_t* value);
		extern IXMLDOMNodeList*		XmlQuery(IXMLDOMNode* pDom, const WString& xpath);
		extern IXMLDOMNode*			XmlQuerySingleNode(IXMLDOMNode* pDom, const WString& xpath);
		extern WString				XmlReadString(IXMLDOMNode* node);
		extern WString				XmlQueryString(IXMLDOMNode* node, const WString& xpath);
		extern void					XmlReadMultipleStrings(IXMLDOMNodeList* textNodes, List<WString>& candidates, int max);
		extern IXMLDOMDocument2*	XmlLoad(const WString& content);

/***********************************************************************
Nestle Utility
***********************************************************************/

		WString						NestleGetSession(const WString& username, const WString& password, const WString& apiKey, const WString& apiSecret);
		WString						NestleGetXml(const WString& path, const WString& cookie);

/***********************************************************************
Nestle Data Structure
***********************************************************************/
	}
}

#endif