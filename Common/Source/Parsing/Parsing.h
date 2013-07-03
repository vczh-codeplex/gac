/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
�﷨������ͨ�ò���
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
�﷨����������
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

				vint										GetResolvableFutureLevels(collections::List<ParsingState::Future*>& futures, vint begin, vint end);
				vint										SearchPathForOneStep(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint& begin, vint& end, collections::List<Ptr<ParsingError>>& errors);
				vint										GetConflictReduceCount(collections::List<ParsingState::Future*>& futures);
				void										GetConflictReduceIndices(collections::List<ParsingState::Future*>& futures, vint conflictReduceCount, collections::Array<vint>& conflictReduceIndices);
				vint										GetAffectedStackNodeCount(collections::List<ParsingState::Future*>& futures, collections::Array<vint>& conflictReduceIndices);
				void										BuildSingleDecisionPath(ParsingState& state, ParsingState::Future* future, collections::List<regex::RegexToken*>& tokens, vint availableTokenCount, vint lastAvailableInstructionCount);
				void										BuildAmbiguousDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors);
				void										BuildDecisions(ParsingState& state, collections::List<ParsingState::Future*>& futures, collections::List<regex::RegexToken*>& tokens, vint begin, vint end, vint resolvableFutureLevels, collections::List<Ptr<ParsingError>>& errors);
			public:
				ParsingAmbiguousParser(Ptr<ParsingTable> _table=0);
				~ParsingAmbiguousParser();
				
				ParsingState::TransitionResult				ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)override;
				void										BeginParse()override;
			};

			class ParsingAutoRecoverAmbiguousParser : public ParsingAmbiguousParser
			{
			public:
				ParsingAutoRecoverAmbiguousParser(Ptr<ParsingTable> _table=0);
				~ParsingAutoRecoverAmbiguousParser();
			};

/***********************************************************************
��������
***********************************************************************/
			
			extern Ptr<ParsingGeneralParser>				CreateStrictParser(Ptr<ParsingTable> table);
			extern Ptr<ParsingGeneralParser>				CreateAutoRecoverParser(Ptr<ParsingTable> table);
			extern Ptr<ParsingGeneralParser>				CreateBootstrapStrictParser();
			extern Ptr<ParsingGeneralParser>				CreateBootstrapAutoRecoverParser();
		}
	}
}

#endif