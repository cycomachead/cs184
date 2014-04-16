#include "as3.h"

/** given the control points of a bezier curve
and a parametric value, return the curve 
point and update the derivative **/
glm::vec3 bezcurveinterp(vector< glm::vec3 > curve, float u, glm::vec3* dPdu) {
	// first, split each of the three segments
    // to form two new ones AB and BC
    glm::vec3 A = curve.at(0) * (float) (1.0 - u) + curve.at(1) * u;
    glm::vec3 B = curve.at(1) * (float) (1.0 - u) + curve.at(2) * u;
    glm::vec3 C = curve.at(2) * (float) (1.0 - u) + curve.at(3) * u;

	// now, split AB and BC to form a new segment DE
    glm::vec3 D = A * (float) (1.0 - u) + B * u;
    glm::vec3 E = B * (float) (1.0 - u) + C * u;

    // finally, pick the right point on DE,
    // this is the point on the curve

    glm::vec3 p = D * (float) (1.0 - u) + E * u;

    // compute derivative also
    *dPdu = (E - D) * (float) 3.0;

    return p;
}

/** given a 16 x 16 get the transpose of the patch
	to invert u and v orientations. **/
vector< vector<glm::vec3> > transpose(vector< vector< glm::vec3 > > patch) {
	vector< glm::vec3 > v1;
	v1.push_back(patch.at(0).at(0));
	v1.push_back(patch.at(1).at(0));
	v1.push_back(patch.at(2).at(0));
	v1.push_back(patch.at(3).at(0));
	vector< glm::vec3 > v2;
	v2.push_back(patch.at(0).at(1));
	v2.push_back(patch.at(1).at(1));
	v2.push_back(patch.at(2).at(1));
	v2.push_back(patch.at(3).at(1));
	vector< glm::vec3 > v3;
	v3.push_back(patch.at(0).at(2));
	v3.push_back(patch.at(1).at(2));
	v3.push_back(patch.at(2).at(2));
	v3.push_back(patch.at(3).at(2));
	vector< glm::vec3 > v4;
	v4.push_back(patch.at(0).at(3));
	v4.push_back(patch.at(1).at(3));
	v4.push_back(patch.at(2).at(3));
	v4.push_back(patch.at(3).at(3));
	vector< vector< glm::vec3> > transpose;
	transpose.push_back(v1);
	transpose.push_back(v2);
	transpose.push_back(v3);
	transpose.push_back(v4);
	return transpose;
}


/** given a control patch and (u,v) values, find 
 the surface point and update normal **/
glm::vec3 bezpatchinterp(vector< vector< glm::vec3 > > patch, float u, float v, glm::vec3* normal) {
	glm::vec3* dPdv = new glm::vec3();
	glm::vec3* dPdu = new glm::vec3();
	// build control points for a Bezier curve in v
	vector<glm::vec3> vcurve;

	vcurve.push_back(bezcurveinterp(patch.at(0), u, dPdv));
	vcurve.push_back(bezcurveinterp(patch.at(1), u, dPdv));
	vcurve.push_back(bezcurveinterp(patch.at(2), u, dPdv));
	vcurve.push_back(bezcurveinterp(patch.at(3), u, dPdv));

	// build control points for a Bezier curve in u
	vector<glm::vec3> ucurve;
	vector< vector< glm::vec3 > > vPatch = transpose(patch);

	ucurve.push_back(bezcurveinterp(vPatch.at(0), v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch.at(1), v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch.at(2), v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch.at(3), v, dPdu));

	// evaluate surface and derivative for u and v
	glm::vec3 p;

	p = bezcurveinterp(vcurve, v, dPdv);
	p = bezcurveinterp(ucurve, u, dPdu);

	// take cross product of partials to find normal
	glm::vec3 c = glm::cross( *dPdu, *dPdv);
	// glm::vec3 c = cross(*dPdu, *dPdv);

	glm::vec3 n = glm::normalize(c);	

	*normal = n;

	return p;
}

/** given a patch, perform uniform subdivision
	result is updated in p and n a vector user passes in.
	Matching index will indicate matching surface point and normal.
	p and n should be empty before the function call. **/
void subdividepatch(vector< vector< glm::vec3 > > patch, float step, 
	vector< vector< glm::vec3 >* >* p, vector< vector< glm::vec3 >* >* n) {
	float u;
	float v;
	// compute how many subdivisions there 
	// are for this step size
	// We are also omitting epsilon.. 
	int numDiv = (int) 1 / step;
	// for each parametric value of u
	for (int iu = 0; iu < numDiv; iu++) {
		u = iu * step;

		// for each parametric value of v
		for (int iv = 0; iv < numDiv; iv++) {
			v = iv * step;
			// evaluate surface
			if (u + step <= 1 || v + step <= 1) {
				glm::vec3* normal = new glm::vec3();
				glm::vec3* normal2 = new glm::vec3();
				glm::vec3* normal3 = new glm::vec3();
				glm::vec3* normal4 = new glm::vec3();
				glm::vec3 point = bezpatchinterp(patch, u, v, normal);
				glm::vec3 point2 = bezpatchinterp(patch, u + step, v, normal2);
				glm::vec3 point3 = bezpatchinterp(patch, u + step , v + step, normal3);
				glm::vec3 point4 = bezpatchinterp(patch, u, v + step, normal4);
				vector< glm::vec3 >* quad = new vector< glm::vec3>();
				vector< glm::vec3 >* normals = new vector< glm::vec3>();
				quad->push_back(point);
				quad->push_back(point2);
				quad->push_back(point3);
				quad->push_back(point4);
				normals->push_back(*normal);
				normals->push_back(*normal2);
				normals->push_back(*normal3);
				normals->push_back(*normal4);
				p->push_back(quad);
				n->push_back(normals);
			}
		}
	}
}











