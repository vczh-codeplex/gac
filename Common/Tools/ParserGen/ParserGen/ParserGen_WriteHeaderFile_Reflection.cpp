#include "ParserGen.h"

void WriteTypeReflectionDeclaration(ParsingSymbolManager* manager, const WString& prefix, const CodegenConfig& config, TextWriter& writer)
{
	writer.WriteLine(L"#ifndef VCZH_DEBUG_NO_REFLECTION");
	List<ParsingSymbol*> types;
	EnumerateAllTypes(manager, manager->GetGlobal(), types);

	FOREACH(ParsingSymbol*, type, types)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"DECL_TYPE_INFO(");
		PrintNamespaces(config.codeNamespaces, writer);
		PrintType(type, config.classPrefix, writer);
		writer.WriteLine(L")");
	}
	writer.WriteLine(L"#endif");

	writer.WriteLine(L"");
	writer.WriteString(prefix);
	writer.WriteString(L"extern bool ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"LoadTypes();");
}