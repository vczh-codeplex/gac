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
			
			class ParsingTokenWalker : public Object, protected collections::IEnumerable<vint>
			{
			protected:
				collections::List<regex::RegexToken>&	tokens;
				Ptr<ParsingTable>						table;
				vint									currentToken;

				vint									GetNextIndex(vint index)const;
				vint									GetTableTokenIndex(vint index)const;
				collections::IEnumerator<vint>*			CreateEnumerator()const override;
			protected:
				class LookAheadEnumerator : public Object, public collections::IEnumerator<vint>
				{
				protected:
					const ParsingTokenWalker*			walker;
					vint								firstToken;
					vint								currentToken;
					vint								currentValue;
					vint								index;

				public:
					LookAheadEnumerator(const ParsingTokenWalker* _walker, vint _currentToken);
					LookAheadEnumerator(const LookAheadEnumerator& _enumerator);

					IEnumerator<vint>*					Clone()const override;
					const vint&							Current()const override;
					vint								Index()const override;
					bool								Next()override;
					void								Reset()override;
				};
			public:
				ParsingTokenWalker(collections::List<regex::RegexToken>& _tokens, Ptr<ParsingTable> _table);
				~ParsingTokenWalker();

				const collections::IEnumerable<vint>&	GetLookahead()const;
				void									Reset();
				bool									Move();
				vint									GetTableTokenIndex()const;
				regex::RegexToken*						GetRegexToken()const;
				vint									GetTokenIndexInStream()const;
			};

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
					ParsingTable::TransitionItem*			selectedItem;
					Future*									previous;
					Future*									next;

					Future()
						:currentState(-1)
						,reduceStateCount(0)
						,selectedToken(-1)
						,selectedItem(0)
						,previous(0)
						,next(0)
					{
					}
				};
			private:
				WString										input;
				Ptr<ParsingTable>							table;
				collections::List<regex::RegexToken>		tokens;
				Ptr<ParsingTokenWalker>						walker;

				collections::List<vint>						stateStack;
				vint										currentState;
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

				bool										TestTransitionItemInFuture(vint tableTokenIndex, Future* future, ParsingTable::TransitionItem* item, const collections::IEnumerable<vint>* lookAheadTokens);
				ParsingTable::TransitionItem*				MatchTokenInFuture(vint tableTokenIndex, Future* future, const collections::IEnumerable<vint>* lookAheadTokens);
				ParsingTable::TransitionItem*				MatchToken(vint tableTokenIndex, const collections::IEnumerable<vint>* lookAheadTokens);
				void										RunTransitionInFuture(ParsingTable::TransitionItem* transition, Future* previous, Future* now);
				ParsingState::TransitionResult				RunTransition(ParsingTable::TransitionItem* transition, regex::RegexToken* regexToken);

				void										Explore(vint tableTokenIndex, Future* previous, collections::List<Future*>& possibilities);
				bool										ReadTokenInFuture(vint tableTokenIndex, Future* previous, Future* now, const collections::IEnumerable<vint>* lookAheadTokens);
				TransitionResult							ReadToken(vint tableTokenIndex, regex::RegexToken* regexToken, const collections::IEnumerable<vint>* lookAheadTokens);
				TransitionResult							ReadToken();
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