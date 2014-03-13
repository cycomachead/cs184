//
//
#ifndef SAMPLER_H
#define SAMPLER_H

#include "simple.h"

class Sampler {
public:
    bool getSample(Sample* sample);

    Sampler(int width, int height) {

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
