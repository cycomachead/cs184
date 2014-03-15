//
//

#ifndef TRACER_H
#define TRACER_H

#include "common.h"
#include "simple.h"

class RayTracer {
public:
    int maxDepth = 5;
    //Scene* scene = NULL;

    RayTracer() {
        // empty constructor
    }

    RayTracer(int depth) {
        maxDepth = depth;
        //scene = &s;
    }

    Color trace(Ray& ray, int depth);
};

#endif
