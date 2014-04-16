// CS184 AS3
// VERTEX.CPP
// TOMO UEDA & MICHAEL BALL

#include "vertex.h"

    // // UV coor for bezier always in [0, 1]
    // glm::vec2* uv;
    //
    // // the point parsed from the input file or interpolated
    // glm::vec3* point;
    //
    // // normal to the surface point
    // glm::vec3* normal;
    //
    // // the surface point -- Bezier curve evaluated for this point.
    // glm::vec3* surface;
    //
    // // A Pointer to the patch this belongs to.
    // // Likely unecessary.
    // Patch* parentPatch;

// empty constructor. Init all pointers.
Vertex::Vertex() {
    this->uvStart = new glm::vec2();
    this->uvEnd   = new glm::vec2();
    this->point   = new glm::vec3();
    this->normal  = new glm::vec3();
    this->surface = new glm::vec3();
}

// Create a new vector
Vertex::Vertex(glm::vec3 c, glm::vec2 s, glm::vec2 e, Patch* p) {
    this->uvStart = new glm::vec2(s);
    this->uvEnd   = new glm::vec2(e);
    this->point   = new glm::vec3(c);
    this->normal  = new glm::vec3();
    this->surface = new glm::vec3();
    this->parentPatch = p;
}

// FIXME -- this should check for init of parent pointer...
void Vertex::calcBezier() {

}