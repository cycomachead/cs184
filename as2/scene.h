//
//
#ifndef SCENE_H
#define SCENE_H

#include "common.h"

#include "sampler.h"
#include "objects.h"
#include "lights.h"
#include "loader.h"
#include "film.h"
#include "camera.h"
#include "tracer.h"

class Scene {
public:
    float fov;

    int width = 800,
        height = 800,
        maxDepth = 5;

    Point lookFrom, lookAt, up;

    string output = "image_out.png";

    // vector<Shape*> shapes; // Is this necessary?
    vector<Primitive*> primitives;
    vector<Light*> lights;
    // Transformations vector / stack?

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

    void loadScene(int sceneNo);

    void loadScene(string filename);

    void initialize();

    void render();

    vector<Primitive*> getPrims() {
        return primitives;
    }
};

extern Scene scene;
#endif