//
//
#ifndef CAMERA_H
#define CAMERA_H

#include <cfloat>
#define _USE_MATH_DEFINES
#include <cmath>
#include "common.h"

class Camera {
/*
Create a ray starting from the camera that passes through the
corresponding pixel (sample.x, sample.y) on the image plane.
(from last week discussion, and also section 10.1 in Shirley’s book)
*/
public:

    float centerX, centerY, fovY; // FOV in horizontal direction

    int pixelWidth, pixelHeight;

    Point lookFrom, lookAt, up, deltaX, deltaY,  dirConst;

    // Potentially useful for different ways of defining a camera
    // Point center, UL, UR, DL, DR;

    Vector3f x, y, z;
    Vector3f camera_up, camera_right, camera_position, camera_direction;

    Camera() {
        // empty constructor
    }

    Camera(Point, Point, Point, float, int, int);

    // modify poiter parameter.
    void generateRay(Sample&, Ray*);

    // same function but return a new ray
    Ray generateRay(Sample&);
};

#endif
