#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>
#include <stdlib.h>

/* For writing PNGs */
#include "lodepng.h"

#define PI 3.14159265  // Should be used from mathlib
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};

struct vec {
    float x;
    float y;
    float z;


    float len() {
        return sqrt(sqr(x) + sqr(y) + sqr(z));
    }

    void normalize() {
        float l = len(); // must compute len before of values change each time.
        x /= l;
        y /= l;
        z /= l;
    }

    float dotProduct(vec v2) {
        return (x * v2.x) + (y * v2.y) + (z * v2.z);
    }
};

struct clr {

    float r;
    float g;
    float b;

    void mult(int i) {
        r *= i, g *= i, b *= i;
    }
};

struct light {
    clr c;
    vec v;
};

//****************************************************
// Basic Functions
//****************************************************
float length(float x, float y, float z) {
    return sqrt(sqr(x) + sqr(y) + sqr(z));
}

float max(float x, float y) {
    return (x > y) ? x : y;
}

float dotProduct(vec v1, vec v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

clr sum3(clr c1, clr c2, clr c3) {
    clr c = {
        (c1.r + c2.r + c3.r),
        (c1.g + c2.g + c3.g),
        (c1.b + c2.b + c3.b)
    };
    return c;
}

// Predeclarations are soooo dumb.
void writePixel(int x, int y, clr px);

/////////////////////////////////
// Mostly unmodified code from lodepng example page
//////////////////////////////////////////
void encodeOneStep(const char* filename, vector<unsigned char>& image,
    unsigned width, unsigned height) {
        //Encode the image
        unsigned error = lodepng::encode(filename, image, width, height);

        if (error) { // if there's an error, display it
            cerr << "encoder error " << error << ":\n\t";
            cerr << lodepng_error_text(error) << endl;
            cerr << "\n debug info:";
            cerr << "\n Vector:\t" << image.size();
            cerr << "\n Width:\t" << width;
            cerr << "\n Height:\t" << height;
            cerr << "\n Expected Size:\t" << (width * height * 4) << "\n\n";
        }
}


//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

// LIGHTS -- Arrays and counters, [0, 5]
int PLCount = 0;
int DLCount = 0;
// Light values, up to 5 of each type.
light pLight[5];
light dLight[5];

// Color params
clr ka; // Ambient term [0, 1]
clr kd; // Diffuse term [0, 1]
clr ks; // specular term [0, 1]
float SP = 0;   // Power coefficient term [0, max_float]

int WRITE_FILE = 0; // High if the program should output a file.
int IMG_SIZE   = 0; // X-Y dimensions for image
// char[] FILE_NAME = char[100];

int CIRCLES = 1; // num of circles to be drawn

vector<unsigned char> pixels; // pixels when writing to a file.

//****************************************************
// Simple init function
//****************************************************
// Handle parsing any command line args.
void initScene(int argc, char *argv[]) {
    int pos = 1; // First arg is program name

    while (pos < argc) {
        string cur = argv[pos];
        if (cur == "-ka") { // ' and " are different in C++...wth?!
            ka.r = atof(argv[pos + 1]);
            ka.g = atof(argv[pos + 2]);
            ka.b = atof(argv[pos + 3]);
            pos += 4;
        } else if (cur == "-kd") {
            kd.r = atof(argv[pos + 1]);
            kd.g = atof(argv[pos + 2]);
            kd.b = atof(argv[pos + 3]);
            pos += 4;
        } else if (cur == "-ks") {
            ks.r = atof(argv[pos + 1]);
            ks.g = atof(argv[pos + 2]);
            ks.b = atof(argv[pos + 3]);
            pos += 4;
        } else if (cur == "-sp") {
            SP = atof(argv[pos + 1]);
            pos += 2;
        } else if (cur == "-pl") {
            pLight[PLCount].v.x = atof(argv[pos + 1]);
            pLight[PLCount].v.y = atof(argv[pos + 2]);
            pLight[PLCount].v.z = atof(argv[pos + 3]);
            pLight[PLCount].c.r = atof(argv[pos + 4]);
            pLight[PLCount].c.g = atof(argv[pos + 5]);
            pLight[PLCount].c.b = atof(argv[pos + 6]);
            PLCount += 1;
            pos += 7;
        } else if (cur == "-dl") {
            dLight[DLCount].v.x = atof(argv[pos + 1]);
            dLight[DLCount].v.y = atof(argv[pos + 2]);
            dLight[DLCount].v.z = atof(argv[pos + 3]);
            dLight[DLCount].c.r = atof(argv[pos + 4]);
            dLight[DLCount].c.g = atof(argv[pos + 5]);
            dLight[DLCount].c.b = atof(argv[pos + 6]);
            DLCount += 1;
            pos += 7;
        } else if (cur == "-file") {
            WRITE_FILE = 1;
            IMG_SIZE   = atoi(argv[pos + 1]);
            pos += 2;
        } else if (cur == "-circles") {
            CIRCLES = atoi(argv[pos + 1]);
            pos += 2;
        } else {
            cerr << "Invalid arguments found; exiting.\n";
            exit(1);
        }
    }
}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0, 0, viewport.w, viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5); // The 0.5 is to target pixel centers
  // Note: Need to check for gap bug on inst machines.
}

