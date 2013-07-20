#include "..\..\..\..\Libraries\GacUI\Public\Source\Vlpp.h"
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
	if(loggable)
	{
		Log(loggable, writer);
	}
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

Ptr<ParsingDefinition> CreateDefinition(Ptr<ParsingGeneralParser> parser, const WString& grammar, StreamWriter& writer)
{
	List<Ptr<ParsingError>> errors;
	Ptr<ParsingTreeNode> definitionNode=parser->Parse(grammar, L"ParserDecl", errors);
	CheckError;

	Ptr<ParsingDefinition> definition=DeserializeDefinition(definitionNode);
	return definition;
}

Ptr<ParsingTable> CreateTable(Ptr<ParsingDefinition> definition, StreamWriter& writer, bool ambiguity)
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
	LogParsingData(jointPDA, L"Marked Joint PDA", writer);
	CheckError;

	Ptr<ParsingTable> table=GenerateTableFromPDA(definition, jointPDA, ambiguity, errors);
	LogParsingData(table, L"Table", writer);
	if(!ambiguity)
	{
		CheckError;
	}
	else if(errors.Count()>0)
	{
		LogErrors(errors, writer);
	}

	return table;
}

/***********************************************************************
Code Config
***********************************************************************/

class CodegenConfig
{
public:
	List<WString>					codeIncludes;
	List<WString>					codeNamespaces;
	Dictionary<WString, WString>	codeParsers;
	WString							codeClassPrefix;
	WString							codeGuard;
	bool							parsingTree;
	bool							parserText;
	bool							ambiguity;

	CodegenConfig()
		:parsingTree(true)
		,parserText(false)
		,ambiguity(false)
	{
	}

	bool ReadConfig(StreamReader& reader)
	{
		Regex regexInclude(L"^include:(<path>/.+)$");
		Regex regexClassPrefix(L"^classPrefix:(<prefix>/.+)$");
		Regex regexGuard(L"^guard:(<guard>/.+)$");
		Regex regexNamespace(L"^namespace:((<namespace>[^.]+)(.(<namespace>[^.]+))*)?$");
		Regex regexParser(L"^parser:(<name>/w+)/((<rule>/w+)/)$");
		Regex regexParsingTree(L"^parsingtree:(<value>enabled|disabled)$");
		Regex regexParserText(L"^parsertext:(<value>enabled|disabled)$");
		Regex regexAmbiguity(L"^ambiguity:(<value>enabled|disabled)$");

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
			else if((match=regexGuard.Match(line)) && match->Success())
			{
				codeGuard=match->Groups().Get(L"guard").Get(0).Value();
			}
			else if((match=regexInclude.Match(line)) && match->Success())
			{
				codeIncludes.Add(match->Groups().Get(L"path").Get(0).Value());
			}
			else if((match=regexNamespace.Match(line)) && match->Success())
			{
				CopyFrom(codeNamespaces, From(match->Groups().Get(L"namespace"))
					.Select([=](RegexString s)->WString
					{
						return s.Value();
					}));
			}
			else if((match=regexParser.Match(line)) && match->Success())
			{
				WString name=match->Groups().Get(L"name").Get(0).Value();
				WString rule=match->Groups().Get(L"rule").Get(0).Value();
				if(!codeParsers.Keys().Contains(name))
				{
					codeParsers.Add(name, rule);
				}
			}
			else if((match=regexParsingTree.Match(line)) && match->Success())
			{
				WString value=match->Groups().Get(L"value").Get(0).Value();
				parsingTree=value==L"enabled";
			}
			else if((match=regexParserText.Match(line)) && match->Success())
			{
				WString value=match->Groups().Get(L"value").Get(0).Value();
				parserText=value==L"enabled";
			}
			else if((match=regexAmbiguity.Match(line)) && match->Success())
			{
				WString value=match->Groups().Get(L"value").Get(0).Value();
				ambiguity=value==L"enabled";
			}
			else
			{
				Console::WriteLine(L"error> Unknown property \""+line+L".");
				return false;
			}
		}
		return true;
	}
};

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
	writer.WriteLine(L"本文件使用Vczh Parsing Generator工具自动生成");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
}

WString WriteFileBegin(const CodegenConfig& config, StreamWriter& writer)
{
	FOREACH(WString, include, config.codeIncludes)
	{
		writer.WriteLine(L"#include "+include);
	}
	writer.WriteLine(L"");
	WString prefix;
	FOREACH(WString, ns, config.codeNamespaces)
	{
		writer.WriteLine(prefix+L"namespace "+ns);
		writer.WriteLine(prefix+L"{");
		prefix+=L"\t";
	}
	return prefix;
}

void WriteFileEnd(const CodegenConfig& config, StreamWriter& writer)
{
	vint counter=config.codeNamespaces.Count();
	FOREACH(WString, ns, config.codeNamespaces)
	{
		counter--;
		for(vint i=0;i<counter;i++) writer.WriteChar(L'\t');
		writer.WriteLine(L"}");
	}
}

/***********************************************************************
Helper Functions
***********************************************************************/

