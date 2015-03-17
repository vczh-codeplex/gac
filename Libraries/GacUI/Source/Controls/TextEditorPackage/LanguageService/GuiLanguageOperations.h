/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILANGUAGEOPERATIONS
#define VCZH_PRESENTATION_CONTROLS_GUILANGUAGEOPERATIONS

#include "../EditorCallback/GuiTextGeneralOperations.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
ParsingInput
***********************************************************************/

			class RepeatingParsingExecutor;

			/// <summary>A data structure storing the parsing input for text box control.</summary>
			struct RepeatingParsingInput
			{
				/// <summary>The text box edit version of the code.</summary>
				vuint													editVersion = 0;
				/// <summary>The code.</summary>
				WString													code;
			};

/***********************************************************************
ParsingOutput
***********************************************************************/

			/// <summary>A data structure storing the parsing result for text box control.</summary>
			struct RepeatingParsingOutput
			{
				/// <summary>The parsed syntax tree.</summary>
				Ptr<parsing::ParsingTreeObject>							node;
				/// <summary>The text box edit version of the code.</summary>
				vuint													editVersion = 0;
				/// <summary>The code.</summary>
				WString													code;
				/// <summary>The cache created from [T:vl.presentation.controls.RepeatingParsingExecutor.IParsingAnalyzer].</summary>
				Ptr<Object>												cache;
			};

/***********************************************************************
PartialParsingOutput
***********************************************************************/
			
			/// <summary>A data structure storing the parsing result for partial updating when a text box control is modified.</summary>
			struct RepeatingPartialParsingOutput
			{
				/// <summary>The input data.</summary>
				RepeatingParsingOutput									input;
				/// <summary>The rule name that can parse the code of the selected context.</summary>
				WString													rule;
				/// <summary>Range of the original context in the input.</summary>
				parsing::ParsingTextRange								originalRange;
				/// <summary>The original context in the syntax tree.</summary>
				Ptr<parsing::ParsingTreeObject>							originalNode;
				/// <summary>The modified context in the syntax tree.</summary>
				Ptr<parsing::ParsingTreeObject>							modifiedNode;
				/// <summary>The modified code of the selected context.</summary>
				WString													modifiedCode;
			};

/***********************************************************************
PartialParsingOutput
***********************************************************************/

			/// <summary>A data structure storing the information for a candidate item.</summary>
			struct ParsingCandidateItem
			{
				/// <summary>Semantic id.</summary>
				vint													semanticId = -1;
				/// <summary>Display name.</summary>
				WString													name;
			};

