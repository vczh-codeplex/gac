/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
			class GuiTextBoxAutoCompleteBase : public Object, public virtual ICommonTextEditCallback
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
				, private RepeatingParsingExecutor::CallbackBase
				, private RepeatingTaskExecutor<RepeatingParsingOutput>
			{
			public:
				/// <summary>The analysed data from an input code.</summary>
				struct Context
				{
					/// <summary>The input data.</summary>
					RepeatingParsingOutput					input;
					/// <summary>The selected context in the syntax tree.</summary>
					parsing::ParsingTreeObject*				contextNode;
					/// <summary>The code of the selected context.</summary>
					WString									contextNodeCode;
					/// <summary>The rule name that can parse the code of the selected context.</summary>
					WString									contextNodeRule;

					Context()
						:contextNode(0)
					{
					}
				};
			private:
				collections::SortedList<WString>			leftRecursiveRules;
				bool										editing;

				SpinLock									editTraceLock;
				collections::List<TextEditNotifyStruct>		editTrace;

				SpinLock									contextLock;
				Context										context;
				
				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)override;
				void										Detach()override;
				void										TextEditNotify(const TextEditNotifyStruct& arguments)override;
				void										TextCaretChanged(const TextCaretChangedStruct& arguments)override;
				void										TextEditFinished(vuint editVersion)override;
				void										OnParsingFinishedAsync(const RepeatingParsingOutput& output)override;
				void										CollectLeftRecursiveRules();

				vint										UnsafeGetEditTraceIndex(vuint editVersion);
				void										ExecuteRefresh(Context& newContext);
				void										ExecuteEdit(Context& newContext);
				void										Execute(const RepeatingParsingOutput& input)override;
				void										PostList(const Context& newContext);
			protected:

				/// <summary>Called when the context of the code is selected.</summary>
				/// <param name="context">The selected context.</param>
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