extern void PrintType(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer);
extern void PrintTypeForValue(ParsingDefinitionType* _type, ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _codeClassPrefix, TextWriter& _writer);
extern void PrintTypeDefinitions(List<Ptr<ParsingDefinitionTypeDefinition>>& types, const WString& prefix, ParsingSymbol* scope, ParsingSymbolManager* manager, const WString& codeClassPrefix, TextWriter& writer);
extern bool PrintType(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer);
extern void PrintTypeForValue(ParsingSymbol* type, const WString& codeClassPrefix, TextWriter& writer);

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

/***********************************************************************
Syntax Tree Declaration Generation
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
	ParsingSymbol*					scope;
	ParsingSymbolManager*			manager;
	WString							prefix;
	WString							codeClassPrefix;
	TextWriter&						writer;
	SortedList<ParsingSymbol*>		leafClasses;

	void LogInternal(ParsingDefinitionTypeDefinition* _this, ParsingDefinitionTypeDefinition* _definition, const WString& _prefix)
	{
		ParsingSymbol* oldScope=scope;
		WString oldPrefix=prefix;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_this->name);
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
		EnumerateAllLeafClass(manager, leafClasses);
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
		List<ParsingSymbol*> children;
		ParsingSymbol* thisType=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(node->name);
		SearchChildClasses(thisType, manager->GetGlobal(), manager, children);

		writer.WriteString(prefix);
		writer.WriteString(L"class ");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->name);
		if(children.Count()>0)
		{
			writer.WriteString(L" abstract");
		}
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
		if(children.Count()>0)
		{
			writer.WriteString(prefix);
			writer.WriteLine(L"\tclass IVisitor : public vl::Interface");
			writer.WriteString(prefix);
			writer.WriteLine(L"\t{");
			writer.WriteString(prefix);
			writer.WriteLine(L"\tpublic:");

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

		PrintTypeDefinitions(node->subTypes, prefix+L"\t", thisType, manager, codeClassPrefix, writer);

		for(int i=0;i<node->members.Count();i++)
		{
			LogInternal(node, node->members[i].Obj(), prefix+L"\t");
		}

		if(node->parentType)
		{
			writer.WriteLine(L"");
			writer.WriteString(prefix);
			writer.WriteString(L"\tvoid Accept(");
			PrintType(thisType->GetDescriptorSymbol(), codeClassPrefix, writer);
			writer.WriteLine(L"::IVisitor* visitor)override;");
		}
		if(leafClasses.Contains(thisType))
		{
			writer.WriteLine(L"");
			writer.WriteString(prefix);
			writer.WriteString(L"\tstatic ");
			PrintTypeForValue(thisType, codeClassPrefix, writer);
			writer.WriteLine(L" Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);");
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
		writer.WriteString(L"struct ");
		writer.WriteString(codeClassPrefix);
		writer.WriteString(node->name);
		writer.WriteLine(L" abstract");
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
			LogInternal(node, node->members[i].Obj(), prefix+L"\t\t");
		}

		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		writer.WriteLine(L"};");
		writer.WriteString(prefix);
		writer.WriteLine(L"};");
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

void WriteHeaderFile(const WString& name, Ptr<ParsingDefinition> definition, Ptr<ParsingTable> table, const CodegenConfig& config, StreamWriter& writer)
{
	WriteFileComment(name, writer);
	if(config.codeGuard!=L"")
	{
		writer.WriteString(L"#ifndef ");
		writer.WriteLine(config.codeGuard);
		writer.WriteString(L"#define ");
		writer.WriteLine(config.codeGuard);
		writer.WriteLine(L"");
	}
	WString prefix=WriteFileBegin(config, writer);

	ParsingSymbolManager manager;
	{
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &manager, errors);
	}
	if(config.parsingTree)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"struct ");
		writer.WriteString(config.codeClassPrefix);
		writer.WriteLine(L"ParserTokenIndex abstract");
		writer.WriteString(prefix);
		writer.WriteLine(L"{");
		for(vint i=0;i<table->GetTokenCount();i++)
		{
			const ParsingTable::TokenInfo& info=table->GetTokenInfo(i);
			if(info.regexTokenIndex!=-1)
			{
				writer.WriteString(prefix);
				writer.WriteString(L"\tstatic const vl::vint ");
				writer.WriteString(info.name);
				writer.WriteString(L" = ");
				writer.WriteString(itow(info.regexTokenIndex));
				writer.WriteLine(L";");
			}
		}
		for(vint i=0;i<table->GetDiscardTokenCount();i++)
		{
			const ParsingTable::TokenInfo& info=table->GetDiscardTokenInfo(i);
			if(info.regexTokenIndex!=-1)
			{
				writer.WriteString(prefix);
				writer.WriteString(L"\tstatic const vl::vint ");
				writer.WriteString(info.name);
				writer.WriteString(L" = ");
				writer.WriteString(itow(info.regexTokenIndex));
				writer.WriteLine(L";");
			}
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"};");
		
		PrintTypeDefinitions(definition->types, prefix, 0, &manager, config.codeClassPrefix, writer);
	}

	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::WString ");
	writer.WriteString(config.codeClassPrefix);
	writer.WriteLine(L"GetParserTextBuffer();");

	if(config.parsingTree)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> ");
		writer.WriteString(config.codeClassPrefix);
		writer.WriteLine(L"ConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);");
	}

	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::Ptr<vl::parsing::tabling::ParsingTable> ");
	writer.WriteString(config.codeClassPrefix);
	writer.WriteLine(L"LoadTable();");

	writer.WriteLine(L"");
	FOREACH(WString, name, config.codeParsers.Keys())
	{
		ParsingSymbol* rule=manager.GetGlobal()->GetSubSymbolByName(config.codeParsers[name]);
		if(rule)
		{
			{
				writer.WriteString(prefix);
				writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeNode> ");
				writer.WriteString(config.codeClassPrefix);
				writer.WriteString(name);
				writer.WriteLine(L"AsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);");
			}
			if(config.parsingTree)
			{
				ParsingSymbol* type=rule->GetDescriptorSymbol();
				writer.WriteString(prefix);
				writer.WriteString(L"extern ");
				PrintTypeForValue(type, config.codeClassPrefix, writer);
				writer.WriteString(L" ");
				writer.WriteString(config.codeClassPrefix);
				writer.WriteString(name);
				writer.WriteLine(L"(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);");
			}
			writer.WriteLine(L"");
		}
	}

	WriteFileEnd(config, writer);
	if(config.codeGuard!=L"")
	{
		writer.WriteString(L"#endif");
	}
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

	void LogInternal(ParsingDefinitionTypeDefinition* _this, ParsingDefinitionTypeDefinition* _definition)
	{
		ParsingSymbol* oldScope=scope;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_this->name);
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
			LogInternal(node, node->subTypes[i].Obj());
		}
		for(int i=0;i<node->members.Count();i++)
		{
			LogInternal(node, node->members[i].Obj());
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
			writer.WriteLine(L"& value, const vl::collections::List<vl::regex::RegexToken>& tokens);");
		}
		writer.WriteLine(L"");
	}
}

void WriteGetParserTextBuffer(ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	writer.WriteString(prefix);
	writer.WriteString(L"vl::WString ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"GetParserTextBuffer()");

	writer.WriteString(prefix);
	writer.WriteLine(L"{");

	writer.WriteString(prefix);
	writer.WriteLine(L"\treturn parserTextBuffer;");

	writer.WriteString(prefix);
	writer.WriteLine(L"}");
	writer.WriteLine(L"");
}

void WriteConvertImpl(ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	SortedList<ParsingSymbol*> leafClasses;
	EnumerateAllLeafClass(manager, leafClasses);
	FOREACH(ParsingSymbol*, scope, leafClasses)
	{
		writer.WriteString(prefix);
		PrintTypeForValue(scope, codeClassPrefix, writer);
		writer.WriteString(L" ");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteLine(L"::Convert(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)");
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

class WriteFillFragmentVisitor : public Object, public ParsingDefinitionTypeDefinition::IVisitor
{
public:
	ParsingSymbol*			scope;
	ParsingSymbolManager*	manager;
	WString					prefix;
	WString					codeClassPrefix;
	TextWriter&				writer;

	void LogInternal(ParsingDefinitionTypeDefinition* _this, ParsingDefinitionTypeDefinition* _definition)
	{
		ParsingSymbol* oldScope=scope;

		scope=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(_this->name);
		if(!scope) scope=oldScope;
		_definition->Accept(this);

		scope=oldScope;
	}

	WriteFillFragmentVisitor(ParsingSymbol* _scope, ParsingSymbolManager* _manager, const WString& _prefix, const WString& _codeClassPrefix, TextWriter& _writer)
		:scope(_scope)
		,manager(_manager)
		,prefix(_prefix)
		,codeClassPrefix(_codeClassPrefix)
		,writer(_writer)
	{
	}

	void Visit(ParsingDefinitionClassMemberDefinition* node)override
	{
		if(node->unescapingFunction==L"")
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\tSetMember(tree->");
			writer.WriteString(node->name);
			writer.WriteString(L", obj->GetMember(L\"");
			writer.WriteString(node->name);
			writer.WriteLine(L"\"), tokens);");
		}
		else
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\tif(SetMember(tree->");
			writer.WriteString(node->name);
			writer.WriteString(L", obj->GetMember(L\"");
			writer.WriteString(node->name);
			writer.WriteLine(L"\"), tokens))");

			writer.WriteString(prefix);
			writer.WriteLine(L"\t{");
			writer.WriteString(prefix);
			writer.WriteString(L"\t\t");
			writer.WriteString(node->unescapingFunction);
			writer.WriteString(L"(tree->");
			writer.WriteString(node->name);
			writer.WriteLine(L", tokens);");
			writer.WriteString(prefix);
			writer.WriteLine(L"\t}");
		}
	}

	void Visit(ParsingDefinitionClassDefinition* node)override
	{
		ParsingSymbol* thisType=(scope?scope:manager->GetGlobal())->GetSubSymbolByName(node->name);
		writer.WriteString(prefix);
		writer.WriteString(L"void Fill(");
		PrintTypeForValue(thisType, codeClassPrefix, writer);
		writer.WriteLine(L" tree, vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)");
		writer.WriteString(prefix);
		writer.WriteLine(L"{");
		FOREACH(Ptr<ParsingDefinitionClassMemberDefinition>, member, node->members)
		{
			LogInternal(node, member.Obj());
		}
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
		writer.WriteLine(L"");
		FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, node->subTypes)
		{
			LogInternal(node, type.Obj());
		}
	}

	void Visit(ParsingDefinitionEnumMemberDefinition* node)override
	{
	}

	void Visit(ParsingDefinitionEnumDefinition* node)override
	{
	}
};

void WriteNodeConverterClassImplFragment(ParsingSymbolManager* manager, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	SortedList<ParsingSymbol*> leafClasses;
	EnumerateAllLeafClass(manager, leafClasses);
	FOREACH(ParsingSymbol*, scope, leafClasses)
	{
		writer.WriteString(L"if(obj->GetType()==L\"");
		writer.WriteString(GetTypeNameForCreateInstruction(scope));
		writer.WriteLine(L"\")");
		writer.WriteString(prefix);
		writer.WriteLine(L"{");

		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		PrintTypeForValue(scope, codeClassPrefix, writer);
		writer.WriteString(L" tree = new ");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteLine(L";");
		writer.WriteString(prefix);
		writer.WriteString(L"\t");
		writer.WriteLine(L"vl::collections::CopyFrom(tree->creatorRules, obj->GetCreatorRules());");

		ParsingSymbol* currentType=scope;
		while(currentType)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\tFill(");
			if(currentType==scope)
			{
				writer.WriteLine(L"tree, obj, tokens);");
			}
			else
			{
				writer.WriteString(L"tree.Cast<");
				PrintType(currentType, codeClassPrefix, writer);
				writer.WriteLine(L">(), obj, tokens);");
			}
			currentType=currentType->GetDescriptorSymbol();
		}
		
		writer.WriteString(prefix);
		writer.WriteLine(L"\treturn tree;");
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
		writer.WriteString(prefix);
		writer.WriteString(L"else ");
	}
}

void WriteSetEnumMemberImpl(ParsingSymbolManager* manager, ParsingSymbol* scope, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	if(scope->GetType()==ParsingSymbol::EnumType)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"bool SetMember(");
		PrintTypeForValue(scope, codeClassPrefix, writer);
		writer.WriteLine(L"& member, vl::Ptr<vl::parsing::ParsingTreeNode> node, const TokenList& tokens)");
		writer.WriteString(prefix);
		writer.WriteLine(L"{");
		writer.WriteString(prefix);
		writer.WriteLine(L"\tvl::Ptr<vl::parsing::ParsingTreeToken> token=node.Cast<vl::parsing::ParsingTreeToken>();");
		writer.WriteString(prefix);
		writer.WriteLine(L"\tif(token)");
		writer.WriteString(prefix);
		writer.WriteLine(L"\t{");
		writer.WriteString(prefix);
		writer.WriteString(L"\t\t");
		for(vint i=0;i<scope->GetSubSymbolCount();i++)
		{
			WString name=scope->GetSubSymbol(i)->GetName();
			writer.WriteString(L"if(token->GetValue()==L\"");
			writer.WriteString(name);
			writer.WriteString(L"\") { member=");
			PrintType(scope, codeClassPrefix, writer);
			writer.WriteString(L"::");
			writer.WriteString(name);
			writer.WriteLine(L"; return true; }");
			writer.WriteString(prefix);
			writer.WriteString(L"\t\telse ");
		}
		writer.WriteString(L"{ member=");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteString(L"::");
		writer.WriteString(scope->GetSubSymbol(0)->GetName());
		writer.WriteLine(L"; return false; }");
		writer.WriteString(prefix);
		writer.WriteLine(L"\t}");
		writer.WriteString(prefix);
		writer.WriteString(L"\tmember=");
		PrintType(scope, codeClassPrefix, writer);
		writer.WriteString(L"::");
		writer.WriteString(scope->GetSubSymbol(0)->GetName());
		writer.WriteLine(L";");
		writer.WriteString(prefix);
		writer.WriteLine(L"\treturn false;");
		writer.WriteString(prefix);
		writer.WriteLine(L"}");
		writer.WriteLine(L"");
	}
	for(vint i=0;i<scope->GetSubSymbolCount();i++)
	{
		WriteSetEnumMemberImpl(manager, scope->GetSubSymbol(i), prefix, codeClassPrefix, writer);
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
	WriteSetEnumMemberImpl(manager, manager->GetGlobal(), prefix+L"\t", codeClassPrefix, writer);
	{
		WriteFillFragmentVisitor visitor(0, manager, prefix+L"\t", codeClassPrefix, writer);
		FOREACH(Ptr<ParsingDefinitionTypeDefinition>, type, definition->types)
		{
			type->Accept(&visitor);
		}
	}
	writer.WriteString(prefix);
	writer.WriteLine(L"\tvl::Ptr<vl::parsing::ParsingTreeCustomBase> ConvertClass(vl::Ptr<vl::parsing::ParsingTreeObject> obj, const TokenList& tokens)override");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t{");
	writer.WriteString(prefix);
	writer.WriteString(L"\t\t");
	WriteNodeConverterClassImplFragment(manager, prefix+L"\t\t", codeClassPrefix, writer);
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
	writer.WriteLine(L"ConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens)");
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

void WriteCppString(const WString& text, TextWriter& writer)
{
	writer.WriteString(L"L\"");
	for(vint i=0;i<text.Length();i++)
	{
		switch(text[i])
		{
		case L'\r':
			writer.WriteString(L"\\r");
			break;
		case L'\n':
			writer.WriteString(L"\\n");
			break;
		case L'\t':
			writer.WriteString(L"\\t");
			break;
		case L'\\':
			writer.WriteString(L"\\\\");
			break;
		case L'\'':
			writer.WriteString(L"\\\'");
			break;
		case L'\"':
			writer.WriteString(L"\\\"");
			break;
		default:
			writer.WriteChar(text[i]);
		}
	}
	writer.WriteString(L"\"");
}

void WriteTable(Ptr<ParsingTable> table, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	writer.WriteString(prefix);
	writer.WriteString(L"vl::Ptr<vl::parsing::tabling::ParsingTable> ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"LoadTable()");
	writer.WriteString(prefix);
	writer.WriteLine(L"{");

	writer.WriteString(prefix);
	writer.WriteString(L"\tvl::Ptr<vl::parsing::tabling::ParsingTable> table=new vl::parsing::tabling::ParsingTable(");
	writer.WriteString(itow(table->GetTokenCount()));
	writer.WriteString(L"-vl::parsing::tabling::ParsingTable::UserTokenStart, ");
	writer.WriteString(itow(table->GetDiscardTokenCount()));
	writer.WriteString(L", ");
	writer.WriteString(itow(table->GetStateCount()));
	writer.WriteString(L", ");
	writer.WriteString(itow(table->GetRuleCount()));
	writer.WriteLine(L");");
	if(table->GetAmbiguity())
	{
		writer.WriteString(prefix);
		writer.WriteLine(L"table->SetAmbiguity(true);");
	}

	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define SET_TOKEN_INFO(INDEX, NAME, REGEX) table->SetTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define SET_DISCARD_TOKEN_INFO(INDEX, NAME, REGEX) table->SetDiscardTokenInfo(INDEX, vl::parsing::tabling::ParsingTable::TokenInfo(NAME, REGEX));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define SET_STATE_INFO(INDEX, RULE, STATE, EXPR) table->SetStateInfo(INDEX, vl::parsing::tabling::ParsingTable::StateInfo(RULE, STATE, EXPR));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define SET_RULE_INFO(INDEX, NAME, TYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, L\"\", STARTSTATE));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define SET_AMBIGUOUS_RULE_INFO(INDEX, NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE) table->SetRuleInfo(INDEX, vl::parsing::tabling::ParsingTable::RuleInfo(NAME, TYPE, AMBIGUOUSTYPE, STARTSTATE));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define BEGIN_TRANSITION_BAG(STATE, TOKEN) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionBag> bag=new vl::parsing::tabling::ParsingTable::TransitionBag; table->SetTransitionBag(STATE, TOKEN, bag);");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define BEGIN_TRANSITION_ITEM(TOKEN, TARGETSTATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::TransitionItem> item=new vl::parsing::tabling::ParsingTable::TransitionItem(TOKEN, TARGETSTATE); bag->transitionItems.Add(item);");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define END_TRANSITION_ITEM }");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define END_TRANSITION_BAG }");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define ITEM_STACK_PATTERN(STATE) item->stackPattern.Add(STATE);");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define ITEM_INSTRUCTION(TYPE, STATE, NAME, VALUE, RULE) item->instructions.Add(vl::parsing::tabling::ParsingTable::Instruction(vl::parsing::tabling::ParsingTable::Instruction::InstructionType::TYPE, STATE, NAME, VALUE, RULE));");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define BEGIN_LOOK_AHEAD(STATE) {vl::Ptr<vl::parsing::tabling::ParsingTable::LookAheadInfo> lookAheadInfo=new vl::parsing::tabling::ParsingTable::LookAheadInfo; item->lookAheads.Add(lookAheadInfo); lookAheadInfo->state=STATE;");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define LOOK_AHEAD(TOKEN) lookAheadInfo->tokens.Add(TOKEN);");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#define END_LOOK_AHEAD }");
	writer.WriteLine(L"");

	for(vint i=0;i<table->GetTokenCount();i++)
	{
		const ParsingTable::TokenInfo& info=table->GetTokenInfo(i);
		writer.WriteString(prefix);
		writer.WriteString(L"\tSET_TOKEN_INFO(");
		writer.WriteString(itow(i));
		writer.WriteString(L", ");
		WriteCppString(info.name, writer);
		writer.WriteString(L", ");
		WriteCppString(info.regex, writer);
		writer.WriteLine(L")");
	}
	writer.WriteLine(L"");

	for(vint i=0;i<table->GetDiscardTokenCount();i++)
	{
		const ParsingTable::TokenInfo& info=table->GetDiscardTokenInfo(i);
		writer.WriteString(prefix);
		writer.WriteString(L"\tSET_DISCARD_TOKEN_INFO(");
		writer.WriteString(itow(i));
		writer.WriteString(L", ");
		WriteCppString(info.name, writer);
		writer.WriteString(L", ");
		WriteCppString(info.regex, writer);
		writer.WriteLine(L")");
	}
	writer.WriteLine(L"");

	for(vint i=0;i<table->GetStateCount();i++)
	{
		const ParsingTable::StateInfo& info=table->GetStateInfo(i);
		writer.WriteString(prefix);
		writer.WriteString(L"\tSET_STATE_INFO(");
		writer.WriteString(itow(i));
		writer.WriteString(L", ");
		WriteCppString(info.ruleName, writer);
		writer.WriteString(L", ");
		WriteCppString(info.stateName, writer);
		writer.WriteString(L", ");
		WriteCppString(info.stateExpression, writer);
		writer.WriteLine(L")");
	}
	writer.WriteLine(L"");

	for(vint i=0;i<table->GetRuleCount();i++)
	{
		const ParsingTable::RuleInfo& info=table->GetRuleInfo(i);
		if(info.ambiguousType==L"")
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\tSET_RULE_INFO(");
			writer.WriteString(itow(i));
			writer.WriteString(L", ");
			WriteCppString(info.name, writer);
			writer.WriteString(L", ");
			WriteCppString(info.type, writer);
			writer.WriteString(L", ");
			writer.WriteString(itow(info.rootStartState));
			writer.WriteLine(L")");
		}
		else
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\tSET_AMBIGUOUS_RULE_INFO(");
			writer.WriteString(itow(i));
			writer.WriteString(L", ");
			WriteCppString(info.name, writer);
			writer.WriteString(L", ");
			WriteCppString(info.type, writer);
			writer.WriteString(L", ");
			WriteCppString(info.ambiguousType, writer);
			writer.WriteString(L", ");
			writer.WriteString(itow(info.rootStartState));
			writer.WriteLine(L")");
		}
	}
	writer.WriteLine(L"");

	for(vint i=0;i<table->GetStateCount();i++)
	{
		for(vint j=0;j<table->GetTokenCount();j++)
		{
			Ptr<ParsingTable::TransitionBag> bag=table->GetTransitionBag(i, j);
			if(bag)
			{
				writer.WriteString(prefix);
				writer.WriteString(L"\tBEGIN_TRANSITION_BAG(");
				writer.WriteString(itow(i));
				writer.WriteString(L", ");
				writer.WriteString(itow(j));
				writer.WriteLine(L")");
				writer.WriteLine(L"");
				for(vint k=0;k<bag->transitionItems.Count();k++)
				{
					Ptr<ParsingTable::TransitionItem> item=bag->transitionItems[k];
					writer.WriteString(prefix);
					writer.WriteString(L"\t\tBEGIN_TRANSITION_ITEM(");
					writer.WriteString(itow(item->token));
					writer.WriteString(L", ");
					writer.WriteString(itow(item->targetState));
					writer.WriteLine(L")");

					FOREACH(Ptr<ParsingTable::LookAheadInfo>, lookAheadInfo, item->lookAheads)
					{
						writer.WriteString(prefix);
						writer.WriteString(L"\t\tBEGIN_LOOK_AHEAD(");
						writer.WriteString(itow(lookAheadInfo->state));
						writer.WriteString(L") ");

						FOREACH(vint, token, lookAheadInfo->tokens)
						{
							writer.WriteString(L"LOOK_AHEAD(");
							writer.WriteString(itow(token));
							writer.WriteString(L") ");
						}
						writer.WriteLine(L"END_LOOK_AHEAD");
					}

					FOREACH(vint, state, item->stackPattern)
					{
						writer.WriteString(prefix);
						writer.WriteString(L"\t\tITEM_STACK_PATTERN(");
						writer.WriteString(itow(state));
						writer.WriteLine(L")");
					}

					FOREACH(ParsingTable::Instruction, ins, item->instructions)
					{
						writer.WriteString(prefix);
						writer.WriteString(L"\t\tITEM_INSTRUCTION(");
						switch(ins.instructionType)
						{
						case ParsingTable::Instruction::Create:
							writer.WriteString(L"Create");
							break;
						case ParsingTable::Instruction::Assign:
							writer.WriteString(L"Assign");
							break;
						case ParsingTable::Instruction::Item:
							writer.WriteString(L"Item");
							break;
						case ParsingTable::Instruction::Using:
							writer.WriteString(L"Using");
							break;
						case ParsingTable::Instruction::Setter:
							writer.WriteString(L"Setter");
							break;
						case ParsingTable::Instruction::Shift:
							writer.WriteString(L"Shift");
							break;
						case ParsingTable::Instruction::Reduce:
							writer.WriteString(L"Reduce");
							break;
						case ParsingTable::Instruction::LeftRecursiveReduce:
							writer.WriteString(L"LeftRecursiveReduce");
							break;
						}
						writer.WriteString(L", ");
						writer.WriteString(itow(ins.stateParameter));
						writer.WriteString(L", ");
						WriteCppString(ins.nameParameter, writer);
						writer.WriteString(L", ");
						WriteCppString(ins.value, writer);
						writer.WriteString(L", ");
						WriteCppString(ins.creatorRule, writer);
						writer.WriteLine(L");");
					}

					writer.WriteString(prefix);
					writer.WriteLine(L"\t\tEND_TRANSITION_ITEM");
					writer.WriteLine(L"");
				}
				writer.WriteString(prefix);
				writer.WriteLine(L"\tEND_TRANSITION_BAG");
				writer.WriteLine(L"");
			}
		}
	}
	
	writer.WriteString(prefix);
	writer.WriteLine(L"\ttable->Initialize();");
	writer.WriteString(prefix);
	writer.WriteLine(L"\treturn table;");

	writer.WriteLine(L"");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef SET_TOKEN_INFO");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef SET_DISCARD_TOKEN_INFO");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef SET_STATE_INFO");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef SET_RULE_INFO");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef SET_AMBIGUOUS_RULE_INFO");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef BEGIN_TRANSITION_BAG");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef BEGIN_TRANSITION_ITEM");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef END_TRANSITION_ITEM");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef END_TRANSITION_BAG");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef ITEM_STACK_PATTERN");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef ITEM_INSTRUCTION");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef BEGIN_LOOK_AHEAD");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef LOOK_AHEAD");
	writer.WriteString(prefix);
	writer.WriteLine(L"\t#undef END_LOOK_AHEAD");

	writer.WriteString(prefix);
	writer.WriteLine(L"}");
	writer.WriteLine(L"");
}

void WriteTable(const WString& parserCode, bool enableAmbiguity, const WString& prefix, const WString& codeClassPrefix, TextWriter& writer)
{
	writer.WriteString(prefix);
	writer.WriteString(L"vl::Ptr<vl::parsing::tabling::ParsingTable> ");
	writer.WriteString(codeClassPrefix);
	writer.WriteLine(L"LoadTable()");
	writer.WriteString(prefix);
	writer.WriteLine(L"{");

	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"vl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateBootstrapStrictParser();");
	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"vl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;");
	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"vl::Ptr<vl::parsing::ParsingTreeNode> definitionNode=parser->Parse(parserTextBuffer, L\"ParserDecl\", errors);");
	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"vl::Ptr<vl::parsing::definitions::ParsingDefinition> definition=vl::parsing::definitions::DeserializeDefinition(definitionNode);");
	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"vl::Ptr<vl::parsing::tabling::ParsingTable> table=vl::parsing::analyzing::GenerateTable(definition, "+WString(enableAmbiguity?L"true":L"false")+L", errors);");
	writer.WriteString(prefix+L"    ");
	writer.WriteLine(L"return table;");

	writer.WriteString(prefix);
	writer.WriteLine(L"}");
	writer.WriteLine(L"");
}

/***********************************************************************
Grammar Text Generation
***********************************************************************/

