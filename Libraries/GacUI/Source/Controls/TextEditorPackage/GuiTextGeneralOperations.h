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
				vint									editVersion;
				WString									code;

				RepeatingParsingInput()
					:editVersion(0)
				{
				}
			};

			/// <summary>A data structure storing the parsing result for text box control.</summary>
			struct RepeatingParsingOutput
			{
				Ptr<parsing::ParsingTreeObject>			node;
				vint									editVersion;
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
				};
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				collections::List<ICallback*>								callbacks;

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

				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAttribute(vint index, const WString& name, vint argumentCount);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetContextColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetSemanticColorAttribute(vint index);
			};
		}
	}
}

#endif