//****************************************************
// Draw a filled circle.
//****************************************************


void circle(float centerX, float centerY, float radius) {
    // Draw inner circle
    if (not WRITE_FILE) {
        glBegin(GL_POINTS);
    }


    // We could eliminate wasted work by only looping over the pixels
    // inside the sphere's radius.  But the example is more clear this
    // way.  In general drawing an object by loopig over the whole
    // screen is wasteful.

    int minI = max(0,(int)floor(centerX-radius));
    int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

    int minJ = max(0,(int)floor(centerY-radius));
    int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));

    // COMMON ELEMENTS
    vec lt = {0}, norm = {0};
    clr final = {0};
    // clr amb = {0}, spec = {0}, diff = {0};
    float reflectZ = 0.0;
    float reflectDot = 0.0;
    float dotProd = 0.0;
    // float centerZ = 0.0;
    int   i = 0;

    for(int iCorr = minI; iCorr < maxI; iCorr++) {
        for(int jCorr = minJ; jCorr < maxJ; jCorr++) {

            // Location of the center of pixel relative to center of sphere
            float x = (iCorr + 0.5 - centerX);
            float y = (jCorr + 0.5 - centerY);

            float dist = sqrt(sqr(x) + sqr(y));

            if (dist <= radius) {
                // This is the front-facing Z coordinate
                float z = sqrt(sqr(radius) - sqr(dist));

                // Common elements
                norm.x = x, norm.y = y, norm.z = z;
                norm.normalize();
                // Color Components
                clr amb = {0}, spec = {0}, diff = {0};

                // AMBIENT CALCULATIONS
                for(i = 0; i < PLCount; i += 1) { // Point Lights
                    amb.r += (ka.r * pLight[i].c.r);
                    amb.g += (ka.g * pLight[i].c.g);
                    amb.b += (ka.b * pLight[i].c.b);
                }
                for(i = 0; i < DLCount; i += 1) { // Directional Lights
                    amb.r += (ka.r * dLight[i].c.r);
                    amb.g += (ka.g * dLight[i].c.g);
                    amb.b += (ka.b * dLight[i].c.b);
                }


                // SPECULAR CALCULATIONS
                for(i = 0; i < PLCount; i += 1) { // Point Lights
                    lt.x = (pLight[i].v.x * radius) - x;
                    lt.y = (pLight[i].v.y * radius) - y;
                    lt.z = (pLight[i].v.z * radius) - z;
                    lt.normalize();

                    dotProd = dotProduct(lt, norm);

                    // float reflectX = (2 * dotProd * norm.x) + (-1 * lt.x);
                    // float reflectY = (2 * dotProd * norm.y) + (-1 * lt.y);
                    reflectZ = (2 * dotProd * norm.z) + (-1 * lt.z);
                    reflectDot = pow(max(0.0, reflectZ), SP);

                    spec.r += pLight[i].c.r * ks.r * reflectDot;
                    spec.g += pLight[i].c.g * ks.g * reflectDot;
                    spec.b += pLight[i].c.b * ks.b * reflectDot;
                }
                for(i = 0; i < DLCount; i += 1) { // Directional Lights
                    lt.x = 0 - dLight[i].v.x;
                    lt.y = 0 - dLight[i].v.y;
                    lt.z = 0 - dLight[i].v.z;
                    lt.normalize();

                    dotProd = dotProduct(lt, norm);

                    // reflectX = (2 * dotProd * norm.x) + (-1 * lt.x);
                    // reflectY = (2 * dotProd * norm.y) + (-1 * lt.y);
                    reflectZ = (2 * dotProd * norm.z) + (-1 * lt.z);
                    reflectDot = pow(max(reflectZ, 0.0), SP);

                    spec.r += dLight[i].c.r * ks.r * reflectDot;
                    spec.g += dLight[i].c.g * ks.g * reflectDot;
                    spec.b += dLight[i].c.b * ks.b * reflectDot;
                }


                // DIFFUSE CALCULATIONS
                for(i = 0; i < PLCount; i += 1) { // Point Lights
                    lt.x = (pLight[i].v.x * radius) - x;
                    lt.y = (pLight[i].v.y * radius) - y;
                    lt.z = (pLight[i].v.z * radius) - z;
                    lt.normalize();

                    dotProd = max(0.0, dotProduct(lt, norm));

                    diff.r += pLight[i].c.r * kd.r * dotProd;
                    diff.g += pLight[i].c.g * kd.g * dotProd;
                    diff.b += pLight[i].c.b * kd.b * dotProd;
                }
                for(i = 0; i < DLCount; i += 1) { // Directional Lights
                    lt.x = 0 - dLight[i].v.x;
                    lt.y = 0 - dLight[i].v.y;
                    lt.z = 0 - dLight[i].v.z;
                    lt.normalize();

                    dotProd = max(0.0, dotProduct(lt, norm));

                    diff.r += dLight[i].c.r * kd.r * dotProd;
                    diff.g += dLight[i].c.g * kd.g * dotProd;
                    diff.b += dLight[i].c.b * kd.b * dotProd;
                }

                // Sum all Parts for pixel values
                final = sum3(amb, spec, diff);

                if (WRITE_FILE) {
                    writePixel(iCorr, jCorr, final);
                } else {
                    setPixel(iCorr, jCorr, final.r, final.g, final.b);
                    //setPixel(iCorr,jCorr, x/radius, y/radius, z/radius);
                }
            } // end if
        } // end jCorr
    } // end iCorr

    if (not WRITE_FILE) {
        glEnd();
    }
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

    if (not WRITE_FILE) {
        glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

        glMatrixMode(GL_MODELVIEW);
        // indicate we are specifying camera transformations
        glLoadIdentity();
        // make sure transformation is "zero'd"
    }


    // Start drawing
    // This probably could be made a bit more fuctional, but each section is
    // a custom set of circles.
    int m = min(viewport.h, viewport.w); // Treat everything as a square.
    if (CIRCLES == 1) {
        circle(m/2, m/2, m/2.25); // modified from given code to be larger
    } else if (CIRCLES == 2) {
        circle(.25 * m, .25 * m, m/4);
        circle(.75 * m, .75 * m, m/4);
    } else if (CIRCLES == 3) { // MICKEY MOUSE! (donald duck... mickey mouse...)
        circle(.2 * m, .75 * m, m/6);
        circle(.8 * m, .75 * m, m/6);
        circle(.5 * m, .45 * m, m/3); // bottom center
    } else if (CIRCLES == 4) { // Four Corners
        circle(.25 * m, .25 * m, m/5);
        circle(.25 * m, .75 * m, m/5);
        circle(.75 * m, .25 * m, m/5);
        circle(.75 * m, .75 * m, m/5);
    } else if (CIRCLES == 5) { // Dice
        circle(.25 * m, .25 * m, m/6);
        circle(.25 * m, .75 * m, m/6);
        circle(.75 * m, .25 * m, m/6);
        circle(.75 * m, .75 * m, m/6);
        circle( .5 * m,  .5 * m, m/6);
    } else if (CIRCLES == 6) { // Pyramid
        circle(.50 * m, .83 * m, m/6.5); // top row
        circle(.33 * m, .52 * m, m/6.5); // middle row
        circle(.67 * m, .52 * m, m/6.5);
        circle(.17 * m, .18 * m, m/6.5); // bottom row
        circle(.50 * m, .18 * m, m/6.5);
        circle(.83 * m, .18 * m, m/6.5);
    } else if (CIRCLES == 7) { // H image
        circle(.16 * m, .83 * m, m/7); // top row
        circle(.83 * m, .83 * m, m/7);
        circle(.16 * m, .50 * m, m/7); // middle row
        circle(.50 * m, .50 * m, m/7);
        circle(.83 * m, .50 * m, m/7);
        circle(.16 * m, .16 * m, m/7); // bottom
        circle(.83 * m, .16 * m, m/7);
    } else if (CIRCLES == 8) { // Playing card style
        circle(.16 * m, .83 * m, m/7); // top row
        circle(.83 * m, .83 * m, m/7);
        circle(.16 * m, .50 * m, m/7); // middle row
        circle(.83 * m, .50 * m, m/7);
        circle(.16 * m, .16 * m, m/7); // bottom
        circle(.83 * m, .16 * m, m/7);
        circle(.50 * m, .67 * m, m/7); // middle column
        circle(.50 * m, .33 * m, m/7);
    } else if (CIRCLES == 9) {
        circle(.16 * m, .83 * m, m/7); // top row
        circle(.50 * m, .83 * m, m/7);
        circle(.83 * m, .83 * m, m/7);
        circle(.16 * m, .50 * m, m/7); // middle row
        circle(.50 * m, .50 * m, m/7);
        circle(.83 * m, .50 * m, m/7);
        circle(.16 * m, .16 * m, m/7); // bottom row
        circle(.50 * m, .16 * m, m/7);
        circle(.83 * m, .16 * m, m/7);
    }

    if (not WRITE_FILE) {
        glFlush();
        glutSwapBuffers(); // swap buffers (we earlier set float buffer)
    }
}

