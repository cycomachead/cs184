CC  = clang -g -xc++
c++ = -std=c++11
X11 = -I/opt/X11/include
STD = -lm -lstdc++ -DOSX -Ilib -stdlib=libc++ -lc++
PNG = -DNO_FREETYPE
LIB = lib/lodepng.h lib/lodepng.cpp

# 

all: rt

default: rt parser

rt:
	$(CC) $(c++) raytracer.cpp $(LIB) $(X11) $(STD)

parser:
	$(CC) parser.cpp $(X11) $(STD)

clean:
	rm -rf *.out *.o *.dSYM