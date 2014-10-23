mkdir Coverage
gcov -o ./Obj/ ../Source/*.cpp ../Source/Collection/*.cpp ../Source/Parsing/*.cpp ../Source/Reflection/*.cpp ../Source/Regex/*.cpp ../Source/Stream/*.cpp
mv *.gcov ./Coverage/