void WriteParserText(const WString& parserText, TextWriter& writer)
{
	writer.WriteLine(L"const wchar_t parserTextBuffer[] = ");
	{
		StringReader reader(parserText);
		while(!reader.IsEnd())
		{
			writer.WriteString(L"L\"\\r\\n\"");
			WString line=reader.ReadLine();
			WriteCppString(line, writer);
			writer.WriteLine(L"");
		}
	}
	writer.WriteLine(L";");
}

/***********************************************************************
Parser Function Generation
***********************************************************************/

void WriteParserFunctions(ParsingSymbolManager* manager, const WString& prefix, const CodegenConfig& config, TextWriter& writer)
{
	FOREACH(WString, name, config.codeParsers.Keys())
	{
		ParsingSymbol* rule=manager->GetGlobal()->GetSubSymbolByName(config.codeParsers[name]);
		if(rule)
		{
			{
				writer.WriteString(prefix);
				writer.WriteString(L"vl::Ptr<vl::parsing::ParsingTreeNode> ");
				writer.WriteString(config.codeClassPrefix);
				writer.WriteString(name);
				writer.WriteLine(L"AsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)");
				writer.WriteString(prefix);
				writer.WriteLine(L"{");
			
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::parsing::tabling::ParsingState state(input, table);");
				writer.WriteString(prefix);
				writer.WriteString(L"\tstate.Reset(L\"");
				writer.WriteString(rule->GetName());
				writer.WriteLine(L"\");");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);");

				writer.WriteString(prefix);
				writer.WriteLine(L"\treturn node;");
				writer.WriteString(prefix);
				writer.WriteLine(L"}");
				writer.WriteLine(L"");
			}
			if(config.parsingTree)
			{
				ParsingSymbol* type=rule->GetDescriptorSymbol();
				writer.WriteString(prefix);
				PrintTypeForValue(type, config.codeClassPrefix, writer);
				writer.WriteString(L" ");
				writer.WriteString(config.codeClassPrefix);
				writer.WriteString(name);
				writer.WriteLine(L"(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table)");
				writer.WriteString(prefix);
				writer.WriteLine(L"{");
			
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::parsing::tabling::ParsingState state(input, table);");
				writer.WriteString(prefix);
				writer.WriteString(L"\tstate.Reset(L\"");
				writer.WriteString(rule->GetName());
				writer.WriteLine(L"\");");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::Ptr<vl::parsing::tabling::ParsingGeneralParser> parser=vl::parsing::tabling::CreateStrictParser(table);");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::collections::List<vl::Ptr<vl::parsing::ParsingError>> errors;");
				writer.WriteString(prefix);
				writer.WriteLine(L"\tvl::Ptr<vl::parsing::ParsingTreeNode> node=parser->Parse(state, errors);");

				writer.WriteString(prefix);
				writer.WriteLine(L"\tif(node)");
				writer.WriteString(prefix);
				writer.WriteLine(L"\t{");
				writer.WriteString(prefix);
				writer.WriteString(L"\t\treturn ");
				writer.WriteString(config.codeClassPrefix);
				writer.WriteString(L"ConvertParsingTreeNode(node, state.GetTokens()).Cast<");
				PrintType(type, config.codeClassPrefix, writer);
				writer.WriteLine(L">();");
				writer.WriteString(prefix);
				writer.WriteLine(L"\t}");
			
				writer.WriteString(prefix);
				writer.WriteLine(L"\treturn 0;");
				writer.WriteString(prefix);
				writer.WriteLine(L"}");
				writer.WriteLine(L"");
			}
		}
	}
}

