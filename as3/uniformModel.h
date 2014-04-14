// Model Object for AS3
#ifndef UNIFORMMODEL_H
#define UNIFORMMODEL_H

#include "as3.h"


using namespace std;

class UniformModel {
public:

    vector< vector <vector<glm::vec4> > > modelPatches;
    vector< vector< glm::vec4 >* >* shapes;
    vector< vector< glm::vec4 >* >* normals;
    float errorBound;
    float stepSize;

    UniformModel() {
        // empty
    }

    UniformModel(vector< vector <vector<glm::vec4> > > patches, float tau);

    void uniformTesselation();

    vector< vector <vector<glm::vec4> > > getPatches();
    vector< vector <glm::vec4>* >* getShapes();
    vector< vector <glm::vec4>* >* getNormals();
}; // end model

#endif