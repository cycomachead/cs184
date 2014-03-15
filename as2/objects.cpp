//
//

#include "objects.h"

//***************************************************************************//
// PRIMITIVES //
//***************************************************************************//
// bool Primitive::intersect(Ray& r, float* t, Intersection* i) {
//     cerr << "ERROR: Primitive intersect shouldn't be called." << endl;
//     return false;
// }
// bool Primitive::intersect(Ray& ray) {
//     cerr << "ERROR: Primitive intersectP shouldn't be called." << endl;
//     return false;
// }
// void Primitive::getBRDF(LocalGeo& local, BRDF* brdf) {
//     cerr << "ERROR: Primitive getBRDF shouldn't be called." << endl;
// }


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
    if (!shape->intersect(oray, thit, &olocal)) {
        return false;
    }
    in->primitive = this;
    in->localGeo = objToWorld*olocal;
    return true;
}

bool GeometricPrimitive::intersectP(Ray& ray) {
    // From the design note
    Ray oray = worldToObj*ray;
    return shape->intersectP(oray);
}

void GeometricPrimitive::getBRDF(LocalGeo& local, BRDF* brdf) {
    mat->getBRDF(local, brdf);
}

//***************************************************************************//
// SHAPES //
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
//  TRANSFORMATION AND MATRICIES AND SUCH //
//***************************************************************************//
// These few were weird and suggested by the design note for multiplications.
Vector3f Transformation::operator* (Normal v){
    Vector4f v4(v(0), v(1), v(2), 0);
    v4 = m * v4;
    Vector3f result;
    result(0) = v4(0);
    result(1) = v4(1);
    result(2) = v4(2);
    result.normalize();
    return result;
}
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

Transformation Transformation::identity() {
    Transformation t;
    t.m << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1;
    t.minvt = t.m;
}

//***************************************************************************//
//  Materials and other random things //
//***************************************************************************//
void Material::getBRDF(LocalGeo& local, BRDF* brdf) {
    // empty function
}



