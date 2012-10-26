#include "NestleSDK.h"

namespace vl
{
	namespace nestle
	{

/***********************************************************************
XML Utility
***********************************************************************/

		void XmlSetProperty(IXMLDOMDocument2* pDom, const wchar_t* name, const wchar_t* value)
		{
			HRESULT hr=S_OK;
			BSTR bstrName=SysAllocString(name);
			BSTR bstrValue=SysAllocString(value);
			if(bstrName && bstrValue)
			{
				VARIANT varValue;
				V_VT(&varValue)=VT_BSTR;
				V_BSTR(&varValue)=bstrValue;
				hr=pDom->setProperty(bstrName, varValue);
			}
			if(bstrName) SysFreeString(bstrName);
			if(bstrValue) SysFreeString(bstrValue);
		}

		IXMLDOMNodeList* XmlQuery(IXMLDOMNode* pDom, const WString& xpath)
		{
			IXMLDOMNodeList* nodes=0;
			BSTR xmlQuery=SysAllocString(xpath.Buffer());
			if(xmlQuery)
			{
				HRESULT hr=pDom->selectNodes(xmlQuery, &nodes);
				if(FAILED(hr))
				{
					nodes=0;
				}
				SysFreeString(xmlQuery);
			}
			return nodes;
		}

		IXMLDOMNode* XmlQuerySingleNode(IXMLDOMNode* pDom, const WString& xpath)
		{
			IXMLDOMNode* node=0;
			BSTR xmlQuery=SysAllocString(xpath.Buffer());
			if(xmlQuery)
			{
				HRESULT hr=pDom->selectSingleNode(xmlQuery, &node);
				if(FAILED(hr))
				{
					node=0;
				}
				SysFreeString(xmlQuery);
			}
			return node;
		}

		WString XmlReadString(IXMLDOMNode* node)
		{
			WString result;
			BSTR text=0;
			HRESULT hr=node->get_text(&text);
			if(SUCCEEDED(hr))
			{
				const wchar_t* candidateItem=text;
				result=candidateItem;
				SysFreeString(text);
			}
			return result;
		}

		WString XmlQueryString(IXMLDOMNode* node, const WString& xpath)
		{
			WString result;
			IXMLDOMNode* textNode=XmlQuerySingleNode(node, xpath);
			if(textNode)
			{
				result=XmlReadString(textNode);
				textNode->Release();
			}
			return result;
		}

		void XmlReadMultipleStrings(IXMLDOMNodeList* textNodes, List<WString>& candidates, int max)
		{
			candidates.Clear();
			while((int)candidates.Count()<max)
			{
				IXMLDOMNode* textNode=0;
				HRESULT hr=textNodes->nextNode(&textNode);
				if(hr==S_OK)
				{
					candidates.Add(XmlReadString(textNode));
					textNode->Release();
				}
				else
				{
					break;
				}
			}
		}

		IXMLDOMDocument2* XmlLoad(const WString& content)
		{
			IXMLDOMDocument2* pDom=0;
			HRESULT hr=CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDom));
			if(SUCCEEDED(hr))
			{
				pDom->put_async(VARIANT_FALSE);
				pDom->put_validateOnParse(VARIANT_FALSE);
				pDom->put_resolveExternals(VARIANT_FALSE);

				//const wchar_t* xmlns=L"xmlns:as='http://schemas.microsoft.com/Search/2008/suggestions'";
				//XmlSetProperty(pDom, L"SelectionLanguage", L"XPath");
				//XmlSetProperty(pDom, L"SelectionNamespaces", xmlns);

				BSTR xmlContent=SysAllocString(content.Buffer());
				if(xmlContent)
				{
					VARIANT_BOOL isSuccessful=0;
					hr=pDom->loadXML(xmlContent, &isSuccessful);
					if(!(SUCCEEDED(hr) && isSuccessful==VARIANT_TRUE))
					{
						pDom->Release();
						pDom=0;
					}
					SysFreeString(xmlContent);
				}
			}
			return pDom;
		}

/***********************************************************************
Nestle Utility
***********************************************************************/

		WString NestleGetSession(const WString& username, const WString& password, const WString& apiKey, const WString& apiSecret)
		{
			WString body=L"api_key="+apiKey+L"&api_secret="+apiSecret+L"&username="+username+L"&password="+password;

			HttpRequest request;
			HttpResponse response;

			request.SetHost(L"https://niaowo.me/account/token/");
			request.method=L"POST";
			request.contentType=L"application/x-www-form-urlencoded";
			request.SetBodyUtf8(body);
			HttpQuery(request, response);

			if(response.statusCode==200)
			{
				return response.cookie;
			}
			else
			{
				return L"";
			}
		}

		WString NestleGetXml(const WString& path, const WString& cookie)
		{
			HttpRequest request;
			HttpResponse response;

			request.SetHost(L"https://niaowo.me"+path+L".xml");
			request.method=L"GET";
			request.cookie=cookie;
			request.acceptTypes.Add(L"application/xml");
			HttpQuery(request, response);
	

			if(response.statusCode==200)
			{
				return response.GetBodyUtf8();
			}
			else
			{
				return L"";
			}
		}
	}
}