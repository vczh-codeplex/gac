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
	};
}

#endif