#include "..\..\..\Source\Console.h"
#include "..\..\..\Source\Pointer.h"
#include "..\..\..\Source\Stream\FileStream.h"
#include "..\..\..\Source\Stream\Accessor.h"
#include "..\..\..\Source\Stream\CharFormat.h"
#include "..\..\..\Source\Collections\List.h"
#include "..\..\..\Source\Collections\Dictionary.h"
#include "..\..\..\Source\Collections\Operation.h"
#include "..\..\..\Source\Parsing\ParsingAutomaton.h"
#include "..\..\..\Source\Parsing\Parsing.h"

#include <windows.h>

using namespace vl;
using namespace vl::console;
using namespace vl::stream;
using namespace vl::collections;
using namespace vl::regex;
using namespace vl::parsing;
using namespace vl::parsing::analyzing;
using namespace vl::parsing::definitions;
using namespace vl::parsing::tabling;

/***********************************************************************
Parser Generation
***********************************************************************/

template<typename TLoggable>
void LogParsingData(TLoggable loggable, const WString& name, StreamWriter& writer)
{
	writer.WriteLine(L"=============================================================");
	writer.WriteLine(name);
	writer.WriteLine(L"=============================================================");
	Log(loggable, writer);
}

void LogErrors(List<Ptr<ParsingError>>& errors, StreamWriter& writer)
{
	writer.WriteLine(L"=============================================================");
	writer.WriteLine(L"Errors");
	writer.WriteLine(L"=============================================================");
	FOREACH(Ptr<ParsingError>, error, errors)
	{
		writer.WriteLine(error->errorMessage);
	}
}

#define CheckError do{ if(errors.Count()>0){ LogErrors(errors, writer); return 0; } }while(0)

Ptr<ParsingDefinition> CreateDefinition(Ptr<ParsingStrictParser> parser, const WString& grammar, StreamWriter& writer)
{
	List<Ptr<ParsingError>> errors;
	Ptr<ParsingTreeNode> definitionNode=parser->Parse(grammar, L"ParserDecl", errors);
	CheckError;

	Ptr<ParsingDefinition> definition=DeserializeDefinition(definitionNode);
	return definition;
}

Ptr<ParsingTable> CreateTable(Ptr<ParsingDefinition> definition, StreamWriter& writer)
{
	ParsingSymbolManager symbolManager;
	List<Ptr<ParsingError>> errors;
	ValidateDefinition(definition, &symbolManager, errors);
	LogParsingData(definition, L"Grammar Definition", writer);
	CheckError;

	Ptr<Automaton> epsilonPDA=CreateEpsilonPDA(definition, &symbolManager);
	Ptr<Automaton> nondeterministicPDA=CreateNondeterministicPDAFromEpsilonPDA(epsilonPDA);
	Ptr<Automaton> jointPDA=CreateJointPDAFromNondeterministicPDA(nondeterministicPDA);
		
	LogParsingData(epsilonPDA, L"Epsilon PDA", writer);
	LogParsingData(nondeterministicPDA, L"Non-deterministic PDA", writer);
	LogParsingData(jointPDA, L"Joint PDA", writer);

	CompactJointPDA(jointPDA);
	LogParsingData(jointPDA, L"Compacted Joint PDA", writer);

	MarkLeftRecursiveInJointPDA(jointPDA, errors);
	LogParsingData(jointPDA, L"Compacted Joint PDA", writer);
	CheckError;

	Ptr<ParsingTable> table=GenerateTable(definition, jointPDA, errors);
	LogParsingData(table, L"Table", writer);
	CheckError;

	return table;
}

/***********************************************************************
Code Generation
***********************************************************************/

void WriteFileComment(const WString& name, StreamWriter& writer)
{
	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Vczh Library++ 3.0");
	writer.WriteLine(L"Developer: 陈梓瀚(vczh)");
	writer.WriteLine(L"Parser::"+name);
	writer.WriteLine(L"");
	writer.WriteLine(L"本文件使用Vczh Functional Macro工具自动生成");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
}

