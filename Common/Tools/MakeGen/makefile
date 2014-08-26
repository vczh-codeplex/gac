CPP = g++ -std=c++11 -g
OBJ = ./Obj/
BIN = ./Bin/

VLPP_CPP = ../../../Libraries/GacUI/Public/Source/Vlpp.cpp
VLPP_H = ../../../Libraries/GacUI/Public/Source/Vlpp.h
VLPP_OBJ = $(OBJ)Vlpp.o

MAKEGEN_CPP = $(wildcard ./*.cpp)
MAKEGEN_OBJ = $(patsubst ./%.cpp, $(OBJ)%.o, $(MAKEGEN_CPP))

ALL_OBJ = $(MAKEGEN_OBJ) $(VLPP_OBJ)

.PHONY : all clean print

all: $(ALL_OBJ)
	$(CPP) -o $(BIN)MakeGen $(ALL_OBJ)

$(VLPP_OBJ) : $(VLPP_CPP) $(VLPP_H)
	$(CPP) -o $(VLPP_OBJ) -c $(VLPP_CPP)

$(MAKEGEN_CPP) : $(VLPP_H)
$(MAKEGEN_OBJ) : $(OBJ)%.o : ./%.cpp
	$(CPP) -o $@ -c $<

clean:
	rm $(OBJ)* -rf
	rm $(OBJ)* -rf

print:
	echo MAKEGEN_CPP : $(MAKEGEN_CPP) > /dev/null
	echo MAKEGEN_OBJ : $(MAKEGEN_OBJ) > /dev/null

