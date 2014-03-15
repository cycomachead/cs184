CC  = clang
c++ = -xc++ -std=c++11
INC = -I/opt/X11/include -Ilib
STD = -stdlib=libc++ -lc++ -lstdc++ #-lm -DOSX
# PNG = -DNO_FREETYPE
ERR = -Wno-deprecated-register
LIB = tracer.cpp camera.cpp objects.cpp
PNG = lib/lodepng.cpp
# -Weverything
# lib/lodepng.h
#

all: rt

default: compile link

rt:
	$(CC) $(c++) main.cpp $(LIB) $(PNG) $(INC) $(STD) $(ERR) -o raytracer

compile:
	$(CC) -c $(c++) main.cpp $(INC) $(ERR) -o main.o
	$(CC) -c $(c++) $(PNG) $(INC) $(ERR) -o png.o
	$(CC) -c $(c++) tracer.cpp $(ERR) -o tracer.o
	$(CC) -c $(c++) camera.cpp $(ERR) -o camera.o
	$(CC) -c $(c++) objects.cpp $(ERR) -o objects.o

link:
	$(CC) -g camera.o main.o tracer.o objects.o png.o $(STD) -o raytracer

parser:
	$(CC) parser.cpp $(INC) $(STD)

clean:
	rm -rf *.out *.o *.dSYM