WString WriteFileBegin(List<WString>& codeIncludes, List<WString>& codeNamespaces, StreamWriter& writer)
{
	FOREACH(WString, include, codeIncludes)
	{
		writer.WriteLine(L"#include "+include);
	}
	writer.WriteLine(L"");
	WString prefix;
	FOREACH(WString, ns, codeNamespaces)
	{
		writer.WriteLine(prefix+L"namespace "+ns);
		writer.WriteLine(prefix+L"{");
		prefix+=L"\t";
	}
	return prefix;
}

void WriteFileEnd(List<WString>& codeNamespaces, StreamWriter& writer)
{
	vint counter=codeNamespaces.Count();
	FOREACH(WString, ns, codeNamespaces)
	{
		counter--;
		for(vint i=0;i<counter;i++) writer.WriteChar(L'\t');
		writer.WriteLine(L"}");
	}
}

/***********************************************************************
Syntax Tree Declaration Generation
***********************************************************************/

extern void PrintType(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer);
extern void PrintTypeForValue(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer);
extern void PrintTypeDefinitions(List<Ptr<ParsingDefinitionTypeDefinition>>& types, const WString& prefix, ParsingSymbol* scope, ParsingSymbolManager* manager, const WString& codeClassPrefix, TextWriter& writer);
extern bool PrintType(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer);
extern void PrintTypeForValue(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer);

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
		_writer.WriteString(L"::Type");
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
		writer.WriteString(L"::Type");
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

class PrintTypeDefinitionVisitor : public Object, public ParsingDefinitionTypeDefinition::IVisitor
{
public:
	ParsingSymbol*			scope;
	ParsingSymbolManager*	manager;
	WString					prefix;
	WString					codeClassPrefix;
	TextWriter&				writer;

	void LogInternal(ParsingDefinitionTypeDefinition* _definition, const WString& _prefix)
	{
		ParsingSymbol* oldScope=scope;
		WString oldPrefix=prefix;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_definition->name);
		if(!scope) scope=oldScope;
		prefix=_prefix;
		_definition->Accept(this);

