// Common AS4 Header file
#ifndef as4_h
#define as4_h

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include "lib/Eigen/Eigen"

using namespace std;

extern int LOGLEVEL;
inline float sqr(float x) { return x*x; }

#endif