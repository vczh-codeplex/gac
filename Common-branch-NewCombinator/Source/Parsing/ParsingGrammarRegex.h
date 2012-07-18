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

		namespace parsing_internal
		{
			template<>
			struct ParsingNodeTypeDowngrader<regex::RegexToken>
			{
				typedef parsing_internal::NullParsingType	Type;
			};
		}

		class RegexGrammar : public Grammar<regex::RegexToken, RegexTokenHelper>
		{
		protected:
			collections::List<WString>			tokenDefinitions;
			collections::List<WString>			tokenNames;

			WString								GetTokenName(vint token)override;
			Node<regex::RegexToken>				tok(vint id);
			vint								CreateToken(const WString& name, const WString& token);
		public:
			RegexGrammar();
			~RegexGrammar();
		};
	}
}

#endif