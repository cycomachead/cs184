//
//
#ifndef LIGHTS_H
#define LIGHTS_H

#include "common.h"
#include "simple.h"
#include <cfloat>

/*
Light
Methods:
    
Notes:
    This is an abstract class that will generate a ray starting from
    the position stored in local to the position of the light source.
    You might want to consider creating 2 derived classes for
    point light source and directional light source.
    For directional light, the origin of the ray is the same, and the ray
    points to the light direction, however, t_max is infinity.

*/

class Light {
public:
    Color color;
    Point position;
    Light() {
        // empty constructor
    }
    virtual void generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor) = 0;
};


class PointLight: public Light {
public:
    PointLight() {
        // empty constructor
    }
    
    PointLight(Color c, Point p) {
        this->color = c;
        this->position = p;
    }
};
typedef PointLight pLight;

class DirectionalLight: public Light {
public:
    DirectionalLight() {
        // empty constructor
    }
    
    DirectionalLight(Color c, Point p) {
        this->color = c;
        this->position = p;
    }
};
typedef DirectionalLight dLight;

#endif