CPP = g++ -std=c++11 -finput-charset=gbk
OBJ = ./obj/
BIN = ./exe/
VLPP = ./Source/

all: $(OBJ)Main.o
	$(CPP) -o $(BIN)UnitTest $(OBJ)Main.o

$(OBJ)Main.o: Main.cpp
	$(CPP) -o $(OBJ)Main.o -c Main.cpp 

clean:
	rm $(BIN)* $(OBJ)*
