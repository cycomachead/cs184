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

// X11 and Eigen don't play nicely...
#ifndef egn_h
#define egn_h
#undef Success
#include "lib/Eigen/Eigen"
#endif

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
class Material;
// class Matrix;   // Likely not needed
// class Normal;   // Do I need this?
// class Point;    // Use Vector3f instead.
class PointLight;
class Primitive;
class Ray;
class RayTracer;
class Sample;
class Sampler;
class Scene;
class Shape;
class Transformation;
typedef Eigen::Vector3f Point;



// GLOBAL VARIABLES
// Stupid logging global because lazy.
extern int LOGGING;
extern Scene scene;


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