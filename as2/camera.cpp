//
//

#include "camera.h"

#include "simple.h"
#include "sampler.h"
#include "film.h"

Camera::Camera(Point from, Point at, Point u, float f, int w, int h) {
    float width, height, aspect;
    lookFrom = from;
    lookAt = at;
    up = u;
    fovY = f;
    pixelWidth = w;
    pixelHeight = h;


    aspect =  pixelWidth/pixelHeight;
    height = 2 * tan(M_PI/180 * .5 * fovY);
    width = height * aspect;


    z = lookAt - lookFrom;
    x = z.cross(up);
    y = x.cross(z);

    deltaX = x * (width / pixelWidth);
    deltaY = y * (height / pixelHeight);

    centerX = pixelWidth/2.0 + 0.5; // 0.5 == pixel centers
    centerY = pixelHeight/2.0 + 0.5;

    if (LOGGING > 4) {
        cout << "CAMERA DATA:\n\tx:\n" << deltaX << "\n\ty:\n" << deltaY << endl;
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
    Vector3f v = deltaY * (sample.y - centerY) + z + deltaX * (sample.x - centerX);
    return Ray(
        lookFrom,
        v,
        0.0f,
        FLT_MAX);
}