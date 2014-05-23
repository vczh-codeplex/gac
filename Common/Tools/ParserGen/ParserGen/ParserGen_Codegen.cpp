#include "ParserGen.h"

void WriteFileComment(const WString& name, StreamWriter& writer)
{
	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Vczh Library++ 3.0");
	writer.WriteLine(L"Developer: Zihan Chen(vczh)");
	writer.WriteLine(L"Parser::"+name);
	writer.WriteLine(L"");
	writer.WriteLine(L"This file is generated by: Vczh Parser Generator");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
}

WString WriteFileBegin(const CodegenConfig& config, StreamWriter& writer)
{
	FOREACH(WString, include, config.includes)
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

void WriteCppString(const WString& text, TextWriter& writer, bool rawString)
{
	if (rawString)
	{
		writer.WriteString(L"LR\"VLPP_PARSER_GEN(");
		writer.WriteString(text);
		writer.WriteString(L")VLPP_PARSER_GEN\"");
	}
	else
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
}