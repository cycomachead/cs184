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

    int width = 100,
        height = 100,
        maxDepth = 5;

    Point lookFrom, lookAt, up;

    string output = "raytracer_out.png";

//     vector<Shape> shapes;
//     vector<Primitive> primitives;
//     vector<Light> lights;

    Camera camera;
    Color color;
    Film film;
    Ray ray;
    Sample sample;
    Sampler sampler;
    RayTracer tracer;

    Scene() { 
    }

    void loadScene(int sceneNo) {
        initialize();
    }

    void loadScene(string filename) {
        // Parse the file and load in what's in the file.
        initialize();
    }

    void initialize() {
        tracer  = RayTracer(maxDepth);
        sampler = Sampler(width, height);
        film    = Film(width, height, output.c_str());
        camera  = Camera(lookFrom, lookAt, up, fov);
    }

    void render() {
        while (!sampler.generateSample(&sample)) {
            camera.generateRay(sample, &ray);
            tracer.trace(ray, 1, &color);
            film.commit(sample, color);
        }
        film.writeImage();
    }

};

#endif