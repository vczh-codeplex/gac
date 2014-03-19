CPP = g++ -std=c++11 -finput-charset=gbk
CPPBOM = g++ -std=c++11

OBJ = ./obj/
BIN = ./exe/
VLPP = ../../../../Common/Source/
WORKFLOW = ../../Source/
UNITTEST = ../WorkflowSrc/

VLPP_STREAM_OBJS = $(OBJ)Accessor.o $(OBJ)BroadcastStream.o $(OBJ)CacheStream.o $(OBJ)CharFormat.o $(OBJ)FileStream.o $(OBJ)MemoryStream.o $(OBJ)MemoryWrapperStream.o $(OBJ)RecorderStream.o

VLPP_REGEX_OBJS = $(OBJ)Regex.o $(OBJ)RegexAutomaton.o $(OBJ)RegexData.o $(OBJ)RegexExpression.o $(OBJ)RegexParser.o $(OBJ)RegexPure.o $(OBJ)RegexRich.o $(OBJ)RegexWriter.o

VLPP_REFLECTION_OBJS = $(OBJ)GuiTypeDescriptor.o $(OBJ)GuiTypeDescriptorBuilder.o $(OBJ)GuiTypeDescriptorPredefined.o

VLPP_PARSING_OBJS = $(OBJ)Parsing.o $(OBJ)ParsingAnalyzer.o $(OBJ)ParsingAutomaton.o $(OBJ)ParsingAutomaton_Closure.o $(OBJ)ParsingAutomaton_EPDA.o $(OBJ)ParsingAutomaton_GenerateTable.o $(OBJ)ParsingAutomaton_JPDA.o $(OBJ)ParsingAutomaton_MergeStates.o $(OBJ)ParsingAutomaton_NPDA.o $(OBJ)ParsingDefinitions.o $(OBJ)ParsingDefinitions_CreateParserDefinition.o $(OBJ)ParsingLogging.o $(OBJ)ParsingState.o $(OBJ)ParsingTable.o $(OBJ)ParsingTree.o $(OBJ)ParsingJson.o $(OBJ)ParsingJson_Parser.o $(OBJ)ParsingXml.o $(OBJ)ParsingXml_Parser.o

VLPP_OBJS = $(OBJ)Basic.o $(OBJ)String.o $(OBJ)Console.o $(OBJ)GlobalStorage.o $(OBJ)Exception.o $(OBJ)Locale.o $(OBJ)UnitTest.o $(VLPP_STREAM_OBJS) $(VLPP_REGEX_OBJS) $(VLPP_REFLECTION_OBJS) $(VLPP_PARSING_OBJS)

WORKFLOW_OBJS = $(OBJ)WfExpression.o $(OBJ)WfExpressionParser.o $(OBJ)WfRuntime.o $(OBJ)WfRuntime_Execution.o $(OBJ)WfAnalyzer.o $(OBJ)WfAnalyzer_BuildScope.o $(OBJ)WfAnalyzer_Errors.o $(OBJ)WfAnalyzer_GenerateAssembly.o $(OBJ)WfAnalyzer_Misc.o $(OBJ)WfAnalyzer_SearchOrderedName.o $(OBJ)WfAnalyzer_TypeInfo.o $(OBJ)WfAnalyzer_ValidateSemantic.o $(OBJ)WfAnalyzer_ValidateStructure.o

UNITTEST_OBJS = $(OBJ)TestSamples.o $(OBJ)TestAnalyzer.o $(OBJ)TestCodegen.o

