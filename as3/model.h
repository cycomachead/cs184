// Model Object for AS3
#ifndef MODEL_H
#define MODEL_H

#include "as3.h"


using namespace std;

class Model {
public:

    vector< vector <vector<glm::vec4> > > modelPatches;
    vector< glm::vec4 > *verticies;
    vector< glm::vec4 > *normals;
    float errorBound;
    float stepSize;

    Model() {
        // empty
    }

    Model(vector< vector <vector<glm::vec4> > > patches, float tau);

    void buildUniformVertexNormal();

    vector< vector <vector<glm::vec4> > > getPatches();
    
    vector <vector<glm::vec4> > getPatchN(int);
    
    vector<glm::vec4> getCorners(int);
}; // end model

#endif