// Model Object for AS3
#ifndef MODEL_H
#define MODEL_H

#include "as3.h"
#include "patch.h"


using namespace std;

class Model {
public:

    vector< vector <vector<glm::vec4> > > modelPatches;
    vector<Patch*> *adapPatches;
    vector< vector< glm::vec4 >* >* uniNormals;
    vector< vector< glm::vec4 >* >* shapes;
    vector< glm::vec4 > *verticies;
    vector< glm::vec4 > *normals;
    float errorBound;
    float stepSize;

    Model() {
        // empty
    }

    Model(vector< vector <vector<glm::vec4> > > patches, float tau);


    vector< vector <vector<glm::vec4> > > getPatches();
    
    vector <vector<glm::vec4> > getPatchN(int);
    
    vector<glm::vec4> getCorners(int);
    
    void buildAdaptive();
    
    void subdivideAll();
    
    vector <vector<glm::vec3> > getAllPolygons();
}; // end model

#endif