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
				
				Ptr<ParsingTable>							GetTable();
				virtual void								BeginParse();
				virtual ParsingState::TransitionResult		ParseStep(ParsingState& state, collections::List<Ptr<ParsingError>>& errors)=0;
				bool										Parse(ParsingState& state, ParsingTransitionProcessor& processor, collections::List<Ptr<ParsingError>>& errors);
				Ptr<ParsingTreeNode>						Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors);
				Ptr<ParsingTreeNode>						Parse(const WString& input, const WString& rule, collections::List<Ptr<ParsingError>>& errors);
			};

/***********************************************************************
语法分析器策略
***********************************************************************/

			class ParsingStrictParser : public ParsingGeneralParser
			{
			protected:

				virtual ParsingState::TransitionResult		OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors);
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

				ParsingState::TransitionResult				OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)override;
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

				virtual void								OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<ParsingState::Future*>& futures, vint& begin, vint& end, vint& insertedTokenCount, vint& skippedTokenCount, collections::List<Ptr<ParsingError>>& errors);
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
			protected:

				void										OnErrorRecover(ParsingState& state, vint currentTokenIndex, const regex::RegexToken* currentToken, collections::List<ParsingState::Future*>& futures, vint& begin, vint& end, vint& insertedTokenCount, vint& skippedTokenCount, collections::List<Ptr<ParsingError>>& errors)override;
			public:
				ParsingAutoRecoverAmbiguousParser(Ptr<ParsingTable> _table=0);
				~ParsingAutoRecoverAmbiguousParser();
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

/***********************************************************************
反射
***********************************************************************/

#ifndef VCZH_DEBUG_NO_REFLECTION

#include "..\Reflection\GuiTypeDescriptorMacros.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
#define PARSINGREFLECTION_TYPELIST(F)\
			F(parsing::ParsingTextPos)\
			F(parsing::ParsingTextRange)\
			F(parsing::ParsingTreeNode)\
			F(parsing::ParsingTreeToken)\
			F(parsing::ParsingTreeObject)\
			F(parsing::ParsingTreeArray)\
			F(parsing::ParsingScope)\
			F(parsing::ParsingScopeSymbol)\
			F(parsing::ParsingScopeFinder)\
			F(parsing::ParsingTreeCustomBase)\
			F(parsing::ParsingToken)\
			F(parsing::ParsingError)\

			PARSINGREFLECTION_TYPELIST(DECL_TYPE_INFO)
		}
	}
}

#endif

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			extern bool								LoadParsingTypes();
		}
	}
}

#endif