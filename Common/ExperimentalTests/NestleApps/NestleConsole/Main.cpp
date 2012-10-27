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
		Console::WriteLine(L"Press [ENTER] to exit.");
		Console::Read();
		goto EXIT;
	}
	
	Console::WriteLine(L"+----------------------------------------------");
	Console::WriteLine(L"Welcome to Vczh Console Nestle Client 1.0");
	Console::WriteLine(L"Enter \"help\" for all commands, commands are all case sensitive.");
	Console::WriteLine(L"Author: vczh@163.com");
	Console::WriteLine(L"+----------------------------------------------");

	while(true)
	{
		Console::Write(username+L">");
		WString command=Console::Read();
		if(command==L"exit")
		{
			break;
		}
		else if(command==L"help")
		{
			Console::WriteLine(L"+----------------------------------------------");
			Console::WriteLine(L"exit                : Close the client.");
			Console::WriteLine(L"help                : Show help.");
			Console::WriteLine(L"pages               : Get total page count.");
			Console::WriteLine(L"topics [page index] : Get all topics of the specified page. Page index starts from 1, default is 1.");
			Console::WriteLine(L"topic <id>          : Open a topic.");
			Console::WriteLine(L"comments            : Show all comments of the current topic.");
			Console::WriteLine(L"comment <content>   : Post a comment in the current topic.");
			Console::WriteLine(L"post <title>        : Begin a new post with a specified title. Then enter content, until one of the following commands are used.");
			Console::WriteLine(L"send post           : Send out the new post.");
			Console::WriteLine(L"cancel post         : Cancel the new post.");
			Console::WriteLine(L"+----------------------------------------------");
		}
	}

EXIT:
	CoUninitialize();
	return 0;
}