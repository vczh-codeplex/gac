/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Parsing::Analyzing

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGANALYZER
#define VCZH_PARSING_PARSINGANALYZER

#include "ParsingDefinitions.h"

namespace vl
{
	namespace parsing
	{
		namespace analyzing
		{

/***********************************************************************
符号表关联对象
***********************************************************************/

			class ParsingSymbol;

			struct DefinitionTypeScopePair
			{
				definitions::ParsingDefinitionType*		type;
				ParsingSymbol*							scope;

				DefinitionTypeScopePair()
				{
				}

				DefinitionTypeScopePair(definitions::ParsingDefinitionType* _type, ParsingSymbol* _scope)
					:type(_type)
					,scope(_scope)
				{
				}

				vint Compare(const DefinitionTypeScopePair& pair)
				{
					if(type<pair.type) return -1;
					if(type>pair.type) return 1;
					if(scope<pair.scope) return -1;
					if(scope>pair.scope) return 1;
					return 0;
				}

				bool operator==(const DefinitionTypeScopePair& pair){return Compare(pair)==0;}
				bool operator!=(const DefinitionTypeScopePair& pair){return Compare(pair)!=0;}
				bool operator>(const DefinitionTypeScopePair& pair){return Compare(pair)>0;}
				bool operator>=(const DefinitionTypeScopePair& pair){return Compare(pair)>=0;}
				bool operator<(const DefinitionTypeScopePair& pair){return Compare(pair)<0;}
				bool operator<=(const DefinitionTypeScopePair& pair){return Compare(pair)<=0;}
			};

/***********************************************************************
符号表
***********************************************************************/

			class ParsingSymbol : public Object
			{
				friend class ParsingSymbolManager;

				typedef collections::Dictionary<WString, ParsingSymbol*>		ParsingSymbolMap;
				typedef collections::List<ParsingSymbol*>						ParsingSymbolList;
			public:
				enum SymbolType
				{
					Global,
					EnumType,
					ClassType,		// descriptor == base type
					ArrayType,		// descriptor == element type
					TokenType,
					EnumItem,		// descriptor == parent
					ClassField,		// descriptor == field type
					TokenDef,		// descriptor == token type
					RuleDef,		// descriptor == rule type
				};

			protected:
				ParsingSymbolManager*			manager;
				SymbolType						type;
				WString							name;
				ParsingSymbol*					descriptorSymbol;
				ParsingSymbol*					parentSymbol;
				ParsingSymbol*					arrayTypeSymbol;
				ParsingSymbolList				subSymbolList;
				ParsingSymbolMap				subSymbolMap;

				bool							AddSubSymbol(ParsingSymbol* subSymbol);

				ParsingSymbol(ParsingSymbolManager* _manager, SymbolType _type, const WString& _name, ParsingSymbol* _descriptorSymbol);
			public:
				~ParsingSymbol();

				ParsingSymbolManager*			GetManager();
				SymbolType						GetType();
				const WString&					GetName();
				vint							GetSubSymbolCount();
				ParsingSymbol*					GetSubSymbol(vint index);
				ParsingSymbol*					GetSubSymbolByName(const WString& name);
				ParsingSymbol*					GetDescriptorSymbol();
				ParsingSymbol*					GetParentSymbol();
				bool							IsType();
				ParsingSymbol*					SearchClassSubSymbol(const WString& name);
				ParsingSymbol*					SearchCommonBaseClass(ParsingSymbol* classType);
			};

			class ParsingSymbolManager : public Object
			{

				typedef collections::List<Ptr<ParsingSymbol>>												ParsingSymbolList;
				typedef collections::Dictionary<DefinitionTypeScopePair, ParsingSymbol*>					DefinitionTypeSymbolMap;
				typedef collections::Dictionary<definitions::ParsingDefinitionGrammar*, ParsingSymbol*>		DefinitionGrammarSymbolMap;
			protected:
				ParsingSymbol*					globalSymbol;
				ParsingSymbol*					tokenTypeSymbol;
				ParsingSymbolList				createdSymbols;
				DefinitionTypeSymbolMap			definitionTypeSymbolCache;
				DefinitionGrammarSymbolMap		definitionGrammarSymbolCache;
				DefinitionGrammarSymbolMap		definitionGrammarTypeCache;

				ParsingSymbol*					TryAddSubSymbol(Ptr<ParsingSymbol> subSymbol, ParsingSymbol* parentSymbol);
			public:
				ParsingSymbolManager();
				~ParsingSymbolManager();

				ParsingSymbol*					GetGlobal();
				ParsingSymbol*					GetTokenType();
				ParsingSymbol*					GetArrayType(ParsingSymbol* elementType);

				ParsingSymbol*					AddClass(const WString& name, ParsingSymbol* baseType, ParsingSymbol* parentType=0);
				ParsingSymbol*					AddField(const WString& name, ParsingSymbol* classType, ParsingSymbol* fieldType);
				ParsingSymbol*					AddEnum(const WString& name, ParsingSymbol* parentType=0);
				ParsingSymbol*					AddEnumItem(const WString& name, ParsingSymbol* enumType);
				ParsingSymbol*					AddTokenDefinition(const WString& name);
				ParsingSymbol*					AddRuleDefinition(const WString& name, ParsingSymbol* ruleType);

				ParsingSymbol*					CacheGetType(definitions::ParsingDefinitionType* type, ParsingSymbol* scope);
				bool							CacheSetType(definitions::ParsingDefinitionType* type, ParsingSymbol* scope, ParsingSymbol* symbol);
				ParsingSymbol*					CacheGetSymbol(definitions::ParsingDefinitionGrammar* grammar);
				bool							CacheSetSymbol(definitions::ParsingDefinitionGrammar* grammar, ParsingSymbol* symbol);
				ParsingSymbol*					CacheGetType(definitions::ParsingDefinitionGrammar* grammar);
				bool							CacheSetType(definitions::ParsingDefinitionGrammar* grammar, ParsingSymbol* type);
			};

/***********************************************************************
语义分析
***********************************************************************/

			extern WString						GetTypeFullName(ParsingSymbol* type);
			extern ParsingSymbol*				FindType(Ptr<definitions::ParsingDefinitionType> type, ParsingSymbolManager* manager, ParsingSymbol* scope, collections::List<Ptr<ParsingError>>& errors);
			extern void							PrepareSymbols(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager, collections::List<Ptr<ParsingError>>& errors);
			extern void							ValidateRuleStructure(Ptr<definitions::ParsingDefinitionRuleDefinition> rule, ParsingSymbolManager* manager, collections::List<Ptr<ParsingError>>& errors);
			extern void							ResolveRuleSymbols(Ptr<definitions::ParsingDefinitionRuleDefinition> rule, ParsingSymbolManager* manager, collections::List<Ptr<ParsingError>>& errors);
			extern void							ResolveSymbols(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager, collections::List<Ptr<ParsingError>>& errors);
			extern void							ValidateDefinition(Ptr<definitions::ParsingDefinition> definition, ParsingSymbolManager* manager, collections::List<Ptr<ParsingError>>& errors);
		}
	}
}

#endif