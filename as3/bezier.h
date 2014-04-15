#ifndef bezier_h
#define bezier_h

#include "as3.h"

/** Uniform Tesselation functions. Look at cpp file for each function
	description.**/
using namespace std;

glm::vec3 bezcurveinterp(vector< glm::vec3 > curve, float u, glm::vec3* dPdu);
vector< vector<glm::vec3> > transpose(vector< vector< glm::vec3 > > patch);
glm::vec3 normalize(glm::vec3 point);
glm::vec3 cross(glm::vec3 a, glm::vec3 b);
glm::vec3 bezpatchinterp(vector< vector< glm::vec3 > > patch, float u, float v, glm::vec3* normal);
void subdividepatch(vector< vector< glm::vec3 > > patch, float step,
	vector< vector< glm::vec3 >*>* p, vector<vector< glm::vec3 >*>* n);

#endif