//
//
#ifndef OBJECTS_H
#define OBJECTS_H

#include "common.h"
#include "simple.h"


class Intersection {
/*
    LocalGeo localGeo
    Primitive* primitive
*/
public:
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
/*
Members:
            Transformation objToWorld, worldToObj;
            Shape* shape;
            Material* mat;

Methods:
        bool intersect(Ray& ray, float* thit, Intersection* in)  {
                    Ray oray = worldToObj*ray;
                    LocalGeo olocal;
                    if (!shape->intersect(oray, thit, &olocal))  return false;
                    in->primitive = this;
                    in->local = objToWorld*olocal;
                    return true;
        }

        bool intersectP(Ray& ray) {
                    Ray oray = worldToObj*ray;
                    return shape->intersectP(oray);
        }

        void getBRDF(LocalGeo& local, BRDF* brdf) {
                    material->getBRDF(local, brdf);
        }
*/
public:
    GeometricPrimitive() {
        // empty constructor
    }
};


class AggregatePrimitive : public Primitive {
/*
Methods:
            AggregatePrimitive(vector<Primitive*> list);
            bool intersect(Ray& ray, float* thit, Intersection* in)
            bool intersectP(Ray& ray)
            void getBRDF(LocalGeo& local, BRDF* brdf) {
                exit(1);
                // This should never get called, because in->primitive will
                // never be an aggregate primitive
            }

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
};


class Shape {
public:
    Shape() {
        // empty constructor
    }
};


#endif