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
	NestleTopicsPage page;
	NestlePost firstPost;

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
		goto EXIT;
	}

	if(!server.GetTopics(0, page))
	{
		Console::WriteLine(L"GetTopics() failed.");
		goto EXIT;
	}

	FOREACH(NestlePost, post, page.posts.Wrap())
	{
		Console::WriteLine(post.title);
	}

	if(!server.GetTopic(page.posts[0].id, firstPost))
	{
		Console::WriteLine(L"GetTopic() failed.");
		goto EXIT;
	}
	Console::WriteLine(L"==============================================================");
	Console::WriteLine(firstPost.title+L" -- "+firstPost.createDateTime+L" -- "+firstPost.author);
	Console::WriteLine(L"==============================================================");
	Console::WriteLine(firstPost.body);
	FOREACH(NestleComment, comment, firstPost.comments.Wrap())
	{
		Console::WriteLine(L"");
		Console::WriteLine(L"Comment by "+comment.author+L" :");
		Console::WriteLine(comment.body);
	}

	//if(!server.PostTopic(L"这是vczh写的程序发的贴，不要理我。", L"都说了不要理了><"))
	//{
	//	Console::WriteLine(L"PostTopic() failed.");
	//	goto EXIT;
	//}
	//if(!server.GetTopics(0, posts))
	//{
	//	Console::WriteLine(L"GetTopics() failed.");
	//	goto EXIT;
	//}
	//FOREACH(NestlePost, post, posts.Wrap())
	//{
	//	if(post.title==L"这是vczh写的程序发的贴，不要理我。")
	//	{
	//		if(!server.PostComment(post.id, L"这是vczh写的程序发的回帖，不要理我。"))
	//		{
	//			Console::WriteLine(L"PostComment() failed.");
	//			goto EXIT;
	//		}
	//	}
	//}

EXIT:
	Console::WriteLine(L"Press [ENTER] to exit.");
	Console::Read();
	CoUninitialize();
	return 0;
}