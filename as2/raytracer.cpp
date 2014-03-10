#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

#include "lib/lodepng.h"

// X11 and Eigen don't play nicely...
#undef Success
#include "lib/Eigen/Eigen"

using namespace std;
using namespace Eigen;
// using namespace pngwriter;

// GLOBAL VARIABLES
// Stupid logging global because lazy.
int LOGGING = 0;
string COMMANDS;


class Scene {
public:
    int width;
    int height;
    Vector3f camera;
    Vector4f corners[4];
   
};

class Sampler {
    /*       
    Methods:
        bool getSample(Sample* sample);
    Notes:
        It will generate (x,y) of a screen sample and return true. Next time it 
        gets called, it will generate another sample for the next pixel. 
        It will return false when all the samples from all the pixels
        are generated. (In our case, we generate 1 sample per pixel, at 
        the pixel sample. Later on, if we want to do multi-sample per pixel, 
        we need to modify this class.
    */


};

class Camera {};

class Raytracer {};

class Primitive {};

class Film {
private:
    vector<unsigned char> image;
    int width, height;
    char const* filename;
    
public:
    Film() {
        
    }
    
    Film(char const* name, int w, int h) {
        filename = name;
        width = w;
        height = h;
    }
    
    void addPixel(Vector3f color) {
        image.push_back(color(0));
        image.push_back(color(1));
        image.push_back(color(2));
        image.push_back(255); // Alpha Channel
    }
    
    void writeImage() {
        unsigned error = lodepng::encode(filename, image, width, height);
        
        if (error and LOGGING > 0) { // if there's an error, display it
            cerr << "encoder error " << error << ":\n\t";
            cerr << lodepng_error_text(error) << endl;
            cerr << "\n debug info:";
            cerr << "\n Vector:\t" << image.size();
            cerr << "\n Expected Size:\t" << width * height * 4;
        }
    }
};


void argParse(int argc, const char ** argv);


int main(int argc, const char ** argv) {

    return 0;
}

void argParse(int argc, const char ** argv) {
    // Basically, all I care about is getting the filename to parse
    // Then the parser will take care of the dirty work.

}

