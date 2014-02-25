#include "lib/CImg.h"
// X11 and Eigen don't play nicely...
#undef Success
#include "lib/Eigen/Core"

#include <cstidint>
// #include <cmath>
// #include <cstdlib>
#include <iostream>

using namespace std;

// GLOBAL VARIABLES
CImg output;

class vec {
    float x;
    float y;
    float z;
};


class Scene {
    int width;
    int height;
    vec cam;
    vec[4] = corners;
};


void argParse(int argc, const char ** argv);


int main(int argc, const char ** argv) {

}

void argParse(int argc, const char ** argv) {
    
}