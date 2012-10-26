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

			request.SetHost(L"https://www.niaowo.me/account/token/");
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

			request.SetHost(L"https://www.niaowo.me"+path+L".xml");
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

		bool NestlePostData(const WString& path, const WString& cookie, const WString& body)
		{
			HttpRequest request;
			HttpResponse response;

			request.SetHost(L"https://www.niaowo.me"+path);
			request.method=L"POST";
			request.contentType=L"application/x-www-form-urlencoded";
			request.SetBodyUtf8(body);
			HttpQuery(request, response);

			return response.statusCode==200;
		}

/***********************************************************************
NestlePost

<topic>
  <body></body>
  <title>ÄñÎÑapiÎÄµµ£¨²Ý¸å£©</title>
  <created-at type="datetime">2012-09-16T10:57:46+08:00</created-at>
  <updated-at type="datetime">2012-10-26T15:26:28+08:00</updated-at>
  <deleted type="boolean">false</deleted>
  <id type="integer">972</id>

  <member-id type="integer">1</member-id>
  <message-id type="integer" nil="true"/>
  <sort-at type="datetime">2012-10-26T15:26:28+08:00</sort-at>
  <viewpoint type="integer">0</viewpoint>
</topic>
***********************************************************************/

		NestlePost::NestlePost(IXMLDOMNode* topicElement)
		{
			if(topicElement)
			{
				body=XmlQueryString(topicElement, L"./body/text()");
				title=XmlQueryString(topicElement, L"./title/text()");
				createDateTime=XmlQueryString(topicElement, L"./created-at/text()");
				updateDateTime=XmlQueryString(topicElement, L"./updated-at/text()");
				deleted=XmlQueryString(topicElement, L"./deleted/text()")==L"true";
				id=wtoi(XmlQueryString(topicElement, L"./id/text()"));
			}
		}

		NestlePost::~NestlePost()
		{
		}

/***********************************************************************
NestleServer
***********************************************************************/

		NestleServer::NestleServer(const WString& _username, const WString& _password, const WString& _apiKey, const WString& _apiSecret)
			:username(_username)
			,password(_password)
			,apiKey(_apiKey)
			,apiSecret(_apiSecret)
		{
			cookie=NestleGetSession(username, password, apiKey, apiSecret);
		}

		NestleServer::~NestleServer()
		{
		}

		bool NestleServer::IsLoginSuccess()
		{
			return cookie!=L"";
		}

		bool NestleServer::GetTopics(int page, List<NestlePost>& posts)
		{
			WString url;
			if(page==0)
			{
				url=L"/topics";
			}
			else
			{
				url=L"/topics/page/"+itow(page+1);
			}
			WString xml=NestleGetXml(url, cookie);
			if(xml!=L"")
			{	
				IXMLDOMDocument2* pDom=XmlLoad(xml);
				if(pDom)
				{
					IXMLDOMNodeList* nodeList=XmlQuery(pDom, L"/topics/topic");
					posts.Clear();
					while(true)
					{
						IXMLDOMNode* node=0;
						HRESULT hr=nodeList->nextNode(&node);
						if(hr==S_OK)
						{
							posts.Add(NestlePost(node));
							node->Release();
						}
						else
						{
							break;
						}
					}
					nodeList->Release();
					pDom->Release();
					return true;
				}
			}
			return false;
		}

		bool NestleServer::GetTopic(int id, NestlePost& post)
		{
			WString url=L"/topics/"+itow(id);
			WString xml=NestleGetXml(url, cookie);
			if(xml!=L"")
			{	
				IXMLDOMDocument2* pDom=XmlLoad(xml);
				if(pDom)
				{
					IXMLDOMNode* node=XmlQuerySingleNode(pDom, L"/topic");
					post=NestlePost(node);
					node->Release();
					pDom->Release();
					return true;
				}
			}
			return false;
		}

		bool NestleServer::PostTopic(const WString& title, const WString& content)
		{
			WString url=L"/topics";
			WString body=L"title="+UrlEncodeQuery(title)+L"&editor="+UrlEncodeQuery(content);
			return NestlePostData(url, cookie, body);
		}

		bool NestleServer::PostComment(int postId, const WString& content)
		{
			WString url=L"/topics";
			WString body=L"topic="+itow(postId)+L"&editor="+UrlEncodeQuery(content);
			return NestlePostData(url, cookie, body);
		}
	}
}