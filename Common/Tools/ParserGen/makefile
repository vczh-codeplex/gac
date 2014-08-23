CPP = g++ -std=c++11 -g
OBJ = ./Obj/
BIN = ./Bin/

VLPP_CPP = ../../../Libraries/GacUI/Public/Source/Vlpp.cpp
VLPP_H = ../../../Libraries/GacUI/Public/Source/Vlpp.h
VLPP_OBJ = $(OBJ)Vlpp.o

PARSERGEN_DIR = ./ParserGen/
PARSERGEN_CPP = $(wildcard $(PARSERGEN_DIR)ParserGen*.cpp) $(PARSERGEN_DIR)Main.cpp
PARSERGEN_H = $(PARSERGEN_DIR)ParserGen.h
PARSERGEN_OBJ = $(patsubst $(PARSERGEN_DIR)%.cpp, $(OBJ)%.o, $(PARSERGEN_CPP))

ALL_OBJ = $(PARSERGEN_OBJ) $(VLPP_OBJ)

.PHONY : all clean print

all: $(ALL_OBJ)
	$(CPP) -o $(BIN)ParserGen $(ALL_OBJ)

$(VLPP_OBJ) : $(VLPP_CPP) $(VLPP_H)
	$(CPP) -o $(VLPP_OBJ) -c $(VLPP_CPP)

$(PARSERGEN_CPP) : $(PARSERGEN_H) $(VLPP_H)
$(PARSERGEN_OBJ) : $(OBJ)%.o : $(PARSERGEN_DIR)%.cpp
	$(CPP) -o $@ -c $<

clean:
	rm $(OBJ)* -rf
	rm $(OBJ)* -rf

print:
	echo PARSERGEN_CPP : $(PARSERGEN_CPP) > /dev/null
	echo PARSERGEN_OBJ : $(PARSERGEN_OBJ) > /dev/null

