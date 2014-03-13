//
//
#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"

#include "simple.h"
#include "tracer.h"

class Scene {
public:
    float fov;

    int width = 500,
        height = 500,
        maxDepth = 5;

    Point lookFrom, lookAt, up;

    string output = "raytracer_out.png";

    // Vector4f corners[4];

    void loadScene(int sceneNo) {

        initialize();
    }

    void loadScene(string filename) {
        // Parse the file and load in what's in the file.

        initialize();
    }

    void initialize() {
        RayTracer tracer = RayTracer(maxDepth);
        Sampler sampler  = Sampler(width, height);
        Film film        = Film(width, height, output.c_str());
        Camera camera    = Camera(lookFrom, lookAt, up, fov);
    }

    void render() {
        Sample sample;
        /*
        while (!sampler.generateSample(sample) {
            camera.generateRay(sample, ray);
            tracer.trace(ray, color);
            film.commit(sample, color);
        }
        film.writeImage();
        */
    }

};

#endif