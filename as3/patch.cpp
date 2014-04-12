// CS184 AS3 Bezier Surfaces
// Tomo Ueda, Michael Ball
// Patch.cpp

#include "patch.h"

// Initialize the Patch from a bez file
Patch::Patch(vector< vector<glm::vec4> > input) {
    this->order = 3;
    this->isQuad = true;
    this->parent = NULL;
    this->originalData = &input;
    this->controlPoints = new vector<glm::vec3>();
    this->corners = new vector<glm::vec3>();
    this->patches = new vector<Patch*>();
    // move the input data to 1D form. convert to vec3.
    for(int i = 0; i < input.size(); i += 1) {
        vector<glm::vec4> item = input.at(i);
        for(int j = 0; j < item.size(); j += 1) {
            glm::vec3 point = glm::vec3(item.at(j));
            this->controlPoints->push_back(point);
        }
    }
    // this is hard coded and bad.. FIXME
    this->corners->at(0) = this->controlPoints->at(0);
    this->corners->at(1) = this->controlPoints->at(3);
    this->corners->at(2) = this->controlPoints->at(12);
    this->corners->at(3) = this->controlPoints->at(15);
}

Patch::Patch(Patch&) {

}

Patch::Patch(vector<glm::vec3>) {
    
}

// Return the control point at that index, usually 0-15
glm::vec3 Patch::getVertex(int i) {
    return this->controlPoints->at(i);
}

/** This is the map of the patch, i must be [0, 9]
0   1   2   3
  0   1   2
4   5   6   7
  3   4   5
8   9   10  11
  6   7   8
12  13  14  15

*/
int* Patch::quadIndicies(int i) {
    // FORMULA?? -- ind[] = ind[0] + n + row jump 
    // Row-jump = floor(n/3) ...?
    int* indices = new int[4];
    int add = ((int) floor(i/this->order)) + i;
    // Corners of 0: {0, 1, 4, 5}
    indices[0] = 0 + add;
    indices[1] = 1 + add;
    indices[2] = 4 + add;
    indices[3] = 5 + add;
    return indices;
}

// Return the 4 corners of the quad ready to be drawn.
vector<glm::vec3> Patch::getQuad(int in) {
    if (!this->isQuad) {
        // FIXME...
        return this->getCorners();
    }
    
    vector<glm::vec3> result = vector<glm::vec3>();
    int* i = this->quadIndicies(in);
    // TL TR LL RR
    for(int j = 0; j < 4; j += 1) {
        result.at(j) = this->getVertex(i[j]);
    }
    return result;
}

// Return the 4 corners of this patch
vector<glm::vec3> Patch::getCorners() {
    return *(this->corners);
}

vector<glm::vec3> Patch::getTri(int n) {
    vector<glm::vec3> result;
    if (n == 1) {
        result.at(0) = this->controlPoints->at(0);
        result.at(1) = this->controlPoints->at(1);
        result.at(2) = this->controlPoints->at(2);
    } else {
        result.at(0) = this->controlPoints->at(0);
        result.at(1) = this->controlPoints->at(1);
        result.at(2) = this->controlPoints->at(2);
    }
    return result;
}

// Subdivide this patch into a bunch of triangles
void Patch::subdivide() {
    if (this->isQuad) {
        subdivideQuad();
    } else {
        subdivideTriangle();
    }
}

// Divide all square patches into triangles.
void Patch::subdivideQuad() {
    int numQuads = 9; // this is really hacky, but is solvable.
    for(int i = 0; i < numQuads; i += 1) {
        // Create a new patch with corners of a current segment
        Patch* tri = new Patch(this->getQuad(i));
        
        this->patches->push_back(tri);
    }
}

// Subdivide the necessary TRIANGULAR pathces into triangles
void Patch::subdivideTriangle() {
    
}