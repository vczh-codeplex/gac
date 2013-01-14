#include "Locale.h"
#include <Windows.h>

namespace vl
{
	using namespace collections;

	BOOL CALLBACK Locale_EnumLocalesProcEx(
		_In_  LPWSTR lpLocaleString,
		_In_  DWORD dwFlags,
		_In_  LPARAM lParam
		)
	{
		((List<Locale>*)lParam)->Add(Locale(lpLocaleString));
		return TRUE;
	}

/***********************************************************************
Locale
***********************************************************************/

	Locale::Locale(const WString& _localeName)
		:localeName(_localeName)
	{
	}

	Locale::~Locale()
	{
	}

	Locale Locale::Invariant()
	{
		return Locale(LOCALE_NAME_INVARIANT);
	}

	Locale Locale::SystemDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetSystemDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}

	Locale Locale::UserDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH+1]={0};
		GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}

	void Locale::Enumerate(collections::List<Locale>& locales)
	{
		EnumSystemLocalesEx(&Locale_EnumLocalesProcEx, LOCALE_ALL, (LPARAM)&locales, NULL);
	}

	const WString& Locale::GetName()const
	{
		return localeName;
	}
}