// Ref: http://www.opengl.org/resources/libraries/glut/spec3/node49.html
// X and Y are mouse coordinates and can be ingnored for AS1.
void keypress(unsigned char key, int x, int y) {

    if (key == 32) { // spacebar.
        exit(0);
    } else if (key == 'l' or key == 'u') {
        CIRCLES += 1;
        CIRCLES = (CIRCLES > 9) ? 9 : CIRCLES;
    } else if (key == 'j' or key == 'd') {
        CIRCLES -= 1;
        CIRCLES = (CIRCLES < 1) ? 1 : CIRCLES;
    }
    // Update display after circles change
    myDisplay();
}


void initializeVector(int size) {
    int len = sqr(size);
    for(int i = 0; i < len; i += 4) { // minor loop unrolling.
        pixels.push_back(0); // rgba, black for outside circle
        pixels.push_back(0);
        pixels.push_back(0);
        pixels.push_back(255);
        pixels.push_back(0); // rgba, black for outside circle
        pixels.push_back(0);
        pixels.push_back(0);
        pixels.push_back(255);
        pixels.push_back(0); // rgba, black for outside circle
        pixels.push_back(0);
        pixels.push_back(0);
        pixels.push_back(255);
        pixels.push_back(0); // rgba, black for outside circle
        pixels.push_back(0);
        pixels.push_back(0);
        pixels.push_back(255);
    }
}

