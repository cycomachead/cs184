//
//
#ifndef CAMERA_H
#define CAMERA_H

#include <cfloat>
#define _USE_MATH_DEFINES
#include <cmath>
#include "raytracer.h"
#include "simple.h"
#include "sampler.h"
#include "film.h"

class Camera {
/*
Create a ray starting from the camera that passes through the
corresponding pixel (sample.x, sample.y) on the image plane.
(from last week discussion, and also section 10.1 in Shirley’s book)
*/

public:

    float fov;

    int width, height;

    Point lookFrom, lookAt, up;

    Point XInc, YInc, UL, UR, DL, DR;

    Vector3f x, y, z;


    Camera() {

    }

    Camera(Point from, Point at, Point u, float f) {
        lookFrom = from;
        lookAt = at;
        up = u;
        fov = f;
        float vTheta = fov/2;
        float h = 2*tan(M_PI*vTheta/180);
        float aspectRatio =  width/height;
        float w = h*aspectRatio;
        //float hTheta = inverseTangent(w/2)
        z = lookAt - lookFrom;
        z.normalize();
        x = z.cross(up)
        x.normalize();
        y = x.cross(z)
        y.normalize(); //normalizing to be safe

        Point center = lookFrom + z;
        UL = center+(y*(h/2)) - x*(w/2);
        UR = center+(y*(h/2)) + x*(w/2);
        DL = center-(y*(h/2)) - x*(w/2);
        DR = center - y*(h/2) + x*(w/2);

        XInc = x*w/width;
        YInc = y*h/height;

    }

    void generateRay(Sample& sample, Ray* ray) {
        ray->pos  = lookFrom;
        ray->tMin = 0.0f;
        ray->tMax = FLT_MAX;
        // The only interesting calculation:
        // (dir is a vector3f)
        ray->dir  =  YInc*(sample.y - height/2.0 + 0.5) + z+ XInc*(sample.x - width/2.0 + 0.5);
    }
};

#endif