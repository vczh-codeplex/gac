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

TEST_CASE(TestLocale)
{
	DateTime dt=DateTime::LocalTime();

	FileStream fileStream(GetPath()+L"Locale.txt", FileStream::WriteOnly);
	BomEncoder encoder(BomEncoder::Utf16);
	EncoderStream encoderStream(fileStream, encoder);
	StreamWriter writer(encoderStream);
	{
		Locale locale=Locale::UserDefault();
		TEST_ASSERT(locale.Compare(L"a", L"b", Locale::None)<0);
		TEST_ASSERT(locale.Compare(L"b", L"a", Locale::None)>0);
		TEST_ASSERT(locale.Compare(L"a", L"a", Locale::None)==0);
		TEST_ASSERT(locale.Compare(L"a", L"A", Locale::None)<0);
		TEST_ASSERT(locale.Compare(L"A", L"a", Locale::None)>0);
		TEST_ASSERT(locale.Compare(L"a", L"A", Locale::IgnoreCase)==0);
		TEST_ASSERT(locale.Compare(L"a9b", L"a10b", Locale::None)>0);
		TEST_ASSERT(locale.Compare(L"a9b", L"a10b", Locale::DigitsAsNumbers)<0);
		
		TEST_ASSERT(locale.CompareOrdinal(L"a", L"b")<0);
		TEST_ASSERT(locale.CompareOrdinal(L"b", L"a")>0);
		TEST_ASSERT(locale.CompareOrdinal(L"a", L"a")==0);
		TEST_ASSERT(locale.CompareOrdinal(L"a", L"A")>0);
		TEST_ASSERT(locale.CompareOrdinal(L"A", L"a")<0);
		TEST_ASSERT(locale.CompareOrdinalIgnoreCase(L"a", L"A")==0);

		TEST_ASSERT(locale.FindFirst(L"abcba", L"b", Locale::None).key==1);
		TEST_ASSERT(locale.FindLast(L"abcba", L"b", Locale::None).key==3);
		TEST_ASSERT(locale.StartsWith(L"abc", L"A", Locale::None)==false);
		TEST_ASSERT(locale.StartsWith(L"abc", L"A", Locale::IgnoreCase)==true);
		TEST_ASSERT(locale.EndsWidth(L"abc", L"C", Locale::None)==false);
		TEST_ASSERT(locale.EndsWidth(L"abc", L"C", Locale::IgnoreCase)==true);
	}

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