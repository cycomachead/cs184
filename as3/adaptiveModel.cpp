// Model Object for AS3

#include "adaptiveModel.h"
#include "bezier.h"

float euclid(glm::vec3 a, glm::vec3 b) {
    return sqr(a.x - b.x) + sqr(a.y - b.y) + sqr(a.z - b.z);
}

bool isflat(glm::vec3 a, glm::vec3 b, float error) {
    return euclid(a, b) < error;
}

void AdaptiveModel::adaptiveTesselation() {
    // iterate over patches and create 1D vectors
    for(int i = 0; i < modelPatches.size(); i += 1) {
        curr = modelPatches.at(i);
        float u1[3] = {0, 0, 1};
        float v1[3] = {0, 1, 1};
        createTriangles(u1, v1);
        float u2[3] = {0, 1, 1};
        float v2[3] = {0, 0, 1};
        createTriangles(u2, v2);
    }
}

void AdaptiveModel::createTriangles(float u[], float v[]) {
    glm::vec3* n1 = new glm::vec3();
    glm::vec3* n2 = new glm::vec3();
    glm::vec3* n3 = new glm::vec3();
    glm::vec3* temp = new glm::vec3();
    glm::vec3 a = bezpatchinterp(curr, u[0], v[0], n1);
    glm::vec3 b = bezpatchinterp(curr, u[1], v[1], n2);
    glm::vec3 c = bezpatchinterp(curr, u[2], v[2], n3);
    float abu = (u[0] + u[1])/2;
    float abv = (v[0] + v[1])/2;
    float bcu = (u[1] + u[2])/2;
    float bcv = (v[1] + v[2])/2;
    float acu = (u[0] + u[2])/2;
    float acv = (v[0] + v[2])/2;
    bool ab = isflat((a + b) / 2.0f, 
        bezpatchinterp(curr, abu, abv, temp), errorBound);
    bool bc = isflat((b + c)/ 2.0f, 
        bezpatchinterp(curr, bcu, bcv, temp), errorBound);
    bool ac = isflat((a + c)/ 2.0f, 
        bezpatchinterp(curr, acu, acv, temp), errorBound);
    if (ab and bc and ac) {
        vector<glm::vec3>* shape = new vector<glm::vec3>();
        vector<glm::vec3>* normal = new vector<glm::vec3>();
        shape->push_back(a);
        shape->push_back(b);
        shape->push_back(c);
        normal->push_back(*n1);
        normal->push_back(*n2);
        normal->push_back(*n3);
        shapes->push_back(shape);
        normals->push_back(normal);
    } else if (ab and bc and !ac) {
        float nextu1[3] = {u[0], u[1], acu};
        float nextv1[3] = {v[0], v[1], acv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[2], acu, u[1]};
        float nextv2[3] = {v[2], acv, v[1]};
        createTriangles(nextu2, nextv2);
    } else if (ab and !bc and ac) {
        float nextu1[3] = {u[0], abu, u[2]};
        float nextv1[3] = {v[0], abv, v[2]};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[1], abu, u[2]};
        float nextv2[3] = {v[1], abv, v[2]};
        createTriangles(nextu2, nextv2);
    } else if (!ab and bc and ac) {
        float nextu1[3] = {u[0], bcu, u[2]};
        float nextv1[3] = {v[0], bcv, v[2]};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], bcu, u[1]};
        float nextv2[3] = {v[0], bcv, v[1]};
        createTriangles(nextu2, nextv2);
    } else if (ab and !bc and !ac) {
        float nextu1[3] = {u[0], abu, acu};
        float nextv1[3] = {v[0], abv, acv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[1], abu, u[2]};
        float nextv2[3] = {v[1], abv, v[2]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[2], abu, acu};
        float nextv3[3] = {v[2], abv, acv};
        createTriangles(nextu3, nextv3);
    } else if (!ab and !bc and ac) {
        float nextu1[3] = {u[0], abu, bcu};
        float nextv1[3] = {v[0], abv, bcv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], bcu, u[2]};
        float nextv2[3] = {v[0], bcv, v[2]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[1], abu, bcu};
        float nextv3[3] = {v[1], abv, bcv};
        createTriangles(nextu3, nextv3);   
    } else if (!ab and bc and !ac) {
        float nextu1[3] = {u[2], acu, bcu};
        float nextv1[3] = {v[2], acv, bcv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], acu, u[1]};
        float nextv2[3] = {v[0], acv, v[1]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[1], acu, bcu};
        float nextv3[3] = {v[1], acv, bcv};
        createTriangles(nextu3, nextv3);  
    } else if (!ab and !bc and !ac) {
        float nextu1[3] = {u[0], acu, abu};
        float nextv1[3] = {v[0], acv, abv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[1], abu, bcu};
        float nextv2[3] = {v[1], abv, bcv};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[2], acu, bcu};
        float nextv3[3] = {v[2], acv, bcv};
        createTriangles(nextu3, nextv3); 
        float nextu4[3] = {abu, acu, bcu};
        float nextv4[3] = {abv, acv, bcv};
        createTriangles(nextu4, nextv4);
    }

}


AdaptiveModel::AdaptiveModel(vector< vector <vector<glm::vec3> > > patches, float error) {
    modelPatches = patches;
    errorBound = sqr(error);
    shapes = new vector< vector<glm::vec3>* >();
    normals = new vector< vector<glm::vec3>* >();
    adaptiveTesselation();
}

vector< vector <vector<glm::vec3> > > AdaptiveModel::getPatches() {
    return modelPatches;
}

vector <vector<glm::vec3>* >* AdaptiveModel::getShapes() {
    return shapes;
}

vector <vector<glm::vec3>* >* AdaptiveModel::getNormals() {
    return normals;
}

