#include "ParsingAutomaton.h"

namespace vl
{
	namespace parsing
	{
		namespace automaton
		{

/***********************************************************************
ParsingSymbol
***********************************************************************/

			bool ParsingSymbol::AddSubSymbol(ParsingSymbol* subSymbol)
			{
				if(subSymbol->GetParentSymbol()) return false;
				if(subSymbolMap.Keys().IndexOf(subSymbol->GetName())!=-1) return false;
				switch(type)
				{
				case Global:
					switch(subSymbol->GetType())
					{
					case EnumType:		break;
					case ClassType:		break;
					case TokenDef:		break;
					case RuleDef:		break;
					default:			return false;
					}
					break;
				case EnumType:
					switch(subSymbol->GetType())
					{
					case EnumItem:		break;
					default:			return false;
					}
					break;
				case ClassType:
					switch(subSymbol->GetType())
					{
					case EnumType:		
					case ClassType:
					case ClassField:	break;
					default:			return false;
					}
					break;
				default:				return false;
				}

				subSymbol->parentSymbol=this;
				subSymbolList.Add(subSymbol);
				subSymbolMap.Add(subSymbol->GetName(), subSymbol);
				return true;
			}

			ParsingSymbol::ParsingSymbol(ParsingSymbolManager* _manager, SymbolType _type, const WString& _name, ParsingSymbol* _descriptorSymbol)
				:manager(_manager)
				,type(_type)
				,name(_name)
				,descriptorSymbol(_descriptorSymbol)
				,parentSymbol(0)
				,arrayTypeSymbol(0)
			{
			}

			ParsingSymbol::~ParsingSymbol()
			{
			}

			ParsingSymbolManager* ParsingSymbol::GetManager()
			{
				return manager;
			}

			ParsingSymbol::SymbolType ParsingSymbol::GetType()
			{
				return type;
			}

			const WString& ParsingSymbol::GetName()
			{
				return name;
			}

			vint ParsingSymbol::GetSubSymbolCount()
			{
				return subSymbolList.Count();
			}

			ParsingSymbol* ParsingSymbol::GetSubSymbol(vint index)
			{
				if(0<=index && index<subSymbolList.Count())
				{
					return subSymbolList[index];
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbol::GetSubSymbolByName(const WString& name)
			{
				vint index=subSymbolMap.Keys().IndexOf(name);
				if(index==-1)
				{
					return 0;
				}
				else
				{
					return subSymbolMap.Values()[index];
				}
			}

			ParsingSymbol* ParsingSymbol::GetDescriptorSymbol()
			{
				return descriptorSymbol;
			}

			ParsingSymbol* ParsingSymbol::GetParentSymbol()
			{
				return parentSymbol;
			}

			bool ParsingSymbol::IsType()
			{
				switch(type)
				{
				case ParsingSymbol::ClassType:
				case ParsingSymbol::EnumType:
				case ParsingSymbol::ArrayType:
				case ParsingSymbol::TokenType:
					return true;
				default:
					return false;
				}
			}

/***********************************************************************
ParsingSymbolManager
***********************************************************************/

			ParsingSymbol* ParsingSymbolManager::TryAddSubSymbol(Ptr<ParsingSymbol> subSymbol, ParsingSymbol* parentSymbol)
			{
				if(parentSymbol->AddSubSymbol(subSymbol.Obj()))
				{
					createdSymbols.Add(subSymbol);
					return subSymbol.Obj();
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbolManager::ParsingSymbolManager()
			{
				globalSymbol=new ParsingSymbol(this, ParsingSymbol::Global, L"", 0);
				tokenTypeSymbol=new ParsingSymbol(this, ParsingSymbol::TokenType, L"token", 0);
				createdSymbols.Add(globalSymbol);
				createdSymbols.Add(tokenTypeSymbol);
			}

			ParsingSymbolManager::~ParsingSymbolManager()
			{
			}

			ParsingSymbol* ParsingSymbolManager::GetGlobal()
			{
				return globalSymbol;
			}

			ParsingSymbol* ParsingSymbolManager::GetTokenType()
			{
				return tokenTypeSymbol;
			}

			ParsingSymbol* ParsingSymbolManager::GetArrayType(ParsingSymbol* elementType)
			{
				if(elementType->IsType())
				{
					if(!elementType->arrayTypeSymbol)
					{
						elementType->arrayTypeSymbol=new ParsingSymbol(this, ParsingSymbol::ArrayType, L"", elementType);
						createdSymbols.Add(elementType->arrayTypeSymbol);
					}
					return elementType->arrayTypeSymbol;
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbolManager::AddClass(const WString& name, ParsingSymbol* baseType, ParsingSymbol* parentType)
			{
				if((!baseType || baseType->GetType()==ParsingSymbol::ClassType) && (!parentType || parentType->IsType()))
				{
					ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::ClassField, name, baseType);
					return TryAddSubSymbol(symbol, parentType?parentType:globalSymbol);
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbolManager::AddField(const WString& name, ParsingSymbol* classType, ParsingSymbol* fieldType)
			{
				if(classType && classType->GetType()==ParsingSymbol::ClassType && fieldType && fieldType->IsType())
				{
					ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::ClassField, name, fieldType);
					return TryAddSubSymbol(symbol, classType);
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbolManager::AddEnum(const WString& name, ParsingSymbol* parentType)
			{
				if(!parentType || parentType->IsType())
				{
					ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::EnumType, name, 0);
					return TryAddSubSymbol(symbol, parentType?parentType:globalSymbol);
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbolManager::AddEnumItem(const WString& name, ParsingSymbol* enumType)
			{
				if(enumType && enumType->GetType()==ParsingSymbol::EnumType)
				{
					ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::EnumItem, name, enumType);
					return TryAddSubSymbol(symbol, enumType);
				}
				else
				{
					return 0;
				}
			}

			ParsingSymbol* ParsingSymbolManager::AddTokenDefinition(const WString& name)
			{
				ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::TokenDef, name, tokenTypeSymbol);
				return TryAddSubSymbol(symbol, globalSymbol);
			}

			ParsingSymbol* ParsingSymbolManager::AddRuleDefinition(const WString& name, ParsingSymbol* ruleType)
			{
				if(ruleType && ruleType->IsType())
				{
					ParsingSymbol* symbol=new ParsingSymbol(this, ParsingSymbol::TokenDef, name, ruleType);
					return TryAddSubSymbol(symbol, globalSymbol);
				}
				else
				{
					return 0;
				}
			}
		}
	}
}