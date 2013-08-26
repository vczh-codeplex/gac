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
				struct TextEditNotifyStruct
				{
					TextPos								originalStart;
					TextPos								originalEnd;
					WString								originalText;
					TextPos								inputStart;
					TextPos								inputEnd;
					WString								inputText;
					vuint								editVersion;

					TextEditNotifyStruct()
						:editVersion(0)
					{
					}
				};

				struct TextCaretChangedStruct
				{
					TextPos								oldBegin;
					TextPos								oldEnd;
					TextPos								newBegin;
					TextPos								newEnd;
					vuint								editVersion;

					TextCaretChangedStruct()
						:editVersion(0)
					{
					}
				};

				virtual void							Attach(elements::GuiColorizedTextElement* element, SpinLock& elementModifyLock, vuint editVersion)=0;
				virtual void							Detach()=0;
				virtual void							TextEditNotify(const TextEditNotifyStruct& arguments)=0;
				virtual void							TextCaretChanged(const TextCaretChangedStruct& arguments)=0;
				virtual void							TextEditFinished(vuint editVersion)=0;
			};

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			/// <summary>A data structure storing the parsing input for text box control.</summary>
			struct RepeatingParsingInput
			{
				/// <summary>The text box edit version of the code.</summary>
				vuint									editVersion;
				/// <summary>The code.</summary>
				WString									code;

				RepeatingParsingInput()
					:editVersion(0)
				{
				}
			};

			/// <summary>A data structure storing the parsing result for text box control.</summary>
			struct RepeatingParsingOutput
			{
				/// <summary>The parsed syntax tree.</summary>
				Ptr<parsing::ParsingTreeObject>			node;
				/// <summary>The text box edit version of the code.</summary>
				vuint									editVersion;
				/// <summary>The code.</summary>
				WString									code;

				RepeatingParsingOutput()
					:editVersion(0)
				{
				}
			};

			/// <summary>Repeating parsing executor.</summary>
			class RepeatingParsingExecutor : public RepeatingTaskExecutor<RepeatingParsingInput>
			{
			public:
				/// <summary>Callback.</summary>
				class ICallback : public virtual Interface
				{
				public:
					/// <summary>Callback when a parsing task is finished.</summary>
					/// <param name="generatedNewNode">True indicates the parsing succeeded. Otherwise failed.</param>
					/// <param name="parsingExecutor">The parsing executor that parses the input.</param>
					virtual void											OnParsingFinishedAsync(const RepeatingParsingOutput& output)=0;
					/// <summary>Callback when <see cref="RepeatingParsingExecutor"/> requires enabling or disabling automatically repeating calling to the SubmitTask function.</summary>
					/// <param name="enabled">Set to true to require an automatically repeating calling to the SubmitTask function</param>
					virtual void											RequireAutoSubmitTask(bool enabled)=0;
				};

				/// <summary>A base class for implementing a callback.</summary>
				class CallbackBase : public virtual ICallback, public virtual ICommonTextEditCallback
				{
				private:
					bool													callbackAutoPushing;
					elements::GuiColorizedTextElement*						callbackElement;
					SpinLock*												callbackElementModifyLock;

				protected:
					Ptr<RepeatingParsingExecutor>							parsingExecutor;

				public:
					CallbackBase(Ptr<RepeatingParsingExecutor> _parsingExecutor);
					~CallbackBase();

					void													RequireAutoSubmitTask(bool enabled)override;
					void													Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, vuint editVersion)override;
					void													Detach()override;
					void													TextEditNotify(const TextEditNotifyStruct& arguments)override;
					void													TextCaretChanged(const TextCaretChangedStruct& arguments)override;
					void													TextEditFinished(vuint editVersion)override;
				};
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				collections::List<ICallback*>								callbacks;
				collections::List<ICallback*>								activatedCallbacks;
				ICallback*													autoPushingCallback;

			protected:

				void														Execute(const RepeatingParsingInput& input)override;
			public:
				/// <summary>Initialize the parsing executor.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule"></param>
				RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule);
				~RepeatingParsingExecutor();
				
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
				/// <summary>Activate a callback. Activating a callback means that the callback owner has an ability to watch a text box modification, e.g., an attached <see cref="ICommonTextEditCallback"/> that is also an <see cref="ICallback"/>. The <see cref="RepeatingParsingExecutor"/> may require one of the activated callback to push code for parsing automatically via a call to <see cref="ICallback::RequireAutoSubmitTask"/>.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														ActivateCallback(ICallback* value);
				/// <summary>Deactivate a callback. See <see cref="ActivateCallback"/> for deatils.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The callback.</param>
				bool														DeactivateCallback(ICallback* value);

				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAttribute(vint index, const WString& name, vint argumentCount);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetContextColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetSemanticColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAutoCompleteCandidateAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAutoCompleteTokenAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAutoCompleteTypeAttribute(vint index);

				/*
				@Color(ColorName)
					field:	color of the token field when the token type is marked with @ContextColor
					token:	color of the token
				@ContextColor(DefaultColor)
					token:	color of the token, and it may be changed if the token field is marked with @Color or @SemanticColor
				@SemanticColor(Type)
					field:	invoke a callback to get the color of the token field when the token type is marked with @ContextColor

				@AutoCompleteCandidate()
					token:	when the token can be available after the editing caret, than it will be in the auto complete list.
				@AutoCompleteToken()
					token:	when the token is editing, an auto complete list will appear if possible
				@AutoCompleteType(Type)
					field:	invoke a callback to get the auto complete list items when the token type is marked with @AutoCompleteToken
				*/
			};
		}
	}
}

#endif