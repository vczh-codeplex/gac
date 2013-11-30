CPP = g++ -std=c++11 -finput-charset=gbk
CPPBOM = g++ -std=c++11
OBJ = ./obj/
BIN = ./exe/
VLPP = ../Source/
UNITTEST = ../UnitTest/UnitTest/
VLPP_OBJS = $(OBJ)Basic.o $(OBJ)String.o $(OBJ)Console.o $(OBJ)UnitTest.o
UNITTEST_OBJS = $(OBJ)TestBasic.o

all:
	echo =====================================
	echo =====================================
	echo =====================================
	echo =====================================
	$(CPP)		-o $(OBJ)Basic.o		-c $(VLPP)Basic.cpp
	$(CPP)		-o $(OBJ)String.o		-c $(VLPP)String.cpp
	$(CPP)		-o $(OBJ)Console.o		-c $(VLPP)Console.cpp
	$(CPP)		-o $(OBJ)UnitTest.o		-c $(VLPP)UnitTest/UnitTest.cpp
	$(CPP)		-o $(OBJ)TestBasic.o	-c $(UNITTEST)TestBasic.cpp
	$(CPP)		-o $(OBJ)Main.o			-c Main.cpp 
	$(CPP)		-o $(BIN)UnitTest $(OBJ)Main.o $(VLPP_OBJS) $(UNITTEST_OBJS)

clean:
	rm $(BIN)* $(OBJ)*