/**
Normal
Members:
                        float x, y, z
            Notes:
                        Constructor from 3 floats
                        Support +, -
                        Note: Need to be normalized after operations (Be careful for 0 vector)
 
Point
            Members:
                        float x, y, z
            Notes:
                        Constructor from 3 floats
                        Support +,- with vector
           
Ray
Members:
                        Point pos
                        Vector dir
                        float t_min, t_max
 
            Notes:
It represent the ray ray(t) = pos + t*dir, where t_min <= t <= t_max
 
Matrix
Members:
                        float mat[4][4]
 
            Notes:
                        Support creation of rotation, translation, scaling matrices
                        May support matrix inversion if needed
Also could support SVD, or other matrix decomposition, for future extension
           
Transformation
Members:
            // Storing matrix m and its inverse transpose, minvt (for transforming normal)
                        Matrix m, minvt
Notes:
                        Support Point, Vector, Normal, Ray, LocalGeo transformation by
operator * overloading
 
Color
            Members:
                        float r, g, b
            Notes:
                        Support +,- with other color
                        Support scalar *, /
                        May support conversion from xyz
 
 
BRDF
// Storing information enough for shading (it is not the actual BRDF function // in the rendering equation that will be covered later in the semester)
Members:
            // kd, ks, ka are diffuse, specular and ambient component respectively
            // kr is the mirror reflection coefficient
                        Color kd, ks, ka, kr
                       
 
Sample
Members:
                        float x, y; // Store screen coordinate
 
LocalGeo
Members:
                        Point pos
                        Normal normal
            Notes: 
                        Store the local geometry at the intersection point. May need to store
                        other quantities (eg. texture coordinate) in a more complicated
raytracer.
           
More Classes
Shape
            Methods:
                        // Test if ray intersects with the shape or not (in object space), if so,
// return intersection point and normal
                        bool intersect(Ray& ray, float* thit, LocalGeo* local)   
                       
                        // Same as intersect, but just return whether there is any intersection or
// not
                        bool intersectP(Ray& ray)
 
            Notes:
                        // Triangle and Sphere are probably best implemented here
                        // The intersection with the ray at t outside the range [t_min, t_max]
                        // should return false.
 
Primitive
            Methods:
                        bool intersect(Ray& ray, float* thit, Intersection* in)     
                        bool intersectP(Ray& ray)
                        void getBRDF(LocalGeo& local, BRDF* brdf);
                       
            Notes:
                        Abstract class for primitives in the scene
 
Intersection
            Members:
                        LocalGeo localGeo
                        Primitive* primitive
 
GeometricPrimitive
            Members:
                        Transformation objToWorld, worldToObj;
                        Shape* shape;
                        Material* mat;
 
            Methods:
                        bool intersect(Ray& ray, float* thit, Intersection* in)  {
                                    Ray oray = worldToObj*ray;
                                    LocalGeo olocal;                                 
                                    if (!shape->intersect(oray, thit, &olocal))  return false;
                                    in->primitive = this;
                                    in->local = objToWorld*olocal;
                                    return true;                               
                        }
 
                        bool intersectP(Ray& ray) {
                                    Ray oray = worldToObj*ray;
                                    return shape->intersectP(oray);                        
}
 
                        void getBRDF(LocalGeo& local, BRDF* brdf) {
                                    material->getBRDF(local, brdf);
}
           
                       
AggregatePrimitive
            Methods:                     
                        AggregatePrimitive(vector<Primitive*> list);                 
                        bool intersect(Ray& ray, float* thit, Intersection* in)     
                        bool intersectP(Ray& ray)
void getBRDF(LocalGeo& local, BRDF* brdf) {
            exit(1);
            // This should never get called, because in->primitive will
            // never be an aggregate primitive
}
           
            Notes:
                        Constructor store the STL vector of pointers to primitives.
                        Intersect just loops through all the primitives in the list and
call the intersect routine. Compare thit and return that of the nearest one (because we want the first hit).
 Also, the in->primitive should be set to the pointer to that primitive.
                        When you implement acceleration structure, it will replace this class.
 
Material
            Members:
                        BRDF constantBRDF;
            Methods:
void getBRDF(LocalGeo& local, BRDF* brdf) {
            return constantBRDF;
}
            Notes:
                        Class for storing material. For this example, it just returns a constant
                        material regardless of what local is. Later on, when we want to support
                        texture mapping, this need to be modified.
 
Camera
            Methods:
                        void generateRay(Sample& sample, Ray* ray);
            Notes:
                        Create a ray starting from the camera that passes through the
                        corresponding pixel (sample.x, sample.y) on the image plane.
                        (from last week discussion, and also section 10.1 in Shirley’s book)
 
RayTracer
            Methods:
                        void trace(Ray& ray, int depth, Color* color) {
                                    if (depth exceed some threshold) {
                                                Make the color black and return
                                    }
                                    if (!primitive.intersect(ray, &thit, &in) {
                                                // No intersection
                                                Make the color black and return
                                    }
                                    // Obtain the brdf at intersection point
                                    in.primitive->getBRDF(in.local, &brdf);
 
                                    // There is an intersection, loop through all light source
                                    for (i = 0; i < #lights; i++) {
                                                lights[i].generateLightRay(in.local, &lray, &lcolor);
                                               
                                                // Check if the light is blocked or not
                                                if (!primitive->intersectP(lray))
// If not, do shading calculation for this
// light source
                                                            *color += shading(in.local, brdf, lray, lcolor);
}
 
// Handle mirror reflection
if (brdf.kr > 0) {
            reflectRay = createReflectRay(in.local, ray);
 
            // Make a recursive call to trace the reflected ray
            trace(reflectRay, depth+1, &tempColor);
            *color += brdf.kr * tempColor;
}
                                   
                        }
 
            Notes:
                        Shading is similar to hw2
                        Beware when you generate reflection ray, make sure the ray don’t start
exactly on the surface, or the intersection routine may return
intersection point at the starting point of the ray. (This apply to light
ray generation as well)
 
Light
            Methods:
                        void generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor);
            Notes:
                        This is an abstract class that will generate a ray starting from
                        the position stored in local to the position of the light source.
                        You might want to consider creating 2 derived classes for
                        point light source and directional light source.
For directional light, the origin of the ray is the same, and the ray points to the light direction, however, t_max is infinity.
  

Scene
            Methods:
                        // This is the main rendering loop
                        void render() {
                                    while (!sampler.generateSample(&sample) {
                                                camera.generateRay(sample, &ray);
                                                raytracer.trace(ray, &color);
                                                film.commit(sample, color);
                                    }
                                    film.writeImage();
                        }


*/