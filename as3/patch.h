// CS184 AS3 Bezier Surfaces
// Tomo Ueda, Michael Ball
// Patch.h

#ifndef PATCH_H
#define PATCH_H

#include "as3.h"
#include "vertex.h"

class Patch {
private:
    // 2D array of parsed patch coordinates
    vector< vector<glm::vec3> > *originalData;

    // 1D set of control points
    vector<glm::vec3> *controlPoints;

    // 1D set of verticies for the patch
    vector<Vertex*> *verticies;

    // Contains all the inner patches.
    vector<Patch*> *patches;

    float error;

    int order;

    bool isQuad;
public:
    Patch *parent;

    Patch();

    Patch(vector< vector<glm::vec3> >, float);

    Patch(vector<Vertex*>);

    Patch(Patch&);

    // int* is an array
    int* quadIndicies(int);

    // FIXME -- verify returning pointer is best.
    vector<Vertex*> getQuad(int);

    vector<Vertex*> getVerticies();

    vector<Vertex*> getTri(int);

    Vertex* getVertex(int);

    void subdivide();

    void subdivideQuad();

    void subdivideTriangle();

    void subdivideTriangle(int);

    int isFlat();

    bool hasChildren();

    // A vector of all polygons
    vector< vector<Vertex*> > getPolygons();

    void setPartentData();
    
    void initializeVerticies();

    vector<glm::vec2> getUVQuad(int);

    // vector<float> getUVTri();

    //void setUV(vector<float>);

    Patch* getOldestParent();

    bool sideIsFlat(Vertex*, float, float);
};

#endif
