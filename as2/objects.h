//
//
#ifndef OBJECTS_H
#define OBJECTS_H

#include "common.h"
#include "simple.h"
#include "brdf.h"

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
        m = Matrix<float, 4, 4>::Identity();
        minvt = Matrix<float, 4, 4>::Identity();
    }
    
    Transformation(Matrix m1) {
        this->m = m1;
        this->minvt = m1.inverse();
    }
    
    Matrix4f getM() {
        return m;
    }
    
    Matrix4f getInv() {
        return minvt;
    }
    
    static Transformation identity();

    LocalGeo operator*(LocalGeo lg);
    Ray      operator*(Ray r);
    //Point    operator*(Point p);
    //Vector3f operator*(Normal n);
    Vector3f operator*(Vector3f n);
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
    /* NOTE: This class is abstract. Implementers are:
    GEOMETRIC and AGGREGATE Primitives. */
protected:
    Primitive() {
        // empty constructor
    }
public:
    // = 0 makes the class abstract virtual.
    virtual bool intersect(Ray& ray, float* thit, Intersection* in) = 0;
    virtual bool intersectP(Ray& ray) = 0;
    virtual void getBRDF(LocalGeo& local, BRDF* brdf) = 0;
    virtual BRDF* getBRDF() = 0;
};


class GeometricPrimitive : virtual public Primitive {
public:

    Transformation objToWorld, worldToObj;
    Shape* thing;
    Material* mat;
    BRDF* brdf;

    GeometricPrimitive() {
        // empty constructor
    }

    GeometricPrimitive(Shape* s, Transformation tr) {
        thing = s;
        this->objToWorld = tr;
        this->worldToObj = tr;
        // FIXME
    }

    GeometricPrimitive(Shape* s) {
        // Constructor w/ no transformation is the default transformation...
        GeometricPrimitive(s, Transformation::identity());
    }

    bool intersect(Ray& ray, float* thit, Intersection* in);

    bool intersectP(Ray& ray);

    void getBRDF(LocalGeo& local, BRDF* brdf);
    
    BRDF* getBRDF();
    
    Shape* getShape() {
        return this->thing;
    }
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
    virtual bool intersect(Ray& ray, float* tHit, LocalGeo* local) =0;

    // Same as intersect, but just return whether there is any intersection or
    // not
    virtual bool intersectP(Ray& ray) =0;

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
/*
Methods:
    void getBRDF(LocalGeo& local, BRDF* brdf) {
        return constantBRDF;
    }
Notes:
    Class for storing material. For this example, it just returns a constant
    material regardless of what local is. Later on, when we want to support
    texture mapping, this need to be modified.
*/
public:
    BRDF constantBRDF;

    Material() {
        // empty constructor
    }

    void getBRDF(LocalGeo& local, BRDF* brdf);
};


#endif