//
//

#include "lights.h"

//***************************************************************************//
// LIGHTS //
// NOTHING SHOULD BE DEFINED HERE.
//***************************************************************************//
// POINT LIGHTS //
//***************************************************************************//
void pLight::generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor) {
    lray->pos = local.pos;
    lray->dir = this->position - local.pos;
    lray->tMin = 0.00001;
    lray->tMax = FLT_MAX;
    *lcolor = this->color;
    
}
//***************************************************************************//
// DIRECTIONAL LIGHTS //
//***************************************************************************//
void dLight::generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor) {
    lray->pos = local.pos;
    lray->dir = -1 * this->position;
    lray->tMin = 0.03;
    lray->tMax = FLT_MAX;
    *lcolor = this->color;
    
}