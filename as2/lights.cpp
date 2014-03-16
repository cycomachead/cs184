//
//

#include "lights.h"

//***************************************************************************//
// LIGHTS //
// NOTHING SHOULD BE DEFINED HERE.
//***************************************************************************//
// POINT LIGHTS //
//***************************************************************************//
pLight::generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor) {
    lray->pos = local.pos;
    lray->dir = this->position - local.pos;
    lray->t_min = 0.00001;
    lray->t_max = FLT_MAX;
    *lcolor = color;
    
}
//***************************************************************************//
// DIRECTIONAL LIGHTS //
//***************************************************************************//
dLight::generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor) {
    lray->pos = local.pos;
    lray->dir = -1 * this->position;
    lray->t_min = 0.03;
    lray->t_max = FLT_MAX;
    *lcolor = color;
    
}