CC = g++
SOURCES = as3.cpp parser.cpp
OBJECTS = $(SOURCES:cpp=o)
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CC = clang -xc++ # Clang handles C++11 better on 10.8
	STD = -std=c++11 -stdlib=libc++
	CFLAGS = -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX -Wno-deprecated
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif

RM = /bin/rm -f

all: main

default: compile link

main:
	$(CC) $(SOURCES) $(INC) $(CFLAGS) $(LDFLAGS) -o as3

compile:
	$(CC) -c $(c++) $(SOURCES) $(INC) $(CFLAGS) $(LDFLAGS) -o $(OBJECTS)

link:
	$(CC) -g $(OBJECTS) $(STD) -o as3

clean:
	$(RM) *.o as3


