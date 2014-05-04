// ARM.H

#ifndef ARM_H
#define ARM_H

#include "as4.h"

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

    Eigen::MatrixXf worldTransformation;

    Eigen::Matrix4f localTransformation;

    Arm() {
        // empty constructor
    }

    Arm(float, Eigen::Matrix4f);

    Arm(Arm*, float, Eigen::Matrix4f);

    // Setters and getters for stored matricies
    void setWorld(Eigen::MatrixXf world) {
        this->worldTransformation = world;
    }

    Eigen::MatrixXf getWorld() {
        return this->worldTransformation;
    }

    void setLocal(Eigen::Matrix4f local) {
        this->localTransformation = local;
    }

    Eigen::Matrix4f getLocal() {
        return this->localTransformation;
    }

    Eigen::Vector3f getLocalOutPos();

    void update(Eigen::Vector3f);
};


#endif

