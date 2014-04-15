// CS184 AS3 Bezier Surfaces
// Tomo Ueda, Michael Ball
// Patch.cpp

#include "patch.h"

// THIS NEEDS TO BE COMMENTED OUT FOR COMPILING TEST.CPP
// LEAVE UNCOMMENTED FOR AS3.CPP
int LOGLEVEL;

// Utility Functions
float min(float a, float b, float c) {
    return min(a, min(b, c));
}

float max(float a, float b, float c) {
    return max(a, max(b, c));
}


// Empty Initializer
Patch::Patch() {

}

// Initialize the Patch from a bez file
Patch::Patch(vector< vector<glm::vec3> > input, float err) {
    this->order = 3;
    this->isQuad = true;
    this->parent = NULL;
    this->error = err;
    this->uStart = 0.0f;
    this->vStart = 0.0f;
    this->uEnd   = 1.0f;
    this->vEnd   = 1.0f;
    this->originalData = &input;
    //this->controlPoints = new vector<glm::vec3>();
    this->corners = new vector<glm::vec3>();
    this->patches = new vector<Patch*>();
    // move the input data to 1D form. convert to vec3.
    for(int i = 0; i < input.size(); i += 1) {
        vector<glm::vec3> item = input.at(i);
        for(int j = 0; j < item.size(); j += 1) {
            glm::vec3 point = glm::vec3(item.at(j));
            this->controlPoints->push_back(point);
        }
    }
    // this is hard coded and bad.. FIXME

    this->corners->push_back(this->controlPoints->at(0));
    this->corners->push_back(this->controlPoints->at(3));
    this->corners->push_back(this->controlPoints->at(12));
    this->corners->push_back(this->controlPoints->at(15));
    if (LOGLEVEL) {
        cout << "CREATED NEW PATCH" << endl;
    }
}

Patch::Patch(Patch&) {

}

Patch::Patch(vector<glm::vec3> corners) {
    // Check if corners are 3 or 4
    // set corners to vector
    // set triangle and order properties.
    // initialize pointers to other arrays so they aren't null?
    if (LOGLEVEL > 5) {
        cout << "Creating new patch from corner points " << endl;
        cout << "Corner Size: " << corners.size() << endl;
    }
    this->order = 3;
    // if this patch is a triangle or not..
    this->isQuad = (corners.size() == 4);
    // this may not be the best thing to do...
    this->controlPoints = new vector<glm::vec3>(corners);
    this->corners = new vector<glm::vec3>(corners);
    this->patches = new vector<Patch*>();
}

// Return the control point at that index, usually 0-15
glm::vec3 Patch::getVertex(int i) {
    if (LOGLEVEL > 4) {
        cout << "Getting control point: " << i << endl;
    }
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
    if (LOGLEVEL > 4) {
        cout << "SUCCESSFULLY CREATED QUAD INDEX VECTOR" << endl;
    }
    return indices;
}

// Return the 4 corners of the quad ready to be drawn.
vector<glm::vec3> Patch::getQuad(int in) {
    // if (!this->isQuad) {
    //     // FIXME...
    //     return this->getCorners();
    // }

    vector<glm::vec3> result = vector<glm::vec3>();
    int* i = this->quadIndicies(in);
    // TL TR LL RR
    for(int j = 0; j < 4; j += 1) {
        result.push_back(this->getVertex(i[j]));
    }
    return result;
}

// Return the 4 corners of this patch
vector<glm::vec3> Patch::getCorners() {
    // FIXME -- sketchy
    return *(this->corners);
}

// Return the U and V points for one of the original quads
// VECTOR: uStart, uEnd, vStart, vEnd
vector<float> Patch::getUVQuad(int n) {
    vector<float> result = vector<float>();
    int m = n % 3;
    float uS = m / 3;
    float vS = (float) floor(n / 3);
    if (vS == 0.0f) {
        vS = 2 / 3;
    } else if (vS == 1.0f) {
        vS = 1 / 3;
    } else {
        vS = 0.0f;
    }

    float uE = uS + (1 / 3);
    float vE = vS + (1 / 3);

    result.push_back(uS);
    result.push_back(uE);
    result.push_back(vS);
    result.push_back(vE);

    return result;
}