		prefix=oldPrefix;
		scope=oldScope;
	}

	PrintTypeDefinitionVisitor(ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _prefix, const WString& _codeClassPrefix, TextWriter& _writer)
		:scope(_scope)
		,manager(_manager)
		,prefix(_prefix)
		,codeClassPrefix(_codeClassPrefix)
		,writer(_writer)
	{
	}

	void Visit(ParsingDefinitionClassMemberDefinition* node)override
	{
		writer.WriteString(prefix);
		{
			PrintTypeForValue(node->type.Obj(), scope, manager, codeClassPrefix, writer);
		}
		writer.WriteString(L" ");
		writer.WriteString(node->name);
		writer.WriteLine(L";");
	}

	void Visit(ParsingDefinitionClassDefinition* node)override
	{
		writer.WriteString(prefix);
		writer.WriteString(L"class ");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->name);
		writer.WriteString(L" : public ");
		if(node->parentType)
		{
			PrintType(node->parentType.Obj(), scope, manager, codeClassPrefix, writer);
		}
		else
		{
			writer.WriteString(L"vl::parsing::ParsingTreeCustomBase");
		}
		writer.WriteLine(L"");

		writer.WriteString(prefix);
		writer.WriteLine(L"{");
		writer.WriteString(prefix);
		writer.WriteLine(L"public:");

		List<ParsingSymbol*> children;
		ParsingSymbol* thisType=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(node->name);
		SearchChildClasses(thisType, manager->GetGlobal(), manager, children);
		if(children.Count()>0)
		{
			writer.WriteString(prefix);
			writer.WriteLine(L"\tclass IVisitor : public Interface");
			writer.WriteString(prefix);
			writer.WriteLine(L"\t{");
			writer.WriteString(prefix);
			writer.WriteLine(L"\t\tpublic:");

			FOREACH(ParsingSymbol*, child, children)
			{
				writer.WriteString(prefix);
				writer.WriteString(L"\t\tvirtual void Visit(");
				PrintType(child, codeClassPrefix, writer);
				writer.WriteLine(L"* node)=0;");
			}

			writer.WriteString(prefix);
			writer.WriteLine(L"\t};");
			writer.WriteLine(L"");
			writer.WriteString(prefix);
			writer.WriteString(L"\tvirtual void Accept(");
			PrintType(thisType, codeClassPrefix, writer);
			writer.WriteLine(L"::IVisitor* visitor)=0;");
			writer.WriteLine(L"");
		}

		PrintTypeDefinitions(node->subTypes, prefix+L"\t", scope, manager, codeClassPrefix, writer);

		for(int i=0;i<node->members.Count();i++)
		{
			LogInternal(node->members[i].Obj(), prefix+L"\t");
		}

		if(node->parentType)
		{
			writer.WriteLine(L"");
			writer.WriteString(prefix);
			writer.WriteString(L"\tvoid Accept(");
			PrintType(thisType->GetDescriptorSymbol(), codeClassPrefix, writer);
			writer.WriteLine(L"::IVisitor* visitor)override;");
		}
		{
			writer.WriteLine(L"");
			writer.WriteString(prefix);
			writer.WriteString(L"\tstatic ");
			PrintTypeForValue(thisType, codeClassPrefix, writer);
			writer.WriteLine(L" Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);");
		}

		writer.WriteString(prefix);
		writer.WriteLine(L"};");
	}

	void Visit(ParsingDefinitionEnumMemberDefinition* node)override
	{
		writer.WriteString(prefix);
		writer.WriteString(node->name);
		writer.WriteLine(L",");
	}

	void Visit(ParsingDefinitionEnumDefinition* node)override
	{
		writer.WriteString(prefix);
		writer.WriteString(L"namespace ");
		writer.WriteString(codeClassPrefix);
		writer.WriteLine(node->name);
		writer.WriteString(prefix);
		writer.WriteLine(L"{");

		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		writer.WriteLine(L"enum Type");
		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		writer.WriteLine(L"{");

		for(int i=0;i<node->members.Count();i++)
		{
			LogInternal(node->members[i].Obj(), prefix+L"\t\t");
		}

		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		writer.WriteLine(L"};");
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
	}
};

class PrintForwardTypeDefinitionVisitor : public Object, public ParsingDefinitionTypeDefinition::IVisitor
{
public:
	WString					prefix;
	WString					codeClassPrefix;
	TextWriter&				writer;
	bool					exists;

	PrintForwardTypeDefinitionVisitor(const WString& _prefix, const WString& _codeClassPrefix, TextWriter& _writer)
		:prefix(_prefix)
		,writer(_writer)
		,codeClassPrefix(_codeClassPrefix)
		,exists(false)
	{
	}

	void Visit(ParsingDefinitionClassMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionClassDefinition* node)override
	{
		writer.WriteString(prefix);
		writer.WriteString(L"class ");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->name);
		writer.WriteLine(L";");
		exists=true;
	}

	void Visit(ParsingDefinitionEnumMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionEnumDefinition* node)override
	{
	}
};

void PrintTypeDefinitions(List<Ptr<ParsingDefinitionTypeDefinition>>& types, const WString& prefix, ParsingSymbol* scope, ParsingSymbolManager* manager, const WString& codeClassPrefix, TextWriter& writer)
{
	{
		PrintForwardTypeDefinitionVisitor visitor(prefix, codeClassPrefix, writer);
		FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, types)
		{
			type->Accept(&visitor);
		}
		if(visitor.exists)
		{
			writer.WriteLine(L"");
		}
	}
	FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, types)
	{
		PrintTypeDefinitionVisitor visitor(scope, manager, prefix, codeClassPrefix, writer);
		type->Accept(&visitor);
		writer.WriteLine(L"");
	}
}

