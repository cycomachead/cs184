// AS4 ARM.CPP

#include "arm.h"

using namespace Eigen;

Matrix3f crossProductMatrix(Vector3f cross) {
    Matrix3f mat = *new Matrix3f();
    mat << 0, -cross[2], cross[1],
           cross[2], 0, -cross[0],
           -cross[1], cross[0], 0;
    return mat;
}

/*
 * If no parent is provided, then this is the first arm in the series.
 * It's starting point would be the origin.
 */
Arm::Arm(float len, float x, float y, float radian) {
    _x = x;
    _y = y;
    _radian = radian;
    this->inPos  = new Vector3f(0.0f, 0.0f, 0.0f);
    this->length = len;

    this->setLocal();
    this->outPos = this->getLocalOutPos();

}

Arm::Arm(Arm* paren, float len, float x, float y, float radian) {
    _x = x;
    _y = y;
    _radian = radian;
    this->parent        = paren;
    this->parent->child = this;

    this->inPos         = &(this->parent->outPos);
    this->length        = len;
    this->setLocal();
    this->outPos = this->getLocalOutPos();
}

Vector3f Arm::getLocalOutPos() {
    Vector3f initial = Vector3f(this->length, 0.0f, 0.0f);

    if (this->inPos != NULL) {
        initial = *(this->inPos);
    }

    initial[0] += this->length;
    initial = this->getLocal() * initial;

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
    // cout << "CHILD??   " << this->parent << endl;
    if (this->parent != NULL and i < 3) {
        // cout << "Draw Recursion" << endl;
        this->parent->drawSystem(i + 1);
    }
}


/*
 * All the hard work goes here...
 */
void Arm::update(Vector3f dest) {
    Vector3f dp = this->outPos - dest;
    Matrix3f J = makeJacobian();
    Matrix3f pseudo = J.transpose() * (J * J.transpose()).inverse();
    Vector3f difs = pseudo * dp;
    _x = _x + difs[0];
    _y = _y + difs[1];
    _radian = _radian + difs[2];
    this->setLocal();
    this->outPos = this->getLocalOutPos();
    if (this->parent != NULL) {
        this->parent->update(*this->inPos);
    }
}

/** Returns this arms jacobian. **/
Matrix3f Arm::makeJacobian() {
    Matrix3f J = crossProductMatrix(this->outPos);
    J = J * this->localTransformation.getMatrix() * -1;
    if (this->parent != NULL) {
        J = this->parent->worldTransformation.getMatrix() * J;
    }
    Arm* child = this->child;
    while (child != NULL) {
        J = J * child->localTransformation.getMatrix();
        child = child->child;
    }
    return J;
}


Arm* Arm::getOldest() {
    Arm* arm = this;

    while (arm != NULL) {
        arm = arm->parent;
    }

    return arm;
}

Arm* Arm::getYoungest() {
    Arm* arm = this;

    while (arm != NULL) {
        arm = arm->child;
    }

    return arm;
}
