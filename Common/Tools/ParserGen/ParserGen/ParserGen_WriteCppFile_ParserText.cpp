#include "ParserGen.h"

/***********************************************************************
WriteGetParserTextBuffer
***********************************************************************/

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

/***********************************************************************
WriteParserText
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