void WriteHeaderFile(const WString& name, Ptr<ParsingDefinition> definition, List<WString>& codeIncludes, List<WString>& codeNamespaces, const WString& codeClassPrefix, StreamWriter& writer)
{
	WriteFileComment(name, writer);
	WString prefix=WriteFileBegin(codeIncludes, codeNamespaces, writer);

	ParsingSymbolManager manager;
	{
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &manager, errors);
	}
	PrintTypeDefinitions(definition->types, prefix, 0, &manager, codeClassPrefix, writer);
	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"ConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens);");

	WriteFileEnd(codeNamespaces, writer);
}

/***********************************************************************
Syntax Tree Implementation Generation
***********************************************************************/

class CollectUnescapingFunctionVisitor : public Object, public ParsingDefinitionTypeDefinition::IVisitor
{
public:
	ParsingSymbol*								scope;
	ParsingSymbolManager*						manager;
	Dictionary<WString, ParsingSymbol*>			functions;

	void LogInternal(ParsingDefinitionTypeDefinition* _definition)
	{
		ParsingSymbol* oldScope=scope;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_definition->name);
		if(!scope) scope=oldScope;
		_definition->Accept(this);

		scope=oldScope;
	}

	CollectUnescapingFunctionVisitor(ParsingSymbol* _scope, ParsingSymbolManager* _manager)
		:scope(_scope)
		,manager(_manager)
	{
	}

	void Visit(ParsingDefinitionClassMemberDefinition* node)override
	{
		if(node->unescapingFunction!=L"" && !functions.Keys().Contains(node->unescapingFunction))
		{
			List<Ptr<ParsingError>> errors;
			ParsingSymbol* type=FindType(node->type.Obj(), manager, scope, errors);
			functions.Add(node->unescapingFunction, type);
		}
	}

	void Visit(ParsingDefinitionClassDefinition* node)override
	{
		for(int i=0;i<node->subTypes.Count();i++)
		{
			LogInternal(node->subTypes[i].Obj());
		}
		for(int i=0;i<node->members.Count();i++)
		{
			LogInternal(node->members[i].Obj());
		}
	}

	void Visit(ParsingDefinitionEnumMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionEnumDefinition* node)override
	{
	}
};

void WriteUnescapingFunctionForwardDeclarations(Ptr<ParsingDefinition> definition, ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	CollectUnescapingFunctionVisitor visitor(0, manager);
	FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, definition->types)
	{
		type->Accept(&visitor);
	}
	if(visitor.functions.Count()>0)
	{
		for(vint i=0;i<visitor.functions.Count();i++)
		{
			WString name=visitor.functions.Keys().Get(i);
			ParsingSymbol* type=visitor.functions.Values().Get(i);

			writer.WriteString(prefix);
			writer.WriteString(L"extern void ");
			writer.WriteString(name);
			writer.WriteString(L"(");
			PrintTypeForValue(type, codeClassPrefix, writer);
			writer.WriteLine(L"& value, vl::collections::List<vl::regex::RegexToken>& tokens);");
		}
		writer.WriteLine(L"");
	}
}

void WriteConvertImpl(ParsingSymbolManager* manager, ParsingSymbol* scope, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	if(scope->GetType()==ParsingSymbol::ClassType)
	{
		writer.WriteString(prefix);
		PrintTypeForValue(scope, codeClassPrefix, writer);
		writer.WriteString(L" ");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteLine(L"::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)");
		writer.WriteString(prefix);
		writer.WriteLine(L"{");
		writer.WriteString(prefix);
		writer.WriteString(L"\treturn ");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(L"ConvertParsingTreeNode(node, tokens).Cast<");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteLine(L">();");
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
		writer.WriteLine(L"");
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		WriteConvertImpl(manager, scope->GetSubSymbol(i), prefix, codeClassPrefix, writer);
	}
}

