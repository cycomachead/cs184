// CS184 AS3 Bezier Surfaces
// Tomo Ueda, Michael Ball
// Patch.h

#ifndef PATCH_H
#define PATCH_H

#include "as3.h"

class Patch {
private:
    // 2D array of parsed patch coordinates
    vector< vector<glm::vec4> > *originalData;

    // 1D set of control points
    vector<glm::vec3> *controlPoints;

    // 1D set of corners for the patch
    vector<glm::vec3> *corners;

    // Should patches contain patches? -- subdivisions
    vector<Patch> *innerPatches;

    Patch *parent;

    int order;

    bool isQuad;
public:

    Patch();

    Patch(vector< vector<glm::vec4> >);
    
    Patch(vector<glm::vec3>, Patch&);

    Patch(Patch&);

    int* quadIndicies(int);
        
    vector<glm::vec3> getQuad(int);

    vector<glm::vec3> getCorners();
    
    vector<glm::vec3> getTri(int);

    glm::vec3 getVertex(int);
    
    void subdivide();
};

#endif
