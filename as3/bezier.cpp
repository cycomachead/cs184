#include "as3.h"

/** given the control points of a bezier curve
and a parametric value, return the curve 
point and update the derivative **/
glm::vec4 bezcurveinterp(vector< glm::vec4 > curve, float u, glm::vec4* dPdu) {
	// first, split each of the three segments
    // to form two new ones AB and BC
    glm::vec4 A = curve.at(0) * (float) (1.0 - u) + curve.at(1) * u;
    glm::vec4 B = curve.at(1) * (float) (1.0 - u) + curve.at(2) * u;
    glm::vec4 C = curve.at(2) * (float) (1.0 - u) + curve.at(3) * u;

	// now, split AB and BC to form a new segment DE
    glm::vec4 D = A * (float) (1.0 - u) + B * u;
    glm::vec4 E = B * (float) (1.0 - u) + C * u;

    // finally, pick the right point on DE,
    // this is the point on the curve

    glm::vec4 p = D * (float) (1.0 - u) + E * u;

    // compute derivative also
    *dPdu = (E - D) * (float) 3.0;

    return p;
}

/** given a 16 x 16 get the transpose of the patch
	to invert u and v orientations. **/
vector< vector<glm::vec4> > transpose(vector< vector< glm::vec4 > > patch) {
	vector< glm::vec4 > v1;
	v1.push_back(patch.at(0).at(0));
	v1.push_back(patch.at(1).at(0));
	v1.push_back(patch.at(2).at(0));
	v1.push_back(patch.at(3).at(0));
	vector< glm::vec4 > v2;
	v2.push_back(patch.at(0).at(1));
	v2.push_back(patch.at(1).at(1));
	v2.push_back(patch.at(2).at(1));
	v2.push_back(patch.at(3).at(1));
	vector< glm::vec4 > v3;
	v3.push_back(patch.at(0).at(2));
	v3.push_back(patch.at(1).at(2));
	v3.push_back(patch.at(2).at(2));
	v3.push_back(patch.at(3).at(2));
	vector< glm::vec4 > v4;
	v4.push_back(patch.at(0).at(3));
	v4.push_back(patch.at(1).at(3));
	v4.push_back(patch.at(2).at(3));
	v4.push_back(patch.at(3).at(3));
	vector< vector< glm::vec4> > transpose;
	transpose.push_back(v1);
	transpose.push_back(v2);
	transpose.push_back(v3);
	transpose.push_back(v4);
	return transpose;
}

/** given a glm::vec4 we normalize the first three elements of point. **/
glm::vec4 normalize(glm::vec4 point) {
	float x = point[0];
	float y = point[1];
	float z = point[2];
	float magnitude = sqrt(x * x + y * y + z * z);
	if (magnitude > .0001) {
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	} else {
		cout << "divide by zero error!";
	}
	glm::vec4 nPoint(x, y, z, 1);
	return nPoint;
}

/** Given a 4D vector, takes the first three element and finds the cross
	product and is again converted 4D vector then returned. **/
glm::vec4 cross(glm::vec4 a, glm::vec4 b) {
	glm::vec3 alpha(a[0], a[1], a[2]);
	glm::vec3 beta(b[0], b[1], b[2]);
	glm::vec3 gamma = glm::cross(alpha, beta);
	glm::vec4 delta(gamma[0], gamma[1], gamma[2], 1);
	return delta;
}

/** given a control patch and (u,v) values, find 
 the surface point and update normal **/
glm::vec4 bezpatchinterp(vector< vector< glm::vec4 > > patch, float u, float v, glm::vec4* normal) {
	glm::vec4* dPdv;
	glm::vec4* dPdu;
	// build control points for a Bezier curve in v
	vector<glm::vec4> vcurve;

	vcurve.push_back(bezcurveinterp(patch[0], u, dPdv));
	vcurve.push_back(bezcurveinterp(patch[1], u, dPdv));
	vcurve.push_back(bezcurveinterp(patch[2], u, dPdv));
	vcurve.push_back(bezcurveinterp(patch[3], u, dPdv));

	// build control points for a Bezier curve in u
	vector<glm::vec4> ucurve;
	vector< vector< glm::vec4 > > vPatch = transpose(patch);

	ucurve.push_back(bezcurveinterp(vPatch[0], v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch[1], v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch[2], v, dPdu));
	ucurve.push_back(bezcurveinterp(vPatch[3], v, dPdu));

	// evaluate surface and derivative for u and v
	glm::vec4 p;

	p = bezcurveinterp(vcurve, v, dPdv);
	p = bezcurveinterp(ucurve, u, dPdu);

	// take cross product of partials to find normal
	glm::vec4 c = cross(*dPdu, *dPdv);
	glm::vec4 n = normalize(c);

	*normal = n;

	return p;
}

/** given a patch, perform uniform subdivision
	result is updated in p and n a vector user passes in.
	Matching index will indicate matching surface point and normal.
	p and n should be empty before the function call. **/
void subdividepatch(vector< vector< glm::vec4 > > patch, float step, 
	vector< glm::vec4 >* p, vector< glm::vec4 >* n) {
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
			glm::vec4* normal;
			glm::vec4 point = bezpatchinterp(patch, u, v, normal);
			p->push_back(point);
			n->push_back(*normal);
		}
	}
}










