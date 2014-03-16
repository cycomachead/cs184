//
//
#include <cfloat>
#include "common.h"
#include "tracer.h"

#include "simple.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"

Color shading(LocalGeo local, BRDF *brdf, Ray lray, Color lcolor, Vector3f view) {
    Vector3f norm = lray.dir;
    norm.normalize();
    Vector3f shadow = norm * -1 + local.normal * 2 * norm.dot(local.normal);
    Color c = brdf->kd * lcolor * max(norm.dot(local.normal), 0.0f);
    c = c + brdf->ks * lcolor * pow(max(shadow.dot(view), 0.0f), brdf->p);
    return c;
}

Ray createReflectRay(LocalGeo local, Ray ray) {
    return Ray();
}

Color RayTracer::trace(Ray& ray, int depth) {
    // cout << " TRACER: POS:\n" << ray.pos << "\n\tDIR:\n" << ray.dir << endl;
    if (depth == maxDepth) {
        if (LOGGING > 6) {
            cout << "Max Depth Reached";
        }
        return Color(0.0f, 0.0f, 0.0f);
    }

    float tHit = FLT_MAX, smallestTime = FLT_MAX;

    //vector<Primitive*> prims = scene.getPrims();

    Intersection ins, closestInt;
    Primitive *p, *closestPrim; //WHY DOESNT Prim* x,y; work????
    bool found = false;

    for(int i = 0; i < scene.primitives.size(); i += 1) {
        if (LOGGING > 11) {
            cout << "Checking Primitive: " << i << endl;
        }

        p = scene.primitives.at(i);
        if (p->intersect(ray, &tHit, &ins)) {
            if (LOGGING > 5) {
                cout << "Primitive Found: \ttHit: " << tHit << endl;
            }
            if (tHit < smallestTime) { // Current Alt to using aggregatePrim
                smallestTime = tHit;
                closestPrim = p;
                closestInt = ins;
                found = true;
            }
        }
    }

    if (not found) {
        return Color(0.0f, 0.0f, 0.0f);
    }

    BRDF* brdf; // Holding Space
    Color retClr;
    Ray lray;
    Color lcolor;
    // Obtain the brdf at intersection point
    brdf = closestPrim->getBRDF();
    //closestPrim->getBRDF(closestInt.localGeo, &brdf);
    // Handle the Ambient Color term. 
    retClr = retClr + brdf->ka;
    
    // There is an intersection, loop through all light source
    // SCENE MEMBER: vector<Light*> lights;
    for (int i = 0; i < scene.lights.size(); i++) {
        Light* lt = scene.lights.at(i);
        lt->generateLightRay(closestInt.localGeo, &lray, &lcolor);

        bool blocked = false;
        // Check if the light is blocked or not
        // Looping again over all primitives. :/
        for (int i = 0; i < scene.primitives.size(); i += 1) {
            p = scene.primitives.at(i);
            // FIXME -- is comparing addresses safe here????
            if (p != closestPrim and p->intersectP(lray)) {
                blocked = true;
            }
        }
        if (!blocked) { // If not, do shading calculation for this light source
            Vector3f view = ray.dir * -1;
            view.normalize();
            retClr = retClr + shading(closestInt.localGeo, brdf, lray, lcolor, view);
        }
    }
    
    // Handle mirror reflection
    /*
    if (brdf.kr > 0) {
        reflectRay = createReflectRay(in.local, ray);

        // Make a recursive call to trace the reflected ray
        retClr = retClr + brdf.kr * trace(reflectRay, depth + 1);
    }
    */
    
    return retClr;
}


/*
Shading is similar to hw2
Beware when you generate reflection ray, make sure the ray don’t start
exactly on the surface, or the intersection routine may return
intersection point at the starting point of the ray. (This apply to light
ray generation as well)
*/
