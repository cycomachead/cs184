// CS184 ASSIGNMENT 4
// TOMO UEDA & MICHAEL BALL

#include "as4.h"
#include "newarm.h"


#include <unistd.h>

#define PI           3.14159265358979323846


using namespace std;
using namespace Eigen;

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

Arm arm;

Vector3f rotation    = Vector3f(0.0f, 0.0f, 0.0f);
Vector3f translation = Vector3f(0.0f, 0.0f, 0.0f);

//****************************************************
// Basic Functions
//****************************************************
void usage() {
    cout << "\t CS184 ASSIGNMENT 4 -- Usage" << endl;
    cout << "./as4 [-l NUM]" << endl;
}

//****************************************************
// Simple init function
//****************************************************
// Handle parsing any command line args.
void initScene(int argc, char *argv[]) {
    LOGLEVEL = 0;
    if (argc < 0) {
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

void setupGlut() {
    // setup defaults
    // Wireframe OFF
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(.0f, .0f, .0f, .0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    GLfloat ambient[] = { .5f, .5f, .5f, 1.f };
    GLfloat diffuse[] = { .5f, .5f, .5f, .6f };
    GLfloat litepos[] = { 0, 2, 3, 1 };
    GLfloat litepos2[] = { 10, -20, 15, 1 };

    // gllighting
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, litepos);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, litepos2);
    glEnable(GL_LIGHT1);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    GLfloat am2[]={.2,.2,.2,0.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, am2);
    GLfloat dif2[]={1.0,0.8,0.0,0.0};
    glMaterialfv(GL_FRONT ,GL_DIFFUSE, dif2);
    GLfloat sp2[]={0.0,0.0,1.0,0.0};
    glMaterialfv(GL_FRONT,GL_SPECULAR, sp2);
    glMaterialf(GL_FRONT ,GL_SHININESS, 64.0);
    GLfloat emission[] = { .5,0.0,0.0,0.0};
    glMaterialfv(GL_BACK,GL_EMISSION,emission);

}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************

void myDisplay() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0, viewport.w, 0, viewport.h);
    glLoadIdentity();

    glTranslatef(translation.x(), translation.y(), translation.z());
    glRotatef(rotation.x(), 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.y(), 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.z(), 0.0f, 0.0f, 1.0f);

    glDisable(GL_LIGHTING);

    arm.draw();

    glFlush();
    glutSwapBuffers(); // swap buffers (we earlier set float buffer)
}


//****************************************************
// handle keypresses
//***************************************************
void changeZoom(float amt) {
    translation[2] += amt;
}

// 0: Left, 1: Up, 2: Right, 3: Down
void rotate(int dir) {
    if (!dir) {
        rotation[0] -= 5;
    } else if (dir == 1) {
        rotation[1] += 5;
    } else if (dir == 2) {
        rotation[0] += 5;
    } else if (dir == 3) {
        rotation[1] -= 5;
    } else if (dir == 4) {
        rotation[2] += 5;
    } else if (dir == 5) {
        rotation[2] -= 5;
    }
}

// 0: Left, 1: Up, 2: Right, 3: Down
void translate(int dir) {
    if (!dir) {
        translation[0] -= 0.1f;
    } else if (dir == 1) {
        translation[1] += 0.1f;
    } else if (dir == 2) {
        translation[0] += 0.1f;
    } else if (dir == 3) {
        translation[1] -= 0.1f;
    }
}


// Ref: http://www.opengl.org/resources/libraries/glut/spec3/node49.html
// X and Y are mouse coordinates and can be ingnored for AS3.
void keypress(unsigned char key, int x, int y) {

    if (LOGLEVEL > 2) {
        cout << "Key Press: " << key << endl;
    }
    if (key == 32) { // spacebar.
        exit(0);
    } else if (key == '-' or key == '_') { // zoom out
        changeZoom(-0.1f);
    } else if (key == '=' or key == '+') { // zoom in
        changeZoom(0.1f);
    } else if (key == 'z' or key == 'Z') { // rotate Z
        rotate(4);
    } else if (key == 'x' or key == 'X') { // rotate Z
        rotate(5);
    }

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

    myDisplay();
}

void createArmsAndRotation() {
    arm = *new Arm(4, 0, 1, PI/4);
    arm.addChild(3, 0, 1, PI/4);
    arm.addChild(2, 0, 1, PI/4);
    arm.addChild(1, 0, 1, PI/4);
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

    //This initializes glut
    glutInit(&argc, argv);

    // This tells glut to use a float-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    createArmsAndRotation();
    // setupGlut();
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
    setupGlut();
    glutMainLoop();

    return 0;
}

