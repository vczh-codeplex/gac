#include "ParsingGrammarRegex.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
RegexGrammar
***********************************************************************/

		WString RegexGrammar::GetTokenName(vint token)
		{
			return tokenNames[token];
		}
			
		Node<regex::RegexToken> RegexGrammar::tok(vint id)
		{
			Ptr<parsing_internal::_Token> result=new parsing_internal::_Token;
			result->token=id;
			result->name=tokenNames[id];
			return result;
		}

		vint RegexGrammar::CreateToken(const WString& name, const WString& token)
		{
			tokenNames.Add(name);
			return tokenDefinitions.Add(token);
		}

		RegexGrammar::RegexGrammar()
		{
		}

		RegexGrammar::~RegexGrammar()
		{
		}
	}
}