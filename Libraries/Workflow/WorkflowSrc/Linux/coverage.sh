mkdir Coverage
gcov -o ./Obj/ ../../Source/*.cpp ../../Source/Expression/*.cpp ../../Source/Analyzer/*.cpp ../../Source/Runtime/*.cpp
mv *.gcov ./Coverage/
