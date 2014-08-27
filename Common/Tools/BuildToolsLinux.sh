cd MakeGen
make clean
make
cd ..
ln -f -s `pwd`/MakeGen/Bin/MakeGen /usr/local/bin/MakeGen

cd ParserGen
MakeGen release makefile
make clean
make
cd ..
ln -f -s `pwd`/ParserGen/Bin/ParserGen /usr/local/bin/ParserGen