void WriteVisitorImpl(ParsingSymbolManager* manager, ParsingSymbol* scope, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	if(scope->GetType()==ParsingSymbol::ClassType)
	{
		ParsingSymbol* parent=scope->GetDescriptorSymbol();
		if(parent)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"void ");
			PrintType(scope, codeClassPrefix, writer);
			writer.WriteString(L"::Accept(");
			PrintType(parent, codeClassPrefix, writer);
			writer.WriteLine(L"::IVisitor* visitor)");
			writer.WriteString(prefix);
			writer.WriteLine(L"{");
			writer.WriteString(prefix);
			writer.WriteLine(L"\tvisitor->Visit(this);");
			writer.WriteString(prefix);
			writer.WriteLine(L"}");
			writer.WriteLine(L"");
		}
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		WriteVisitorImpl(manager, scope->GetSubSymbol(i), prefix, codeClassPrefix, writer);
	}
}

/***********************************************************************
Node Conversion Generation
***********************************************************************/

class WriteNodeConverterClassImplFragmentVisitor : public Object, public ParsingDefinitionTypeDefinition::IVisitor
{
public:
	ParsingSymbol*			scope;
	ParsingSymbolManager*	manager;
	WString					prefix;
	WString					codeClassPrefix;
	TextWriter&				writer;

	void LogInternal(ParsingDefinitionTypeDefinition* _definition)
	{
		ParsingSymbol* oldScope=scope;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_definition->name);
		if(!scope) scope=oldScope;
		_definition->Accept(this);

		scope=oldScope;
	}

	WriteNodeConverterClassImplFragmentVisitor(ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _prefix, const WString& _codeClassPrefix, TextWriter& _writer)
		:scope(_scope)
		,manager(_manager)
		,prefix(_prefix)
		,codeClassPrefix(_codeClassPrefix)
		,writer(_writer)
	{
	}

	void Visit(ParsingDefinitionClassMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionClassDefinition* node)override
	{
		ParsingSymbol* thisType=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(node->name);
		if(thisType->GetType()==ParsingSymbol::ClassType)
		{
			writer.WriteString(L"if(obj->GetType()==L\"");
			writer.WriteString(GetTypeNameForCreateInstruction(thisType));
			writer.WriteLine(L"\")");
			writer.WriteString(prefix);
			writer.WriteLine(L"{");
			FOREACH(Ptr<ParsingDefinitionClassMemberDefinition>, member, node->members)
			{
				LogInternal(member.Obj());
			}
			writer.WriteString(prefix);
			writer.WriteLine(L"}");
			writer.WriteString(prefix);
			writer.WriteString(L"else ");
		}
		FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, node->subTypes)
		{
			LogInternal(type.Obj());
		}
	}

	void Visit(ParsingDefinitionEnumMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionEnumDefinition* node)override
	{
	}
};

void WriteNodeConverterClassImplFragment(Ptr<ParsingDefinition> definition, ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	WriteNodeConverterClassImplFragmentVisitor visitor(0, manager, prefix, codeClassPrefix, writer);
	FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, definition->types)
	{
		type->Accept(&visitor);
	}
}

void WriteNodeConverterClassImpl(Ptr<ParsingDefinition> definition, ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	writer.WriteString(prefix);
	writer.WriteString(L"class ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"TreeConverter : public vl::parsing::ParsingTreeConverter");
	writer.WriteString(prefix);
	writer.WriteLine(L"{");
	writer.WriteString(prefix);
	writer.WriteLine(L"public:");
	writer.WriteString(prefix);
	writer.WriteLine(L"\tusing vl::parsing::ParsingTreeConverter::SetMember;");
	writer.WriteLine(L"");
	writer.WriteString(prefix);
	writer.WriteLine(L"\tPtr<ParsingTreeCustomBase> ConvertClass(Ptr<ParsingTreeObject> obj, const TokenList& tokens)override");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t{");
	writer.WriteString(prefix);
	writer.WriteString(L"\t\t");
	WriteNodeConverterClassImplFragment(definition, manager, prefix+L"\t\t", codeClassPrefix, writer);
	writer.WriteLine(L"");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t\t\treturn 0;");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t}");
	writer.WriteString(prefix);
	writer.WriteLine(L"};");
	writer.WriteLine(L"");

	writer.WriteString(prefix);
	writer.WriteString(L"vl::Ptr<vl::parsing::ParsingTreeCustomBase> ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"ConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, vl::collections::List<vl::regex::RegexToken>& tokens)");
	writer.WriteString(prefix);
	writer.WriteLine(L"{");
	
	writer.WriteString(prefix);
	writer.WriteString(L"\t");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"TreeConverter converter;");
	writer.WriteString(prefix);
	writer.WriteLine(L"\tvl::Ptr<vl::parsing::ParsingTreeCustomBase> tree;");
	writer.WriteString(prefix);
	writer.WriteLine(L"\tconverter.SetMember(tree, node, tokens);");
	writer.WriteString(prefix);
	writer.WriteLine(L"\treturn tree;");

	writer.WriteString(prefix);
	writer.WriteLine(L"}");
	writer.WriteLine(L"");
}

