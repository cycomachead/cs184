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
    Vector3f initial = Vector3f(0.0f, 0.0f, 0.0f);

    if (this->inPos != NULL) {
        initial = *(this->inPos);
    }

    COLOR_BLUE
    glPushMatrix();
    glTranslatef(initial.x(), initial.y(), initial.z());
    // glRotatef(65, -1.0, 0.0, 0.0);
    // Radius, Length, Slices, subdivisions
    glutSolidCone(2, this->length, 50, 50);
    glPopMatrix();

    glPointSize(3.0f);
    COLOR_YELLOW
    glBegin(GL_POINTS);
    glVertex3f(initial.x(), initial.y(), initial.z());
    glVertex3f(this->outPos.x(), this->outPos.y(), this->outPos.z());
    glEnd();
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

