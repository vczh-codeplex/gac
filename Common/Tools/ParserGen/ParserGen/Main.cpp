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

Ptr<ParsingTable> CreateTable(Ptr<ParsingStrictParser> parser, const WString& grammar, StreamWriter& writer)
{
	ParsingSymbolManager symbolManager;
	List<Ptr<ParsingError>> errors;

	Ptr<ParsingTreeNode> definitionNode=parser->Parse(grammar, L"ParserDecl", errors);
	CheckError;

	Ptr<ParsingDefinition> definition=DeserializeDefinition(definitionNode);
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

			Ptr<ParsingTable> table;
			{
				FileStream fileStream(logPath, FileStream::WriteOnly);
				BomEncoder encoder(BomEncoder::Utf16);
				EncoderStream encoderStream(fileStream, encoder);
				StreamWriter writer(encoderStream);
				table=CreateTable(parser, codeGrammar, writer);

				if(!table)
				{
					Console::WriteLine(L"error> Error happened. Open \""+logPath+L" for details.");
					goto STOP_PARSING;
				}
			}
		}
	STOP_PARSING:;
	}
	Console::WriteLine(L"Finished!");
	return 0;
}