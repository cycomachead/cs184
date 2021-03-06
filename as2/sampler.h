//
//
#ifndef SAMPLER_H
#define SAMPLER_H

#include "simple.h"

class Sampler {
public:
    int width, height,
        currX, currY, x, y;
    bool done = false;

    Sampler() {
        // empty constructor
    }

    Sampler(int w, int h) {
        width = w;
        height = h;
        currX = 0;
        currY = 0;
        done = false;
    }

    bool isDone() {
        return done;
    }

    Sample generateSample() {
        x = currX;
        y = currY;
        if (currX < width - 1) {
            currX += 1;
        } else { // (currX == width - 1)
            if (currY < height - 1) {
                currY += 1;
                currX = 0;
            } else {
                done = true;
            }
        }
        return Sample(x, y);
    }


/*
Notes:
    It will generate (x,y) of a screen sample and return true. Next time it
    gets called, it will generate another sample for the next pixel.
    It will return false when all the samples from all the pixels
    are generated. (In our case, we generate 1 sample per pixel, at
    the pixel sample. Later on, if we want to do multi-sample per pixel,
    we need to modify this class.
*/

};

#endif
