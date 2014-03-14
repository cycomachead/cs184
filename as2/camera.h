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

    float centerX, centerY, fovY; // FOV in horizontal direction

    int pixelWidth, pixelHeight;

    Point lookFrom, lookAt, up, deltaX, deltaY ,  dirConst;

    // Potentially useful for different ways of defining a camera
    // Point center, UL, UR, DL, DR;

    Vector3f x, y, z;


    Camera() {
        // empty constructor
    }

    Camera(Point from, Point at, Point u, float f, int w, int h) {
        float width, height, aspect;
        lookFrom = from;
        lookAt = at;
        up = u;
        fovY = f;
        pixelWidth = w;
        pixelHeight = h;
        
        
        aspect =  pixelWidth/pixelHeight;
        height = 2 * tan(M_PI/180 * .5 * fovY);
        width = height * aspect;
        
        
        z = lookAt - lookFrom;
        x = z.cross(up);
        y = x.cross(z);
        x.normalize(); // Is normalization necessary? Some suggest it....
        y.normalize();
        z.normalize();

        deltaX = x * width / pixelWidth;
        deltaY = y * height / pixelHeight;

        centerX = pixelWidth/2.0 + 0.5; // 0.5 == pixel centers
        centerY = pixelHeight/2.0 + 0.5;
        
        // ORIGINCAL FORMULA
        // deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX)
        // CAN I FACTOR???
        // dirConst = deltaY * -1 * centerY + z + deltaX * -1 * centerX;

    }

    // modify poiter parameter.
    void generateRay(Sample& sample, Ray* ray) {
        ray->pos  = lookFrom;
        ray->tMin = 0.0f;
        ray->tMax = FLT_MAX;
        // The only interesting calculation:
        // (dir is a vector3f)
        ray->dir  = deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX);
    }

    // same function but return a new ray
    Ray generateRay(Sample& sample) {
        return Ray(
            lookFrom,
            deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX),
            0.0f,
            FLT_MAX);
    }
};

#endif