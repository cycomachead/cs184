//
//

#include "objects.h"

//***************************************************************************//
// PRIMITIVES //
// NOTHING SHOULD BE DEFINED HERE.
//***************************************************************************//
// AGGREGATEPRIMITIVES //
//***************************************************************************//


//***************************************************************************//
// GEOMETRICPRIMITIVES //
//***************************************************************************//
bool GeometricPrimitive::intersect(Ray& ray, float* thit, Intersection* in)  {
    // From the design note
    Ray oray = worldToObj*ray;
    LocalGeo olocal;
    // thit already a pointer.
    
    if (!getShape()->intersect(oray, thit, &olocal)) {
        return false;
    }
    in->primitive = this;
    in->localGeo = objToWorld*olocal;
    return true;
}

bool GeometricPrimitive::intersectP(Ray& ray) {
    // From the design note
    Ray oray = worldToObj*ray;
    return getShape()->intersectP(oray);
}

void GeometricPrimitive::getBRDF(LocalGeo& local, BRDF* brdf) {
    *brdf = this->brdf;
    //mat->getBRDF(local, brdf);
}

BRDF* GeometricPrimitive::getBRDF() {
    return this->brdf;
    //mat->getBRDF(local, brdf);
}

//***************************************************************************//
// SHAPES //
// NOTHING SHOULD BE DEFINED HERE.
//***************************************************************************//
// SPHERES //
//***************************************************************************//
Sphere::Sphere(float x, float y, float z, float rad) {
    center(0) = x;
    center(1) = y;
    center(2) = z;
    r = rad;
}

bool Sphere::intersect(Ray& ray, float* tHit, LocalGeo* local) {
    // Quadratic Formula
    float a, b, c, disc, discRt, q;
    // (p-c)
    Vector3f pc = ( ray.pos - center );

    // INTERSECTION: (p-c) dot (p-c) = r^2
    // helpful: http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
    a = ray.dir.dot(ray.dir);
    b = 2 * ray.pos.dot(ray.dir);
    c =  pc.dot(pc) - (r * r);
    disc = b * b - 4 * a * c;

    if (disc < 0) {
        return false;
    }

    discRt = sqrt(disc);
    if (b < 0) {
        q = (-b - discRt)/2.0;
    }
    else {
        q = (-b + discRt)/2.0;
    }

    // FIXME:
    // compute t0 and t1
    float t0 = q / a;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1) { // if t0 is bigger than t1 swap
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < 0) {
        return false;
    }

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0) {
        *tHit = t1;
    }
    else { // else the intersection point is at t0
        *tHit = t0;
    }

    // Update localGeo parameter
    Normal norm((ray.pos + (ray.dir * *tHit)) - center);
    (*local).normal = norm;
    (*local).pos = (ray.pos + (ray.dir * *tHit));

    return true;
}

bool Sphere::intersectP(Ray& ray) {
    float* uselessF;
    LocalGeo* uselessLocal;
    return intersect(ray, uselessF, uselessLocal);
}

//***************************************************************************//
// TRIANGLES //
//***************************************************************************//

//***************************************************************************//
//  TRANSFORMATION AND MATRICIES AND SUCH //
//***************************************************************************//
// These few were weird and suggested by the design note for multiplications.
Ray Transformation::operator* (Ray r) {
    Ray result;
    result.pos = (*this) * r.pos;
    result.dir = (*this) * r.dir;
    result.tMin = r.tMin;
    result.tMax = r.tMax;
    return result;
}

LocalGeo Transformation::operator* (LocalGeo local) {
    LocalGeo result;
    result.pos = (*this) * local.pos;
    result.normal = (*this) * local.normal;
    return result;
}

// Point Transformation::operator* (Point p) {
//     return (*this) * (Vector3f) p;
// }

Vector3f Transformation::operator* (Vector3f v) {
    Vector4f v4 = m * Vector4f(v(0), v(1), v(2), 0);
    // v4 = m * v4;
    Vector3f result;
    result(0) = v4(0);
    result(1) = v4(1);
    result(2) = v4(2);
    // result.normalize();
    return result;
}

// Vector3f Transformation::operator* (Normal v) {
//     Vector4f v4(v(0), v(1), v(2), 0);
//     v4 = m * v4;
//     Vector3f result;
//     result(0) = v4(0);
//     result(1) = v4(1);
//     result(2) = v4(2);
//     result.normalize();
//     return result;
// }

Transformation Transformation::identity() {
    Transformation t;
    t.m = Matrix<float, 4, 4>::Identity();
    t.minvt = t.m;
    return t;
}

//***************************************************************************//
//  Materials and other random things //
//***************************************************************************//
void Material::getBRDF(LocalGeo& local, BRDF* brdf) {
    // empty function
}



