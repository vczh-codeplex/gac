#include "..\..\..\Source\HttpUtility.h"
#include "..\..\..\Source\Console.h"

using namespace vl;
using namespace vl::console;

int main(int argc, wchar_t* argv[])
{

	WString responseAuth;
	WString cookie;
	WString responseTopics;
	{
		WString username;
		WString password;
		Console::Write(L"Username: ");
		username=Console::Read();
		Console::Write(L"Password: ");
		password=Console::Read();
		WString api_key=L"e1424eb84b6d4169a10c03fb1e73e140";
		WString api_secret=L"9814021f20054b558105fca1df6559a7";
		WString body=L"api_key="+api_key+L"&api_secret="+api_secret+L"&username="+username+L"&password="+password;

		HttpRequest request;
		HttpResponse response;

		request.SetHost(L"https://niaowo.me/account/token/");
		request.method=L"POST";
		request.contentType=L"application/x-www-form-urlencoded";
		request.SetBodyUtf8(body);
		HttpQuery(request, response);

		if(response.statusCode!=200)
		{
			Console::WriteLine(L"Login failed.");
			return 0;
		}
		responseAuth=response.GetBodyUtf8();
		cookie=response.cookie;
	}
	{
		Console::WriteLine(responseAuth);
		Console::WriteLine(cookie);
	}
	{
		HttpRequest request;
		HttpResponse response;

		request.SetHost(L"https://niaowo.me/topics.xml");
		request.method=L"GET";
		request.cookie=cookie;
		request.acceptTypes.Add(L"application/xml");
		HttpQuery(request, response);

		responseTopics=response.GetBodyUtf8();
	}
	{
		Console::WriteLine(L"================================");
		Console::WriteLine(responseTopics);
	}
	return 0;
}