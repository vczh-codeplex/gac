/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Grammar with RegexLexer

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGGRAMMARREGEX
#define VCZH_PARSING_PARSINGGRAMMARREGEX

#include "Parsing.h"
#include "..\Regex\Regex.h"

namespace vl
{
	namespace parsing
	{
		struct RegexTokenHelper
		{
			static vint Id(const regex::RegexToken& token)
			{
				return token.token;
			}
		};

		template<>
		struct ParserNodeTypeDowngrader<regex::RegexToken>
		{
			typedef parsing_internal::NullParserType	Type;
		};

		class RegexGrammar : public Grammar<regex::RegexToken, RegexTokenHelper>
		{
		protected:
			collections::List<WString>			tokenDefinitions;
			collections::List<WString>			tokenNames;
		public:
			RegexGrammar();
			~RegexGrammar();
			
			Node<regex::RegexToken>				tok(vint id);
			vint								CreateToken(const WString& name, const WString& token);
		};
	}
}

#endif