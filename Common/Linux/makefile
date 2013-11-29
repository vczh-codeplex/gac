CPP = g++ -std=c++11 -finput-charset=gbk
OBJ = ./obj/
BIN = ./exe/
VLPP = ../Source/

all: $(OBJ)Main.o
	$(CPP) -o $(BIN)UnitTest $(OBJ)Main.o

$(OBJ)Main.o: Main.cpp $(OBJ)UnitTest.o
	$(CPP) -o $(OBJ)Main.o -c Main.cpp 

$(OBJ)Basic.o: $(VLPP)Basic.cpp $(VLPP)Basic.h
	$(CPP) -o $(OBJ)Basic.o -c $(VLPP)Basic.cpp

$(OBJ)String.o: $(VLPP)String.cpp $(VLPP)String.h $(OBJ)Basic.o
	$(CPP) -o $(OBJ)String.o -c $(VLPP)String.cpp

$(OBJ)Console.o: $(VLPP)Console.cpp $(VLPP)Console.h $(OBJ)String.o
	$(CPP) -o $(OBJ)Console.o -c $(VLPP)Console.cpp

$(OBJ)UnitTest.o: $(VLPP)UnitTest/UnitTest.cpp $(VLPP)UnitTest/UnitTest.h $(OBJ)Console.o
	$(CPP) -o $(OBJ)UnitTest/UnitTest.o -c $(VLPP)UnitTest/UnitTest.cpp

clean:
	rm $(BIN)* $(OBJ)*
