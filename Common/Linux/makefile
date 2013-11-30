CPP = g++ -std=c++11 -finput-charset=gbk
CPPBOM = g++ -std=c++11

OBJ = ./obj/
BIN = ./exe/
VLPP = ../Source/
UNITTEST = ../UnitTest/UnitTest/

VLPP_STREAM_OBJS = $(OBJ)Accessor.o $(OBJ)BroadcastStream.o $(OBJ)CacheStream.o $(OBJ)CharFormat.o $(OBJ)FileStream.o $(OBJ)MemoryStream.o $(OBJ)MemoryWrapperStream.o $(OBJ)RecorderStream.o
VLPP_OBJS = $(OBJ)Basic.o $(OBJ)String.o $(OBJ)Console.o $(OBJ)UnitTest.o $(VLPP_STREAM_OBJS)

UNITTEST_OBJS = $(OBJ)TestBasic.o $(OBJ)TestString.o $(OBJ)TestList.o $(OBJ)TestFunction.o $(OBJ)TestStream.o

all:
	echo =====================================
	echo =====================================
	echo =====================================
	echo =====================================
	$(CPP)		-o $(OBJ)Basic.o				-c $(VLPP)Basic.cpp
	$(CPP)		-o $(OBJ)String.o				-c $(VLPP)String.cpp
	$(CPP)		-o $(OBJ)Console.o				-c $(VLPP)Console.cpp
	$(CPP)		-o $(OBJ)UnitTest.o				-c $(VLPP)UnitTest/UnitTest.cpp
	$(CPP)		-o $(OBJ)Accessor.o				-c $(VLPP)Stream/Accessor.cpp
	$(CPP)		-o $(OBJ)BroadcastStream.o		-c $(VLPP)Stream/BroadcastStream.cpp
	$(CPP)		-o $(OBJ)CacheStream.o			-c $(VLPP)Stream/CacheStream.cpp
	$(CPP)		-o $(OBJ)CharFormat.o			-c $(VLPP)Stream/CharFormat.cpp
	$(CPP)		-o $(OBJ)FileStream.o			-c $(VLPP)Stream/FileStream.cpp
	$(CPP)		-o $(OBJ)MemoryStream.o			-c $(VLPP)Stream/MemoryStream.cpp
	$(CPP)		-o $(OBJ)MemoryWrapperStream.o	-c $(VLPP)Stream/MemoryWrapperStream.cpp
	$(CPP)		-o $(OBJ)RecorderStream.o		-c $(VLPP)Stream/RecorderStream.cpp
	$(CPP)		-o $(OBJ)TestBasic.o			-c $(UNITTEST)TestBasic.cpp
	$(CPP)		-o $(OBJ)TestString.o			-c $(UNITTEST)TestString.cpp
	$(CPP)		-o $(OBJ)TestList.o				-c $(UNITTEST)TestList.cpp
	$(CPP)		-o $(OBJ)TestFunction.o			-c $(UNITTEST)TestFunction.cpp
	$(CPPBOM)	-o $(OBJ)TestStream.o			-c $(UNITTEST)TestStream.cpp
	$(CPP)		-o $(OBJ)Main.o					-c Main.cpp 
	$(CPP)		-o $(BIN)UnitTest $(OBJ)Main.o $(VLPP_OBJS) $(UNITTEST_OBJS)

clean:
	rm $(BIN)* $(OBJ)*
