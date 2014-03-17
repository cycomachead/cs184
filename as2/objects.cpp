//
//

#include "objects.h"

//***************************************************************************//
// PRIMITIVES //
// NOTHING SHOULD BE DEFINED HERE.
//***************************************************************************//
// AGGREGATEPRIMITIVES //
//***************************************************************************//
// TODO -- for the future

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

    return true;
}

//***************************************************************************//
// TRIANGLES //
//***************************************************************************//
bool Triangle::intersect(Ray& ray, float* tHit, LocalGeo* local) {
//http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/ray-triangle-intersection-geometric-solution/

    // WHY IS THIS BUGGY??????
    /*
    Vector3f u = b - a;
    Vector3f v = c - a;
    Vector3f w = a - c;

    Vector3f norm = u.cross(v);
    float dp = norm.dot(ray.dir);

    if (dp == 0) {
        return false;
    }

    float t = norm.dot((a - ray.pos) / dp);

    if (t < 0 ) { // Suggested is < 0... t < ray.tMin || t > ray.tMax
        return false;
    }

    Point p = ray.pos + ray.dir * t;

    bool edge1 = norm.dot(u.cross(p - a)) < 0;
    bool edge2 = norm.dot(v.cross(p - b)) < 0;
    bool edge3 = norm.dot(w.cross(p - c)) < 0;

    if (edge1 && edge2 && edge3) {
        *tHit = t;
        (*local).normal = norm;
        (*local).pos = p;

        return true;
    }

    return false;
    */
    // compute plane's normal
    Vector3f A, B;
    A = b - a;
    B = c - a;
    // no need to normalize
    Vector3f N = A.cross(B); // N

    //
    // Step 1: finding P
    //

    // check if ray and plane are parallel ?
    float NdotRayDirection = N.dot(ray.dir);
    if (NdotRayDirection == 0) {
        return false; // they are parallel so they don't intersect !
    }

    // compute d parameter using equation 2
    float d = N.dot(a);

    // compute t (equation 3)
    float t = -(N.dot(ray.pos) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) {
        return false; // the triangle is behind
    }
    // compute the intersection point using equation 1
    Point P = ray.pos + t * ray.dir;

    //
    // Step 2: inside-outside test
    //

    Vector3f C; // vector perpendicular to triangle's plane

    // edge 0
    Vector3f edge0 = b - a;
    Vector3f VP0 = P - a;
    C = edge0.cross(VP0);
    if (N.dot(C) < 0) {
        return false; // P is on the right side
    }

    // edge 1
    Vector3f edge1 = c - b;
    Vector3f VP1 = P - b;
    C = edge1.cross(VP1);
    if (N.dot(C) < 0)
        return false; // P is on the right side

    // edge 2
    Vector3f edge2 = a - c;
    Vector3f VP2 = P - c;
    C = edge2.cross(VP2);
    if (N.dot(C) < 0)
        return false; // P is on the right side;


    *tHit = t;
    (*local).normal = N;
    (*local).pos = P;

    return true; // this ray hits the triangle
}

bool Triangle::intersectP(Ray& ray) {
    Vector3f A, B;
    A = b - a;
    B = c - a;
    // no need to normalize
    Vector3f N = A.cross(B); // N

    //
    // Step 1: finding P
    //

    // check if ray and plane are parallel ?
    float NdotRayDirection = N.dot(ray.dir);
    if (NdotRayDirection == 0) {
        return false; // they are parallel so they don't intersect !
    }

    // compute d parameter using equation 2
    float d = N.dot(a);

    // compute t (equation 3)
    float t = -(N.dot(ray.pos) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) {
        return false; // the triangle is behind
    }
    // compute the intersection point using equation 1
    Point P = ray.pos + t * ray.dir;

    //
    // Step 2: inside-outside test
    //

    Vector3f C; // vector perpendicular to triangle's plane

    // edge 0
    Vector3f edge0 = b - a;
    Vector3f VP0 = P - a;
    C = edge0.cross(VP0);
    if (N.dot(C) < 0) {
        return false; // P is on the right side
    }

    // edge 1
    Vector3f edge1 = c - b;
    Vector3f VP1 = P - b;
    C = edge1.cross(VP1);
    if (N.dot(C) < 0)
        return false; // P is on the right side

    // edge 2
    Vector3f edge2 = a - c;
    Vector3f VP2 = P - c;
    C = edge2.cross(VP2);
    if (N.dot(C) < 0) {
        return false; // P is on the right side;
    }

    return true; // this ray hits the triangle
}

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

Transformation Transformation::inverse() {
    Transformation t;
    t.m = this->minvt;
    t.minvt = this->m;
    return t;
}


//***************************************************************************//
//  Materials and other random things //
//***************************************************************************//
void Material::getBRDF(LocalGeo& local, BRDF* brdf) {
    // empty function
}



