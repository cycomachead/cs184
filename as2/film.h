#include "raytracer.h"

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