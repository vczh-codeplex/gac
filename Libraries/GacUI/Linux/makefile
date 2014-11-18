# This file is generated from "./makefile.debug-compacted.makegen" by Vczh Makefile Generator

.PHONY : all clean

bin_TARGET = ././Bin/
obj_TARGET = ././Obj/

GACUI_DIR = ./../Public/Source/
GACUI_cpp = $(GACUI_DIR)Vlpp.cpp $(GACUI_DIR)VlppWorkflow.cpp $(GACUI_DIR)GacUI.cpp $(GACUI_DIR)GacUIReflection.cpp
GACUI_h = $(GACUI_DIR)Vlpp.h $(GACUI_DIR)VlppWorkflow.h $(GACUI_DIR)GacUI.h $(GACUI_DIR)GacUIReflection.h
GACUI_MAIN_DIR = ././
GACUI_MAIN_cpp = $(GACUI_MAIN_DIR)MainCompacted.cpp

GACUI_o = $(patsubst $(GACUI_DIR)%.cpp, $(obj_TARGET)%.o, $(GACUI_cpp))
GACUI_MAIN_o = $(patsubst $(GACUI_MAIN_DIR)%.cpp, $(obj_TARGET)%.o, $(GACUI_MAIN_cpp))

ALL_o = $(GACUI_o) $(GACUI_MAIN_o)
all : $(ALL_o)
	clang++ -std=c++11 -g -o $(bin_TARGET)UnitTest $(ALL_o)

$(GACUI_h) : $(GACUI_cpp)
$(GACUI_MAIN_cpp) : $(GACUI_h)

$(GACUI_o) : $(obj_TARGET)%.o : $(GACUI_DIR)%.cpp
	clang++ -std=c++11 -g -o $@ -c $<
$(GACUI_MAIN_o) : $(obj_TARGET)%.o : $(GACUI_MAIN_DIR)%.cpp
	clang++ -std=c++11 -g -o $@ -c $<

clean:
	rm $(bin_TARGET)* -rf
	rm $(obj_TARGET)* -rf
