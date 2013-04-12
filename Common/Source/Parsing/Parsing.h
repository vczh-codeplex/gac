/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Parser

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSING
#define VCZH_PARSING_PARSING

#include "ParsingState.h"

namespace vl
{
	namespace parsing
	{
		namespace tabling
		{

/***********************************************************************
语法分析器通用策略
***********************************************************************/

			class ParsingGeneralParser : public Object
			{
			protected:
				Ptr<ParsingTable>							table;
				
			public:
				ParsingGeneralParser(Ptr<ParsingTable> _table);
				~ParsingGeneralParser();
				
				virtual ParsingState::TransitionResult		ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)=0;
				virtual void								BeginParse();
				Ptr<ParsingTreeNode>						Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors);
				Ptr<ParsingTreeNode>						Parse(const WString& input, const WString& rule, collections::List<Ptr<ParsingError>>& errors);
			};

/***********************************************************************
语法分析器策略
***********************************************************************/

			class ParsingStrictParser : public ParsingGeneralParser
			{
			protected:

				virtual ParsingState::TransitionResult		OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors);
			public:
				ParsingStrictParser(Ptr<ParsingTable> _table=0);
				~ParsingStrictParser();
				
				ParsingState::TransitionResult				ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)override;
			};

			class ParsingAutoRecoverParser : public ParsingStrictParser
			{
			protected:
				collections::Array<ParsingState::Future>	recoverFutures;
				vint										recoveringFutureIndex;

				ParsingState::TransitionResult				OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)override;
			public:
				ParsingAutoRecoverParser(Ptr<ParsingTable> _table=0);
				~ParsingAutoRecoverParser();
			};

			class ParsingAmbiguousParser : public ParsingGeneralParser
			{
				typedef collections::List<ParsingState::TransitionResult>		DecisionList;
			protected:

				DecisionList								decisions;
				vint										consumedDecisionCount;

				bool										IsAmbiguityResolvable(collections::List<ParsingState::Future*>& futures, vint begin, vint end);
				void										SearchPath(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors);
				void										CheckAmbiguousFutures(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors);
				void										BuildDeterministicDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors);
				void										BuildDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, collections::List<Ptr<ParsingError>>& errors);
			public:
				ParsingAmbiguousParser(Ptr<ParsingTable> _table=0);
				~ParsingAmbiguousParser();
				
				ParsingState::TransitionResult				ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)override;
				void										BeginParse()override;
			};

/***********************************************************************
辅助函数
***********************************************************************/
			
			extern Ptr<ParsingGeneralParser>				CreateStrictParser(Ptr<ParsingTable> table);
			extern Ptr<ParsingGeneralParser>				CreateAutoRecoverParser(Ptr<ParsingTable> table);
			extern Ptr<ParsingGeneralParser>				CreateBootstrapStrictParser();
			extern Ptr<ParsingGeneralParser>				CreateBootstrapAutoRecoverParser();
		}
	}
}

#endif