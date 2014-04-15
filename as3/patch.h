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

    // Contains all the inner patches.
    vector<Patch*> *patches;

    float uStart;
    float uEnd;
    float vStart;
    float vEnd;

    float error;

    int order;

    bool isQuad;
public:
    Patch *parent;

    Patch();

    Patch(vector< vector<glm::vec4> >, float);

    Patch(vector<glm::vec3>);

    Patch(Patch&);
    
    // int* is an array
    int* quadIndicies(int);

    vector<glm::vec3> getQuad(int);

    vector<glm::vec3> getCorners();

    vector<glm::vec3> getTri(int);

    glm::vec3 getVertex(int);

    void subdivide();

    void subdivideQuad();

    void subdivideTriangle();

    void subdivideTriangle(int);

    int isFlat(vector<glm::vec3>);

    bool hasChildren();

    vector< vector<glm::vec3> > getPolygons();

    void setPartentData();

    vector<float> getUVQuad(int);

    vector<float> getUVTri();

    void setUV(vector<float>);

    Patch* getOldestParent();
    
    bool sideIsFlat(glm::vec3, float, float);
};

#endif
