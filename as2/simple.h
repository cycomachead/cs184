// Basic utilities for the raytracer
#ifndef SIMPLE_H
#define SIMPLE_H

#ifndef egn_h
#define egn_h
#undef Success
#include "lib/Eigen/Eigen"
#endif

class Color: public Vector3f {
/*
Members:
            float r, g, b
Notes:
            Support +,- with other color
            Support scalar *, /
            May support conversion from xyz
*/
private:
    Vector3f data;

public:
    Color() {
        // empty constructor
    }

    Color(Vector3f d) { data = d; }

    Color(float r, float g, float b) {
        data(0) = r;
        data(1) = g;
        data(2) = b;
    }

    float r() { return data(0); }

    float g() { return data(1); }

    float b() { return data(2); }

    void setR(float d) { data(0) = d; }

    void setG(float d) { data(1) = d; }

    void setB(float d) { data(2) = d; }

    void max() { // Re-write later.
        if (data(0) > 255) {
            data(0) = 255;
        }
        if (data(1) > 255) {
            data(1) = 255;
        }
        if (data(2) > 255) {
            data(2) = 255;
        }
    }

    void scale() {
        data = data * 255;
    }
};

// TODO: try to get these to work...
// #define x() (0)
// #define y() (1)
// #define z() (2)

// class Point : public Vector3f {
// // Members:
// //             float x, y, z
// // Notes:
// //             Constructor from 3 floats
// //             Support +,- with vector
// public:
//     Point() : Vector3f() {
//         // empty parent constructor
//     }
//
//     Point(float f1, float f2, float f3) : Vector3f(f1, f2, f3) {
//         // empty parent constructor
//     }
//
// };

class Ray {
public:
    Vector3f pos;
    Vector3f dir;
    float tMin, tMax;

// Represents the ray ray(t) = pos + t*dir, where t_min <= t <= t_max
    Ray() {
    }

    Ray(Vector3f p, Vector3f d, float min, float max) {
        pos = p;
        dir = d;
        tMin = min;
        tMax = max;
    }
};

class Sample {
    // currently a sample just stores X,Y values which are updated on each ray
    // generation. It's just a way of keeping track of the current place.
public:
    int x, y;
    Sample() {
        // empty constructor
    }

    Sample(int x1, int y1) {
        x = x1;
        y = y1;
    }
};


/**
Normal
Members:
    float x, y, z
Notes:
    Constructor from 3 floats
    Support +, -
    Note: Need to be normalized after operations (Be careful for 0 vector)

Matrix
Members:
    float mat[4][4]

Notes:
    Support creation of rotation, translation, scaling matrices
    May support matrix inversion if needed
    Also could support SVD, or other matrix decomposition, for future extension

*/
#endif