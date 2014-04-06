// CS184 ASSIGNMENT 3
// TOMO UEDA & MICHAEL BALL

#include "as3.h"

#define COLOR_RED glColor3f(1.0f, 0.0f, 0.0f);
#define COLOR_GREEN glColor3f(0.0f, 1.0f, 0.0f);
#define COLOR_BLUE glColor3f(0.0f, 0.0f, 1.0f);
#define COLOR_YELLOW glColor3f(1.0f, 1.0f, 0.0f);
#define COLOR_CYAN glColor3f(0.0f, 1.0f, 1.0f);
#define COLOR_MAGENTA glColor3f(1.0f, 0.0f, 1.0f);
#define COLOR_BLACK glColor3f(0.0f, 0.0f, 0.0f);
#define COLOR_WHITE glColor3f(1.0f, 1.0f, 1.0f);


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

//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

// Command Line Params
string inputFile;
float subDivParam;
float errorParam;
bool useAdaptiveMode = false;

// Other Initial Settings
bool useSmoothShading  = false; // controlled by 's'
bool useWireframeMode  = false; // controlled by 'w'
bool useHiddenLineMode = false; // controlled by 'h' OPTIONAL
float zoomLevel = 1.0f;

vector< vector <vector<glm::vec4> > > patches;

//****************************************************
// Basic Functions
//****************************************************
void usage() {
    cout << "\t CS184 ASSIGNMENT 3 -- Usage" << endl;
    cout << "./as3 input- file subdivision-param [ -a ] (adaptive shading)" << endl;
}


//****************************************************
// Simple init function
//****************************************************
// Handle parsing any command line args.
void initScene(int argc, char *argv[]) {
    LOGLEVEL = 0;
    if (argc < 2) {
        usage();
        cerr << "Invalid arguments found; exiting.\n" << endl;
        exit(1);
    }

    inputFile = (string) argv[1];

    int pos = 2;
    while (pos < argc) {
        string curr = argv[pos];
        if (curr == "-a") {
            useAdaptiveMode = true;
        } else if (curr == "-l" or curr == "-log") { // debugging switch
            pos += 1;
            LOGLEVEL = atoi(argv[pos]);
        } else if (curr == "-o") {
            // OPTIONAL .obj output files
            pos += 1;
        } else {
            errorParam = subDivParam = atof(argv[pos]);
        }
        pos += 1;
    }

    if (LOGLEVEL > 0) {
        cout << "Input Params: " << endl;
        cout << "File: " << inputFile << endl;
        cout << "Subdivision Parameter: " << subDivParam << endl;
        cout << "Error Parameter: " << errorParam << endl;
        cout << "Using Adaptive Tessellation: " << useAdaptiveMode << endl;
        cout << "LOGLEVEL: " << LOGLEVEL << endl;
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
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

    glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

    glMatrixMode(GL_MODELVIEW);
    // indicate we are specifying camera transformations
    glLoadIdentity();
    // make sure transformation is "zero'd"

    // Shading model:https://www.opengl.org/sdk/docs/man2/xhtml/glShadeModel.xml
    if (useSmoothShading) {
        glShadeModel(GL_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
    }

    // Start drawing
    // OPENGL Options:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd318361.aspx
    if (useWireframeMode) {
        glBegin(GL_LINES);
        COLOR_BLUE;
        glEnd();
    } else if (useAdaptiveMode) {
        glBegin(GL_TRIANGLES);
        COLOR_BLUE;
        glEnd();
    } else { // Subdivision shading.
        glBegin(GL_QUADS);
        COLOR_BLUE;
        glEnd();
    }


    glFlush();
    glutSwapBuffers(); // swap buffers (we earlier set float buffer)
}


void toggleWireframe() {
    useSmoothShading = !useSmoothShading;
}

void toggleShading() {
    useWireframeMode = !useWireframeMode;
}

// OPTIONAL: Extra Credit...
void toggleHiddenLines() {
    useHiddenLineMode = !useHiddenLineMode;
}

void changeZoom(float amt) {
    zoomLevel += amt;
}

// 0: Left, 1: Up, 2: Right, 3: Down
void rotate(int dir) {

}

// 0: Left, 1: Up, 2: Right, 3: Down
void translate(int dir) {

}

//****************************************************
// handle keypresses
//***************************************************
// Ref: http://www.opengl.org/resources/libraries/glut/spec3/node49.html
// X and Y are mouse coordinates and can be ingnored for AS3.
void keypress(unsigned char key, int x, int y) {

    if (LOGLEVEL > 2) {
        cout << "Key Press: " << key << endl;
    }
    if (key == 32) { // spacebar.
        exit(0);
    } else if (key == 'w' or key == 'W') { // Filled or Wireframe
        toggleWireframe();
    } else if (key == 's' or key == 'S') { // Smooth or flat shading
        toggleShading();
    } else if (key == '-' or key == '_') { // zoom out
        changeZoom(-0.1f);
    } else if (key == '=' or key == '+') { // zoom in
        changeZoom(0.1f);
    } else if (key == 'h' or key == 'H') { // OPTIONAL: toggleHiddenLines
        toggleHiddenLines();
    }

    // Update display after circles change
    myDisplay();
}

// Reference: http://www.swiftless.com/tutorials/opengl/keyboard.html
// Keys: 100-103: Left, Up, Right, Down
void specialkeypress(int key, int x, int y) {
    if (LOGLEVEL > 2) {
        cout << "Key Press: " << key << endl;
    }

    if (key >= 100 and key <= 103) {
        int shift = glutGetModifiers();
        if (shift == 1) {
            translate(key - 100);
        } else {
            rotate(key - 100);
        }
    }

    // Update display after circles change
    myDisplay();
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
    viewport.w = 800;
    viewport.h = 800;

    //The size and position of the window
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    initScene(argc, argv);  // Parse command line args here.

    // TODO: detect file type... OPTIONAL
    loadPatches(inputFile);

    glutKeyboardFunc(keypress); // Detect key presses
    glutSpecialFunc(specialkeypress); // Detect SPECIAL (arrow) keys
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutMainLoop();

    return 0;
}

