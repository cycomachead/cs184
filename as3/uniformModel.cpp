// Model Object for AS3

#include "UniformModel.h"
#include "bezier.h"


void UniformModel::uniformTesselation() {
    // iterate over patches and create 1D vectors
    for(int i = 0; i < modelPatches.size(); i += 1) {
        vector <vector<glm::vec4> > patch = modelPatches.at(i);
        subdividepatch(patch, errorBound, shapes, normals);
    }
}


UniformModel::UniformModel(vector< vector <vector<glm::vec4> > > patches, float tau) {
    modelPatches = patches;
    errorBound = stepSize = tau;
    shapes = new vector< vector<glm::vec4>* >();
    normals = new vector< vector<glm::vec4>* >();
    uniformTesselation();
}

vector< vector <vector<glm::vec4> > > UniformModel::getPatches() {
    return modelPatches;
}

vector <vector<glm::vec4>* >* UniformModel::getShapes() {
    return shapes;
}

vector <vector<glm::vec4>* >* UniformModel::getNormals() {
    return normals;
}

