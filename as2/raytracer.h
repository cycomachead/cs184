// Michael Ball CS184 RayTracer Header
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <map>
#include <iostream>

// Image File Writing
#include "lib/lodepng.h"
#include "lib/lodepng.cpp"

// X11 and Eigen don't play nicely...
#undef Success
#include "lib/Eigen/Eigen"

// Class Includes
// SUGGESTED LIST FROM DESIGN NOTE
class AggregatePrimitive;
class BRDF;
class Camera;
class Color;
class DirectionalLight;
class Film;
class GeometricPrimitive;
class Intersection;
class Light;
class LocalGeo;
class Material; // Not needed now
class Matrix;   // Likely not needed
class Normal;
class Point;
class PointLight;
class Primitive;
class Ray;
class RayTracer;
class Sample;
class Sampler;
class Scene;
class Shape;
class Transformation;


// GLOBAL VARIABLES
// Stupid logging global because lazy.
extern int LOGGING;


/*
 * Main Functions:
 */
// Assists in parsing command args.
void argParse(int, const char**);

// THIS IS A TERRIBLE IDEA BUT I'M DOING IT ANYWAY
// RUNNING OUT OF TIMEEEEEEEEEE :'(
using namespace std;
using namespace Eigen;

// close out include guard.
#endif