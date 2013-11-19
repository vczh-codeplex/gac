#include "ParserGen.h"

void WriteTypeReflectionDeclaration(ParsingSymbolManager* manager, const WString& prefix, const CodegenConfig& config, TextWriter& writer)
{
	writer.WriteLine(L"#ifndef VCZH_DEBUG_NO_REFLECTION");
	List<ParsingSymbol*> types, leafClasses;
	EnumerateAllTypes(manager, manager->GetGlobal(), types);
	EnumerateAllLeafClass(manager, manager->GetGlobal(), leafClasses);

	FOREACH(ParsingSymbol*, type, types)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"DECL_TYPE_INFO(");
		PrintNamespaces(config.codeNamespaces, writer);
		PrintType(type, config.classPrefix, writer);
		writer.WriteLine(L")");
	}

	FOREACH(ParsingSymbol*, type, types)
	{
		if (type->GetType() == ParsingSymbol::ClassType && !type->GetDescriptorSymbol() && !leafClasses.Contains(type))
		{
			writer.WriteString(prefix);
			writer.WriteString(L"DECL_TYPE_INFO(");
			PrintNamespaces(config.codeNamespaces, writer);
			PrintType(type, config.classPrefix, writer);
			writer.WriteLine(L"::IVisitor)");
		}
	}
	writer.WriteLine(L"#endif");

	writer.WriteLine(L"");
	writer.WriteString(prefix);
	writer.WriteString(L"extern bool ");
	writer.WriteString(config.classPrefix);
	writer.WriteLine(L"LoadTypes();");
}