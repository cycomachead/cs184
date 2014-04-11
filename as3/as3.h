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
#include <map>
#include <stdlib.h>

#include "lib/glm/glm.hpp"
#include "bezier.h"
// #include "model.h"
// #include "patch.h"


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

using namespace std;

extern int LOGLEVEL;
extern vector< vector <vector<glm::vec4> > > patches;

void loadPatches(string);

#endif