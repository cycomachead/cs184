// Model Object for AS3

#include "model.h"

Model::Model(vector< vector <vector<glm::vec3> > > patches, float tau) {
    modelPatches = patches;
    errorBound = stepSize = tau;
    verticies = new vector<glm::vec3>();
    normals = new vector<glm::vec3>();
    adapPatches = new vector<Patch*>();
}

vector< vector <vector<glm::vec3> > > Model::getPatches() {
    return modelPatches;
}

vector <vector<glm::vec3> > Model::getPatchN(int i) {
    return modelPatches.at(i);
}

// Create the patch objects for adaptive subdivision
void Model::buildAdaptive() {
    for(int i = 0; i < modelPatches.size(); i += 1) {
        vector< vector<glm::vec3> > temp = modelPatches.at(i);
        Patch* p = new Patch(temp, this->errorBound);
        adapPatches->push_back(p);
    }
}

vector <vector<glm::vec3> > Model::getAllPolygons() {
    vector <vector<glm::vec3> > result = vector <vector<glm::vec3> >();
    for(int i = 0; i < this->adapPatches->size(); i += 1) {
        Patch* p = this->adapPatches->at(i);
        vector <vector<glm::vec3> > poly = p->getPolygons();
        result.insert(result.end(), poly.begin(), poly.end());
    }
    return result;
}

void Model::subdivideAll() {
    for(int i = 0; i < this->adapPatches->size(); i += 1) {
        Patch* p = this->adapPatches->at(i);
        p->subdivideQuad();
    }
}