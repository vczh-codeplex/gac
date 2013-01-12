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

				virtual void								OnReset();
				virtual ParsingState::TransitionResult		OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)=0;
			public:
				ParsingGeneralParser(Ptr<ParsingTable> _table);
				~ParsingGeneralParser();
				
				Ptr<ParsingTreeNode>						Parse(ParsingState& state, collections::List<Ptr<ParsingError>>& errors);
				Ptr<ParsingTreeNode>						Parse(const WString& input, const WString& rule, collections::List<Ptr<ParsingError>>& errors);
			};

/***********************************************************************
�﷨����������
***********************************************************************/

			class ParsingStrictParser : public ParsingGeneralParser
			{
			protected:

				ParsingState::TransitionResult				OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)override;
			public:
				ParsingStrictParser(Ptr<ParsingTable> _table=0);
				~ParsingStrictParser();
			};

			class ParsingAutoRecoverParser : public ParsingGeneralParser
			{
			protected:
				collections::Array<ParsingState::Future>	recoverFutures;
				vint										recoveringFutureIndex;

				ParsingState::TransitionResult				OnErrorRecover(ParsingState& state, const regex::RegexToken* currentToken, collections::List<Ptr<ParsingError>>& errors)override;
			public:
				ParsingAutoRecoverParser(Ptr<ParsingTable> _table=0);
				~ParsingAutoRecoverParser();
			};

/***********************************************************************
��������
***********************************************************************/

			extern Ptr<ParsingStrictParser>					CreateBootstrapStrictParser();
			extern Ptr<ParsingAutoRecoverParser>			CreateBootstrapAutoRecoverParser();
		}
	}
}

#endif