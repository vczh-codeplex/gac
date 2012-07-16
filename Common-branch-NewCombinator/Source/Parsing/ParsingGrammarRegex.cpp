#include "ParsingGrammarRegex.h"

namespace vl
{
	namespace parsing
	{

/***********************************************************************
RegexGrammar
***********************************************************************/

		RegexGrammar::RegexGrammar()
		{
		}

		RegexGrammar::~RegexGrammar()
		{
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
	}
}