//
//

#include "camera.h"

#include "simple.h"
#include "sampler.h"
#include "film.h"

Camera::Camera(Point from, Point at, Point u, float f, int w, int h) {
/*
http://run.usc.edu/cs480-s11/lec15-ray-tracing/15-ray-tracing-6up.pdf
http://www.cse.ohio-state.edu/~parent/classes/681/Lectures/08.RTgeometryHO.pdf
*/
    float width, height, aspect;
    lookFrom = from;
    lookAt = at;
    up = u;
    fovY = f;
    pixelWidth = w;
    pixelHeight = h;


    aspect =  (float)pixelWidth/(float)pixelHeight;
    height = 2 * tan(M_PI/180 * .5 * fovY);
    width = height * aspect;

    // http://stackoverflow.com/questions/13078243/ray-tracing-camera
    // camera_position = lookFrom;
    camera_direction = lookAt - lookFrom;
    camera_direction.normalize(); // Z-Vector
    up.normalize();
    camera_right = camera_direction.cross(up); // X-Vector
    camera_up = camera_right.cross(camera_direction); // Y-Vector
    
    // z = lookAt - lookFrom;
//     z.normalize();
//     up.normalize();
//     x = z.cross(up);
//     y = x.cross(z);

    deltaX = camera_right * (width / pixelWidth);
    deltaY = camera_up * (height / pixelHeight);

    centerX = pixelWidth/2.0 + 0.5; // 0.5 == pixel centers
    centerY = pixelHeight/2.0 + 0.5;

    if (LOGGING > 4) {
        cout << "CAMERA DATA:\n\tx:\n" << deltaX << "\n\ty:\n" << deltaY << endl;
        cout << "WIDTH: " << width << "\tHEIGHT: " << height << "\t ASPECT: ";
        cout << aspect << endl << "CENTER X:" << centerX << "\tCENTER Y";
        cout << centerY << endl;
    }

    // ORIGINCAL FORMULA
    // deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX)
    // CAN I FACTOR???
    // dirConst = deltaY * -1 * centerY + z + deltaX * -1 * centerX;

}

// modify poiter parameter.
void Camera::generateRay(Sample& sample, Ray* ray) {
    ray->pos  = lookFrom;
    ray->tMin = 0.0f;
    ray->tMax = FLT_MAX;
    // The only interesting calculation:
    // (dir is a vector3f)
    ray->dir  = deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX);
}

// same function but return a new ray
Ray Camera::generateRay(Sample& sample) {
    return Ray(
        lookFrom,
        deltaY * (sample.y - centerY) + camera_direction + deltaX * (sample.x - centerX),
        0.0f,
        FLT_MAX);
}