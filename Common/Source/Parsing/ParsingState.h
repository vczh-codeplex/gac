/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::State

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGSTATE
#define VCZH_PARSING_PARSINGSTATE

#include "ParsingTable.h"

namespace vl
{
	namespace parsing
	{
		namespace tabling
		{

/***********************************************************************
Óï·¨·ÖÎöÆ÷
***********************************************************************/

			class ParsingState : public Object
			{
			public:
				struct ShiftReduceRange
				{
					regex::RegexToken*							shiftToken;
					regex::RegexToken*							reduceToken;

					ShiftReduceRange()
						:shiftToken(0)
						,reduceToken(0)
					{
					}
				};

				struct TransitionResult
				{
					vint										tableTokenIndex;
					vint										tableStateSource;
					vint										tableStateTarget;
					vint										tokenIndexInStream;
					regex::RegexToken*							token;
					ParsingTable::TransitionItem*				transition;
					Ptr<collections::List<ShiftReduceRange>>	shiftReduceRanges;

					TransitionResult()
						:tableTokenIndex(-1)
						,tableStateSource(-1)
						,tableStateTarget(-1)
						,tokenIndexInStream(-1)
						,token(0)
						,transition(0)
					{
					}

					operator bool()const
					{
						return transition!=0;
					}

					void AddShiftReduceRange(regex::RegexToken* shiftToken, regex::RegexToken* reduceToken)
					{
						ShiftReduceRange range;
						range.shiftToken=shiftToken;
						range.reduceToken=reduceToken;
						if(!shiftReduceRanges)
						{
							shiftReduceRanges=new collections::List<ShiftReduceRange>();
						}
						shiftReduceRanges->Add(range);
					}
				};

				struct Future
				{
					vint									currentState;
					vint									reduceStateCount;
					collections::List<vint>					shiftStates;
					vint									selectedToken;
					Future*									previous;
					Future*									next;

					Future()
						:currentState(-1)
						,reduceStateCount(0)
						,selectedToken(-1)
						,previous(0)
						,next(0)
					{
					}
				};
			private:
				WString										input;
				Ptr<ParsingTable>							table;
				collections::List<regex::RegexToken>		tokens;

				collections::List<vint>						stateStack;
				vint										currentState;
				vint										currentToken;
				vint										tokenSequenceIndex;
				
				collections::List<regex::RegexToken*>		shiftTokenStack;
				regex::RegexToken*							shiftToken;
				regex::RegexToken*							reduceToken;
			public:
				ParsingState(const WString& _input, Ptr<ParsingTable> _table, vint codeIndex=-1);
				~ParsingState();

				const WString&								GetInput();
				Ptr<ParsingTable>							GetTable();
				const collections::List<regex::RegexToken>&	GetTokens();
				regex::RegexToken*							GetToken(vint index);

				vint										Reset(const WString& rule);
				vint										GetCurrentToken();
				const collections::List<vint>&				GetStateStack();
				vint										GetCurrentState();

				ParsingTable::TransitionItem*				MatchToken(vint tableTokenIndex);
				ParsingTable::TransitionItem*				MatchTokenInFuture(vint tableTokenIndex, Future* future);
				TransitionResult							ReadToken(vint tableTokenIndex, regex::RegexToken* regexToken);
				TransitionResult							ReadToken();
				bool										ReadTokenInFuture(vint tableTokenIndex, Future* previous, Future* now);
			};

/***********************************************************************
Óï·¨Ê÷Éú³ÉÆ÷
***********************************************************************/

			class ParsingTreeBuilder : public Object
			{
			protected:
				Ptr<ParsingTreeNode>						createdObject;
				Ptr<ParsingTreeObject>						operationTarget;
				collections::List<Ptr<ParsingTreeObject>>	nodeStack;
			public:
				ParsingTreeBuilder();
				~ParsingTreeBuilder();

				void										Reset();
				bool										Run(const ParsingState::TransitionResult& result);
				Ptr<ParsingTreeObject>						GetNode();
			};
		}
	}
}

#endif