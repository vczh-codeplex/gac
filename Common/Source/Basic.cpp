#include "Basic.h"
#ifdef VCZH_WINDOWS
#include <Windows.h>
#endif

namespace vl
{

/***********************************************************************
NotCopyable
***********************************************************************/

	NotCopyable::NotCopyable()
	{
	}

	NotCopyable::NotCopyable(const NotCopyable&)
	{
	}

	NotCopyable& NotCopyable::operator=(const NotCopyable&)
	{
		return *this;
	}

/***********************************************************************
Error
***********************************************************************/

	Error::Error(wchar_t* _description)
	{
		description=_description;
	}

	wchar_t* Error::Description()const
	{
		return description;
	}

/***********************************************************************
Object
***********************************************************************/

	Object::~Object()
	{
	}

/***********************************************************************
DateTime
***********************************************************************/

#ifdef VCZH_WINDOWS
	DateTime SystemTimeToDateTime(const SYSTEMTIME& systemTime)
	{
		DateTime dateTime;
		dateTime.year=systemTime.wYear;
		dateTime.month=systemTime.wMonth;
		dateTime.dayOfWeek=systemTime.wDayOfWeek;
		dateTime.day=systemTime.wDay;
		dateTime.hour=systemTime.wHour;
		dateTime.minute=systemTime.wMinute;
		dateTime.second=systemTime.wSecond;
		dateTime.milliseconds=systemTime.wMilliseconds;

		FILETIME fileTime;
		SystemTimeToFileTime(&systemTime, &fileTime);
		ULARGE_INTEGER largeInteger;
		largeInteger.HighPart=fileTime.dwHighDateTime;
		largeInteger.LowPart=fileTime.dwLowDateTime;
		dateTime.filetime=largeInteger.QuadPart;
		dateTime.totalMilliseconds=dateTime.filetime/10000;

		return dateTime;
	}

	SYSTEMTIME DateTimeToSystemTime(const DateTime& dateTime)
	{
		ULARGE_INTEGER largeInteger;
		largeInteger.QuadPart=dateTime.filetime;
		FILETIME fileTime;
		fileTime.dwHighDateTime=largeInteger.HighPart;
		fileTime.dwLowDateTime=largeInteger.LowPart;

		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);
		return systemTime;
	}
#endif

	DateTime DateTime::LocalTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		return SystemTimeToDateTime(systemTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime DateTime::UtcTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME utcTime;
		GetSystemTime(&utcTime);
		return SystemTimeToDateTime(utcTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime DateTime::FromDateTime(vint _year, vint _month, vint _day, vint _hour, vint _minute, vint _second, vint _milliseconds)
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME systemTime;
		memset(&systemTime, 0, sizeof(systemTime));
		systemTime.wYear=(WORD)_year;
		systemTime.wMonth=(WORD)_month;
		systemTime.wDay=(WORD)_day;
		systemTime.wHour=(WORD)_hour;
		systemTime.wMinute=(WORD)_minute;
		systemTime.wSecond=(WORD)_second;
		systemTime.wMilliseconds=(WORD)_milliseconds;

		FILETIME fileTime;
		SystemTimeToFileTime(&systemTime, &fileTime);
		FileTimeToSystemTime(&fileTime, &systemTime);
		return SystemTimeToDateTime(systemTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime DateTime::FromFileTime(vuint64_t filetime)
	{
#if defined VCZH_WINDOWS
		ULARGE_INTEGER largeInteger;
		largeInteger.QuadPart=filetime;
		FILETIME fileTime;
		fileTime.dwHighDateTime=largeInteger.HighPart;
		fileTime.dwLowDateTime=largeInteger.LowPart;

		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&fileTime, &systemTime);
		return SystemTimeToDateTime(systemTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime::DateTime()
		:year(0)
		,month(0)
		,day(0)
		,hour(0)
		,minute(0)
		,second(0)
		,milliseconds(0)
		,filetime(0)
	{
	}

	DateTime DateTime::ToLocalTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME utcTime=DateTimeToSystemTime(*this);
		SYSTEMTIME localTime;
		SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);
		return SystemTimeToDateTime(localTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime DateTime::ToUtcTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME localTime=DateTimeToSystemTime(*this);
		SYSTEMTIME utcTime;
		TzSpecificLocalTimeToSystemTime(NULL, &localTime, &utcTime);
		return SystemTimeToDateTime(utcTime);
#elif defined VCZH_LINUX
#endif
	}

	DateTime DateTime::Forward(vuint64_t milliseconds)
	{
		return FromFileTime(filetime+milliseconds*10000);
	}

	DateTime DateTime::Backward(vuint64_t milliseconds)
	{
		return FromFileTime(filetime-milliseconds*10000);
	}

/***********************************************************************
Interface
***********************************************************************/

	Interface::~Interface()
	{
	}
}
