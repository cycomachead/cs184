// CS184 ASSIGNMENT 4
// TOMO UEDA & MICHAEL BALL

#include "as4.h"
#include "arm.h"

#include <unistd.h>

#define COLOR_RED glColor3f(1.0f, 0.0f, 0.0f);
#define COLOR_GREEN glColor3f(0.0f, 1.0f, 0.0f);
#define COLOR_BLUE glColor3f(0.0f, 0.0f, 1.0f);
#define COLOR_YELLOW glColor3f(1.0f, 1.0f, 0.0f);
#define COLOR_CYAN glColor3f(0.0f, 1.0f, 1.0f);
#define COLOR_MAGENTA glColor3f(1.0f, 0.0f, 1.0f);
#define COLOR_BLACK glColor3f(0.0f, 0.0f, 0.0f);
#define COLOR_WHITE glColor3f(1.0f, 1.0f, 1.0f);


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
Viewport viewport;


// Global Variables
int LOGLEVEL;

Arm _arm;


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

    int pos = 1;
    while (pos < argc) {
        string curr = argv[pos];
        if (curr == "-l" or curr == "-log") { // debugging switch
        
        }
        pos += 1;
    }

    if (LOGLEVEL > 0) {
        // cout << "Input Params: " << endl;
        // cout << "File: " << inputFile << endl;
        // cout << "Subdivision Parameter: " << subDivParam << endl;
        // cout << "Error Parameter: " << errorParam << endl;
        // cout << "Using Adaptive Tessellation: " << useAdaptiveMode << endl;
        // cout << "LOGLEVEL: " << LOGLEVEL << endl;
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
    gluPerspective(45.f, (GLfloat) viewport.w / viewport.h, .1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0, viewport.w, 0, viewport.h);
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************

void myDisplay() {

    glFlush();
    glutSwapBuffers(); // swap buffers (we earlier set float buffer)
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

    // Update display after circles change
    myDisplay();
}

void createArms() {
    // Arm* furthest = new Arm(1, )
    // _arm = new Arm()
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

    //This initializes glut
    glutInit(&argc, argv);

    // This tells glut to use a float-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    void createArms();
    // Initalize theviewport size
    viewport.w = 800;
    viewport.h = 800;

    //The size and position of the window
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    initScene(argc, argv);  // Parse command line args here.

    glutKeyboardFunc(keypress); // Detect key presses
    glutSpecialFunc(specialkeypress); // Detect SPECIAL (arrow) keys

    glutDisplayFunc(myDisplay);

    glutReshapeFunc(myReshape);
    glutMainLoop();

    return 0;
}

