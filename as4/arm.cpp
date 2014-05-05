// AS4 ARM.CPP

#include "arm.h"

using namespace Eigen;

/*
 * If no parent is provided, then this is the first arm in the series.
 * It's starting point would be the origin.
 */
Arm::Arm(float len, Transformation transform) {
    this->inPos  = NULL;
    this->length = len;

    this->setLocal(transform);
    this->outPos = this->getLocalOutPos();

    // TODO outPos;

}

Arm::Arm(Arm* parent, float len, Transformation transform) {
    this->parent        = parent;
    this->parent->child = this;

    this->inPos         = &(this->parent->outPos);
    this->length        = len;
    this->setLocal(transform);
    this->outPos = this->getLocalOutPos();
}

Vector3f Arm::getLocalOutPos() {
    Vector3f initial = Vector3f(0.0f, 0.0f, 0.0f);

    if (this->inPos != NULL) {
        initial = *(this->inPos);
    }

    initial = this->getLocal() * initial;

    return initial;
}

/*
 * Draw this object as a solid cone in OpenGL GLUT.
 * Must be called from within a GLUT drawing loop.
 */
void Arm::draw() {

}

/*
 * Draw the entire arm system. We go through one by one
 * until we hit a child, everytime we call draw.
 */
void Arm::drawSystem() {
    this->draw();
    if (child != NULL) {
        child->drawSystem();
    }
}