all:
	echo =====================================
	echo =====================================
	echo =====================================
	echo =====================================
	$(CPP)		-o $(OBJ)Basic.o			-c $(VLPP)Basic.cpp
	$(CPP)		-o $(OBJ)String.o			-c $(VLPP)String.cpp
	$(CPP)		-o $(OBJ)Console.o			-c $(VLPP)Console.cpp
	$(CPP)		-o $(OBJ)GlobalStorage.o		-c $(VLPP)GlobalStorage.cpp
	$(CPP)		-o $(OBJ)Exception.o			-c $(VLPP)Exception.cpp
	$(CPP)		-o $(OBJ)Locale.o			-c $(VLPP)Locale.cpp
	$(CPP)		-o $(OBJ)UnitTest.o			-c $(VLPP)UnitTest/UnitTest.cpp
	$(CPP)		-o $(OBJ)Accessor.o			-c $(VLPP)Stream/Accessor.cpp
	$(CPP)		-o $(OBJ)BroadcastStream.o		-c $(VLPP)Stream/BroadcastStream.cpp
	$(CPP)		-o $(OBJ)CacheStream.o			-c $(VLPP)Stream/CacheStream.cpp
	$(CPP)		-o $(OBJ)CharFormat.o			-c $(VLPP)Stream/CharFormat.cpp
	$(CPP)		-o $(OBJ)FileStream.o			-c $(VLPP)Stream/FileStream.cpp
	$(CPP)		-o $(OBJ)MemoryStream.o			-c $(VLPP)Stream/MemoryStream.cpp
	$(CPP)		-o $(OBJ)MemoryWrapperStream.o		-c $(VLPP)Stream/MemoryWrapperStream.cpp
	$(CPP)		-o $(OBJ)RecorderStream.o		-c $(VLPP)Stream/RecorderStream.cpp
	$(CPP)		-o $(OBJ)Regex.o			-c $(VLPP)Regex/Regex.cpp
	$(CPP)		-o $(OBJ)RegexAutomaton.o		-c $(VLPP)Regex/RegexAutomaton.cpp
	$(CPP)		-o $(OBJ)RegexData.o			-c $(VLPP)Regex/RegexData.cpp
	$(CPP)		-o $(OBJ)RegexExpression.o		-c $(VLPP)Regex/RegexExpression.cpp
	$(CPP)		-o $(OBJ)RegexParser.o			-c $(VLPP)Regex/RegexParser.cpp
	$(CPP)		-o $(OBJ)RegexPure.o			-c $(VLPP)Regex/RegexPure.cpp
	$(CPP)		-o $(OBJ)RegexRich.o			-c $(VLPP)Regex/RegexRich.cpp
	$(CPP)		-o $(OBJ)RegexWriter.o			-c $(VLPP)Regex/RegexWriter.cpp
	$(CPP)		-o $(OBJ)GuiTypeDescriptor.o		-c $(VLPP)Reflection/GuiTypeDescriptor.cpp
	$(CPP)		-o $(OBJ)GuiTypeDescriptorBuilder.o	-c $(VLPP)Reflection/GuiTypeDescriptorBuilder.cpp
	$(CPP)		-o $(OBJ)GuiTypeDescriptorPredefined.o	-c $(VLPP)Reflection/GuiTypeDescriptorPredefined.cpp
	$(CPP)		-o $(OBJ)Parsing.o			-c $(VLPP)Parsing/Parsing.cpp
	$(CPP)		-o $(OBJ)ParsingAnalyzer.o		-c $(VLPP)Parsing/ParsingAnalyzer.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton.o		-c $(VLPP)Parsing/ParsingAutomaton.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_Closure.o		-c $(VLPP)Parsing/ParsingAutomaton_Closure.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_EPDA.o		-c $(VLPP)Parsing/ParsingAutomaton_EPDA.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_GenerateTable.o	-c $(VLPP)Parsing/ParsingAutomaton_GenerateTable.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_JPDA.o		-c $(VLPP)Parsing/ParsingAutomaton_JPDA.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_MergeStates.o		-c $(VLPP)Parsing/ParsingAutomaton_MergeStates.cpp
	$(CPP)		-o $(OBJ)ParsingAutomaton_NPDA.o		-c $(VLPP)Parsing/ParsingAutomaton_NPDA.cpp
	$(CPP)		-o $(OBJ)ParsingDefinitions.o		-c $(VLPP)Parsing/ParsingDefinitions.cpp
	$(CPP)		-o $(OBJ)ParsingDefinitions_CreateParserDefinition.o		-c $(VLPP)Parsing/ParsingDefinitions_CreateParserDefinition.cpp
	$(CPP)		-o $(OBJ)ParsingLogging.o		-c $(VLPP)Parsing/ParsingLogging.cpp
	$(CPP)		-o $(OBJ)ParsingState.o			-c $(VLPP)Parsing/ParsingState.cpp
	$(CPP)		-o $(OBJ)ParsingTable.o			-c $(VLPP)Parsing/ParsingTable.cpp
	$(CPP)		-o $(OBJ)ParsingTree.o			-c $(VLPP)Parsing/ParsingTree.cpp
	$(CPP)		-o $(OBJ)ParsingJson.o			-c $(VLPP)Parsing/Json/ParsingJson.cpp
	$(CPP)		-o $(OBJ)ParsingJson_Parser.o		-c $(VLPP)Parsing/Json/ParsingJson_Parser.cpp
	$(CPP)		-o $(OBJ)ParsingXml.o			-c $(VLPP)Parsing/Xml/ParsingXml.cpp
	$(CPP)		-o $(OBJ)ParsingXml_Parser.o		-c $(VLPP)Parsing/Xml/ParsingXml_Parser.cpp
	$(CPP)		-o $(OBJ)WfExpression.o			-c $(WORKFLOW)Expression/WfExpression.cpp
	$(CPP)		-o $(OBJ)WfExpressionParser.o		-c $(WORKFLOW)Expression/WfExpressionParser.cpp
	$(CPP)		-o $(OBJ)WfRuntime.o			-c $(WORKFLOW)Runtime/WfRuntime.cpp
	$(CPP)		-o $(OBJ)WfRuntime_Execution.o		-c $(WORKFLOW)Runtime/WfRuntime_Execution.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer.o			-c $(WORKFLOW)Analyzer/WfAnalyzer.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_BuildScope.o	-c $(WORKFLOW)Analyzer/WfAnalyzer_BuildScope.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_Errors.o		-c $(WORKFLOW)Analyzer/WfAnalyzer_Errors.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_GenerateAssembly.o	-c $(WORKFLOW)Analyzer/WfAnalyzer_GenerateAssembly.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_Misc.o		-c $(WORKFLOW)Analyzer/WfAnalyzer_Misc.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_SearchOrderedName.o	-c $(WORKFLOW)Analyzer/WfAnalyzer_SearchOrderedName.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_TypeInfo.o		-c $(WORKFLOW)Analyzer/WfAnalyzer_TypeInfo.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_ValidateSemantic.o	-c $(WORKFLOW)Analyzer/WfAnalyzer_ValidateSemantic.cpp
	$(CPP)		-o $(OBJ)WfAnalyzer_ValidateStructure.o	-c $(WORKFLOW)Analyzer/WfAnalyzer_ValidateStructure.cpp
	$(CPP)		-o $(OBJ)TestBasic.o			-c $(UNITTEST)TestSamples.cpp
	$(CPP)		-o $(OBJ)TestString.o			-c $(UNITTEST)TestAnalyzer.cpp
	$(CPP)		-o $(OBJ)TestList.o			-c $(UNITTEST)TestCodegen.cpp
	$(CPP)		-o $(BIN)UnitTest $(OBJ)Main.o $(VLPP_OBJS) $(WORKFLOW_OBJS) $(UNITTEST_OBJS)

clean:
	rm $(BIN)* $(OBJ)*