// Get the U and V values based on the corners of the current patch.
// Store them in the standard vector format.
// This basically creates a bounding box.
vector<float> Patch::getUVTri() {
    vector<float> result = vector<float>();

    // where MIN is taken, this value should be 1, so as to not affect it
    // where MAX, this should be 0 -- all values are in [0.0, 1.0]
    float uS4 = 1.0f;
    float uE4 = 0.0f;
    float vS4 = 1.0f;
    float vE4 = 0.0f;

    if (this->corners->size() == 4) {
        glm::vec3 pt = this->corners->at(3);
        uS4 = pt.x;
        uE4 = pt.x;
        vS4 = pt.y;
        vE4 = pt.y;
    }

    float uS = min(this->corners->at(0).x, this->corners->at(1).x,
        this->corners->at(2).x);
    float uE = max(this->corners->at(0).x, this->corners->at(1).x,
        this->corners->at(2).x);
    float vS = min(this->corners->at(0).y, this->corners->at(1).y,
        this->corners->at(2).y);
    float vE = max(this->corners->at(0).y, this->corners->at(1).y,
        this->corners->at(2).y);

    result.push_back(uS);
    result.push_back(uE);
    result.push_back(vS);
    result.push_back(vE);

    return result;
}

// Set the U and V values for the patch based on a vector generated by a get
// method.
void Patch::setUV(vector<float> in) {
    this->uStart = in.at(0);
    this->uEnd   = in.at(1);
    this->vStart = in.at(2);
    this->vEnd   = in.at(3);
}

// this works for cases of two but will need to be fixed for up to 4...
// FIXME -- this coulf use a constructor to build the result from corners.
// FIXME -- does this work for 2, 3??
vector<glm::vec3> Patch::getTri(int n) {
    vector<glm::vec3> result;

    if (LOGLEVEL > 5) {
        cout << "GETTING TRIANGLES " << this->controlPoints->size() << endl;
    }
    for(int i = n; i < n + 3; i += 1) {
        result.push_back(this->controlPoints->at(i));
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
        // update the U V start and end values for the new patch
        tri->setUV(this->getUVQuad(i));
        if (LOGLEVEL > 4) {
            cout << "CREATED A NEW PATCH... " << i << endl;
        }
        // This will make the call that can recursively split triangles.
        tri->subdivideTriangle();
        tri->parent = this;
        tri->controlPoints = this->controlPoints;
        // This will add 1 patch to the list which contains 2
        this->patches->push_back(tri);
    }
}

// Subdivide the necessary TRIANGULAR pathces into triangles
void Patch::subdivideTriangle() {
    // if this is a triangle, create TWO new patches based on corners.
    // Patch 1 is a triangle, corners 0, 1, 2. Patch 2 uses corners 1, 2, 3
    // This needs checking and protection against other checking.
    int num = 2;
    for(int i = 0; i < num; i += 1) {
        if (LOGLEVEL > 5) {
            cout << "SUBDIVISION: n: " << i << endl;
        }
        Patch* p = new Patch(this->getTri(i));
        p->parent = this;
        p->controlPoints = this->controlPoints;
        // set U and V values for the new patch.
        //p->setUV(this->getTri(i)->getUVTri());
        if (LOGLEVEL > 5) {
            cout << "PATCH MADE..." << endl;
        }
        this->patches->push_back(p);
        if (LOGLEVEL > 5) {
            cout << "New Patch added to list" << endl;
        }
        int flat = p->isFlat(p->getCorners());
        if (!flat) {
            p->subdivideTriangle(flat);
        }
    }
}

// Subdivide based on the method given. These correspend to the last slide of
// lecture 12, numbered 1-7.
// 0, implies no division.
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
    glm::vec3 midA, midB, midC;
    float midU, midV;
    bool sideA, sideB, sideC;
    midA = (this->corners->at(0) + this->corners->at(1)) / 2.0f;
    midB = (this->corners->at(1) + this->corners->at(2)) / 2.0f;
    midC = (this->corners->at(0) + this->corners->at(2)) / 2.0f;
    midU = (this->uStart + this->uEnd) / 2;
    midV = (this->vStart + this->vEnd) / 2;
    sideA = this->sideIsFlat(midA, midU, midV);
    sideB = this->sideIsFlat(midB, midU, midV);
    sideC = this->sideIsFlat(midC, midU, midV);
    return 0;
}

bool Patch::sideIsFlat(glm::vec3 midpt, float midU, float midV) {
    return false;
}

// A DFS of all the polygons in this vector
// eventually it will call .corners on the polygon and append to the list.
vector< vector<glm::vec3> > Patch::getPolygons() {
    vector< vector<glm::vec3> > all = vector< vector<glm::vec3> >();
    for(int i = 0; i < patches->size(); i += 1) {
        Patch* p = this->patches->at(i);
        vector< vector<glm::vec3> > newVect = p->getPolygons();
        all.insert(all.end(), newVect.begin(), newVect.end());
    }
    if (!this->hasChildren()) {
        all.push_back(this->getCorners());
    }
    return all;
}