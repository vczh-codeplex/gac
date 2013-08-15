#include "GuiTextGeneralOperations.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace elements::text;
			using namespace compositions;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace collections;

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			void RepeatingParsingExecutor::Execute(const WString& input)
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingTreeObject> node=grammarParser->Parse(input, grammarRule, errors).Cast<ParsingTreeObject>();
				if(node)
				{
					node->InitializeQueryCache();

					SpinLock::Scope scope(parsingTreeLock);
					parsingTreeNode=node;
				}

				bool generatedNewNode=node;
				node=0;
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnParsingFinished(generatedNewNode, this);
				}
			}

			RepeatingParsingExecutor::RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
				:grammarParser(_grammarParser)
				,grammarRule(_grammarRule)
			{
			}

			RepeatingParsingExecutor::~RepeatingParsingExecutor()
			{
				EnsureTaskFinished();
				SpinLock::Scope scope(parsingTreeLock);
				parsingTreeNode=0;
			}

			Ptr<parsing::ParsingTreeObject> RepeatingParsingExecutor::ThreadSafeGetTreeNode()
			{
				parsingTreeLock.Enter();
				return parsingTreeNode;
			}

			void RepeatingParsingExecutor::ThreadSafeReturnTreeNode()
			{
				parsingTreeLock.Leave();
			}

			Ptr<parsing::tabling::ParsingGeneralParser> RepeatingParsingExecutor::GetParser()
			{
				return grammarParser;
			}

			bool RepeatingParsingExecutor::AttachCallback(ICallback* value)
			{
				if(callbacks.Contains(value)) return false;
				callbacks.Add(value);
				return true;
			}

			bool RepeatingParsingExecutor::DetachCallback(ICallback* value)
			{
				if(!callbacks.Contains(value)) return false;
				callbacks.Remove(value);
				return true;
			}
		}
	}
}