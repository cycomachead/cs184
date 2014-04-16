// CS184 AS3
// VERTEX.H
// TOMO UEDA & MICHAEL BALL
#ifndef VERTEX_H
#define VERTEX_H

#include "as3.h"
#include "patch.h"

class Vertex {
    // FIXME -- lazy option for now.
public:
    // UV coor for bezier always in [0, 1]
    glm::vec2* uvStart;

    // UV ending coor for bezier; always in [0, 1]
    glm::vec2* uvEnd;
    
    // the point parsed from the input file or interpolated
    glm::vec3* point;

    // normal to the surface point
    glm::vec3* normal;

    // the surface point -- Bezier curve evaluated for this point.
    glm::vec3* surface;

    // A Pointer to the patch this belongs to.
    Patch* parentPatch;

    Vertex();

    Vertex(glm::vec3, glm::vec2, glm::vec2, Patch*);

    void calcBezier();
};

#endif
