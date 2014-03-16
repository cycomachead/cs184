CC  = clang
c++ = -xc++ -std=c++11
INC = -I/opt/X11/include #-Ilib
# Necessary for C++11 on 10.8 machines.
STD = -stdlib=libc++ -lstdc++ #-lm -DOSX -lc++
# PNG = -DNO_FREETYPE
ERR = -Wno-deprecated-register
SOURCES = main.cpp tracer.cpp camera.cpp objects.cpp brdf.cpp scene.cpp \
		lib/lodepng.cpp
OBJECTS = $(SOURCES:cpp=o)
# -Weverything
# lib/lodepng.h
#

all: rt

default: compile link

rt:
	$(CC) $(c++) $(SOURCES) $(INC) $(STD) $(ERR) -o raytracer

compile:
	$(CC) -c $(c++) $(SOURCES) $(INC) $(ERR) -o $(OBJECTS)

link:
	$(CC) -g $(OBJECTS) $(STD) -o raytracer

parser:
	$(CC) parser.cpp $(INC) $(STD)

clean:
	rm -rf *.out *.o *.dSYM