void writePixel(int x, int y, clr px) {
    // the vector is a linear ordering of all pixels.
    // (final.b * 255) > 255 ? 255 : (final.b * 255)

    int pos = 4 * ((x * IMG_SIZE) + y);
    px.mult(255);
    pixels.at(pos + 0) = px.r > 255 ? 255 : px.r;
    pixels.at(pos + 1) = px.g > 255 ? 255 : px.g;
    pixels.at(pos + 2) = px.b > 255 ? 255 : px.b;
    pixels.at(pos + 3) = 255;
}
//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //This initializes glut
    glutInit(&argc, argv);

    // This tells glut to use a float-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Initalize theviewport size
    viewport.w = 400;
    viewport.h = 400;

    //The size and position of the window
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    initScene(argc, argv);  // Parse command line args here.

    if (WRITE_FILE) {
        viewport.w = IMG_SIZE;
        viewport.h = IMG_SIZE;
        cout << "\nInitializing Image...\n";
        initializeVector(IMG_SIZE);
        cout << "\nGenerating Image...\n";
        myDisplay();
        cout << "\nWriting File...\n";
        encodeOneStep("as1.png", pixels, IMG_SIZE, IMG_SIZE);
        exit(0);
    }

    glutKeyboardFunc(keypress); // Detect key presses
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutMainLoop();

  return 0;
}

