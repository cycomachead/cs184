// ARM.H

#ifndef ARM_H
#define ARM_H

#include "as4.h"
#include "transformation.h"

/*
 * Arm class which stores all the data about each elment in an IK system
 * Functions as a doubly linked list.
 */
class Arm {
private:

public:
    // Links to other arms
    Arm* parent;
    Arm* child;

    // My start and ending positions
    Eigen::Vector3f* inPos;
    Eigen::Vector3f outPos;

    float length;

    // Transformation Matricies

    Transformation worldTransformation;

    Transformation localTransformation;

    Arm() {
        // empty constructor
    }

    Arm(float, Transformation);

    Arm(Arm*, float, Transformation);

    // Setters and getters for stored matricies
    void setWorld(Transformation world) {
        this->worldTransformation = world;
    }

    Transformation getWorld() {
        return this->worldTransformation;
    }

    void setLocal(Transformation local) {
        this->localTransformation = local;
    }

    Transformation getLocal() {
        return this->localTransformation;
    }

    Eigen::Vector3f getLocalOutPos();

    void update(Eigen::Vector3f);

    // Drawing uses GLUT and must be called from within a GLUT loop.
    void draw();

    void drawSystem(int);



#endif

