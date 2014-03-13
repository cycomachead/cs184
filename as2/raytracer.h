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
class Color;
class Scene;
class Ray;
class Sampler;
class Camera;
class Raytracer;
class Primitive;
class Film;
class Normal;
class Point;
class Matrix;
class Transformation;
class Color;
class BRDF;
class Sample;
class LocalGeo;
class Shape;
class Primitive;
class Intersection;
class GeometricPrimitive;
class AggregatePrimitive;
class Material; // Not needed now
class RayTracer;
class Light;
class PointLight;
class DirectionalLight;


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