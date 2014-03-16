//
//
#ifndef BRDF_H
#define BRDF_H

#include "common.h"
#include "simple.h"

class BRDF {
public:
    Color kd, ks, ka, kr;
    float p; // specular coefficient
    BRDF() {
        this->ka = Color(0.0f, 0.0f, 0.0f);
        this->kd = Color(0.0f, 0.0f, 0.0f);
        this->kr = Color(0.0f, 0.0f, 0.0f);
        this->ks = Color(0.0f, 0.0f, 0.0f);
        this->p = 1;
    }
    
    BRDF(Color a, Color d, Color r, Color s, float p1) {
        this->ka = a;
        this->kd = d;
        this->kr = r;
        this->ks = s;
        this->p = p1;
    }
// Storing information enough for shading (it is not the actual BRDF function
// in the rendering equation that will be covered later in the semester)
// Members:
    // kd, ks, ka are diffuse, specular and ambient component respectively
    // kr is the mirror reflection coefficient
    
    BRDF operator=(BRDF *other) {
        return BRDF(other->ka, other->kd, other->kr, other->ks, other->p);
    }
    
    BRDF operator=(BRDF other) {
        return BRDF(other.ka, other.kd, other.kr, other.ks, other.p);
    }
};

#endif