/***********************************************************************
Parsing Table Generation
***********************************************************************/

void WriteCppFile(const WString& name, Ptr<ParsingDefinition> definition, Ptr<ParsingTable> table, List<WString>& codeIncludes, List<WString>& codeNamespaces, const WString& codeClassPrefix, StreamWriter& writer)
{
	WString prefix=WriteFileBegin(codeIncludes, codeNamespaces, writer);

	ParsingSymbolManager manager;
	{
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &manager, errors);
	}

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Unescaping Function Foward Declarations");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteUnescapingFunctionForwardDeclarations(definition, &manager, prefix, codeClassPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parsing Tree Conversion Driver Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteNodeConverterClassImpl(definition, &manager, prefix, codeClassPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parsing Tree Conversion Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteConvertImpl(&manager, manager.GetGlobal(), prefix, codeClassPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Visitor Pattern Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteVisitorImpl(&manager, manager.GetGlobal(), prefix, codeClassPrefix, writer);

	WriteFileEnd(codeNamespaces, writer);
}

/***********************************************************************
include:xxxx
include:xxxx
...
namespace:abc.def.ghi
classPrefix:abc
grammar:
***********************************************************************/

WString GetDirectory(const WString& fileName)
{
	int index=0;
	for(int i=0;i<fileName.Length();i++)
	{
		if(fileName[i]==L'\\')
		{
			index=i;
		}
	}
	return fileName.Left(index+1);
}

