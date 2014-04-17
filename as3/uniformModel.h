// Model Object for AS3
#ifndef UNIFORMMODEL_H
#define UNIFORMMODEL_H

#include "as3.h"


using namespace std;

class UniformModel {
public:

    vector< vector <vector<glm::vec3> > > modelPatches;
    vector< vector< glm::vec3 >* >* shapes;
    vector< vector< glm::vec3 >* >* normals;
    float stepSize;

    UniformModel() {
        // empty
    }

    UniformModel(vector< vector <vector<glm::vec3> > > patches, float step);

    void uniformTesselation();

    vector< vector <vector<glm::vec3> > > getPatches();
    vector< vector <glm::vec3>* >* getShapes();
    vector< vector <glm::vec3>* >* getNormals();
}; // end model

#endif