/***********************************************************************
ParsingContext
***********************************************************************/

			/// <summary>A data structure storing the context of a token.</summary>
			struct ParsingTokenContext
			{
				/// <summary>Token syntax tree for the selected token.</summary>
				parsing::ParsingTreeToken*								foundToken = nullptr;
				/// <summary>The object syntax tree parent of the token.</summary>
				parsing::ParsingTreeObject*								tokenParent = nullptr;
				/// <summary>Type of the parent.</summary>
				WString													type;
				/// <summary>Field of the parent that contains the token.</summary>
				WString													field;
				/// <summary>All acceptable semantic ids.</summary>
				Ptr<collections::List<vint>>							acceptableSemanticIds;

				static bool												RetriveContext(ParsingTokenContext& output, parsing::ParsingTreeNode* foundNode, RepeatingParsingExecutor* executor);
				static bool												RetriveContext(ParsingTokenContext& output, parsing::ParsingTextPos pos, parsing::ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor);
				static bool												RetriveContext(ParsingTokenContext& output, parsing::ParsingTextRange range, parsing::ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor);
			};

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			/// <summary>Repeating parsing executor.</summary>
			class RepeatingParsingExecutor : public RepeatingTaskExecutor<RepeatingParsingInput>, public Description<RepeatingParsingExecutor>
			{
			public:
				/// <summary>Callback.</summary>
				class ICallback : public virtual Interface
				{
				public:
					/// <summary>Callback when a parsing task is finished.</summary>
					/// <param name="output">the result of the parsing.</param>
					virtual void											OnParsingFinishedAsync(const RepeatingParsingOutput& output)=0;
					/// <summary>Callback when <see cref="RepeatingParsingExecutor"/> requires enabling or disabling automatically repeating calling to the SubmitTask function.</summary>
					/// <param name="enabled">Set to true to require an automatically repeating calling to the SubmitTask function</param>
					virtual void											RequireAutoSubmitTask(bool enabled)=0;
				};

				/// <summary>Parsing analyzer.</summary>
				class IParsingAnalyzer : public virtual Interface
				{
				public:
					/// <summary>Called when a <see cref="RepeatingParsingExecutor"/> is created.</summary>
					/// <param name="executor">The releated <see cref="RepeatingParsingExecutor"/>.</param>
					virtual void											Attach(RepeatingParsingExecutor* executor) = 0;

					/// <summary>Called when a <see cref="RepeatingParsingExecutor"/> is destroyed.</summary>
					/// <param name="executor">The releated <see cref="RepeatingParsingExecutor"/>.</param>
					virtual void											Detach(RepeatingParsingExecutor* executor) = 0;

					/// <summary>Called when a new parsing result is produced. A parsing analyzer can create a cache to be attached to the output containing anything necessary.</summary>
					/// <param name="output">The new parsing result.</param>
					/// <returns>The created cache object, which can be null.</returns>
					virtual Ptr<Object>										CreateCache(const RepeatingParsingOutput& output) = 0;

					/// <summary>Called when an semantic id for a token is needed. If an semantic id is returned, a context sensitive color can be assigned to this token.</summary>
					/// <param name="tokenContext">The token context.</param>
					/// <param name="output">The current parsing result.</param>
					/// <returns>The semantic id.</returns>
					virtual vint											GetSemanticIdForToken(const ParsingTokenContext& tokenContext, const RepeatingParsingOutput& output) = 0;

					virtual void											GetCandidateItems(const ParsingTokenContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems) = 0;
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
					void													Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, compositions::GuiGraphicsComposition* _ownerComposition, vuint editVersion)override;
					void													Detach()override;
					void													TextEditPreview(TextEditPreviewStruct& arguments)override;
					void													TextEditNotify(const TextEditNotifyStruct& arguments)override;
					void													TextCaretChanged(const TextCaretChangedStruct& arguments)override;
					void													TextEditFinished(vuint editVersion)override;
				};

				struct TokenMetaData
				{
					vint													tableTokenIndex;
					vint													lexerTokenIndex;
					vint													defaultColorIndex;
					bool													hasContextColor;
					bool													hasAutoComplete;
					bool													isCandidate;
					WString													unescapedRegexText;
				};

				struct FieldMetaData
				{
					vint													colorIndex;
					Ptr<collections::List<vint>>							semantics;
				};
			private:
				Ptr<parsing::tabling::ParsingGeneralParser>					grammarParser;
				WString														grammarRule;
				Ptr<IParsingAnalyzer>										analyzer;
				collections::List<ICallback*>								callbacks;
				collections::List<ICallback*>								activatedCallbacks;
				ICallback*													autoPushingCallback;

				typedef collections::Pair<WString, WString>					FieldDesc;
				collections::Dictionary<WString, vint>						tokenIndexMap;
				collections::SortedList<WString>							semanticIndexMap;
				collections::Dictionary<vint, TokenMetaData>				tokenMetaDatas;
				collections::Dictionary<FieldDesc, FieldMetaData>			fieldMetaDatas;

			protected:

				void														Execute(const RepeatingParsingInput& input)override;
				void														PrepareMetaData();

				/// <summary>Called when semantic analyzing is needed. It is encouraged to set the "cache" fields in "context" argument. If there is an <see cref="RepeatingParsingExecutor::IParsingAnalyzer"/> binded to the <see cref="RepeatingParsingExecutor"/>, this function can be automatically done.</summary>
				/// <param name="context">The parsing result.</param>
				virtual void												OnContextFinishedAsync(RepeatingParsingOutput& context);
			public:
				/// <summary>Initialize the parsing executor.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule">The rule name to parse a complete code.</param>
				/// <param name="_analyzer">The parsing analyzer to create semantic metadatas, it can be null.</param>
				RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule, Ptr<IParsingAnalyzer> _analyzer = 0);
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
				/// <summary>Get the parsing analyzer.</summary>
				/// <returns>The parsing analyzer.</returns>
				Ptr<IParsingAnalyzer>										GetAnalyzer();

				vint														GetTokenIndex(const WString& tokenName);
				vint														GetSemanticId(const WString& name);
				WString														GetSemanticName(vint id);
				const TokenMetaData&										GetTokenMetaData(vint regexTokenIndex);
				const FieldMetaData&										GetFieldMetaData(const WString& type, const WString& field);

				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAttribute(vint index, const WString& name, vint argumentCount);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetContextColorAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetSemanticAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetCandidateAttribute(vint index);
				Ptr<parsing::tabling::ParsingTable::AttributeInfo>			GetAutoCompleteAttribute(vint index);

				/*
				@Color(ColorName)
					field:	color of the token field when the token type is marked with @ContextColor
					token:	color of the token
				@ContextColor()
					token:	the color of the token may be changed if the token field is marked with @Color or @Semantic
				@Semantic(Type1, Type2, ...)
					field:	After resolved symbols for this field, only types of symbols that specified in the arguments are acceptable.
				@Candidate()
					token:	when the token can be available after the editing caret, than it will be in the auto complete list.
				@AutoComplete()
					token:	when the token is editing, an auto complete list will appear if possible
				*/
			};
		}
	}
}

#endif