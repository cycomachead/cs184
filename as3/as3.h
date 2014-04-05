// Common AS3 Header file
#ifndef as3_h
#define as3_h

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <stdlib.h>


// Libraries
// #undef Success
// #include "lib/Eigen/Eigen"

using namespace std;


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

extern int LOGLEVEL;
extern vector< vector <vector<float> > > patches;

void loadPatches(string);

#endif