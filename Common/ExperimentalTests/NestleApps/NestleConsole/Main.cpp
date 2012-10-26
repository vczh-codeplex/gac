#include "..\..\..\Source\Console.h"
#include "NestleSDK.h"

using namespace vl;
using namespace vl::nestle;
using namespace vl::console;

/***********************************************************************
Main
***********************************************************************/

int main(int argc, wchar_t* argv[])
{
	CoInitialize(NULL);
	WString cookie;
	{
		WString apiKey=L"e1424eb84b6d4169a10c03fb1e73e140";
		WString apiSecret=L"9814021f20054b558105fca1df6559a7";
		WString username;
		WString password;
		// geniusvczh_apptest
		// fuckkula
		Console::Write(L"Username: ");
		username=Console::Read();
		Console::Write(L"Password: ");
		password=Console::Read();
		cookie=NestleGetSession(username, password, apiKey, apiSecret);
		if(cookie==L"")
		{
			Console::WriteLine(L"Login failed.");
			return 0;
		}
	}
	
	{
		WString responseTopics=NestleGetXml(L"/topics", cookie);
		Console::WriteLine(L"Topics");
		bool loadTopics=false;
		IXMLDOMDocument2* pDom=XmlLoad(responseTopics);
		if(pDom)
		{
			List<WString> topics;
			IXMLDOMNodeList* nodeList=XmlQuery(pDom, L"/topics/topic/title/text()");
			if(nodeList)
			{
				XmlReadMultipleStrings(nodeList, topics, 20);
				FOREACH(WString, topic, topics.Wrap())
				{
					Console::WriteLine(topic);
				}
				nodeList->Release();
			}
			pDom->Release();
		}
		if(loadTopics)
		{
			Console::WriteLine(L"Failed to load topics.");
		}
	}
	
	{
		WString responseTopicBody=NestleGetXml(L"/topics/972", cookie);
	}
	CoUninitialize();
	return 0;
}