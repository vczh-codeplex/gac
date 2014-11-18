#include "../../Source/UnitTest/UnitTest.h"
#include "../../Source/Locale.h"
#include "../../Source/Stream/Accessor.h"
#include "../../Source/Stream/CharFormat.h"
#include "../../Source/Stream/FileStream.h"
#include "../../Source/Collections/OperationForEach.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;

extern WString GetPath();

TEST_CASE(TestLocaleFormat)
{
	DateTime dt = DateTime::FromDateTime(2000, 1, 2, 13, 2, 3);
	auto df = L"yyyy,MM,MMM,MMMM,dd,ddd,dddd";
	auto ds = L"2000,01,Jan,January,02,Sun,Sunday";
	auto tf = L"hh,HH,mm,ss,tt";
	auto ts = L"01,13,02,03,PM";
	TEST_ASSERT(INVLOC.FormatDate(df, dt) == ds);
	TEST_ASSERT(INVLOC.FormatTime(tf, dt) == ts);
}

TEST_CASE(TestLocaleCompare)
{
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", Locale::Normalization::None) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", Locale::Normalization::None) != 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"abc", Locale::Normalization::IgnoreCase) == 0);
	TEST_ASSERT(INVLOC.Compare(L"abc", L"ABC", Locale::Normalization::IgnoreCase) == 0);
	
	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinal(L"abc", L"ABC") != 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"abc") == 0);
	TEST_ASSERT(INVLOC.CompareOrdinalIgnoreCase(L"abc", L"ABC") == 0);
}

TEST_CASE(TestLocaleFind)
{
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", Locale::Normalization::None).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"vczh", Locale::Normalization::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"vczh", Locale::Normalization::IgnoreCase).key == 3);
	TEST_ASSERT(INVLOC.FindFirst(L"abc", L"Vczh", Locale::Normalization::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindFirst(L"abcvczhdefvczhghi", L"Vczh", Locale::Normalization::IgnoreCase).key == 3);
	
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", Locale::Normalization::None).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", Locale::Normalization::None).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"vczh", Locale::Normalization::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"vczh", Locale::Normalization::IgnoreCase).key == 10);
	TEST_ASSERT(INVLOC.FindLast(L"abc", L"Vczh", Locale::Normalization::IgnoreCase).key == -1);
	TEST_ASSERT(INVLOC.FindLast(L"abcvczhdefvczhghi", L"Vczh", Locale::Normalization::IgnoreCase).key == 10);

	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", Locale::Normalization::None) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"a", Locale::Normalization::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"A", Locale::Normalization::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"c", Locale::Normalization::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.StartsWith(L"abc", L"C", Locale::Normalization::IgnoreCase) == false);

	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", Locale::Normalization::None) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", Locale::Normalization::None) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"a", Locale::Normalization::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"A", Locale::Normalization::IgnoreCase) == false);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"c", Locale::Normalization::IgnoreCase) == true);
	TEST_ASSERT(INVLOC.EndsWith(L"abc", L"C", Locale::Normalization::IgnoreCase) == true);
}

TEST_CASE(TestLocaleCase)
{
	TEST_ASSERT(INVLOC.ToUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLower(L"abcABC") == L"abcabc");
	TEST_ASSERT(INVLOC.ToLinguisticUpper(L"abcABC") == L"ABCABC");
	TEST_ASSERT(INVLOC.ToLinguisticLower(L"abcABC") == L"abcabc");
}

