#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

#include "lib/CImg.h"
// X11 and Eigen don't play nicely...
#undef Success
#include "lib/Eigen/Eigen"

using namespace std;

// GLOBAL VARIABLES
// cimg_library::CImg output;

class vec {
    float x;
    float y;
    float z;
};


class Scene {
    int width;
    int height;
    vec cam;
    vec corners[4];
};


void argParse(int argc, const char ** argv);


int main(int argc, const char ** argv) {

}

void argParse(int argc, const char ** argv) {

}