void WriteCppFile(const WString& name, const WString& parserCode, Ptr<ParsingDefinition> definition, Ptr<ParsingTable> table, const CodegenConfig& config, StreamWriter& writer)
{
	WString prefix=WriteFileBegin(config, writer);

	ParsingSymbolManager manager;
	{
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &manager, errors);
	}

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"ParserText");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteParserText(parserCode, writer);
	writer.WriteLine(L"");
	WriteGetParserTextBuffer(&manager, prefix, config.codeClassPrefix, writer);

	if(config.parsingTree)
	{
		writer.WriteLine(L"/***********************************************************************");
		writer.WriteLine(L"Unescaping Function Foward Declarations");
		writer.WriteLine(L"***********************************************************************/");
		writer.WriteLine(L"");
		WriteUnescapingFunctionForwardDeclarations(definition, &manager, prefix, config.codeClassPrefix, writer);

		writer.WriteLine(L"/***********************************************************************");
		writer.WriteLine(L"Parsing Tree Conversion Driver Implementation");
		writer.WriteLine(L"***********************************************************************/");
		writer.WriteLine(L"");
		WriteNodeConverterClassImpl(definition, &manager, prefix, config.codeClassPrefix, writer);

		writer.WriteLine(L"/***********************************************************************");
		writer.WriteLine(L"Parsing Tree Conversion Implementation");
		writer.WriteLine(L"***********************************************************************/");
		writer.WriteLine(L"");
		WriteConvertImpl(&manager, prefix, config.codeClassPrefix, writer);

		writer.WriteLine(L"/***********************************************************************");
		writer.WriteLine(L"Visitor Pattern Implementation");
		writer.WriteLine(L"***********************************************************************/");
		writer.WriteLine(L"");
		WriteVisitorImpl(&manager, manager.GetGlobal(), prefix, config.codeClassPrefix, writer);
	}

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parser Function");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteParserFunctions(&manager, prefix, config, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Table Generation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	if(config.parserText)
	{
		WriteTable(parserCode, config.ambiguity, prefix, config.codeClassPrefix, writer);
	}
	else
	{
		WriteTable(table, prefix, config.codeClassPrefix, writer);
	}

	WriteFileEnd(config, writer);
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
	Ptr<ParsingGeneralParser> parser=CreateBootstrapStrictParser();

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

			CodegenConfig config;
			WString codeGrammar;
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

				if(!config.ReadConfig(reader))
				{
					goto STOP_PARSING;
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
				
				if(codeGrammar==L"<bootstrap-grammar>")
				{
					definition=CreateParserDefinition();
					MemoryStream bootstrapStream;
					{
						StreamWriter bootstrapWriter(bootstrapStream);
						Log(definition, bootstrapWriter);
					}
					bootstrapStream.SeekFromBegin(0);
					StreamReader bootstrapReader(bootstrapStream);
					codeGrammar=bootstrapReader.ReadToEnd();
				}
				else
				{
					definition=CreateDefinition(parser, codeGrammar, writer);
				}
				if(!definition)
				{
					Console::WriteLine(L"error> Error happened. Open \""+logPath+L" for details.");
					goto STOP_PARSING;
				}

				table=CreateTable(definition, writer, config.ambiguity);
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
				WriteHeaderFile(name, definition, table, config, writer);
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
				
				config.codeIncludes.Clear();
				config.codeIncludes.Add(L"\""+name+L".h\"");
				WriteCppFile(name, codeGrammar, definition, table, config, writer);
			}
		}
	STOP_PARSING:;
	}
	Console::WriteLine(L"Finished!");
	return 0;
}