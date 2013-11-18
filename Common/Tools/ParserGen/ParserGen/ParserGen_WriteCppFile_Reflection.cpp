#include "ParserGen.h"

void WriteTypeReflectionImplementation(ParsingSymbolManager* manager, const WString& prefix, const CodegenConfig& config, TextWriter& writer)
{
	writer.WriteLine(L"#ifndef VCZH_DEBUG_NO_REFLECTION");
	if (config.codeNamespaces.Count() > 0)
	{
		writer.WriteString(prefix);
		FOREACH_INDEXER(WString, ns, index, config.codeNamespaces)
		{
			writer.WriteString(index == 0 ? L"using namespace " : L"::");
			writer.WriteString(ns);
		}
		writer.WriteLine(L";");
	}

	SortedList<ParsingSymbol*> types;
	EnumerateAllTypes(manager, manager->GetGlobal(), types);
	
	writer.WriteLine(L"");
	FOREACH(ParsingSymbol*, type, types)
	{
		writer.WriteString(prefix);
		writer.WriteString(L"IMPL_TYPE_INFO_RENAME(");
		PrintType(type, config.classPrefix, writer);
		writer.WriteString(L", ");
		PrintNamespaces(config.reflectionNamespaces, writer);
		PrintType(type, config.classPrefix, writer);
		writer.WriteLine(L")");
	}

	FOREACH(ParsingSymbol*, type, types)
	{
		switch (type->GetType())
		{
		case ParsingSymbol::EnumType:
			{
				writer.WriteLine(L"");
				writer.WriteString(prefix);
				writer.WriteString(L"BEGIN_ENUM_ITEM(");
				PrintType(type, config.classPrefix, writer);
				writer.WriteLine(L")");

				writer.WriteString(prefix);
				writer.WriteString(L"\tENUM_ITEM_NAMESPACE(");
				PrintType(type, config.classPrefix, writer);
				writer.WriteLine(L")");

				vint count = type->GetSubSymbolCount();
				for (vint i = 0; i < count; i++)
				{
					writer.WriteString(prefix);
					writer.WriteString(L"\tENUM_NAMESPACE_ITEM(");
					writer.WriteString(type->GetSubSymbol(i)->GetName());
					writer.WriteLine(L")");
				}

				writer.WriteString(prefix);
				writer.WriteString(L"END_ENUM_ITEM(");
				PrintType(type, config.classPrefix, writer);
				writer.WriteLine(L")");
			}
			break;
		case ParsingSymbol::ClassType:
			{
				writer.WriteLine(L"");
				writer.WriteString(prefix);
				writer.WriteString(L"BEGIN_CLASS_MEMBER(");
				PrintType(type, config.classPrefix, writer);
				writer.WriteLine(L")");

				if (auto parent = type->GetDescriptorSymbol())
				{
					writer.WriteString(prefix);
					writer.WriteString(L"\tCLASS_MEMBER_BASE(");
					PrintType(parent, config.classPrefix, writer);
					writer.WriteLine(L")");
					writer.WriteLine(L"");
				}

				vint count = type->GetSubSymbolCount();

				for (vint i = 0; i < count; i++)
				{
					ParsingSymbol* field = type->GetSubSymbol(i);
					if (field->GetType() == ParsingSymbol::ClassField)
					{
						ParsingSymbol* fieldType = field->GetDescriptorSymbol();
						if (fieldType->GetType() == ParsingSymbol::TokenType)
						{
							writer.WriteString(prefix);
							writer.WriteString(L"\tCLASS_MEMBER_EXTERNALMETHOD(get_");
							writer.WriteString(field->GetName());
							writer.WriteString(L", NO_PARAMETER, vl::WString(");
							PrintType(type, config.classPrefix, writer);
							writer.WriteString(L"::*)(), [](");
							PrintType(type, config.classPrefix, writer);
							writer.WriteString(L"* node){ return node->");
							writer.WriteString(field->GetName());
							writer.WriteLine(L".value; })");
							
							writer.WriteString(prefix);
							writer.WriteString(L"\tCLASS_MEMBER_EXTERNALMETHOD(set_");
							writer.WriteString(field->GetName());
							writer.WriteString(L", {L\"value\"}, void(");
							PrintType(type, config.classPrefix, writer);
							writer.WriteString(L"::*)(const vl::WString&), [](");
							PrintType(type, config.classPrefix, writer);
							writer.WriteString(L"* node, const vl::WString& value){ node->");
							writer.WriteString(field->GetName());
							writer.WriteLine(L".value = value; })");
						}
					}
				}
				writer.WriteLine(L"");

				for (vint i = 0; i < count; i++)
				{
					ParsingSymbol* field = type->GetSubSymbol(i);
					if (field->GetType() == ParsingSymbol::ClassField)
					{
						ParsingSymbol* fieldType = field->GetDescriptorSymbol();
						if (fieldType->GetType() == ParsingSymbol::TokenType)
						{
							writer.WriteString(prefix);
							writer.WriteString(L"\tCLASS_MEMBER_PROPERTY(");
							writer.WriteString(field->GetName());
							writer.WriteString(L", get_");
							writer.WriteString(field->GetName());
							writer.WriteString(L", set_");
							writer.WriteString(field->GetName());
							writer.WriteLine(L")");
						}
						else
						{
							writer.WriteString(prefix);
							writer.WriteString(L"\tCLASS_MEMBER_FIELD(");
							writer.WriteString(field->GetName());
							writer.WriteLine(L")");
						}
					}
				}

				writer.WriteString(prefix);
				writer.WriteString(L"END_CLASS_MEMBER(");
				PrintType(type, config.classPrefix, writer);
				writer.WriteLine(L")");
			}
			break;
		}
	}
	writer.WriteLine(L"#endif");
}