#ifndef VCZH_NESTLESDK
#define VCZH_NESTLESDK

#include "..\..\..\Source\HttpUtility.h"
#include "..\..\..\Source\Collections\OperationForEach.h"
#include "..\..\..\Source\Collections\OperationCopyFrom.h"

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
		WString						NestleGetXml(const WString& path, const WString& query, const WString& cookie);
		WString						NestlePostXml(const WString& path, const WString& query, const WString& cookie, const WString& body);

/***********************************************************************
Nestle Data Structure
***********************************************************************/

		class NestleComment : public Object
		{
		public:
			NestleComment(IXMLDOMNode* commentElement=0);
			~NestleComment();

			bool					operator==(const NestleComment&)const{return false;}
			bool					operator!=(const NestleComment&)const{return true;}

			WString					body;
			WString					createDateTime;
			WString					author;
			int						id;
		};

		class NestlePost : public Object
		{
		public:
			NestlePost(IXMLDOMNode* topicElement=0, IXMLDOMNodeList* commentElements=0);
			NestlePost(const NestlePost& post);
			~NestlePost();

			bool					operator==(const NestlePost&)const{return false;}
			bool					operator!=(const NestlePost&)const{return true;}
			NestlePost&				operator=(const NestlePost& post);

			WString					title;
			WString					description;
			WString					body;
			WString					createDateTime;
			WString					author;
			int						id;
			List<NestleComment>		comments;
		};

		class NestleTopicsPage
		{
		public:
			NestleTopicsPage(IXMLDOMNode* rootElement=0);
			NestleTopicsPage(const NestleTopicsPage& page);
			~NestleTopicsPage();

			bool					operator==(const NestleTopicsPage&)const{return false;}
			bool					operator!=(const NestleTopicsPage&)const{return true;}
			NestleTopicsPage&		operator=(const NestleTopicsPage& page);

			List<NestlePost>		posts;
			int						totalPages;
			int						currentPage;
		};

		class NestleServer : public Object
		{
		protected:
			WString					username;
			WString					password;
			WString					apiKey;
			WString					apiSecret;
			WString					cookie;
		public:
			NestleServer(const WString& _username, const WString& _password, const WString& _apiKey, const WString& _apiSecret);
			~NestleServer();

			bool					IsLoginSuccess();
			Ptr<NestleTopicsPage>	GetTopics(int pageIndex);
			Ptr<NestlePost>			GetTopic(int id);
			Ptr<NestlePost>			PostTopic(const WString& title, const WString& content);
			Ptr<NestleComment>		PostComment(int postId, const WString& content);
		};
	}
}

#endif