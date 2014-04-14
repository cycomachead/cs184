// Model Object for AS3

#include "UniformModel.h"


void UniformModel::uniformTesselation() {
    // iterate over patches and create 1D vectors
    for(int i = 0; i < modelPatches.size(); i += 1) {
        vector <vector<glm::vec4> > patch = modelPatches.at(i);
        subdividepatch(patch, errorBound, shapes, normals);
    }
}

void UniformModel::adaptiveTesselation() {
    //fill in later;
}

UniformModel::Model(vector< vector <vector<glm::vec4> > > patches, bool adapt, float tau) {
    modelPatches = patches;
    errorBound = stepSize = tau;
    shapes = new vector< vector<glm::vec4>* >();
    normals = new vector< vector<glm::vec4>* >();
    adaptive = adapt;
    if (!adaptive) {
        uniformTesselation();
    } else {
        adaptiveTesselation();
    }
}

vector< vector <vector<glm::vec4> > > Model::getPatches() {
    return modelPatches;
}

vector <vector<glm::vec4>* >* Model::getShapes() {
    return shapes;
}

vector <vector<glm::vec4>* >* Model::getNormals() {
    return normals;
}

