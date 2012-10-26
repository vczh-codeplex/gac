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

	NestleServer server(username, password, apiKey, apiSecret);
	if(!server.IsLoginSuccess())
	{
		Console::WriteLine(L"Login failed.");
		return 0;
	}

	List<NestlePost> posts;
	if(!server.GetTopics(0, posts))
	{
		Console::WriteLine(L"GetTopics() failed.");
		return 0;
	}

	FOREACH(NestlePost, post, posts.Wrap())
	{
		Console::WriteLine(post.title);
	}

	NestlePost firstPost;
	if(!server.GetTopic(posts[0].id, firstPost))
	{
		Console::WriteLine(L"GetTopic() failed.");
		return 0;
	}
	Console::WriteLine(L"==============================================================");
	Console::WriteLine(firstPost.title+L" -- "+firstPost.createDateTime+L" -- "+firstPost.updateDateTime);
	Console::WriteLine(L"==============================================================");
	Console::WriteLine(firstPost.body);

	if(!server.PostTopic(L"这是vczh写的程序发的贴，不要理我。", L"都说了不要理了><"))
	{
		Console::WriteLine(L"PostTopic() failed.");
		return 0;
	}
	if(!server.GetTopics(0, posts))
	{
		Console::WriteLine(L"GetTopics() failed.");
		return 0;
	}
	FOREACH(NestlePost, post, posts.Wrap())
	{
		if(post.title==L"这是vczh写的程序发的贴，不要理我。")
		{
			if(!server.PostComment(post.id, L"这是vczh写的程序发的回帖，不要理我。"))
			{
				Console::WriteLine(L"PostComment() failed.");
				return 0;
			}
		}
	}

	CoUninitialize();
	return 0;
}