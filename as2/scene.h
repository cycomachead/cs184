//
//
#ifndef SCENE_H
#define SCENE_H

#include "raytracer.h"

#include "simple.h"
#include "objects.h"
#include "lights.h"
#include "tracer.h"

class Scene {
public:
    float fov;

    int width = 100,
        height = 100,
        maxDepth = 5;

    Point lookFrom, lookAt, up;

    string output = "raytracer_out.png";

    vector<Shape> shapes;
    vector<Primitive> primitives;
    vector<Light> lights;

    Camera camera;
    Color color;
    Film film;
    Ray ray;
    Sample sample;
    Sampler sampler;
    RayTracer tracer;

    Scene() {
        // empty constructor
    }

    void loadScene(int sceneNo) {
        initialize();
    }

    void loadScene(string filename) {
        // Parse the file and load in what's in the file.
        initialize();
    }

    void initialize() {
        tracer  = RayTracer(maxDepth, this);
        sampler = Sampler(width, height);
        film    = Film(width, height, output.c_str());
        camera  = Camera(lookFrom, lookAt, up, fov, width, height);
    }

    void render() {
        while (!sampler.isDone()) {
            sample = sampler.generateSample();
            ray    = camera.generateRay(sample);
            color  = tracer.trace(ray, 1);
            film.commit(sample, color);
        }
        film.writeImage();
    }

};

#endif