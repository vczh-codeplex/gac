/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILANGUAGESERVICE
#define VCZH_PRESENTATION_CONTROLS_GUILANGUAGESERVICE

#include "GuiLanguageOperations.h"

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
LanguageProvider
***********************************************************************/

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

/***********************************************************************
ParsingAnalyzer
***********************************************************************/

			class ParsingAnalyzer : public Object, public virtual RepeatingParsingExecutor::IParsingAnalyzer
			{
			protected:
				class Cache : public Object
				{
				public:
					Ptr<ParsingScopeSymbol>					symbol;
					Ptr<ParsingScopeFinder>					finder;
				};

				Ptr<ILanguageProvider>						languageProvider;
				RepeatingParsingExecutor*					executor = nullptr;

			public:
				ParsingAnalyzer(Ptr<ILanguageProvider> _languageProvider);
				~ParsingAnalyzer();

				void										Attach(RepeatingParsingExecutor* _executor)override;
				void										Detach(RepeatingParsingExecutor* _executor)override;
				Ptr<Object>									CreateCache(const RepeatingParsingOutput& output)override;
				vint										GetSemanticIdForToken(const ParsingTokenContext& tokenContext, const RepeatingParsingOutput& output)override;
				void										GetCandidateItems(const ParsingTokenContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems)override;
			};
		}
	}
}

#endif