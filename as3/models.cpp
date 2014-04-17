#include "models.h"
#include "bezier.h"

/** Utility Functions **/
/** Euclid takes in two points: A and B and returns
	the Euclidean distance between the two points. **/
float euclid(glm::vec3 a, glm::vec3 b) {
    return sqr(b.x - a.x) + sqr(b.y - a.y) + sqr(b.z - a.z);
}

/** Is Flat takes in two points A and B and returns 
	true if the distance between the two points is less than 
	error. Returns false otherwise. **/
bool isflat(glm::vec3 a, glm::vec3 b, float error) {
    return euclid(a, b) < error;
}


/** AdaptiveModel Function implementations **/

/** The constructor for the Adaptive Model class.
	The constructor takes in PATCHES containing the control 
	point of the surface. ERROR will be used to in
	the isFlat function where we determine if two points during
	the adaptive tesselation are close enough to be considered flat. **/
AdaptiveModel::AdaptiveModel(vector< vector <vector<glm::vec3> > > patches, float error) {
    modelPatches = patches;
    errorBound = sqr(error);
    shapes = new vector< vector<glm::vec3>* >();
    normals = new vector< vector<glm::vec3>* >();
    adaptiveTesselation();
}

/** For each patch in patches, we split one patch in two triangles,
	and start the tesselation of the triangles. **/
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

/** U and V represent the three vertices on the triangle we are
	trying to tesselate. We determine if the midpoint between the 
	vertices are flat. If they aren't we divide and recursively 
	call createTriangles until all triangles are flat. **/
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
    if (bc and ab and ac) {
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
    } else if (bc and ab and !ac) {
        float nextu1[3] = {u[0], u[1], acu};
        float nextv1[3] = {v[0], v[1], acv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[2], acu, u[1]};
        float nextv2[3] = {v[2], acv, v[1]};
        createTriangles(nextu2, nextv2);
    } else if (bc and !ab and ac) {
        float nextu1[3] = {u[0], abu, u[2]};
        float nextv1[3] = {v[0], abv, v[2]};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[1], abu, u[2]};
        float nextv2[3] = {v[1], abv, v[2]};
        createTriangles(nextu2, nextv2);
    } else if (!bc and ab and ac) {
        float nextu1[3] = {u[0], bcu, u[2]};
        float nextv1[3] = {v[0], bcv, v[2]};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], bcu, u[1]};
        float nextv2[3] = {v[0], bcv, v[1]};
        createTriangles(nextu2, nextv2);
    } else if (bc and !ab and !ac) {
        float nextu1[3] = {u[0], abu, acu};
        float nextv1[3] = {v[0], abv, acv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[1], abu, u[2]};
        float nextv2[3] = {v[1], abv, v[2]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[2], abu, acu};
        float nextv3[3] = {v[2], abv, acv};
        createTriangles(nextu3, nextv3);
    } else if (!bc and !ab and ac) {
        float nextu1[3] = {u[0], abu, bcu};
        float nextv1[3] = {v[0], abv, bcv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], bcu, u[2]};
        float nextv2[3] = {v[0], bcv, v[2]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[1], abu, bcu};
        float nextv3[3] = {v[1], abv, bcv};
        createTriangles(nextu3, nextv3);   
    } else if (!bc and ab and !ac) {
        float nextu1[3] = {u[2], acu, bcu};
        float nextv1[3] = {v[2], acv, bcv};
        createTriangles(nextu1, nextv1);
        float nextu2[3] = {u[0], acu, u[1]};
        float nextv2[3] = {v[0], acv, v[1]};
        createTriangles(nextu2, nextv2);
        float nextu3[3] = {u[1], acu, bcu};
        float nextv3[3] = {v[1], acv, bcv};
        createTriangles(nextu3, nextv3);  
    } else if (!bc and !ab and !ac) {
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

/** returns the patches that represent the surface. **/
vector< vector <vector<glm::vec3> > > AdaptiveModel::getPatches() {
    return modelPatches;
}

/** Returns the shapes to be drawn. **/
vector <vector<glm::vec3>* >* AdaptiveModel::getShapes() {
    return shapes;
}

/** Returns the Normals to be drawn. **/
vector <vector<glm::vec3>* >* AdaptiveModel::getNormals() {
    return normals;
}

/** Uniform Model Functions **/

/** This constructor takes in PATCHES, which contains control point for the surface.
	It also takes in STEP, which is used to deterime: 1 / STEP, the number
	of subdivision for each patch. **/
UniformModel::UniformModel(vector< vector <vector<glm::vec3> > > patches, float step) {
    modelPatches = patches;
    stepSize = step;
    shapes = new vector< vector<glm::vec3>* >();
    normals = new vector< vector<glm::vec3>* >();
    uniformTesselation();
}

/** This will simply pass each patch to the bezier function subdividepatch
	and construct rectangles there. **/
void UniformModel::uniformTesselation() {
    // iterate over patches and create 1D vectors
    for(int i = 0; i < modelPatches.size(); i += 1) {
        vector <vector<glm::vec3> > patch = modelPatches.at(i);
        subdividepatch(patch, stepSize, shapes, normals);
    }
}

/** Returns the patches that represents the control points. **/
vector< vector <vector<glm::vec3> > > UniformModel::getPatches() {
    return modelPatches;
}

/** Returns the rectangle that needs to be drawn. **/
vector <vector<glm::vec3>* >* UniformModel::getShapes() {
    return shapes;
}

/** Returns the normal for each vertice of the shape that needs
	to be drawn. **/
vector <vector<glm::vec3>* >* UniformModel::getNormals() {
    return normals;
}

