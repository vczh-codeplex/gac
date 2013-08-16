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
				}
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnParsingFinishedAsync(node, input);
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

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetAttribute(vint index, const WString& name, vint argumentCount)
			{
				if(index!=-1)
				{
					Ptr<ParsingTable::AttributeInfo> att=grammarParser->GetTable()->GetAttributeInfo(index)->FindFirst(name);
					if(att && att->arguments.Count()==argumentCount)
					{
						return att;
					}
				}
				return 0;
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetColorAttribute(vint index)
			{
				return GetAttribute(index, L"Color", 1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetContextColorAttribute(vint index)
			{
				return GetAttribute(index, L"ContextColor", 1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetSemanticColorAttribute(vint index)
			{
				return GetAttribute(index, L"SemanticColor", 1);
			}
		}
	}
}