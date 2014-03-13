//
//
#ifndef CAMERA_H
#define CAMERA_H

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

    Point lookFrom,
          lookAt,
          up;

    Camera(Point from, Point at, Point up, float f) {

    }

    void generateRay(Sample& sample, Ray* ray);
};

#endif