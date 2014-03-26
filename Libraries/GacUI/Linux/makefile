CPP = g++ -std=c++11

OBJ = ./obj/
BIN = ./exe/
GAC = ../Public/Source/

GAC_OBJS = $(OBJ)Vlpp.o $(OBJ)VlppWorkflow.o

all:
	echo =====================================
	echo =====================================
	echo =====================================
	echo =====================================
	$(CPP)		-o $(OBJ)Vlpp.o				-c $(GAC)Vlpp.cpp
	$(CPP)		-o $(OBJ)VlppWorkflow.o			-c $(GAC)VlppWorkflow.cpp
	$(CPP)		-o $(OBJ)Main.o				-c Main.cpp 
	$(CPP)		-o $(BIN)GaclibDemo $(OBJ)Main.o $(GAC_OBJS)

clean:
	rm $(BIN)* $(OBJ)*
