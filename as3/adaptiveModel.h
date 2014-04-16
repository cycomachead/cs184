#ifndef ADAPTIVEMODEL_H
#define ADAPTIVEMODEL_H


#include "as3.h"

using namespace std;
class Vertex;
float sqr(float x) {
	return x * x;
}

class AdaptiveModel {
public:

    vector< vector <vector<glm::vec3> > > modelPatches;
    vector< vector< glm::vec3 >* >* shapes;
    vector< vector< glm::vec3 >* >* normals;
    float errorBound;

    AdaptiveModel() {
        // empty
    }

    AdaptiveModel(vector< vector <vector<glm::vec3> > > patches, float error);
    void adaptiveTesselation();
    void createTriangles(float u[], float v[], vector <vector<glm::vec3> > patch);
    vector< vector <vector<glm::vec3> > > getPatches();
    vector< vector <glm::vec3>* >* getShapes();
    vector< vector <glm::vec3>* >* getNormals();
}; // end model

class Vertex {
public:
	float u;
	float v;
	glm::vec3* point;
	glm::vec3* surface;
	glm::vec3* normal;
	float errorBound;

	Vertex() {
		//empty
	}

	Vertex(float u_, float v_, float error,
		glm::vec3* p, vector< vector< glm::vec3 > > patch) {
		u = u_;
		v = v_;
		point = p;
		glm::vec3 surfaceTemp = bezpatchinterp(patch, u, v, normal);
		surface = new glm::vec3(surfaceTemp);
		errorBound = error;
	}

	/** Mid point between vertex a and vertex b. **/
	Vertex(Vertex a, Vertex b, vector< vector< glm::vec3 > > patch) {
		u = (a.u + b.u) / 2.0f;
		v = (a.v + b.v) / 2.0f;
		glm::vec3 tempPoint = (*a.point + *b.point) / 2.0f;
		point = new glm::vec3(tempPoint);
		glm::vec3 surfaceTemp = bezpatchinterp(patch, u, v, normal);
		surface = new glm::vec3(surfaceTemp);
		errorBound = a.errorBound;
	}

	bool isFlat() {
		return sqrt(sqr(point->x - surface->x) +
			sqr(point->y - surface->y) + 
			sqr(point->z - surface->z)) < errorBound;
	}

	glm::vec3 getPoint() {
		return *point;
	}

	glm::vec3 getSurface() {
		return *surface;
	}

	glm::vec3 getNormal() {
		return *normal;
	}
};

#endif