CC  = clang -g
c++ = -xc++ -std=c++11
INC = -I/opt/X11/include -Ilib
STD = -stdlib=libc++ -lc++ -lm -lstdc++ -DOSX
# PNG = -DNO_FREETYPE
ERR = -Wno-deprecated-register
LIB = lib/lodepng.h lib/lodepng.cpp scene.cpp
# -Weverything

#

all: rt

default: rt

rt:
	$(CC) $(c++) raytracer.cpp $(LIB) $(INC) $(STD) $(ERR) -o rt.out

parser:
	$(CC) parser.cpp $(INC) $(STD)

clean:
	rm -rf *.out *.o *.dSYM