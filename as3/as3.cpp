// CS184 ASSIGNMENT 3
// TOMO UEDA & MICHAEL BALL

#include "as3.h"
#include "model.h"
#include "UniformModel.h"
#include "parser.h"

#include <unistd.h>

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
Viewport viewport;

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
// glm::vec2 rotation = glm::vec2(0.0f, 0.0f);
glm::vec3 translation = glm::vec3(0.0f, 0.0f, -5.5f);
float rotationX = 0;
float rotationY = 0;

vector< vector <vector<glm::vec4> > > patches;

vector< vector<glm::vec3> > adaptiveTri;

Model *mainModel;

UniformModel* uniModel;

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
            cout << "LOGLEVEL FOUND: " << LOGLEVEL << endl;
        } else if (curr == "-o") {
            // OPTIONAL .obj output files
            pos += 1;
        } else {
            errorParam = subDivParam = atof(argv[pos]);
            cout << "PARAM FOUND: " << errorParam << endl;
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
    gluPerspective(45.f, (GLfloat) viewport.w / viewport.h, .1f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    //gluOrtho2D(0, viewport.w, 0, viewport.h);
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
    // glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, litepos2);
    glEnable(GL_LIGHT1);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLfloat am2[]={.2,.2,.2,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, am2);
    GLfloat dif2[]={1.0,0.8,0.0,1.0};
    glMaterialfv(GL_FRONT ,GL_DIFFUSE, dif2);
    GLfloat sp2[]={0.0,0.0,1.0,1.0};
    glMaterialfv(GL_FRONT,GL_SPECULAR, sp2);
    glMaterialf(GL_FRONT ,GL_SHININESS, 80.0);
    GLfloat emission[] = { .5,0.0,0.0,1.0};
    glMaterialfv(GL_BACK,GL_EMISSION,emission);


    //uleep(20000);
    glutSolidSphere(2.0f, 20, 20);
    if (LOGLEVEL > 1) {
        cout << "SETUP COMPLETE";
    }
}

void myDisplay() {
    if (LOGLEVEL > 6) {
        cout << "MY DISPLAY CALLED" << endl;
    }

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(.0f, 0.f, translation.z);

    // Start drawing
    // OPENGL Options:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd318361.aspx

    if (useAdaptiveMode) {
        // COLOR_BLUE;
        glutSolidSphere(.5f, 10, 10);
        // COLOR_GREEN;
        // glDisable(GL_LIGHTING);
        // iterate over model polygons/faces
        for(int i = 0; i < adaptiveTri.size(); i += 1) {
            vector<glm::vec3> tri = adaptiveTri.at(i);
            if (LOGLEVEL > 5) {
                cout << "DRAWING TRIANGLE   " << i << endl;
            }
            glPointSize(10.0f);
            glBegin(GL_TRIANGLES);
            COLOR_GREEN;
            for(int j = 0; j < tri.size(); j += 1) {
                glm::vec3 point = tri.at(j);
                if (LOGLEVEL > 5) {
                    cout << "\tx: " << point.x << endl;
                    cout << "\ty: " << point.y << endl;
                    cout << "\tz: " << point.z << endl;
                }
                glVertex3f(point.x, point.y, point.z);
                //glNormal3f(point.x, point.y, point.z);
            }
            glEnd();
        }
    } else {

        vector <vector<glm::vec4>* >* shapes = uniModel->getShapes();
        vector <vector<glm::vec4>* >* normals = uniModel->getNormals();
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glTranslatef(.0f, 0.f, translation.z);
        glRotatef(rotationX, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationY, 1.0f, 0.0f, 0.0f);
        // COLOR_BLUE;
        // glutSolidSphere(.5f, 10, 10);
        // COLOR_GREEN;
        // glDisable(GL_LIGHTING);
        for (int i = 0; i < shapes->size(); i++) {
            glPointSize(10.0f);
            glBegin(GL_QUADS);
            COLOR_GREEN;
            // iterate over model polygons/faces
            vector<glm::vec4>* shape = shapes->at(i);
            vector<glm::vec4>* normal = normals->at(i);
            // cout << "shape " << i << "\n";
            // cout << shape->at(0)[0] << ", " << shape->at(0)[1] << ", " << shape->at(0)[2] << "\n";
            // cout << shape->at(1)[0] << ", " << shape->at(1)[1] << ", " << shape->at(1)[2] << "\n";
            // cout << shape->at(2)[0] << ", " << shape->at(2)[1] << ", " << shape->at(2)[2] << "\n";
            // cout << shape->at(3)[0] << ", " << shape->at(3)[1] << ", " << shape->at(3)[2] << "\n";
            glNormal3f(normal->at(0)[0], normal->at(0)[1], normal->at(0)[2]);
            glVertex3f(shape->at(0)[0], shape->at(0)[1], shape->at(0)[2]);
            glNormal3f(normal->at(1)[0], normal->at(1)[1], normal->at(1)[2]);
            glVertex3f(shape->at(1)[0], shape->at(1)[1], shape->at(1)[2]);
            glNormal3f(normal->at(2)[0], normal->at(2)[1], normal->at(2)[2]);
            glVertex3f(shape->at(2)[0], shape->at(2)[1], shape->at(2)[2]);
            glNormal3f(normal->at(3)[0], normal->at(3)[1], normal->at(3)[2]);
            glVertex3f(shape->at(3)[0], shape->at(3)[1], shape->at(3)[2]);
            glEnd();
        }
    }
    glFlush();
    glutSwapBuffers(); // swap buffers (we earlier set float buffer)
}

void toggleWireframe() {
    useWireframeMode = !useWireframeMode;
    if (useWireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void toggleShading() {
    useSmoothShading = !useSmoothShading;
    // Shading model:
    // https://www.opengl.org/sdk/docs/man2/xhtml/glShadeModel.xml
    if (useSmoothShading) {
        glShadeModel(GL_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
    }
}

// OPTIONAL: Extra Credit...
void toggleHiddenLines() {
    useHiddenLineMode = !useHiddenLineMode;
}

void changeZoom(float amt) {
    translation.z += amt;
}

// 0: Left, 1: Up, 2: Right, 3: Down
void rotate(int dir) {
    if (!dir) {
        rotationX -= 5;
    } else if (dir == 1) {
        rotationY += 5;
    } else if (dir == 2) {
        rotationX += 5;
    } else if (dir == 3) {
        rotationY -= 5;
    }
}

// 0: Left, 1: Up, 2: Right, 3: Down
void translate(int dir) {
    if (!dir) {
        rotationX -= 5;
    } else if (dir == 1) {
        rotationY += 5;
    } else if (dir == 2) {
        rotationX += 5;
    } else if (dir == 3) {
        rotationY -= 5;
    }
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

    cout << "LOGLEVEL" << LOGLEVEL << endl;
    // TODO: detect file type... OPTIONAL
    loadPatches(inputFile);
    // Create the Main Model
    uniModel = new UniformModel(patches, errorParam);
    mainModel = new Model(patches, errorParam);
    mainModel->buildAdaptive();
    mainModel->subdivideAll();
    adaptiveTri = mainModel->getAllPolygons();
    glutKeyboardFunc(keypress); // Detect key presses
    glutSpecialFunc(specialkeypress); // Detect SPECIAL (arrow) keys
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    setupGlut();
    glutMainLoop();

    return 0;
}

