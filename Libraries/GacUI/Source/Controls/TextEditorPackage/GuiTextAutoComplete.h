/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTAUTOCOMPLETE
#define VCZH_PRESENTATION_CONTROLS_GUITEXTAUTOCOMPLETE

#include "GuiTextGeneralOperations.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiTextBoxAutoCompleteBase
***********************************************************************/
			
			/// <summary>The base class of text box auto complete controller.</summary>
			class GuiTextBoxAutoCompleteBase : public Object, public ICommonTextEditCallback
			{
			protected:
				elements::GuiColorizedTextElement*			element;
				SpinLock*									elementModifyLock;

			public:
				GuiTextBoxAutoCompleteBase();
				~GuiTextBoxAutoCompleteBase();

				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)override;
				void										Detach()override;
				void										TextEditNotify(const TextEditNotifyStruct& arguments)override;
				void										TextCaretChanged(const TextCaretChangedStruct& arguments)override;
				void										TextEditFinished(vuint editVersion)override;
			};

/***********************************************************************
GuiGrammarAutoComplete
***********************************************************************/
			
			/// <summary>Grammar based auto complete controller.</summary>
			class GuiGrammarAutoComplete
				: public GuiTextBoxAutoCompleteBase
				, private RepeatingParsingExecutor::ICallback
				, private RepeatingTaskExecutor<RepeatingParsingOutput>
			{
			public:
				struct Context
				{
					RepeatingParsingOutput					input;
					parsing::ParsingTreeObject*				contextNode;
					WString									contextNodeCode;
					WString									contextNodeRule;

					Context()
						:contextNode(0)
					{
					}
				};
			private:
				Ptr<RepeatingParsingExecutor>				parsingExecutor;
				bool										autoPushing;
				collections::SortedList<WString>			leftRecursiveRules;
				bool										editing;

				SpinLock									contextLock;
				Context										context;
				
				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)override;
				void										Detach()override;
				void										TextEditNotify(const TextEditNotifyStruct& arguments)override;
				void										TextCaretChanged(const TextCaretChangedStruct& arguments)override;
				void										TextEditFinished(vuint editVersion)override;
				void										OnParsingFinishedAsync(const RepeatingParsingOutput& output)override;
				void										RequireAutoSubmitTask(bool enabled)override;
				void										CollectLeftRecursiveRules();
				void										Execute(const RepeatingParsingOutput& input)override;
			protected:

				/// <summary>Called when the context of the code is selected.</summary>
				virtual void								OnContextFinishedAsync(const Context& context);

				/// <summary>Call this function in the derived class's destructor when it overrided <see cref="OnContextFinishedAsync"/>.</summary>
				void										EnsureAutoCompleteFinished();
			public:
				/// <summary>Create the auto complete controller with a created parsing executor.</summary>
				/// <param name="_parsingExecutor">The parsing executor.</param>
				GuiGrammarAutoComplete(Ptr<RepeatingParsingExecutor> _parsingExecutor);
				/// <summary>Create the auto complete controller with a specified grammar and start rule to create a <see cref="RepeatingParsingExecutor"/>.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule"></param>
				GuiGrammarAutoComplete(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				~GuiGrammarAutoComplete();

				/// <summary>Get the internal parsing executor.</summary>
				/// <returns>The parsing executor.</returns>
				Ptr<RepeatingParsingExecutor>				GetParsingExecutor();
			};
		}
	}
}

#endif