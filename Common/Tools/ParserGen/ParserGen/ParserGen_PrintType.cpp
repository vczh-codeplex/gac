#include "ParserGen.h"

/***********************************************************************
Helper Functions
***********************************************************************/

void EnumerateAllClass(ParsingSymbolManager* manager, ParsingSymbol* scope, SortedList<ParsingSymbol*>& types)
{
	if(scope->GetType()==ParsingSymbol::ClassType)
	{
		types.Add(scope);
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		EnumerateAllClass(manager, scope->GetSubSymbol(i), types);
	}
}

void EnumerateAllLeafClass(ParsingSymbolManager* manager, SortedList<ParsingSymbol*>& types)
{
	EnumerateAllClass(manager, manager->GetGlobal(), types);
	for(vint i=types.Count()-1;i>=0;i--)
	{
		types.Remove(types[i]->GetDescriptorSymbol());
	}
}

void SearchChildClasses(ParsingSymbol* parent, ParsingSymbol* scope, ParsingSymbolManager* manager, List<ParsingSymbol*>& children)
{
	if(scope->GetType()==ParsingSymbol::ClassType && scope->GetDescriptorSymbol()==parent)
	{
		children.Add(scope);
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		SearchChildClasses(parent, scope->GetSubSymbol(i), manager, children);
	}
}

/***********************************************************************
PrintTypeVisitor
***********************************************************************/

class PrintTypeVisitor : public Object, public ParsingDefinitionType::IVisitor
{
public:
	ParsingSymbol*				scope;
	ParsingSymbolManager*		manager;
	WString						codeClassPrefix;
	TextWriter&					writer;

	PrintTypeVisitor(ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer)
		:scope(_scope)
		,manager(_manager)
		,codeClassPrefix(_codeClassPrefix)
		,writer(_writer)
	{
	}

	void Visit(ParsingDefinitionPrimitiveType* node)override
	{
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->name);
	}

	void Visit(ParsingDefinitionTokenType* node)override
	{
		writer.WriteString(L"vl::parsing::ParsingToken");
	}

	void Visit(ParsingDefinitionSubType* node)override
	{
		node->parentType->Accept(this);
		writer.WriteString(L"::");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->subTypeName);
	}

	void Visit(ParsingDefinitionArrayType* node)override
	{
		writer.WriteString(L"vl::collections::List<");
		PrintTypeForValue(node->elementType.Obj(), scope, manager, codeClassPrefix, writer);
		writer.WriteString(L">");
	}
};

/***********************************************************************
PrintType
***********************************************************************/

void PrintType(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer)
{
	PrintTypeVisitor visitor(_scope, _manager, _codeClassPrefix, _writer);
	_type->Accept(&visitor);
}

void PrintTypeForValue(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer)
{
	List<Ptr<ParsingError>> errors;
	ParsingSymbol* type=FindType(_type, _manager, _scope, errors);
	if(type->GetType()==ParsingSymbol::EnumType)
	{
		PrintType(_type, _scope, _manager, _codeClassPrefix, _writer);
	}
	else if(type->GetType()==ParsingSymbol::ClassType)
	{
		_writer.WriteString(L"vl::Ptr<");
		PrintType(_type, _scope, _manager, _codeClassPrefix, _writer);
		_writer.WriteString(L">");
	}
	else
	{
		PrintType(_type, _scope, _manager, _codeClassPrefix, _writer);
	}
}

bool PrintType(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer)
{
	if(type->GetType()==ParsingSymbol::ClassType || type->GetType()==ParsingSymbol::EnumType)
	{
		if(PrintType(type->GetParentSymbol(), codeClassPrefix, writer))
		{
			writer.WriteString(L"::");
		}
		writer.WriteString(codeClassPrefix);
		writer.WriteString(type->GetName());
		return true;
	}
	else if(type->GetType()==ParsingSymbol::TokenType)
	{
		writer.WriteString(L"vl::parsing::ParsingToken");
		return true;
	}
	else if(type->GetType()==ParsingSymbol::ArrayType)
	{
		writer.WriteString(L"vl::collections::List<");
		PrintTypeForValue(type->GetDescriptorSymbol(), codeClassPrefix, writer);
		writer.WriteString(L">");
		return true;
	}
	else
	{
		return false;
	}
}

void PrintTypeForValue(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer)
{
	if(type->GetType()==ParsingSymbol::EnumType)
	{
		PrintType(type, codeClassPrefix, writer);
	}
	else if(type->GetType()==ParsingSymbol::ClassType)
	{
		writer.WriteString(L"vl::Ptr<");
		PrintType(type, codeClassPrefix, writer);
		writer.WriteString(L">");
	}
	else
	{
		PrintType(type, codeClassPrefix, writer);
	}
}