// AS4 ARM.CPP

#include "arm.h"

using namespace Eigen;

/*
 * If no parent is provided, then this is the first arm in the series.
 * It's starting point would be the origin.
 */
Arm::Arm(float len, Matrix4f transform) {
    this->inPos = NULL;
    this->length = len;
    this->setLocal(transform);
    // TODO outPos;

}

Arm::Arm(Arm* parent, float len, Matrix4f transform) {
    this->parent = parent;
    this->parent->child = this;

    this->inPos = &this->parent->outPos;
    this->length = len;
    this->setLocal(transform);
    // TODO outPos;
}

Vector3f Arm::getLocalOutPos() {
    Vector3f initial = Vector3f(0.0f, 0.0f, 0.0f);

    if (this->inPos != NULL) {
        initial = *this->inPos;
    }
    // FIXME
    return initial;
}