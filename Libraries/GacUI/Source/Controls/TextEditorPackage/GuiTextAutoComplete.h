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

				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)override;
				void										Detach()override;
				void										TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
				void										TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)override;
				void										TextEditFinished()override;
			};

/***********************************************************************
GuiGrammarAutoComplete
***********************************************************************/
			
			/// <summary>Grammar based auto complete controller.</summary>
			class GuiGrammarAutoComplete
				: public GuiTextBoxAutoCompleteBase
				, private RepeatingParsingExecutor::ICallback
				, private RepeatingTaskExecutor<collections::Pair<Ptr<parsing::ParsingTreeObject>, WString>>
			{
				typedef collections::Pair<Ptr<parsing::ParsingTreeObject>, WString>			TaskArgumentType;
			public:
				struct Context
				{
					Ptr<parsing::ParsingTreeObject>			root;
					WString									rootCode;
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
				collections::SortedList<WString>			leftRecursiveRules;
				bool										editing;

				SpinLock									contextLock;
				Context										context;
				
				void										Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock)override;
				void										Detach()override;
				void										TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
				void										TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)override;
				void										TextEditFinished()override;
				void										OnParsingFinishedAsync(Ptr<parsing::ParsingTreeObject> node, const WString& code)override;
				void										CollectLeftRecursiveRules();
				void										Execute(const TaskArgumentType& input)override;
			protected:

				/// <summary>Called when the context of the code is selected.</summary>
				virtual void								OnContextFinishedAsync(Context& context);

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