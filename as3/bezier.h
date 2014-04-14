#ifndef bezier_h
#define bezier_h

#include "as3.h"

/** Uniform Tesselation functions. Look at cpp file for each function
	description.**/
using namespace std;

glm::vec4 bezcurveinterp(vector< glm::vec4 > curve, float u, glm::vec4* dPdu);
vector< vector<glm::vec4> > transpose(vector< vector< glm::vec4 > > patch);
glm::vec4 normalize(glm::vec4 point);
glm::vec4 cross(glm::vec4 a, glm::vec4 b);
glm::vec4 bezpatchinterp(vector< vector< glm::vec4 > > patch, float u, float v, glm::vec4* normal);
void subdividepatch(vector< vector< glm::vec4 > > patch, float step,
	vector< vector< glm::vec4 >*>* p, vector<vector< glm::vec4 >*>* n);

#endif