// Model Object for AS3

#include "model.h"


void Model::buildUniformVertexNormal() {
    // iterate over patches and create 1D vectors
    for(int i = 0; i < modelPatches.size(); i += 1) {
        vector <vector<glm::vec4> > patch = modelPatches.at(i);
        subdividepatch(patch, errorBound, verticies, normals);
    }
}

Model::Model(vector< vector <vector<glm::vec4> > > patches, float tau) {
    modelPatches = patches;
    errorBound = stepSize = tau;
    verticies = new vector<glm::vec4>();
    normals = new vector<glm::vec4>();
    buildUniformVertexNormal();
}

vector< vector <vector<glm::vec4> > > Model::getPatches() {
    return modelPatches;
}

vector <vector<glm::vec4> > Model::getPatchN(int i) {
    return modelPatches.at(i);
}

// TODO:
// vector<glm::vec4> Model::getCorners(int n) {
//     
// }