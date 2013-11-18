#include "ParserGen.h"

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
	WriteGetParserTextBuffer(&manager, prefix, config.classPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Unescaping Function Foward Declarations");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteUnescapingFunctionForwardDeclarations(definition, &manager, prefix, config.classPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parsing Tree Conversion Driver Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteNodeConverterClassImpl(definition, &manager, prefix, config.classPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parsing Tree Conversion Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteConvertImpl(&manager, prefix, config.classPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Visitor Pattern Implementation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteVisitorImpl(&manager, manager.GetGlobal(), prefix, config.classPrefix, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Parser Function");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");
	WriteParserFunctions(&manager, prefix, config, writer);

	writer.WriteLine(L"/***********************************************************************");
	writer.WriteLine(L"Table Generation");
	writer.WriteLine(L"***********************************************************************/");
	writer.WriteLine(L"");

	WriteTable(parserCode, config.ambiguity, prefix, config.classPrefix, writer);

	WriteFileEnd(config, writer);
}