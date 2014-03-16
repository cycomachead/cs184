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
    lookFrom << 0, 0, 4;
    lookAt << 0, 0, 0;
    up << 0, 1, 0;
    fov = 90;
    if (sceneNo == 0) {
        sphere = new Sphere(0, 0, 0, 1);
        cout << "WHAT THE FUCK " << sphere << endl;
        GeometricPrimitive* geoPrimitive = new GeometricPrimitive();
        geoPrimitive->thing = sphere;
        geoPrimitive->objToWorld = Transformation::identity();
        geoPrimitive->worldToObj = Transformation::identity();
        primitives.push_back(geoPrimitive);
        cout << "SHAPPPE?: " << (*geoPrimitive).getShape() << endl;
        cout << "WHAT THE FUCK " << sphere << endl;
    }
    initialize();
}

void Scene::loadScene(string filename) {
    // Parse the file and load in what's in the file.
    initialize();
}