//
//
#ifndef OBJECTS_H
#define OBJECTS_H

#include "common.h"
#include "simple.h"

class LocalGeo {
public:
    Point pos;
    Normal normal;
    LocalGeo() {
        // empty constructor
    }
};

class Transformation : public Matrix4f {
public:
// Storing matrix m and its inverse transpose, minvt (for transforming normal)
// Support Point, Vector, Normal, Ray, LocalGeo transformation by * overloading
    Matrix4f m, minvt;
    Transformation() {
        // empty constructor
    }

    Ray operator* (Ray r);
    LocalGeo operator* (LocalGeo lg);
    // Point operator* (Point p);
    Vector3f operator* (Normal n);
};



class Intersection {
public:
    LocalGeo localGeo;
    Primitive* primitive;

    Intersection() {
        // empty contructor
    }
};


class Primitive {
/*
Notes:
        Abstract class for primitives in the scene
*/
public:
    Primitive() {
        // empty constructor
    }

    bool intersect(Ray& ray, float* thit, Intersection* in);
    bool intersect(Ray& ray);
    void getBRDF(LocalGeo& local, BRDF* brdf);
};


class GeometricPrimitive : public Primitive {
public:

    Transformation objToWorld, worldToObj;
    Shape* shape;
    Material* mat;

    GeometricPrimitive() {
        // empty constructor
    }

    bool intersect(Ray& ray, float* thit, Intersection* in);

    bool intersectP(Ray& ray);

    void getBRDF(LocalGeo& local, BRDF* brdf);
};


class AggregatePrimitive : public Primitive {
/*
Notes:
    Constructor store the STL vector of pointers to primitives.
    Intersect just loops through all the primitives in the list and call the
    intersect routine. Compare thit and return that of the nearest one
    (because we want the first hit). Also, the in->primitive should be set to
    the pointer to that primitive. When you implement acceleration structure,
    it will replace this class.
*/
public:
    AggregatePrimitive() {
        // empty constructor
    }

    AggregatePrimitive(vector<Primitive*> list);
    bool intersect(Ray& ray, float* thit, Intersection* in);
    bool intersectP(Ray& ray);

    void getBRDF(LocalGeo& local, BRDF* brdf) {
        exit(1);
        // This should never get called, because in->primitive will
        // never be an aggregate primitive
    }
};


class Shape {
public:
    Shape() {
        // empty constructor
    }

    // Test if ray intersects with the shape or not (in object space),
    // if so, return intersection point and normal
    bool intersect(Ray& ray, float* tHit, LocalGeo* local);

    // Same as intersect, but just return whether there is any intersection or
    // not
    bool intersectP(Ray& ray);

// Triangle and Sphere are probably best implemented here
// The intersection with the ray at t outside the range [t_min, t_max]
// should return false.
};

class Sphere : public Shape {
public:
    Point center;
    float r;

    Sphere() {
        // empty constructor
    }

    Sphere(float x, float y, float z, float rad);

    bool intersect(Ray& ray, float* tHit, LocalGeo* local);
    bool intersectP(Ray& ray);

};

class Triangle : public Shape {
public:
    Triangle() {
        // empty constructor
    }
};


class Material {
public:
    Material() {
        // empty constructor
    }

    void getBRDF(LocalGeo& local, BRDF* brdf);
};


#endif