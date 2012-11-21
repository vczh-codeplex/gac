/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Parsing::Automaton

Classes:
***********************************************************************/

#ifndef VCZH_PARSING_PARSINGAUTOMATON
#define VCZH_PARSING_PARSINGAUTOMATON

#include "ParsingDefinitions.h"

namespace vl
{
	namespace parsing
	{
		namespace automaton
		{

/***********************************************************************
·ûºÅ±í
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
			};

			class ParsingSymbolManager : public Object
			{
				typedef collections::List<Ptr<ParsingSymbol>>						ParsingSymbolList;
			protected:
				ParsingSymbol*					globalSymbol;
				ParsingSymbol*					tokenTypeSymbol;
				ParsingSymbolList				createdSymbols;

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
			};
		}
	}
}

#endif