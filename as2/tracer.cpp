//
//
#include <cfloat>
#include "common.h"
#include "tracer.h"

#include "simple.h"
#include "scene.h"
#include "objects.h"
#include "lights.h"

Color RayTracer::trace(Ray& ray, int depth) {
    // FIXME
    // cout << " TRACER: POS:\n" << ray.pos << "\n\tDIR:\n" << ray.dir << endl;
    if (depth == maxDepth) {
        if (LOGGING > 6) {
            cout << "Max Depth Reached";
        }
        return Color(0.0f, 0.0f, 0.0f);
    }

    float tHit = FLT_MAX, smallestTime = FLT_MAX;

    vector<Primitive*> prims = scene.getPrims();

    Intersection ins, closestInt;
    Primitive *p, *closestPrim; //WHY DOESNT Prim* x,y; work????
    bool found = false;

    for(int i = 0; i < prims.size(); i += 1) {
        if (LOGGING > 5) {
            cout << "Checking Primitive: " << i << endl;
        }

        p = prims.at(i);
        if (p->intersect(ray, &tHit, &ins)) {
            if (LOGGING > 4) {
                cout << "Primitive Found: \ttHit:" << tHit << endl;
            }
            if (tHit < smallestTime) {
                smallestTime = tHit;
                closestPrim = p;
                closestInt = ins;
                found = true;
            }
        }
    }

    if (not found) {
        return Color(1.0f, 1.0f, 0.0f); // FIXME
    } else {
        return Color(0.0f, 0.0f, 1.0f);
    }

    return Color(0.0f, 0.0f, 0.0f);
}
/*

        if (!primitive.intersect(ray, &thit, &in) {
            // No intersection
            // Make the color black and return
        }
        // Obtain the brdf at intersection point
        in.primitive->getBRDF(in.local, &brdf);

        // There is an intersection, loop through all light source
        for (i = 0; i < #lights; i++) {
            lights[i].generateLightRay(in.local, &lray, &lcolor);

            // Check if the light is blocked or not
            if (!primitive->intersectP(lray))
            // If not, do shading calculation for this light source
            *color += shading(in.local, brdf, lray, lcolor);
        }

    // Handle mirror reflection
    if (brdf.kr > 0) {
        reflectRay = createReflectRay(in.local, ray);

        // Make a recursive call to trace the reflected ray
        trace(reflectRay, depth+1, &tempColor);
        *color += brdf.kr * tempColor;
    }
}

Notes:
    Shading is similar to hw2
    Beware when you generate reflection ray, make sure the ray don’t start
exactly on the surface, or the intersection routine may return
intersection point at the starting point of the ray. (This apply to light
ray generation as well)
*/
