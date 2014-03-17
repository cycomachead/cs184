//
//
#include "scene.h"
#include "loader.cpp"

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
    lookFrom << 3, 3, 3;
    lookAt << 0, 0, 0;
    up << 0, 1, 0; 
    fov = 60;
    if (sceneNo == 0) {
        Sphere* sphere = new Sphere(0, 0 , 0, 1.5);
        GeometricPrimitive* geoPrimitive = new GeometricPrimitive();
        geoPrimitive->thing = sphere;
        geoPrimitive->objToWorld = Transformation::identity();
        geoPrimitive->worldToObj = Transformation::identity();
        BRDF* b = new BRDF();
        // b->ka = Color(0.5f, 0.1f, 0.2f);
        b->kd = Color(1.0f, 0.0f, 2.0f);
        pLight* l = new pLight();
        l->position = Vector3f(500, -50, 40);
        l->color = Color(0.8f, 0.4f, 0.4f);
        lights.push_back(l);
        geoPrimitive->brdf = b;
        primitives.push_back(geoPrimitive);
    }
    initialize();
}

void Scene::loadScene(string filename) {
    // Parse the file and load in what's in the file.
    if (LOGGING > 1) {
        cout << "PARSING FILE..." << endl;
    }
    loadX(filename);
    initialize();
}