TEST_CASE(TestLocale)
{
	DateTime dt=DateTime::LocalTime();

	FileStream fileStream(GetPath()+L"Locale.txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);

	writer.WriteLine(L"Invariant locale: "+Locale::Invariant().GetName());
	writer.WriteLine(L"User default locale: "+Locale::UserDefault().GetName());
	writer.WriteLine(L"System default locale: "+Locale::SystemDefault().GetName());

	writer.WriteLine(L"========================================");
	{
		Locale locale=Locale::UserDefault();
		WString input=L"abcdeABCDEａｂｃｄｅＡＢＣＤＥ战斗戰鬥あいうえおアイウエオｱｲｳｴｵ";
		writer.WriteLine(L"[Normal] => "+input);
		writer.WriteLine(L"[ToFullWidth] => "+locale.ToFullWidth(input));
		writer.WriteLine(L"[ToHalfWidth] => "+locale.ToHalfWidth(input));
		writer.WriteLine(L"[ToHiragana] => "+locale.ToHiragana(input));
		writer.WriteLine(L"[ToKatagana] => "+locale.ToKatagana(input));
		writer.WriteLine(L"[ToLower] => "+locale.ToLower(input));
		writer.WriteLine(L"[ToUpper] => "+locale.ToUpper(input));
		writer.WriteLine(L"[ToLinguisticLower] => "+locale.ToLinguisticLower(input));
		writer.WriteLine(L"[ToLinguisticUpper] => "+locale.ToLinguisticUpper(input));
		writer.WriteLine(L"[ToSimplifiedChinese] => "+locale.ToSimplifiedChinese(input));
		writer.WriteLine(L"[ToTraditionalChinese] => "+locale.ToTraditionalChinese(input));
		writer.WriteLine(L"[ToTileCase] => "+locale.ToTileCase(input));
	}

	List<Locale> locales;
	Locale::Enumerate(locales);
	locales.Insert(0, Locale::Invariant());
	FOREACH(Locale, locale, locales)
	{
		writer.WriteLine(L"========================================");
		writer.WriteLine(L"Locale: "+locale.GetName());
#ifdef VCZH_MSVC
		writer.WriteLine(L"[Number 0] => "+locale.FormatNumber(L"0"));
		writer.WriteLine(L"[Number 1] => "+locale.FormatNumber(L"1"));
		writer.WriteLine(L"[Number -1] => "+locale.FormatNumber(L"-1"));
		writer.WriteLine(L"[Number 100.2] => "+locale.FormatNumber(L"100.2"));
		writer.WriteLine(L"[Number -100.2] => "+locale.FormatNumber(L"-100.2"));
		writer.WriteLine(L"[Currency 0] => "+locale.FormatCurrency(L"0"));
		writer.WriteLine(L"[Currency 1] => "+locale.FormatCurrency(L"1"));
		writer.WriteLine(L"[Currency -1] => "+locale.FormatCurrency(L"-1"));
		writer.WriteLine(L"[Currency 100.2] => "+locale.FormatCurrency(L"100.2"));
		writer.WriteLine(L"[Currency -100.2] => "+locale.FormatCurrency(L"-100.2"));
#endif
		{
			writer.WriteString(L"[ShortDayOfWeek]");
			for(vint i=0;i<=6;i++)
			{
				writer.WriteString(L" "+locale.GetShortDayOfWeekName(i));
			}
			writer.WriteLine(L"");
			
			writer.WriteString(L"[LongDayOfWeek]");
			for(vint i=0;i<=6;i++)
			{
				writer.WriteString(L" "+locale.GetLongDayOfWeekName(i));
			}
			writer.WriteLine(L"");
			
			writer.WriteString(L"[ShortMonth]");
			for(vint i=1;i<=12;i++)
			{
				writer.WriteString(L" "+locale.GetShortMonthName(i));
			}
			writer.WriteLine(L"");
			
			writer.WriteString(L"[LongMonth]");
			for(vint i=1;i<=12;i++)
			{
				writer.WriteString(L" "+locale.GetLongMonthName(i));
			}
			writer.WriteLine(L"");
		}
		{
			List<WString> formats;
			locale.GetLongDateFormats(formats);
			FOREACH(WString, format, formats)
			{
				writer.WriteLine(L"[LongDate]"+format+L" => "+locale.FormatDate(format, dt));
			}
		}
		{
			List<WString> formats;
			locale.GetShortDateFormats(formats);
			FOREACH(WString, format, formats)
			{
				writer.WriteLine(L"[ShortDate]"+format+L" => "+locale.FormatDate(format, dt));
			}
		}
		{
			List<WString> formats;
			locale.GetYearMonthDateFormats(formats);
			FOREACH(WString, format, formats)
			{
				writer.WriteLine(L"[YearMonth]"+format+L" => "+locale.FormatDate(format, dt));
			}
		}
		{
			List<WString> formats;
			locale.GetLongTimeFormats(formats);
			FOREACH(WString, format, formats)
			{
				writer.WriteLine(L"[LongTime]"+format+L" => "+locale.FormatTime(format, dt));
			}
		}
		{
			List<WString> formats;
			locale.GetShortTimeFormats(formats);
			FOREACH(WString, format, formats)
			{
				writer.WriteLine(L"[ShortTime]"+format+L" => "+locale.FormatTime(format, dt));
			}
		}
	}
}