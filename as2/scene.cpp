#include "raytracer.h"

using namespace std;
using namespace Eigen;

class Scene {
public:
    int width = 500;
    int height = 500;
    int maxDepth = 5;

    Vector4f corners[4];

    void loadScene(int sceneNo) {

    }

    void loadScene(string filename) {

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