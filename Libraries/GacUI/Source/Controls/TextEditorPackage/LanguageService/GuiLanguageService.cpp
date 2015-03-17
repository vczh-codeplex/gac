#include "GuiLanguageService.h"
#include "../../../../../../Common/Source/Regex/RegexExpression.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace regex_internal;

/***********************************************************************
ParsingScope
***********************************************************************/

			const ParsingScope::SymbolList ParsingScope::emptySymbolList;

			ParsingScope::ParsingScope(ParsingScopeSymbol* _ownerSymbol)
				:ownerSymbol(_ownerSymbol)
			{
			}

			ParsingScope::~ParsingScope()
			{
			}

			ParsingScopeSymbol* ParsingScope::GetOwnerSymbol()
			{
				return ownerSymbol;
			}

			bool ParsingScope::AddSymbol(Ptr<ParsingScopeSymbol> value)
			{
				if(!value) return false;
				if(value->parentScope) return false;
				symbols.Add(value->GetName(), value);
				value->parentScope=this;
				return true;
			}

			bool ParsingScope::RemoveSymbol(Ptr<ParsingScopeSymbol> value)
			{
				if(!value) return false;
				if(value->parentScope!=this) return false;
				vint index=symbols.Keys().IndexOf(value->GetName());
				if(index==-1) return false;
				const SymbolList& values=symbols.GetByIndex(index);
				index=values.IndexOf(value.Obj());
				if(index==-1) return false;
				symbols.Remove(value->GetName(), value.Obj());
				value->parentScope=0;
				return true;
			}

			const ParsingScope::SymbolKeyList& ParsingScope::GetSymbolNames()
			{
				return symbols.Keys();
			}

			const ParsingScope::SymbolList& ParsingScope::GetSymbols(const WString& name)
			{
				vint index=symbols.Keys().IndexOf(name);
				return index==-1
					?emptySymbolList
					:symbols.GetByIndex(index);
			}

/***********************************************************************
ParsingScopeSymbol
***********************************************************************/

			WString ParsingScopeSymbol::GetDisplayInternal(vint semanticId)
			{
				return name;
			}

			ParsingScopeSymbol::ParsingScopeSymbol(const WString& _name, vint _semanticId)
				:parentScope(0)
				,name(_name)
			{
				if(_semanticId!=-1)
				{
					semanticIds.Add(_semanticId);
				}
			}

			ParsingScopeSymbol::~ParsingScopeSymbol()
			{
			}

			ParsingScope* ParsingScopeSymbol::GetParentScope()
			{
				return parentScope;
			}

			const WString& ParsingScopeSymbol::GetName()
			{
				return name;
			}

			const collections::List<vint>& ParsingScopeSymbol::GetSemanticIds()
			{
				return semanticIds;
			}

			bool ParsingScopeSymbol::AddSemanticId(vint semanticId)
			{
				if(semanticId==-1 || semanticIds.Contains(semanticId)) return false;
				semanticIds.Add(semanticId);
				return true;
			}

			Ptr<parsing::ParsingTreeObject> ParsingScopeSymbol::GetNode()
			{
				return node;
			}

			void ParsingScopeSymbol::SetNode(Ptr<parsing::ParsingTreeObject> value)
			{
				node=value;
			}

			bool ParsingScopeSymbol::CreateScope()
			{
				if(scope) return false;
				scope=new ParsingScope(this);
				return true;
			}

			bool ParsingScopeSymbol::DestroyScope()
			{
				if(!scope) return false;
				scope=0;
				return true;
			}

			ParsingScope* ParsingScopeSymbol::GetScope()
			{
				return scope.Obj();
			}

			WString ParsingScopeSymbol::GetDisplay(vint semanticId)
			{
				return semanticIds.Contains(semanticId)?GetDisplayInternal(semanticId):L"";
			}

