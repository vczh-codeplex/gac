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

	WriteTokenDefinition(table, prefix, config.classPrefix, writer);
	WriteTypeDefinitions(definition->types, prefix, 0, &manager, config.classPrefix, writer);
	WriteMetaDefinition(prefix, config.classPrefix, writer);
	WriteParserFunctions(&manager, config.parsers, prefix, config.classPrefix, writer);

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