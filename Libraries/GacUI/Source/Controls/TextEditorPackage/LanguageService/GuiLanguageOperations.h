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
Scope
***********************************************************************/

			class ParsingScope;
			class ParsingScopeSymbol;
			class ParsingScopeFinder;

			class ParsingScope : public Object, public reflection::Description<ParsingScope>
			{
				typedef collections::SortedList<WString>							SymbolKeyList;
				typedef collections::List<Ptr<ParsingScopeSymbol>>					SymbolList;
				typedef collections::Group<WString, Ptr<ParsingScopeSymbol>>		SymbolGroup;

				friend class ParsingScopeSymbol;
				friend class ParsingScopeFinder;
			protected:
				static const SymbolList					emptySymbolList;

				ParsingScopeSymbol*						ownerSymbol;
				SymbolGroup								symbols;

			public:
				ParsingScope(ParsingScopeSymbol* _ownerSymbol);
				~ParsingScope();

				ParsingScopeSymbol*						GetOwnerSymbol();
				bool									AddSymbol(Ptr<ParsingScopeSymbol> value);
				bool									RemoveSymbol(Ptr<ParsingScopeSymbol> value);
				const SymbolKeyList&					GetSymbolNames();
				const SymbolList&						GetSymbols(const WString& name);
			};

			class ParsingScopeSymbol : public Object, public reflection::Description<ParsingScopeSymbol>
			{
				friend class ParsingScope;
			protected:
				ParsingScope*							parentScope;
				WString									name;
				collections::List<vint>					semanticIds;
				Ptr<parsing::ParsingTreeObject>			node;
				Ptr<ParsingScope>						scope;

				virtual WString							GetDisplayInternal(vint semanticId);
			public:
				ParsingScopeSymbol(const WString& _name=L"", vint _semanticId=-1);
				~ParsingScopeSymbol();

				ParsingScope*							GetParentScope();
				const WString&							GetName();
				const collections::List<vint>&			GetSemanticIds();
				bool									AddSemanticId(vint semanticId);
				Ptr<parsing::ParsingTreeObject>			GetNode();
				void									SetNode(Ptr<parsing::ParsingTreeObject> value);
				bool									CreateScope();
				bool									DestroyScope();
				ParsingScope*							GetScope();
				WString									GetDisplay(vint semanticId);
			};

			class ParsingScopeFinder : public Object, public reflection::Description<ParsingScopeFinder>
			{
				typedef collections::Dictionary<parsing::ParsingTreeObject*, ParsingScopeSymbol*>	NodeSymbolMap;
				typedef collections::LazyList<Ptr<ParsingScopeSymbol>>								LazySymbolList;
			public:
				class SymbolMapper : public Object, public reflection::Description<SymbolMapper>
				{
				public:
					virtual parsing::ParsingTreeNode*	ParentNode(parsing::ParsingTreeNode* node)=0;
					virtual parsing::ParsingTreeNode*	Node(parsing::ParsingTreeNode* node)=0;
					virtual ParsingScope*				ParentScope(ParsingScopeSymbol* symbol)=0;
					virtual ParsingScopeSymbol*			Symbol(ParsingScopeSymbol* symbol)=0;
				};

				class DirectSymbolMapper : public SymbolMapper, public reflection::Description<DirectSymbolMapper>
				{
				public:
					DirectSymbolMapper();
					~DirectSymbolMapper();

					parsing::ParsingTreeNode*			ParentNode(parsing::ParsingTreeNode* node)override;
					parsing::ParsingTreeNode*			Node(parsing::ParsingTreeNode* node)override;
					ParsingScope*						ParentScope(ParsingScopeSymbol* symbol)override;
					ParsingScopeSymbol*					Symbol(ParsingScopeSymbol* symbol)override;
				};

				class IndirectSymbolMapper  : public SymbolMapper, public reflection::Description<IndirectSymbolMapper>
				{
				protected:
					ParsingScopeSymbol*					originalSymbol;
					ParsingScopeSymbol*					replacedSymbol;
					parsing::ParsingTreeNode*			originalNode;
					parsing::ParsingTreeNode*			replacedNode;
				public:
					IndirectSymbolMapper(ParsingScopeSymbol* _originalSymbol, ParsingScopeSymbol* _replacedSymbol, parsing::ParsingTreeNode* _originalNode, parsing::ParsingTreeNode* _replacedNode);
					~IndirectSymbolMapper();

					parsing::ParsingTreeNode*			ParentNode(parsing::ParsingTreeNode* node)override;
					parsing::ParsingTreeNode*			Node(parsing::ParsingTreeNode* node)override;
					ParsingScope*						ParentScope(ParsingScopeSymbol* symbol)override;
					ParsingScopeSymbol*					Symbol(ParsingScopeSymbol* symbol)override;
				};
			protected:
				NodeSymbolMap							nodeSymbols;
				Ptr<SymbolMapper>						symbolMapper;
				ParsingScopeFinder*						previousFinder;

				void									InitializeQueryCacheInternal(ParsingScopeSymbol* symbol);
			public:
				ParsingScopeFinder(Ptr<SymbolMapper> _symbolMapper=new DirectSymbolMapper);
				~ParsingScopeFinder();

				parsing::ParsingTreeNode*				ParentNode(parsing::ParsingTreeNode* node);
				parsing::ParsingTreeNode*				ParentNode(Ptr<parsing::ParsingTreeNode> node);
				parsing::ParsingTreeNode*				Node(parsing::ParsingTreeNode* node);
				Ptr<parsing::ParsingTreeNode>			Node(Ptr<parsing::ParsingTreeNode> node);
				ParsingScope*							ParentScope(ParsingScopeSymbol* symbol);
				ParsingScope*							ParentScope(Ptr<ParsingScopeSymbol> symbol);
				ParsingScopeSymbol*						Symbol(ParsingScopeSymbol* symbol);
				Ptr<ParsingScopeSymbol>					Symbol(Ptr<ParsingScopeSymbol> symbol);
				LazySymbolList							Symbols(const ParsingScope::SymbolList& symbols);

				template<typename T>
				T* Obj(T* node)
				{
					return dynamic_cast<T*>(Node(node));
				}

				template<typename T>
				Ptr<T> Obj(Ptr<T> node)
				{
					return Node(node).template Cast<T>();
				}
			
				void									InitializeQueryCache(ParsingScopeSymbol* symbol, ParsingScopeFinder* _previousFinder=0);
				ParsingScopeSymbol*						GetSymbolFromNode(parsing::ParsingTreeObject* node);
				ParsingScope*							GetScopeFromNode(parsing::ParsingTreeNode* node);
				LazySymbolList							GetSymbols(ParsingScope* scope, const WString& name);
				LazySymbolList							GetSymbols(ParsingScope* scope);
				LazySymbolList							GetSymbolsRecursively(ParsingScope* scope, const WString& name);
				LazySymbolList							GetSymbolsRecursively(ParsingScope* scope);
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
				/// <summary>The root symbol from semantic analyzing.</summary>
				Ptr<ParsingScopeSymbol>					symbol;
				/// <summary>The finder for the root symbol from semantic analyzing.</summary>
				Ptr<ParsingScopeFinder>					finder;

				RepeatingParsingOutput()
					:editVersion(0)
				{
				}
			};

			class RepeatingParsingExecutor;

			/// <summary>Language semantic metadata provider for text box that editing code.</summary>
			class ILanguageProvider : public IDescriptable, public Description<ILanguageProvider>
			{
			public:
				/// <summary>Create a symbol from a node.</summary>
				/// <param name="obj">The node.</param>
				/// <param name="executor">The executor storing metadatas for a grammar.</param>
				/// <param name="finder">The finder for traversing nodes and symbols.</param>
				/// <returns>The created symbol.</returns>
				virtual Ptr<ParsingScopeSymbol>								CreateSymbolFromNode(Ptr<parsing::ParsingTreeObject> obj, RepeatingParsingExecutor* executor, ParsingScopeFinder* finder)=0;
				
				/// <summary>Get all referenced symbols (in most cases, one) for a node.</summary>
				/// <param name="obj">The node.</param>
				/// <param name="finder">The finder for traversing nodes and symbols.</param>
				/// <returns>All referenced symbols.</returns>
				virtual collections::LazyList<Ptr<ParsingScopeSymbol>>		FindReferencedSymbols(parsing::ParsingTreeObject* obj, ParsingScopeFinder* finder)=0;
				
				/// <summary>Get all possible symbols for a specified field of a node.</summary>
				/// <param name="obj">The node.</param>
				/// <param name="field">The field name.</param>
				/// <param name="finder">The finder for traversing nodes and symbols.</param>
				/// <returns>All possible symbols.</returns>
				virtual collections::LazyList<Ptr<ParsingScopeSymbol>>		FindPossibleSymbols(parsing::ParsingTreeObject* obj, const WString& field, ParsingScopeFinder* finder)=0;
			};

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
				Ptr<ILanguageProvider>										languageProvider;
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

				/// <summary>Called when semantic analyzing is needed. It is encouraged to set the "symbol" and "finder" fields in "context" argument.</summary>
				/// <param name="context">The parsing result.</param>
				virtual void												OnContextFinishedAsync(RepeatingParsingOutput& context);
			public:
				/// <summary>Initialize the parsing executor.</summary>
				/// <param name="_grammarParser">Parser generated from a grammar.</param>
				/// <param name="_grammarRule">The rule name to parse a complete code.</param>
				/// <param name="_languageProvider">The language provider to create semantic metadats, it can be null.</param>
				RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule, Ptr<ILanguageProvider> _languageProvider=0);
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
				/// <summary>Get the language provider.</summary>
				/// <returns>The language provider.</returns>
				Ptr<ILanguageProvider>										GetLanguageProvider();

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

/***********************************************************************
ParsingContext
***********************************************************************/

			struct ParsingContext
			{
				/// <summary>Token syntax tree for the selected token.</summary>
				parsing::ParsingTreeToken*								foundToken;
				/// <summary>The object syntax tree parent of the token.</summary>
				parsing::ParsingTreeObject*								tokenParent;
				/// <summary>Type of the parent.</summary>
				WString													type;
				/// <summary>Field of the parent that contains the token.</summary>
				WString													field;
				/// <summary>All acceptable semantic ids.</summary>
				Ptr<collections::List<vint>>							acceptableSemanticIds;
				
				ParsingContext()
					:foundToken(0)
					,tokenParent(0)
				{
				}

				static bool												RetriveContext(ParsingContext& output, parsing::ParsingTreeNode* foundNode, RepeatingParsingExecutor* executor);
				static bool												RetriveContext(ParsingContext& output, parsing::ParsingTextPos pos, parsing::ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor);
				static bool												RetriveContext(ParsingContext& output, parsing::ParsingTextRange range, parsing::ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor);
			};
		}
	}
}

#endif