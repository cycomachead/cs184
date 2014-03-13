#include "raytracer.h"

class Scene {
public:
    int width = 500,
        height = 500,
        maxDepth = 5;

    Vector4f corners[4];

    void loadScene(int sceneNo) {
        
        initialize();
    }

    void loadScene(string filename) {
        
        initialize();
    }

    void initialize() {
        tracer  = RayTracer(maxDepth);
        sampler = Sampler(width, height);
        film    = Film(width, height, outputName.c_str());
        camera  = Camera(lookfromx,lookfromy,lookfromz,lookatx,lookaty,lookatz,upx,upy,upz,fov);
    }

    void render() {
        /*
        while (!sampler.generateSample(&sample) {
            camera.generateRay(sample, &ray);
            raytracer.trace(ray, &color);
            film.commit(sample, color);
        }
        film.writeImage();
        */
    }

};