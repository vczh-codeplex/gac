CPP = g++ -std=c++11 -finput-charset=gbk
CPPBOM = g++ -std=c++11

OBJ = ./obj/
BIN = ./exe/
VLPP = ../Source/
UNITTEST = ../UnitTest/UnitTest/

VLPP_STREAM_OBJS = $(OBJ)Accessor.o $(OBJ)BroadcastStream.o $(OBJ)CacheStream.o $(OBJ)CharFormat.o $(OBJ)FileStream.o $(OBJ)MemoryStream.o $(OBJ)MemoryWrapperStream.o $(OBJ)RecorderStream.o

VLPP_REGEX_OBJS = $(OBJ)Regex.o $(OBJ)RegexAutomaton.o $(OBJ)RegexData.o $(OBJ)RegexExpression.o $(OBJ)RegexParser.o $(OBJ)RegexPure.o $(OBJ)RegexRich.o $(OBJ)RegexWriter.o

VLPP_REFLECTION_OBJS = $(OBJ)GuiTypeDescriptor.o $(OBJ)GuiTypeDescriptorBuilder.o $(OBJ)GuiTypeDescriptorPredefined.o

VLPP_OBJS = $(OBJ)Basic.o $(OBJ)String.o $(OBJ)Console.o $(OBJ)GlobalStorage.o $(OBJ)Exception.o $(OBJ)Locale.o $(OBJ)UnitTest.o $(VLPP_STREAM_OBJS) $(VLPP_REGEX_OBJS) $(VLPP_REFLECTION_OBJS)

UNITTEST_OBJS = $(OBJ)TestBasic.o $(OBJ)TestString.o $(OBJ)TestList.o $(OBJ)TestFunction.o $(OBJ)TestGlobalStorage.o $(OBJ)TestStream.o $(OBJ)TestRegex.o

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
	$(CPP)		-o $(OBJ)TestBasic.o			-c $(UNITTEST)TestBasic.cpp
	$(CPP)		-o $(OBJ)TestString.o			-c $(UNITTEST)TestString.cpp
	$(CPP)		-o $(OBJ)TestList.o			-c $(UNITTEST)TestList.cpp
	$(CPP)		-o $(OBJ)TestFunction.o			-c $(UNITTEST)TestFunction.cpp
	$(CPPBOM)	-o $(OBJ)TestGlobalStorage.o		-c $(UNITTEST)TestGlobalStorage.cpp
	$(CPPBOM)	-o $(OBJ)TestStream.o			-c $(UNITTEST)TestStream.cpp
	$(CPPBOM)	-o $(OBJ)TestRegex.o			-c $(UNITTEST)TestRegex.cpp
	$(CPPBOM)	-o $(OBJ)TestReflection.o		-c $(UNITTEST)TestReflection.cpp
	$(CPP)		-o $(OBJ)Main.o				-c Main.cpp 
	$(CPP)		-o $(BIN)UnitTest $(OBJ)Main.o $(VLPP_OBJS) $(UNITTEST_OBJS)

clean:
	rm $(BIN)* $(OBJ)*
