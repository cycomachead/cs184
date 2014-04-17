#ifndef MODELS_H
#define MODELS_H

#include "as3.h"

using namespace std;

class AdaptiveModel {
public:

    vector< vector <vector<glm::vec3> > > modelPatches;
    vector< vector< glm::vec3 >* >* shapes;
    vector< vector< glm::vec3 >* >* normals;
    vector <vector<glm::vec3> > curr;
    float errorBound;

    AdaptiveModel() {
        // empty
    }

    AdaptiveModel(vector< vector <vector<glm::vec3> > > patches, float error);
    void adaptiveTesselation();
    void createTriangles(float u[], float v[]);
    vector< vector <vector<glm::vec3> > > getPatches();
    vector< vector <glm::vec3>* >* getShapes();
    vector< vector <glm::vec3>* >* getNormals();
};

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