#include "Basic.h"
#if defined VCZH_WINDOWS
#include <Windows.h>
#elif defined VCZH_LINUX
#include <time.h>
#include <memory.h>
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

#if defined VCZH_WINDOWS
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
#elif defined VCZH_LINUX
	tm ConvertDateTimeToTM(const DateTime& dt)
	{
		tm timeinfo;
		memset(&timeinfo, 0, sizeof(timeinfo));
		timeinfo.tm_year = dt.year;
		timeinfo.tm_mon = dt.month;
		timeinfo.tm_mday = dt.day;
		timeinfo.tm_hour = dt.hour;
		timeinfo.tm_min = dt.minute;
		timeinfo.tm_sec = dt.second;
		return timeinfo;
	}

	DateTime ConvertTMToDateTime(tm* timeinfo)
	{
		DateTime dt;
		dt.year = timeinfo->tm_year;
		dt.month = timeinfo->tm_mon;
		dt.day = timeinfo->tm_mday;
		dt.hour = timeinfo->tm_hour;
		dt.minute = timeinfo->tm_min;
		dt.second = timeinfo->tm_sec;
		dt.milliseconds = 0;
		dt.filetime = mktime(timeinfo);
		dt.totalMilliseconds = dt.filetime*1000;
		return dt;
	}
#endif

	DateTime DateTime::LocalTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);
		return SystemTimeToDateTime(systemTime);
#elif defined VCZH_LINUX
		time_t timer = time(nullptr);
		tm* timeinfo = localtime(&timer);
		return ConvertTMToDateTime(timeinfo);
#endif
	}

	DateTime DateTime::UtcTime()
	{
#if defined VCZH_WINDOWS
		SYSTEMTIME utcTime;
		GetSystemTime(&utcTime);
		return SystemTimeToDateTime(utcTime);
#elif defined VCZH_LINUX
		time_t timer = time(nullptr);
		tm* timeinfo = gmtime(&timer);
		return ConvertTMToDateTime(timeinfo);
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
		tm timeinfo;
		memset(&timeinfo, 0, sizeof(timeinfo));
		timeinfo.tm_year = _year;
		timeinfo.tm_mon = _month;
		timeinfo.tm_mday = _day;
		timeinfo.tm_hour = _hour;
		timeinfo.tm_min = _minute;
		timeinfo.tm_sec = _second;
		return ConvertTMToDateTime(&timeinfo);
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
		time_t timer = (time_t)filetime;
		tm* timeinfo = localtime(&timer);
		return ConvertTMToDateTime(timeinfo);
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
		time_t localTimer = time(nullptr);
		time_t utcTimer = mktime(gmtime(&localTimer));
		time_t timer = (time_t)filetime + localTimer - utcTimer;
		tm* timeinfo = localtime(&timer);
		return ConvertTMToDateTime(timeinfo);
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
		time_t timer = (time_t)filetime;
		tm* timeinfo = gmtime(&timer);
		return ConvertTMToDateTime(timeinfo);
#endif
	}

	DateTime DateTime::Forward(vuint64_t milliseconds)
	{
		return FromFileTime(filetime+milliseconds/1000);
	}

	DateTime DateTime::Backward(vuint64_t milliseconds)
	{
		return FromFileTime(filetime-milliseconds/1000);
	}

/***********************************************************************
Interface
***********************************************************************/

	Interface::~Interface()
	{
	}
}