int wmain(int argc, wchar_t* argv[])
{
	WString baseDirectory;
	{
		wchar_t currentDirectory[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH, currentDirectory);
		baseDirectory=currentDirectory;
		if(baseDirectory[baseDirectory.Length()-1]!=L'\\')
		{
			baseDirectory+=L'\\';
		}
	}

	Regex regexPathSplitter(L"[///\\]");
	Regex regexInclude(L"^include:(<path>/.+)$");
	Regex regexClassPrefix(L"^classPrefix:(<prefix>/.+)$");
	Regex regexNamespace(L"^namespace:((<namespace>[^.]+)(.(<namespace>[^.]+))*)?$");
	Ptr<ParsingStrictParser> parser=CreateBootstrapStrictParser();

	Console::SetTitle(L"Vczh Parser Generator for C++");
	Console::SetColor(false, true, false, true);
	Console::WriteLine(L"parsing>Files : "+itow(argc-1));
	for(int i=1;i<argc;i++)
	{
		Console::WriteLine(L"------------------------------------------------------------");
		WString inputPath=argv[i];
		if(inputPath.Length()<2 || inputPath[1]!=L':')
		{
			inputPath=baseDirectory+inputPath;
		}
		Console::WriteLine(L"parsing>Making : "+inputPath);
		if(inputPath.Length()<11 || inputPath.Right(11)!=L".parser.txt")
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(L"error> The extenion name of the input file path must be \".parser.txt\".");
			Console::SetColor(false, true, false, true);
		}
		else
		{
			WString name;
			{
				List<Ptr<RegexMatch>> matches;
				regexPathSplitter.Split(inputPath, true, matches);
				name=matches[matches.Count()-1]->Result().Value();
				name=name.Left(name.Length()-11);
			}
			WString outputMetaPath=inputPath.Left(inputPath.Length()-11);
			WString outputHeaderPath=outputMetaPath+L".h";
			WString outputCppPath=outputMetaPath+L".cpp";
			WString logPath=outputMetaPath+L".log";
			Console::WriteLine(L"parsing>Output header path : "+outputHeaderPath);
			Console::WriteLine(L"parsing>Output cpp path : "+outputCppPath);
			Console::WriteLine(L"parsing>Log path : "+logPath);

			List<WString> codeIncludes, codeNamespaces;
			WString codeGrammar, codeClassPrefix;
			{
				FileStream fileStream(inputPath, FileStream::ReadOnly);
				if(!fileStream.IsAvailable())
				{
					Console::WriteLine(L"error> Cannot open \""+inputPath+L" for read.");
					goto STOP_PARSING;
				}
				BomDecoder decoder;
				DecoderStream decoderStream(fileStream, decoder);
				StreamReader reader(decoderStream);

				while(!reader.IsEnd())
				{
					WString line=reader.ReadLine();
					Ptr<RegexMatch> match;
					if(line==L"grammar:")
					{
						break;
					}
					else if((match=regexClassPrefix.Match(line)) && match->Success())
					{
						codeClassPrefix=match->Groups().Get(L"prefix").Get(0).Value();
					}
					else if((match=regexInclude.Match(line)) && match->Success())
					{
						codeIncludes.Add(match->Groups().Get(L"path").Get(0).Value());
					}
					else if((match=regexNamespace.Match(line)) && match->Success())
					{
						CopyFrom(codeNamespaces, match->Groups().Get(L"namespace")
							>>Select<RegexString, WString>([=](RegexString s)
							{
								return s.Value();
							}));
					}
					else
					{
						Console::WriteLine(L"error> Unknown property \""+line+L".");
						goto STOP_PARSING;
					}
				}
				codeGrammar=reader.ReadToEnd();
			}

			Ptr<ParsingDefinition> definition;
			Ptr<ParsingTable> table;
			{
				FileStream fileStream(logPath, FileStream::WriteOnly);
				if(!fileStream.IsAvailable())
				{
					Console::WriteLine(L"error> Cannot open \""+logPath+L" for write.");
					goto STOP_PARSING;
				}
				BomEncoder encoder(BomEncoder::Utf16);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);

				definition=CreateDefinition(parser, codeGrammar, writer);
				if(!definition)
				{
					Console::WriteLine(L"error> Error happened. Open \""+logPath+L" for details.");
					goto STOP_PARSING;
				}

				table=CreateTable(definition, writer);
				if(!table)
				{
					Console::WriteLine(L"error> Error happened. Open \""+logPath+L" for details.");
					goto STOP_PARSING;
				}
			}
			{
				FileStream fileStream(outputHeaderPath, FileStream::WriteOnly);
				if(!fileStream.IsAvailable())
				{
					Console::WriteLine(L"error> Cannot open \""+outputHeaderPath+L" for write.");
					goto STOP_PARSING;
				}
				BomEncoder encoder(BomEncoder::Mbcs);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);
				WriteHeaderFile(name, definition, codeIncludes, codeNamespaces, codeClassPrefix, writer);
			}
			{
				FileStream fileStream(outputCppPath, FileStream::WriteOnly);
				if(!fileStream.IsAvailable())
				{
					Console::WriteLine(L"error> Cannot open \""+outputCppPath+L" for write.");
					goto STOP_PARSING;
				}
				BomEncoder encoder(BomEncoder::Mbcs);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);
				List<WString> cppIncludes;
				cppIncludes.Add(L"\""+name+L".h\"");
				WriteCppFile(name, definition, table, cppIncludes, codeNamespaces, codeClassPrefix, writer);
			}
		}
	STOP_PARSING:;
	}
	Console::WriteLine(L"Finished!");
	return 0;
}