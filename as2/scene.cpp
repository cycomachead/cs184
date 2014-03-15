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


void Scene::loadScene(int sceneNo) {
    if (sceneNo == 0) {
        Sphere* sphere = new Sphere(0, 0, 0, 2);
        GeometricPrimitive* geoPrimitive = new GeometricPrimitive(sphere);
        primitives.push_back(geoPrimitive);
    }
    initialize();
}

void Scene::loadScene(string filename) {
    // Parse the file and load in what's in the file.
    initialize();
}