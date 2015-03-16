#include "GuiLanguageOperations.h"
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
RepeatingParsingExecutor::CallbackBase
***********************************************************************/

			RepeatingParsingExecutor::CallbackBase::CallbackBase(Ptr<RepeatingParsingExecutor> _parsingExecutor)
				:parsingExecutor(_parsingExecutor)
				,callbackAutoPushing(false)
				,callbackElement(0)
				,callbackElementModifyLock(0)
			{
			}

			RepeatingParsingExecutor::CallbackBase::~CallbackBase()
			{
			}

			void RepeatingParsingExecutor::CallbackBase::RequireAutoSubmitTask(bool enabled)
			{
				callbackAutoPushing=enabled;
			}

			void RepeatingParsingExecutor::CallbackBase::Attach(elements::GuiColorizedTextElement* _element, SpinLock& _elementModifyLock, compositions::GuiGraphicsComposition* _ownerComposition, vuint editVersion)
			{
				if(_element)
				{
					SPIN_LOCK(_elementModifyLock)
					{
						callbackElement=_element;
						callbackElementModifyLock=&_elementModifyLock;
					}
				}
				
				parsingExecutor->ActivateCallback(this);
				if(callbackElement && callbackElementModifyLock && callbackAutoPushing)
				{
					SPIN_LOCK(*callbackElementModifyLock)
					{
						RepeatingParsingInput input;
						input.editVersion=editVersion;
						input.code=callbackElement->GetLines().GetText();
						parsingExecutor->SubmitTask(input);
					}
				}
			}

			void RepeatingParsingExecutor::CallbackBase::Detach()
			{
				if(callbackElement && callbackElementModifyLock)
				{
					SPIN_LOCK(*callbackElementModifyLock)
					{
						callbackElement=0;
						callbackElementModifyLock=0;
					}
				}
				
				parsingExecutor->DeactivateCallback(this);
			}

			void RepeatingParsingExecutor::CallbackBase::TextEditPreview(TextEditPreviewStruct& arguments)
			{
			}

			void RepeatingParsingExecutor::CallbackBase::TextEditNotify(const TextEditNotifyStruct& arguments)
			{
			}

			void RepeatingParsingExecutor::CallbackBase::TextCaretChanged(const TextCaretChangedStruct& arguments)
			{
			}

			void RepeatingParsingExecutor::CallbackBase::TextEditFinished(vuint editVersion)
			{
				if(callbackElement && callbackElementModifyLock && callbackAutoPushing)
				{
					SPIN_LOCK(*callbackElementModifyLock)
					{
						RepeatingParsingInput input;
						input.editVersion=editVersion;
						input.code=callbackElement->GetLines().GetText();
						parsingExecutor->SubmitTask(input);
					}
				}
			}

