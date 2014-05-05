// AS4 ARM.CPP

#include "arm.h"

using namespace Eigen;

/*
 * If no parent is provided, then this is the first arm in the series.
 * It's starting point would be the origin.
 */
Arm::Arm(float len, Transformation transform) {
    this->inPos  = new Vector3f(0.0f, 0.0f, 0.0f);
    this->length = len;

    this->setLocal(transform);
    this->outPos = this->getLocalOutPos();

}

Arm::Arm(Arm* paren, float len, Transformation transform) {
    this->parent        = paren;
    this->parent->child = this;

    this->inPos         = &(this->parent->outPos);
    this->length        = len;
    this->setLocal(transform);
    this->outPos = this->getLocalOutPos();
}

Vector3f Arm::getLocalOutPos() {
    Vector3f initial = Vector3f(this->length, 0.0f, 0.0f);

    cout << "IN POS NULL?? " << this->inPos << endl;
    if (this->inPos != NULL) {
        initial = *(this->inPos);
    }

    initial[0] += this->length;
    initial = this->getLocal() * initial;

    cout << "RESULT: " << initial << endl;
    return initial;
}

/*
 * Draw this object as a solid cone in OpenGL GLUT.
 * Must be called from within a GLUT drawing loop.
 */
void Arm::draw() {
    glDisable(GL_LIGHTING);
    Vector3f initial = Vector3f(0.0f, 0.0f, 0.0f);

    if (this->inPos != NULL) {
        initial = *(this->inPos);
    }

    COLOR_BLUE
    glPushMatrix();
    glTranslatef(initial.x(), initial.y(), initial.z());
    // glutWireCone(.67, this->length, 50, 50);
    // glRotatef(65, -1.0, 0.0, 0.0);
    // Radius, Length, Slices, subdivisions
    glPopMatrix();


    glLineWidth(10.0f);
    COLOR_GREEN
    glBegin(GL_LINES);
    glVertex3f(initial.x(), initial.y(), initial.z());
    glVertex3f(this->outPos.x(), this->outPos.y(), this->outPos.z());
    glEnd();
}

/*
 * Draw the entire arm system. We go through one by one
 * until we hit a child, everytime we call draw.
 */
void Arm::drawSystem(int i) {
    this->draw();
    cout << "CHILD??   " << this->parent << endl;
    if (this->parent != NULL and i < 3) {
        cout << "Draw Recursion" << endl;
        this->parent->drawSystem(i + 1);
    }
}

