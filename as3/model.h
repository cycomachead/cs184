// Model Object for AS3
#ifndef MODEL_H
#define MODEL_H

#include "as3.h"
#include "patch.h"


using namespace std;

class Model {
public:

    vector< vector <vector<glm::vec3> > > modelPatches;
    vector<Patch*> *adapPatches;
    vector< vector< glm::vec3 >* >* uniNormals;
    vector< vector< glm::vec3 >* >* shapes;
    vector< glm::vec3 > *verticies;
    vector< glm::vec3 > *normals;
    float errorBound;
    float stepSize;

    Model() {
        // empty
    }

    Model(vector< vector <vector<glm::vec3> > > patches, float tau);


    vector< vector <vector<glm::vec3> > > getPatches();
    
    vector <vector<glm::vec3> > getPatchN(int);
    
    vector<glm::vec3> getVerticies(int);
    
    void buildAdaptive();
    
    void subdivideAll();
    
    vector <vector<glm::vec3> > getAllPolygons();
}; // end model

#endif