/***********************************************************************
RepeatingParsingExecutor
***********************************************************************/

			void RepeatingParsingExecutor::Execute(const RepeatingParsingInput& input)
			{
				List<Ptr<ParsingError>> errors;
				Ptr<ParsingTreeObject> node=grammarParser->Parse(input.code, grammarRule, errors).Cast<ParsingTreeObject>();
				if(node)
				{
					node->InitializeQueryCache();
				}

				RepeatingParsingOutput result;
				result.node=node;
				result.editVersion=input.editVersion;
				result.code=input.code;
				if(node)
				{
					OnContextFinishedAsync(result);
					FOREACH(ICallback*, callback, callbacks)
					{
						callback->OnParsingFinishedAsync(result);
					}
				}
			}

			void RepeatingParsingExecutor::PrepareMetaData()
			{
				Ptr<ParsingTable> table=grammarParser->GetTable();
				tokenIndexMap.Clear();
				semanticIndexMap.Clear();
				tokenMetaDatas.Clear();
				fieldMetaDatas.Clear();

				Dictionary<vint, Ptr<ParsingTable::AttributeInfo>> tokenColorAtts, tokenContextColorAtts, tokenCandidateAtts, tokenAutoCompleteAtts;
				Dictionary<FieldDesc, Ptr<ParsingTable::AttributeInfo>> fieldColorAtts, fieldSemanticAtts;

				{
					vint tokenCount=table->GetTokenCount();
					for(vint token=ParsingTable::UserTokenStart;token<tokenCount;token++)
					{
						const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(token);
						vint tokenIndex=token-ParsingTable::UserTokenStart;
						tokenIndexMap.Add(tokenInfo.name, tokenIndex);

						if(Ptr<ParsingTable::AttributeInfo> att=GetColorAttribute(tokenInfo.attributeIndex))
						{
							tokenColorAtts.Add(tokenIndex, att);
						}
						if(Ptr<ParsingTable::AttributeInfo> att=GetContextColorAttribute(tokenInfo.attributeIndex))
						{
							tokenContextColorAtts.Add(tokenIndex, att);
						}
						if(Ptr<ParsingTable::AttributeInfo> att=GetCandidateAttribute(tokenInfo.attributeIndex))
						{
							tokenCandidateAtts.Add(tokenIndex, att);
						}
						if(Ptr<ParsingTable::AttributeInfo> att=GetAutoCompleteAttribute(tokenInfo.attributeIndex))
						{
							tokenAutoCompleteAtts.Add(tokenIndex, att);
						}
					}
				}
				{
					vint fieldCount=table->GetTreeFieldInfoCount();
					for(vint field=0;field<fieldCount;field++)
					{
						const ParsingTable::TreeFieldInfo& fieldInfo=table->GetTreeFieldInfo(field);
						FieldDesc fieldDesc(fieldInfo.type, fieldInfo.field);

						if(Ptr<ParsingTable::AttributeInfo> att=GetColorAttribute(fieldInfo.attributeIndex))
						{
							fieldColorAtts.Add(fieldDesc, att);
						}
						if(Ptr<ParsingTable::AttributeInfo> att=GetSemanticAttribute(fieldInfo.attributeIndex))
						{
							fieldSemanticAtts.Add(fieldDesc, att);
						}
					}
				}

				FOREACH(Ptr<ParsingTable::AttributeInfo>, att, 
					From(tokenColorAtts.Values())
						.Concat(tokenContextColorAtts.Values())
						.Concat(fieldColorAtts.Values())
						.Concat(fieldSemanticAtts.Values())
					)
				{
					FOREACH(WString, argument, att->arguments)
					{
						if(!semanticIndexMap.Contains(argument))
						{
							semanticIndexMap.Add(argument);
						}
					}
				}

				vint index=0;
				FOREACH(vint, tokenIndex, tokenIndexMap.Values())
				{
					TokenMetaData md;
					md.tableTokenIndex=tokenIndex+ParsingTable::UserTokenStart;
					md.lexerTokenIndex=tokenIndex;
					md.defaultColorIndex=-1;
					md.hasContextColor=false;
					md.hasAutoComplete=false;
					md.isCandidate=false;

					if((index=tokenColorAtts.Keys().IndexOf(tokenIndex))!=-1)
					{
						md.defaultColorIndex=semanticIndexMap.IndexOf(tokenColorAtts.Values()[index]->arguments[0]);
					}
					md.hasContextColor=tokenContextColorAtts.Keys().Contains(tokenIndex);
					md.hasAutoComplete=tokenAutoCompleteAtts.Keys().Contains(tokenIndex);
					if((md.isCandidate=tokenCandidateAtts.Keys().Contains(tokenIndex)))
					{
						const ParsingTable::TokenInfo& tokenInfo=table->GetTokenInfo(md.tableTokenIndex);
						if(IsRegexEscapedListeralString(tokenInfo.regex))
						{
							md.unescapedRegexText=UnescapeTextForRegex(tokenInfo.regex);
						}
						else
						{
							md.isCandidate=false;
						}
					}

					tokenMetaDatas.Add(tokenIndex, md);
				}
				{
					vint fieldCount=table->GetTreeFieldInfoCount();
					for(vint field=0;field<fieldCount;field++)
					{
						const ParsingTable::TreeFieldInfo& fieldInfo=table->GetTreeFieldInfo(field);
						FieldDesc fieldDesc(fieldInfo.type, fieldInfo.field);

						FieldMetaData md;
						md.colorIndex=-1;

						if((index=fieldColorAtts.Keys().IndexOf(fieldDesc))!=-1)
						{
							md.colorIndex=semanticIndexMap.IndexOf(fieldColorAtts.Values()[index]->arguments[0]);
						}
						if((index=fieldSemanticAtts.Keys().IndexOf(fieldDesc))!=-1)
						{
							md.semantics=new List<vint>;
							FOREACH(WString, argument, fieldSemanticAtts.Values()[index]->arguments)
							{
								md.semantics->Add(semanticIndexMap.IndexOf(argument));
							}
						}

						fieldMetaDatas.Add(fieldDesc, md);
					}
				}
			}

			void RepeatingParsingExecutor::OnContextFinishedAsync(RepeatingParsingOutput& context)
			{
				if(languageProvider)
				{
					context.finder=new ParsingScopeFinder();
					context.symbol=languageProvider->CreateSymbolFromNode(context.node, this, context.finder.Obj());
					context.finder->InitializeQueryCache(context.symbol.Obj());
				}
			}

			RepeatingParsingExecutor::RepeatingParsingExecutor(Ptr<parsing::tabling::ParsingGeneralParser> _grammarParser, const WString& _grammarRule, Ptr<ILanguageProvider> _languageProvider)
				:grammarParser(_grammarParser)
				,grammarRule(_grammarRule)
				,languageProvider(_languageProvider)
				,autoPushingCallback(0)
			{
				PrepareMetaData();
			}

			RepeatingParsingExecutor::~RepeatingParsingExecutor()
			{
				EnsureTaskFinished();
			}

			Ptr<parsing::tabling::ParsingGeneralParser> RepeatingParsingExecutor::GetParser()
			{
				return grammarParser;
			}

			bool RepeatingParsingExecutor::AttachCallback(ICallback* value)
			{
				if(!value) return false;
				if(callbacks.Contains(value)) return false;
				callbacks.Add(value);
				return true;
			}

			bool RepeatingParsingExecutor::DetachCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				DeactivateCallback(value);
				callbacks.Remove(value);
				return true;
			}

			bool RepeatingParsingExecutor::ActivateCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				if(activatedCallbacks.Contains(value)) return false;
				activatedCallbacks.Add(value);

				if(!autoPushingCallback)
				{
					autoPushingCallback=value;
					autoPushingCallback->RequireAutoSubmitTask(true);
				}
				return true;
			}

			bool RepeatingParsingExecutor::DeactivateCallback(ICallback* value)
			{
				if(!value) return false;
				if(!callbacks.Contains(value)) return false;
				if(!activatedCallbacks.Contains(value)) return false;

				if(autoPushingCallback==value)
				{
					autoPushingCallback->RequireAutoSubmitTask(false);
					autoPushingCallback=0;
				}
				activatedCallbacks.Remove(value);
				if(!autoPushingCallback && activatedCallbacks.Count()>0)
				{
					autoPushingCallback=activatedCallbacks[0];
					autoPushingCallback->RequireAutoSubmitTask(true);
				}
				return true;
			}

			Ptr<ILanguageProvider> RepeatingParsingExecutor::GetLanguageProvider()
			{
				return languageProvider;
			}

			vint RepeatingParsingExecutor::GetTokenIndex(const WString& tokenName)
			{
				vint index=tokenIndexMap.Keys().IndexOf(tokenName);
				return index==-1?-1:tokenIndexMap.Values()[index];
			}

			vint RepeatingParsingExecutor::GetSemanticId(const WString& name)
			{
				return semanticIndexMap.IndexOf(name);
			}

			WString RepeatingParsingExecutor::GetSemanticName(vint id)
			{
				return 0<=id&&id<semanticIndexMap.Count()?semanticIndexMap[id]:L"";
			}

			const RepeatingParsingExecutor::TokenMetaData& RepeatingParsingExecutor::GetTokenMetaData(vint regexTokenIndex)
			{
				return tokenMetaDatas[regexTokenIndex];
			}

			const RepeatingParsingExecutor::FieldMetaData& RepeatingParsingExecutor::GetFieldMetaData(const WString& type, const WString& field)
			{
				return fieldMetaDatas[FieldDesc(type, field)];
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetAttribute(vint index, const WString& name, vint argumentCount)
			{
				if(index!=-1)
				{
					Ptr<ParsingTable::AttributeInfo> att=grammarParser->GetTable()->GetAttributeInfo(index)->FindFirst(name);
					if(att && (argumentCount==-1 || att->arguments.Count()==argumentCount))
					{
						return att;
					}
				}
				return 0;
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetColorAttribute(vint index)
			{
				return GetAttribute(index, L"Color", 1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetContextColorAttribute(vint index)
			{
				return GetAttribute(index, L"ContextColor", 0);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetSemanticAttribute(vint index)
			{
				return GetAttribute(index, L"Semantic", -1);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetCandidateAttribute(vint index)
			{
				return GetAttribute(index, L"Candidate", 0);
			}

			Ptr<parsing::tabling::ParsingTable::AttributeInfo> RepeatingParsingExecutor::GetAutoCompleteAttribute(vint index)
			{
				return GetAttribute(index, L"AutoComplete", 0);
			}

/***********************************************************************
ParsingContext
***********************************************************************/

			bool ParsingContext::RetriveContext(ParsingContext& output, parsing::ParsingTreeNode* foundNode, RepeatingParsingExecutor* executor)
			{
				ParsingTreeToken* foundToken=dynamic_cast<ParsingTreeToken*>(foundNode);
				if(!foundToken) return false;
				ParsingTreeObject* tokenParent=dynamic_cast<ParsingTreeObject*>(foundNode->GetParent());
				if(!tokenParent) return false;
				vint index=tokenParent->GetMembers().Values().IndexOf(foundNode);
				if(index==-1) return false;

				WString type=tokenParent->GetType();
				WString field=tokenParent->GetMembers().Keys().Get(index);
				const RepeatingParsingExecutor::FieldMetaData& md=executor->GetFieldMetaData(type, field);

				output.foundToken=foundToken;
				output.tokenParent=tokenParent;
				output.type=type;
				output.field=field;
				output.acceptableSemanticIds=md.semantics;
				return true;
			}

			bool ParsingContext::RetriveContext(ParsingContext& output, parsing::ParsingTextPos pos, parsing::ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor)
			{
				ParsingTreeNode* foundNode=rootNode->FindDeepestNode(pos);
				if(!foundNode) return false;
				return RetriveContext(output, foundNode, executor);
			}

			bool ParsingContext::RetriveContext(ParsingContext& output, parsing::ParsingTextRange range, ParsingTreeObject* rootNode, RepeatingParsingExecutor* executor)
			{
				ParsingTreeNode* foundNode=rootNode->FindDeepestNode(range);
				if(!foundNode) return false;
				return RetriveContext(output, foundNode, executor);
			}
		}
	}
}