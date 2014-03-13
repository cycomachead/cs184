//
//
#ifndef FILM_H
#define FILM_H


#include "raytracer.h"

class Film {
public:
    int width = 0, height = 0;
    char const* filename;
    vector<unsigned char> image;

    Film() { 
    
    }

    Film(int w, int h, char const* name) {
        filename = name;
        width = w;
        height = h;
        for(int i = 0; i < w*h; i += 4) { // minor loop unrolling.
            image.push_back(0); // rgba, black for outside circle
            image.push_back(0);
            image.push_back(0);
            image.push_back(255);
            image.push_back(0); // rgba, black for outside circle
            image.push_back(0);
            image.push_back(0);
            image.push_back(255);
            image.push_back(0); // rgba, black for outside circle
            image.push_back(0);
            image.push_back(0);
            image.push_back(255);
            image.push_back(0); // rgba, black for outside circle
            image.push_back(0);
            image.push_back(0);
            image.push_back(255);
        }
    }

    void commit(Sample sample, Color color) {
        // the vector is a linear ordering of all pixels.
        // (final.b * 255) > 255 ? 255 : (final.b * 255)

        int pos = 4 * ((sample.x * width*height) + sample.y);
        color *= 255;
        color.max();
        image.at(pos + 0) = color.r();
        image.at(pos + 1) = color.g();
        image.at(pos + 2) = color.b();
        image.at(pos + 3) = 255;
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

#endif