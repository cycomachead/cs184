CC = clang
X11 = -I/opt/X11/include
STD = -std=c++11


all: rt
	
default: rt

rt: 
	$(CC) raytracer.cpp $(X11) $(STD)
	
clean:
	rm -f *.out *.o