/***********************************************************************
ParsingScopeFinder::DirectSymbolMapper
***********************************************************************/

			ParsingScopeFinder::DirectSymbolMapper::DirectSymbolMapper()
			{
			}

			ParsingScopeFinder::DirectSymbolMapper::~DirectSymbolMapper()
			{
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::DirectSymbolMapper::ParentNode(parsing::ParsingTreeNode* node)
			{
				return node->GetParent();
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::DirectSymbolMapper::Node(parsing::ParsingTreeNode* node)
			{
				return node;
			}

			ParsingScope* ParsingScopeFinder::DirectSymbolMapper::ParentScope(ParsingScopeSymbol* symbol)
			{
				return symbol->GetParentScope();
			}

			ParsingScopeSymbol* ParsingScopeFinder::DirectSymbolMapper::Symbol(ParsingScopeSymbol* symbol)
			{
				return symbol;
			}

/***********************************************************************
ParsingScopeFinder::IndirectSymbolMapper
***********************************************************************/

			ParsingScopeFinder::IndirectSymbolMapper::IndirectSymbolMapper(ParsingScopeSymbol* _originalSymbol, ParsingScopeSymbol* _replacedSymbol, parsing::ParsingTreeNode* _originalNode, parsing::ParsingTreeNode* _replacedNode)
				:originalSymbol(_originalSymbol)
				,replacedSymbol(_replacedSymbol)
				,originalNode(_originalNode)
				,replacedNode(_replacedNode)
			{
			}

			ParsingScopeFinder::IndirectSymbolMapper::~IndirectSymbolMapper()
			{
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::IndirectSymbolMapper::ParentNode(parsing::ParsingTreeNode* node)
			{
				return (node==replacedNode?originalNode:node)->GetParent();
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::IndirectSymbolMapper::Node(parsing::ParsingTreeNode* node)
			{
				return node==originalNode?replacedNode:node;
			}

			ParsingScope* ParsingScopeFinder::IndirectSymbolMapper::ParentScope(ParsingScopeSymbol* symbol)
			{
				return (symbol==replacedSymbol?originalSymbol:symbol)->GetParentScope();
			}

			ParsingScopeSymbol* ParsingScopeFinder::IndirectSymbolMapper::Symbol(ParsingScopeSymbol* symbol)
			{
				return symbol==originalSymbol?replacedSymbol:symbol;
			}

/***********************************************************************
ParsingScopeFinder::Traversal Functions
***********************************************************************/

			parsing::ParsingTreeNode* ParsingScopeFinder::ParentNode(parsing::ParsingTreeNode* node)
			{
				return symbolMapper->ParentNode(node);
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::ParentNode(Ptr<parsing::ParsingTreeNode> node)
			{
				return symbolMapper->ParentNode(node.Obj());
			}

			parsing::ParsingTreeNode* ParsingScopeFinder::Node(parsing::ParsingTreeNode* node)
			{
				return symbolMapper->Node(node);
			}

			Ptr<parsing::ParsingTreeNode> ParsingScopeFinder::Node(Ptr<parsing::ParsingTreeNode> node)
			{
				return symbolMapper->Node(node.Obj());
			}

			ParsingScope* ParsingScopeFinder::ParentScope(ParsingScopeSymbol* symbol)
			{
				return symbolMapper->ParentScope(symbol);
			}

			ParsingScope* ParsingScopeFinder::ParentScope(Ptr<ParsingScopeSymbol> symbol)
			{
				return symbolMapper->ParentScope(symbol.Obj());
			}

			ParsingScopeSymbol* ParsingScopeFinder::Symbol(ParsingScopeSymbol* symbol)
			{
				return symbolMapper->Symbol(symbol);
			}

			Ptr<ParsingScopeSymbol> ParsingScopeFinder::Symbol(Ptr<ParsingScopeSymbol> symbol)
			{
				return symbolMapper->Symbol(symbol.Obj());
			}

			ParsingScopeFinder::LazySymbolList ParsingScopeFinder::Symbols(const ParsingScope::SymbolList& symbols)
			{
				return From(symbols).Select([this](Ptr<ParsingScopeSymbol> symbol)
				{
					return Symbol(symbol);
				});
			}

/***********************************************************************
ParsingScopeFinder
***********************************************************************/

			void ParsingScopeFinder::InitializeQueryCacheInternal(ParsingScopeSymbol* symbol)
			{
				if(ParsingTreeObject* obj=Obj(symbol->GetNode().Obj()))
				{
					nodeSymbols.Add(obj, symbol);
				}
				if(symbol->GetScope())
				{
					ParsingScope* scope=symbol->GetScope();
					FOREACH(WString, name, scope->GetSymbolNames())
					{
						FOREACH(Ptr<ParsingScopeSymbol>, subSymbol, Symbols(scope->GetSymbols(name)))
						{
							InitializeQueryCacheInternal(subSymbol.Obj());
						}
					}
				}
			}

			ParsingScopeFinder::ParsingScopeFinder( Ptr<SymbolMapper> _symbolMapper)
				:symbolMapper(_symbolMapper)
				,previousFinder(0)
			{
			}

			ParsingScopeFinder::~ParsingScopeFinder()
			{
			}

			void ParsingScopeFinder::InitializeQueryCache(ParsingScopeSymbol* symbol, ParsingScopeFinder* _previousFinder)
			{
				previousFinder=_previousFinder;
				InitializeQueryCacheInternal(symbol);
			}

			ParsingScopeSymbol* ParsingScopeFinder::GetSymbolFromNode(parsing::ParsingTreeObject* node)
			{
				vint index=nodeSymbols.Keys().IndexOf(node);
				if(index!=-1)
				{
					return nodeSymbols.Values()[index];
				}
				else if(previousFinder)
				{
					return previousFinder->GetSymbolFromNode(node);
				}
				else
				{
					return 0;
				}
			}

			ParsingScope* ParsingScopeFinder::GetScopeFromNode(parsing::ParsingTreeNode* node)
			{
				while(node)
				{
					ParsingTreeObject* obj=dynamic_cast<ParsingTreeObject*>(node);
					if(obj)
					{
						ParsingScopeSymbol* symbol=GetSymbolFromNode(obj);
						if(symbol && symbol->GetScope())
						{
							return symbol->GetScope();
						}
					}
					node=ParentNode(node);
				}
				if(previousFinder)
				{
					return previousFinder->GetScopeFromNode(node);
				}
				else
				{
					return 0;
				}
			}

			ParsingScopeFinder::LazySymbolList ParsingScopeFinder::GetSymbols(ParsingScope* scope, const WString& name)
			{
				if(!scope) return LazySymbolList();
				return Symbols(scope->GetSymbols(name));
			}

			ParsingScopeFinder::LazySymbolList ParsingScopeFinder::GetSymbols(ParsingScope* scope)
			{
				if(!scope) return LazySymbolList();
				return From(scope->GetSymbolNames())
					.SelectMany([=](const WString& name)
					{
						return Symbols(scope->GetSymbols(name));
					});
			}

			ParsingScopeFinder::LazySymbolList ParsingScopeFinder::GetSymbolsRecursively(ParsingScope* scope, const WString& name)
			{
				if(!scope) return LazySymbolList();
				while(scope)
				{
					const ParsingScope::SymbolList& symbols=scope->GetSymbols(name);
					if(symbols.Count()>0) return Symbols(symbols);

					if(scope->ownerSymbol)
					{
						scope=ParentScope(scope->ownerSymbol);
					}
					else
					{
						break;
					}
				}
				if(previousFinder)
				{
					return previousFinder->GetSymbols(scope, name);
				}
				else
				{
					return ParsingScope::emptySymbolList;
				}
			}

			ParsingScopeFinder::LazySymbolList ParsingScopeFinder::GetSymbolsRecursively(ParsingScope* scope)
			{
				if(!scope) return LazySymbolList();
				LazySymbolList result;
				while(scope)
				{
					result=result.Concat(GetSymbols(scope));

					if(scope->ownerSymbol)
					{
						scope=ParentScope(scope->ownerSymbol);
					}
					else
					{
						break;
					}
				}
				return result;
			}

/***********************************************************************
ParsingAnalyzer
***********************************************************************/

			ParsingAnalyzer::ParsingAnalyzer(Ptr<ILanguageProvider> _languageProvider)
				:languageProvider(_languageProvider)
			{
			}

			ParsingAnalyzer::~ParsingAnalyzer()
			{
			}

			void ParsingAnalyzer::Attach(RepeatingParsingExecutor* _executor)
			{
				executor = _executor;
			}

			void ParsingAnalyzer::Detach(RepeatingParsingExecutor* _executor)
			{
				executor = nullptr;
			}

			Ptr<Object> ParsingAnalyzer::CreateCache(const RepeatingParsingOutput& output)
			{
				auto cache = MakePtr<Cache>();
				cache->finder=new ParsingScopeFinder();
				cache->symbol=languageProvider->CreateSymbolFromNode(output.node, executor, cache->finder.Obj());
				cache->finder->InitializeQueryCache(cache->symbol.Obj());
				return cache;
			}

			vint ParsingAnalyzer::GetSemanticIdForToken(const ParsingContext& tokenContext, const RepeatingParsingOutput& output)
			{
				auto cache = output.cache.Cast<Cache>();
				vint semanticId = -1;
				if (Ptr<ParsingScopeSymbol> symbol = languageProvider->FindReferencedSymbols(tokenContext.tokenParent, cache->finder.Obj()).First(0))
				{
					semanticId = From(symbol->GetSemanticIds())
						.Intersect(*tokenContext.acceptableSemanticIds.Obj())
						.First(-1);
				}
				return semanticId;
			}

			void ParsingAnalyzer::GetCandidateItems(const ParsingContext& tokenContext, const RepeatingPartialParsingOutput& partialOutput, collections::List<ParsingCandidateItem>& candidateItems)
			{
				auto cache = partialOutput.input.cache.Cast<Cache>();
				ParsingTreeObject* originalNode=partialOutput.originalNode.Obj();
				ParsingTreeObject* replacedNode=partialOutput.modifiedNode.Obj();
				ParsingScopeSymbol* originalSymbol=0;
				{
					ParsingTreeNode* originalSymbolNode=originalNode;
					while(originalSymbolNode && !originalSymbol)
					{
						originalSymbol=cache->finder->GetSymbolFromNode(dynamic_cast<ParsingTreeObject*>(originalSymbolNode));
						originalSymbolNode=originalSymbolNode->GetParent();
					}
				}
				if(originalSymbol)
				{
					Ptr<ParsingScopeFinder> newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(0, 0, originalNode, replacedNode));
					Ptr<ParsingScopeSymbol> replacedSymbol=languageProvider->CreateSymbolFromNode(newFinder->Obj(originalSymbol->GetNode()), executor, newFinder.Obj());
				
					if(replacedSymbol)
					{
						newFinder=new ParsingScopeFinder(new ParsingScopeFinder::IndirectSymbolMapper(originalSymbol, replacedSymbol.Obj(), originalNode, replacedNode));
						newFinder->InitializeQueryCache(replacedSymbol.Obj(), cache->finder.Obj());
						LazyList<Ptr<ParsingScopeSymbol>> symbols=languageProvider->FindPossibleSymbols(tokenContext.tokenParent, tokenContext.field, newFinder.Obj());
						CopyFrom(
							candidateItems,
							From(symbols)
								.Where([&tokenContext](Ptr<ParsingScopeSymbol> symbol)
								{
									return From(symbol->GetSemanticIds())
										.Intersect(*tokenContext.acceptableSemanticIds.Obj())
										.First(-1) != -1;
								})
								.SelectMany([](Ptr<ParsingScopeSymbol> symbol)
								{
									return From(symbol->GetSemanticIds())
										.Select([symbol](vint semanticId)
										{
											ParsingCandidateItem item;
											item.semanticId = semanticId;
											item.name = symbol->GetDisplay(semanticId);
											return item;
										});
								})
							);
					}
				}
			}
		}
	}
}