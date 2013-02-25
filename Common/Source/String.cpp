#include <stdlib.h>
#include <windows.h>
#include "String.h"

namespace vl
{
	vint atoi_test(const AString& string, bool& success)
	{
		char* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return strtol(string.Buffer(), &endptr, 10);
	}

	vint wtoi_test(const WString& string, bool& success)
	{
		wchar_t* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return wcstol(string.Buffer(), &endptr, 10);
	}

	__int64 atoi64_test(const AString& string, bool& success)
	{
		char* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return _strtoi64(string.Buffer(), &endptr, 10);
	}

	__int64 wtoi64_test(const WString& string, bool& success)
	{
		wchar_t* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return _wcstoi64(string.Buffer(), &endptr, 10);
	}

	vuint atou_test(const AString& string, bool& success)
	{
		char* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return strtoul(string.Buffer(), &endptr, 10);
	}

	vuint wtou_test(const WString& string, bool& success)
	{
		wchar_t* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return wcstoul(string.Buffer(), &endptr, 10);
	}

	unsigned __int64 atou64_test(const AString& string, bool& success)
	{
		char* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return _strtoui64(string.Buffer(), &endptr, 10);
	}

	unsigned __int64 wtou64_test(const WString& string, bool& success)
	{
		wchar_t* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return _wcstoui64(string.Buffer(), &endptr, 10);
	}

	double atof_test(const AString& string, bool& success)
	{
		char* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return strtod(string.Buffer(), &endptr);
	}

	double wtof_test(const WString& string, bool& success)
	{
		wchar_t* endptr=0;
		success=endptr==string.Buffer()+string.Length();
		return wcstod(string.Buffer(), &endptr);
	}

	vint atoi(const AString& string)
	{
		bool success=false;
		return atoi_test(string, success);
	}

	vint wtoi(const WString& string)
	{
		bool success=false;
		return wtoi_test(string, success);
	}

	__int64 atoi64(const AString& string)
	{
		bool success=false;
		return atoi64_test(string, success);
	}

	__int64 wtoi64(const WString& string)
	{
		bool success=false;
		return wtoi64_test(string, success);
	}

	vuint atou(const AString& string)
	{
		bool success=false;
		return atou_test(string, success);
	}

	vuint wtou(const WString& string)
	{
		bool success=false;
		return wtou_test(string, success);
	}

	unsigned __int64 atou64(const AString& string)
	{
		bool success=false;
		return atou64_test(string, success);
	}

	unsigned __int64 wtou64(const WString& string)
	{
		bool success=false;
		return wtou64_test(string, success);
	}

	double atof(const AString& string)
	{
		bool success=false;
		return atof_test(string, success);
	}

	double wtof(const WString& string)
	{
		bool success=false;
		return wtof_test(string, success);
	}

	AString itoa(vint number)
	{
		char buffer[100];
		ITOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString itow(vint number)
	{
		wchar_t buffer[100];
		ITOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString i64toa(__int64 number)
	{
		char buffer[100];
		I64TOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString i64tow(__int64 number)
	{
		wchar_t buffer[100];
		I64TOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString utoa(vuint number)
	{
		char buffer[100];
		UITOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString utow(vuint number)
	{
		wchar_t buffer[100];
		UITOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString u64toa(unsigned __int64 number)
	{
		char buffer[100];
		UI64TOA_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	WString u64tow(unsigned __int64 number)
	{
		wchar_t buffer[100];
		UI64TOW_S(number, buffer, sizeof(buffer)/sizeof(*buffer), 10);
		return buffer;
	}

	AString ftoa(double number)
	{
		char buffer[100];
		_gcvt_s(buffer, 100, number, 30);
		vint len=(vint)strlen(buffer);
		if(buffer[len-1]=='.')
		{
			buffer[len-1]='\0';
		}
		return buffer;
	}

	WString ftow(double number)
	{
		return atow(ftoa(number));
	}

	vint _wtoa(const wchar_t* w, char* a, vint chars)
	{
		return WideCharToMultiByte(CP_THREAD_ACP, 0, w, -1, a, (int)(a?chars:0), 0, 0);
	}

	AString wtoa(const WString& string)
	{
		vint len=_wtoa(string.Buffer(), 0, 0);
		char* buffer=new char[len];
		_wtoa(string.Buffer(), buffer, (int)len);
		AString s=buffer;
		delete[] buffer;
		return s;
	}

	vint _atow(const char* a, wchar_t* w, vint chars)
	{
		return MultiByteToWideChar(CP_THREAD_ACP, 0, a, -1, w, (int)(w?chars:0));
	}

	WString atow(const AString& string)
	{
		vint len=_atow(string.Buffer(), 0, 0);
		wchar_t* buffer=new wchar_t[len];
		_atow(string.Buffer(), buffer, (int)len);
		WString s=buffer;
		delete[] buffer;
		return s;
	}

	AString alower(const AString& string)
	{
		AString result=string.Buffer();
		_strlwr_s((char*)result.Buffer(), result.Length()+1);
		return result;
	}

	WString wlower(const WString& string)
	{
		WString result=string.Buffer();
		_wcslwr_s((wchar_t*)result.Buffer(), result.Length()+1);
		return result;
	}

	AString aupper(const AString& string)
	{
		AString result=string.Buffer();
		_strupr_s((char*)result.Buffer(), result.Length()+1);
		return result;
	}

	WString wupper(const WString& string)
	{
		WString result=string.Buffer();
		_wcsupr_s((wchar_t*)result.Buffer(), result.Length()+1);
		return result;
	}
}