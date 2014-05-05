// CS184 ASSIGNMENT 4
// TOMO UEDA & MICHAEL BALL

#include "as4.h"
#include "arm.h"
#include "transformation.h"


#include <unistd.h>

#define COLOR_RED glColor3f(1.0f, 0.0f, 0.0f);
#define COLOR_GREEN glColor3f(0.0f, 1.0f, 0.0f);
#define COLOR_BLUE glColor3f(0.0f, 0.0f, 1.0f);
#define COLOR_YELLOW glColor3f(1.0f, 1.0f, 0.0f);
#define COLOR_CYAN glColor3f(0.0f, 1.0f, 1.0f);
#define COLOR_MAGENTA glColor3f(1.0f, 0.0f, 1.0f);
#define COLOR_BLACK glColor3f(0.0f, 0.0f, 0.0f);
#define COLOR_WHITE glColor3f(1.0f, 1.0f, 1.0f);
#define PI           3.14159265358979323846


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

Arm arm;
Transformation t1, t2, t3, t4;


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

    arm.drawSystem();

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

void createArmsAndRotation() {
    t1 = *new Transformation();
    t1.add_rotation(1, 0, 0, PI/4);
    Arm* furthest = new Arm(1, t1);
    t2 = *new Transformation();
    t2.add_rotation(0, 1, 0, PI/4);
    Arm* further = new Arm(furthest, 2, t2);
    t3 = *new Transformation();
    t3.add_rotation(1, 0, 0, PI/4);
    Arm* far = new Arm(further, 3, t3);
    t4 = *new Transformation();
    t4.add_rotation(0, 1, 0, PI/4);
    arm = *new Arm(far, 4, t4);
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

    createArmsAndRotation();
    setupGlut();
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

