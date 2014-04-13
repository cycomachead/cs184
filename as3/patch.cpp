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

Patch::Patch(vector<glm::vec3> corners) {
    // Check if corners are 3 or 4
    // set corners to vector
    // set triangle and order properties.
    // initialize pointers to other arrays so they aren't null?

    this->order = 3;
    // if this patch is a triangle or not..
    this->isQuad = (corners.size() == 4);
    // this may not be the best thing to do...
    this->controlPoints = &corners;
    this->corners = new vector<glm::vec3>(corners);
    this->patches = new vector<Patch*>();
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

// this works for cases of two but will need to be fixed for up to 4...
// FIXME -- this coulf use a constructor to build the result from corner.
vector<glm::vec3> Patch::getTri(int n) {
    vector<glm::vec3> result;
    if (n == 0) {
        result.at(0) = this->controlPoints->at(0);
        result.at(1) = this->controlPoints->at(1);
        result.at(2) = this->controlPoints->at(2);
    } else {
        result.at(0) = this->controlPoints->at(1);
        result.at(1) = this->controlPoints->at(2);
        result.at(2) = this->controlPoints->at(3);
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
        // This will make the call that can recursively split triangles.
        tri->subdivideTriangle();
        tri->parent = this;
        // This will add 1 patch to the list which contains 2
        this->patches->push_back(tri);
    }
}

// Subdivide the necessary TRIANGULAR pathces into triangles
void Patch::subdivideTriangle() {
    // if this is a triangle, create TWO new patches based on corners.
    // Patch 1 is a triangle, corners 0, 1, 2. Patch 2 uses corners 1, 2, 3
    // Use isFlat to check if the new patches need to be subdivided.
    int num = 2;
    for(int i = 0; i < num; i += 1) {
        Patch* p = new Patch(this->getTri(i));
        this->patches->push_back(p);
        int flat = p->isFlat(p->getCorners());
        if (!flat) {
            p->subdivideTriangle(flat);
        }
    }
}

//
void Patch::subdivideTriangle(int method) {

}


// Return true IFF there are patches to recurse into.
bool Patch::hasChildren() {
    return this->patches->size() > 0;
}

// Return all the polygons for drawing the object.
// vector< vector<glm::vec3> > Patch::getPolygons() {
// }

// check the flatness of a triangle.
int Patch::isFlat(vector<glm::vec3> poly) {
    // FIXME... for now, nothing is flat.
    return 0;
}