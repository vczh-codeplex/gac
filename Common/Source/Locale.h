/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Locale

Interfaces:
***********************************************************************/

#ifndef VCZH_LOCALE
#define VCZH_LOCALE

#include "String.h"
#include "Collections\List.h"

namespace vl
{
	class Locale : public Object
	{
	protected:
		WString						localeName;

	public:
		Locale(const WString& _localeName=WString::Empty);
		~Locale();

		static Locale				Invariant();
		static Locale				SystemDefault();
		static Locale				UserDefault();
		static void					Enumerate(collections::List<Locale>& locales);

		const WString&				GetName()const;

		void						GetShortDateFormats(collections::List<WString>& formats);
		void						GetLongDateFormats(collections::List<WString>& formats);
		void						GetYearMonthDateFormats(collections::List<WString>& formats);
		void						GetLongTimeFormats(collections::List<WString>& formats);
		void						GetShortTimeFormats(collections::List<WString>& formats);

		WString						FormatDate(const WString& format, DateTime date);
		WString						FormatTime(const WString& format, DateTime time);
		WString						FormatNumber(const WString& number);
		WString						FormatCurrency(const WString& currency);

		WString						GetShortDayOfWeekName(vint dayOfWeek);
		WString						GetLongDayOfWeekName(vint dayOfWeek);
		WString						GetShortMonthName(vint month);
		WString						GetLongMonthName(vint month);

		WString						ToFullWidth(const WString& str);
		WString						ToHalfWidth(const WString& str);
		WString						ToHiragana(const WString& str);
		WString						ToKatagana(const WString& str);
		WString						ToLower(const WString& str);
		WString						ToUpper(const WString& str);
		WString						ToLinguisticLower(const WString& str);
		WString						ToLinguisticUpper(const WString& str);
		WString						ToSimplifiedChinese(const WString& str);
		WString						ToTraditionalChinese(const WString& str);
		WString						ToTileCase(const WString& str);

		enum Normalization
		{
			None=0,
			IgnoreCase=1,
			IgnoreCaseLinguistic=2,
			IgnoreKanaType=4,
			IgnoreNonSpace=8,
			IgnoreSymbol=16,
			IgnoreWidth=32,
			DigitsAsNumbers=64,
			StringSoft=128,
		};
		vint									Compare(const WString& s1, const WString& s2, Normalization normalization);
		vint									CompareOrdinal(const WString& s1, const WString& s2);
		vint									CompareOrdinalIgnoreCase(const WString& s1, const WString& s2);
		collections::Pair<vint, vint>			FindFirst(const WString& text, const WString& find, Normalization normalization);
		collections::Pair<vint, vint>			FindLast(const WString& text, const WString& find, Normalization normalization);
		bool									StartsWith(const WString& text, const WString& find, Normalization normalization);
		bool									EndsWidth(const WString& text, const WString& find, Normalization normalization);
	};

#define INVLOC vl::Locale::Invariant()
}

#endif