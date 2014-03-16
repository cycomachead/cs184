//
//
#include "scene.h"

///////////////////////////////////////////////////////////////////////////////
void Scene::initialize() {
    tracer  = RayTracer(maxDepth);
    sampler = Sampler(width, height);
    film    = Film(width, height, output.c_str());
    camera  = Camera(lookFrom, lookAt, up, fov, width, height);
}

void Scene::render() {
    while (!sampler.isDone()) {
        sample = sampler.generateSample();
        ray    = camera.generateRay(sample);
        color  = tracer.trace(ray, 1);
        film.commit(sample, color);
    }
    film.writeImage();
}
///////////////////////////////////////////////////////////////////////////////

Sphere* sphere;
void Scene::loadScene(int sceneNo) {
    lookFrom << 5, 5, 5;
    lookAt << 0, 0, 0;
    up << 0, 1, 0; 
    fov = 100;
    if (sceneNo == 0) {
        sphere = new Sphere(0, 0 , 0, 1);
        GeometricPrimitive* geoPrimitive = new GeometricPrimitive();
        geoPrimitive->thing = sphere;
        geoPrimitive->objToWorld = Transformation::identity();
        geoPrimitive->worldToObj = Transformation::identity();
        BRDF* b = new BRDF();
        b->ka = Color(0.5f, 0.1f, 0.2f);
        geoPrimitive->brdf = b;
        primitives.push_back(geoPrimitive);
    }
    initialize();
}

void Scene::loadScene(string filename) {
    // Parse the file and load in what's in the file.
    initialize();
}