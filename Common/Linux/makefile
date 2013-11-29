CPP = g++ -std=c++11

all: obj/Main.o
	$(CPP) -o exe/UnitTest obj/Main.o

obj/Main.o: Main.cpp
	$(CPP) -o obj/Main.o -c Main.cpp 

clean:
	rm exe/* obj/*
