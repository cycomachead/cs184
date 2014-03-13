// Basic utilities for the raytracer


class Color: public Vector3f {
private:
    Vector3f data;

public:
    Color() {}

    Color(Vector3f d) { data = d; }

    Color(float r, float g, float b) {
        data(0) = r;
        data(1) = g;
        data(2) = b;
    }

    float r() { return data(0); }

    float g() { return data(1); }

    float b() { return data(2); }

    void setR(float d) { data(0) = d; }

    void setG(float d) { data(1) = d; }

    void setB(float d) { data(2) = d; }
};

class Ray {
// Members:
//      Point pos
//      Vector dir
//      float t_min, t_max
// Notes:
// It represent the ray ray(t) = pos + t*dir, where t_min <= t <= t_max


};

class Sampler {
/*
Notes:
    It will generate (x,y) of a screen sample and return true. Next time it
    gets called, it will generate another sample for the next pixel.
    It will return false when all the samples from all the pixels
    are generated. (In our case, we generate 1 sample per pixel, at
    the pixel sample. Later on, if we want to do multi-sample per pixel,
    we need to modify this class.
*/

public:
    bool getSample(Sample* sample);

};

class Camera {
/*
Methods:
Notes:
            Create a ray starting from the camera that passes through the
            corresponding pixel (sample.x, sample.y) on the image plane.
            (from last week discussion, and also section 10.1 in Shirley’s book)
*/
public:
     void generateRay(Sample& sample, Ray* ray);
};

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