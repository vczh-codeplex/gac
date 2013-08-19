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
RepeatingParsingExecutor::CallbackBase
***********************************************************************/

			RepeatingParsingExecutor::CallbackBase::CallbackBase(Ptr<RepeatingParsingExecutor> _parsingExecutor)
				:parsingExecutor(_parsingExecutor)
				,callbackAutoPushing(false)
				,callbackElement(0)
				,callbackElementModifyLock(0)
			{
			}

			RepeatingParsingExecutor::CallbackBase::~CallbackBase()
			{
			}

			void RepeatingParsingExecutor::CallbackBase::RequireAutoSubmitTask(bool enabled)
			{
				callbackAutoPushing=enabled;
			}

			void RepeatingParsingExecutor::CallbackBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)
			{
				if(_element)
				{
					SpinLock::Scope scope(_elementModifyLock);
					callbackElement=_element;
					callbackElementModifyLock=&_elementModifyLock;
				}
				
				parsingExecutor->ActivateCallback(this);
				if(callbackElement && callbackElementModifyLock && callbackAutoPushing)
				{
					SpinLock::Scope scope(*callbackElementModifyLock);
					RepeatingParsingInput input;
					input.editVersion=editVersion;
					input.code=callbackElement->GetLines().GetText();
					parsingExecutor->SubmitTask(input);
				}
			}

			void RepeatingParsingExecutor::CallbackBase::Detach()
			{
				if(callbackElement && callbackElementModifyLock)
				{
					SpinLock::Scope scope(*callbackElementModifyLock);
					callbackElement=0;
					callbackElementModifyLock=0;
				}
				
				parsingExecutor->DeactivateCallback(this);
			}

			void RepeatingParsingExecutor::CallbackBase::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
			}

			void RepeatingParsingExecutor::CallbackBase::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
			}

			void RepeatingParsingExecutor::CallbackBase::TextEditFinished(vuint editVersion)
			{
				if(callbackElement && callbackElementModifyLock && callbackAutoPushing)
				{
					SpinLock::Scope scope(*callbackElementModifyLock);
					RepeatingParsingInput input;
					input.editVersion=editVersion;
					input.code=callbackElement->GetLines().GetText();
					parsingExecutor->SubmitTask(input);
				}
			}

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			void RepeatingParsingExecutor::Execute(const RepeatingParsingInput& input)
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingTreeObject> node=grammarParser->Parse(input.code, grammarRule, errors).Cast<ParsingTreeObject>();
				if(node)
				{
					node->InitializeQueryCache();
				}

				RepeatingParsingOutput result;
				result.node=node;
				result.editVersion=input.editVersion;
				result.code=input.code;
				FOREACH(ICallback*, callback, callbacks)
				{
					callback->OnParsingFinishedAsync(result);
				}
			}

			RepeatingParsingExecutor::RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule)
				:grammarParser(_grammarParser)
				,grammarRule(_grammarRule)
				,autoPushingCallback(0)
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
				if(!value) return false;
				if(callbacks.Contains(value)) return false;
				callbacks.Add(value);
				return true;
			}

			bool RepeatingParsingExecutor::DetachCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				DeactivateCallback(value);
				callbacks.Remove(value);
				return true;
			}

			bool RepeatingParsingExecutor::ActivateCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				if(activatedCallbacks.Contains(value)) return false;
				activatedCallbacks.Add(value);

				if(!autoPushingCallback)
				{
					autoPushingCallback=value;
					autoPushingCallback->RequireAutoSubmitTask(true);
				}
				return true;
			}

			bool RepeatingParsingExecutor::DeactivateCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				if(!activatedCallbacks.Contains(value)) return false;

				if(autoPushingCallback==value)
				{
					autoPushingCallback->RequireAutoSubmitTask(false);
					autoPushingCallback=0;
				}
				activatedCallbacks.Remove(value);
				if(!autoPushingCallback && activatedCallbacks.Count()>0)
				{
					autoPushingCallback=activatedCallbacks[0];
					autoPushingCallback->RequireAutoSubmitTask(true);
				}
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