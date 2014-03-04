CC  = clang
X11 = -I/opt/X11/include
STD = -std=c++11 -stdlib=libc++ -lc++


all: rt

default: rt parser

rt:
	$(CC) raytracer.cpp $(X11) $(STD)

parser:
	$(CC) parser.cpp $(X11) $(STD)

clean:
	rm -f *.out *.o