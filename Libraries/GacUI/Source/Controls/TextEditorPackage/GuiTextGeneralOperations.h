/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR
#define VCZH_PRESENTATION_CONTROLS_GUITEXTELEMENTOPERATOR

#include "..\GuiContainerControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Common Operations
***********************************************************************/

			class ICommonTextEditCallback : public virtual IDescriptable, public Description<ICommonTextEditCallback>
			{
			public:
				virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock)=0;
				virtual void							Detach()=0;
				virtual void							TextEditNotify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
				virtual void							TextCaretChanged(TextPos oldBegin, TextPos oldEnd, TextPos newBegin, TextPos newEnd)=0;
				virtual void							TextEditFinished()=0;
			};

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			/// <summary>Repeating parsing executor.</summary>
			class RepeatingParsingExecutor : public RepeatingTaskExecutor<WString>
			{
			public:
				/// <summary>Callback.</summary>
				class ICallback : public virtual Interface
				{
				public:
					/// <summary>Callback when a parsing task is finished. <see cref="RepeatingParsingExecutor::ThreadSafeGetTreeNode"/> and <see cref="RepeatingParsingExecutor::ThreadSafeReturnTreeNode"/> can be used to get the new syntax tree.</summary>
					/// <param name="generatedNewNode">True indicates the parsing succeeded. Otherwise failed.</param>
					/// <param name="parsingExecutor">The parsing executor that parses the input.</param>
					virtual void											OnParsingFinished(bool generatedNewNode, RepeatingParsingExecutor* parsingExecutor)=0;
				};
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				SpinLock													parsingTreeLock;
				Ptr<parsing::ParsingTreeObject>								parsingTreeNode;
				collections::List<ICallback*>								callbacks;

			protected:

				void														Execute(const WString& input)override;
			public:
				/// <summary>Initialize the parsing executor.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule"></param>
				RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				~RepeatingParsingExecutor();
				
				/// <summary>Get the parsed syntax tree and block all threads when calling this function.</summary>
				/// <returns>The parsed syntax tree.</returns>
				Ptr<parsing::ParsingTreeObject>								ThreadSafeGetTreeNode();
				/// <summary>Unblock all threads that calling <see cref="ThreadSafeGetTreeNode"/>.</summary>
				void														ThreadSafeReturnTreeNode();
				/// <summary>Get the internal parser that parse the text.</summary>
				Ptr<parsing::tabling::ParsingGeneralParser>					GetParser();
				/// <summary>Detach callback.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														AttachCallback(ICallback* value);
				/// <summary>Detach callback.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														DetachCallback(ICallback* value);
			};
		}
	}
}

#endif