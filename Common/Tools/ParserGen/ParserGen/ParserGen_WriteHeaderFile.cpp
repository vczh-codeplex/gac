#include "ParserGen.h"

void WriteHeaderFile(const WString& name, Ptr<ParsingDefinition> definition, Ptr<ParsingTable> table, const CodegenConfig& config, StreamWriter& writer)
{
	WriteFileComment(name, writer);
	if(config.guard!=L"")
	{
		writer.WriteString(L"#ifndef ");
		writer.WriteLine(config.guard);
		writer.WriteString(L"#define ");
		writer.WriteLine(config.guard);
		writer.WriteLine(L"");
	}
	WString prefix=WriteFileBegin(config, writer);

	ParsingSymbolManager manager;
	{
		List<Ptr<ParsingError>> errors;
		ValidateDefinition(definition, &manager, errors);
	}

	writer.WriteString(prefix);
	writer.WriteString(L"enum class ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"ParserTokenIndex");
	writer.WriteString(prefix);
	writer.WriteLine(L"{");
	for(vint i=0;i<table->GetTokenCount();i++)
	{
		const ParsingTable::TokenInfo& info=table->GetTokenInfo(i);
		if(info.regexTokenIndex!=-1)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\t");
			writer.WriteString(info.name);
			writer.WriteString(L" = ");
			writer.WriteString(itow(info.regexTokenIndex));
			writer.WriteLine(L",");
		}
	}
	for(vint i=0;i<table->GetDiscardTokenCount();i++)
	{
		const ParsingTable::TokenInfo& info=table->GetDiscardTokenInfo(i);
		if(info.regexTokenIndex!=-1)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"\t");
			writer.WriteString(info.name);
			writer.WriteString(L" = ");
			writer.WriteString(itow(info.regexTokenIndex));
			writer.WriteLine(L",");
		}
	}
	writer.WriteString(prefix);
	writer.WriteLine(L"};");
		
	PrintTypeDefinitions(definition->types, prefix, 0, &manager, config.classPrefix, writer);

	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::WString ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"GetParserTextBuffer();");

	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeCustomBase> ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"ConvertParsingTreeNode(vl::Ptr<vl::parsing::ParsingTreeNode> node, const vl::collections::List<vl::regex::RegexToken>& tokens);");

	writer.WriteString(prefix);
	writer.WriteString(L"extern vl::Ptr<vl::parsing::tabling::ParsingTable> ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"LoadTable();");

	writer.WriteLine(L"");
	FOREACH(WString, name, config.parsers.Keys())
	{
		ParsingSymbol* rule=manager.GetGlobal()->GetSubSymbolByName(config.parsers[name]);
		if(rule)
		{
			writer.WriteString(prefix);
			writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeNode> ");
			writer.WriteString(config.classPrefix);
			writer.WriteString(name);
			writer.WriteLine(L"AsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);");
				
			writer.WriteString(prefix);
			writer.WriteString(L"extern vl::Ptr<vl::parsing::ParsingTreeNode> ");
			writer.WriteString(config.classPrefix);
			writer.WriteString(name);
			writer.WriteLine(L"AsParsingTreeNode(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);");

			ParsingSymbol* type=rule->GetDescriptorSymbol();
			writer.WriteString(prefix);
			writer.WriteString(L"extern ");
			PrintTypeForValue(type, config.classPrefix, writer);
			writer.WriteString(L" ");
			writer.WriteString(config.classPrefix);
			writer.WriteString(name);
			writer.WriteLine(L"(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table, vl::collections::List<vl::Ptr<vl::parsing::ParsingError>>& errors);");
				
			writer.WriteString(prefix);
			writer.WriteString(L"extern ");
			PrintTypeForValue(type, config.classPrefix, writer);
			writer.WriteString(L" ");
			writer.WriteString(config.classPrefix);
			writer.WriteString(name);
			writer.WriteLine(L"(const vl::WString& input, vl::Ptr<vl::parsing::tabling::ParsingTable> table);");

			writer.WriteLine(L"");
		}
	}

	WriteFileEnd(config, writer);
	if(config.guard!=L"")
	{
		writer.WriteString(L"#endif");
	}
}