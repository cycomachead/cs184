// CS184 ASSIGNMENT 3
// TOMO UEDA & MICHAEL BALL

#include "as3.h"
#include "models.h"
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
string outputFile;

bool objInput = false;
bool sceneFile = false;
bool writeFile = false;

float subDivParam;
float errorParam;
bool useAdaptiveMode = false;

// Global Variables
int LOGLEVEL;

int modelNum = 0;

vector< vector <vector<glm::vec3> > > patches;


vector<glm::vec3> vertices;
vector<glm::vec3> normals;

vector< vector<glm::vec3> > shapes;
vector< vector<glm::vec3> > shapeNormals;
bool noNormal;


// Things specific to each model.
vector<Model*> models = vector<Model*>();

// Other Initial Settings
vector<bool> useSmoothShading  = vector<bool>(); // controlled by 's'
vector<bool> useWireframeMode  = vector<bool>(); // controlled by 'w'
vector<bool> useHiddenLineMode = vector<bool>(); // controlled by 'h'
vector<bool> normalDisplay = vector<bool>();
vector<bool> multiColor = vector<bool>();
vector<glm::vec3> translation = vector<glm::vec3>();
vector<glm::vec3> rotation = vector<glm::vec3>();


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
        if (curr == "-a") {
            useAdaptiveMode = true;
        } else if (curr == "-l" or curr == "-log") { // debugging switch
            pos += 1;
            LOGLEVEL = atoi(argv[pos]);
        } else if (curr == "-o") {
            // OPTIONAL .obj output files
            writeFile = true;
            pos += 1;
            outputFile = argv[pos];
        } else {
            inputFile = (string) argv[pos];
            // Check file type
            size_t found = inputFile.find(".obj");
            size_t scene = inputFile.find(".scene");
            sceneFile = (scene != string::npos);
            // If found, use a different parser.
            objInput = (found != string::npos);
            if (!objInput) {
                pos += 1;
                errorParam = subDivParam = atof(argv[pos]);
            }
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
    gluOrtho2D(0, viewport.w, 0, viewport.h);
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

    for (int i = 0; i < models.size(); i += 1) {
        translation.push_back(glm::vec3(0.0f, 0.0f, -13.5f));
        rotation.push_back(glm::vec3(0.0f, 0.0f, -13.5f));
        useSmoothShading.push_back(false);
        useWireframeMode.push_back(false);
        useHiddenLineMode.push_back(false);
        normalDisplay.push_back(false);
        multiColor.push_back(false);
    }

    if (LOGLEVEL > 1) {
        cout << "SETUP COMPLETE";
    }
}

void displayNormal(vector<glm::vec3>* shape, vector<glm::vec3>* normal) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    COLOR_GREEN
    glVertex3f(normal->at(0)[0] + shape->at(0)[0], normal->at(0)[1] + shape->at(0)[1], normal->at(0)[2] + shape->at(0)[2]);
    glVertex3f(shape->at(0)[0], shape->at(0)[1], shape->at(0)[2]);
    COLOR_BLUE
    glVertex3f(normal->at(1)[0] + shape->at(1)[0], normal->at(1)[1] + shape->at(1)[1], normal->at(1)[2] + shape->at(1)[2]);
    glVertex3f(shape->at(1)[0], shape->at(1)[1], shape->at(1)[2]);
    COLOR_RED
    glVertex3f(normal->at(2)[0] + shape->at(2)[0], normal->at(2)[1] + shape->at(2)[1], normal->at(2)[2] + shape->at(2)[2]);
    glVertex3f(shape->at(2)[0], shape->at(2)[1], shape->at(2)[2]);
    if  (shape->size() == 4) { // removed !useAdaptive has problems with obj.
        COLOR_YELLOW
        glVertex3f(normal->at(3)[0] + shape->at(3)[0], normal->at(3)[1] + shape->at(3)[1], normal->at(3)[2] + shape->at(3)[2]);
        glVertex3f(shape->at(3)[0], shape->at(3)[1], shape->at(3)[2]);
    }

    glEnd();
    glEnable(GL_LIGHTING);
}

void setMultiColor() {
    float r = ((double) rand() / (RAND_MAX));
    if (r <= .2) {
        COLOR_GREEN
    } else if (r > .2 && r <= .4) {
        COLOR_BLUE
    } else if (r > .4 && r <= .6) {
        COLOR_RED
    } else if (r > .6 && r <= .8) {
        COLOR_CYAN
    } else if (r > .8) {
        COLOR_YELLOW
    }
}

void drawNormals(Model* m) {
    vector <vector<glm::vec3>* >* shapes = m->getShapes();
    vector <vector<glm::vec3>* >* normals = m->getNormals();
    for (int i = 0; i < shapes->size(); i++) {
        vector<glm::vec3>* shape = shapes->at(i);
        vector<glm::vec3>* normal = normals->at(i);
        displayNormal(shape, normal);
    }
}

void drawObject(Model* m) {
    // Start drawing
    // OPENGL Options:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd318361.aspx
    // iterate over m polygons/faces

    vector <vector<glm::vec3>* >* shapes = m->getShapes();
    vector <vector<glm::vec3>* >* normals = m->getNormals();
    if (m->hasNormals()) {
        for (int i = 0; i < shapes->size(); i++) {
            vector<glm::vec3>* shape = shapes->at(i);
            vector<glm::vec3>* normal = normals->at(i);
            if (multiColor.at(modelNum)) {
                glDisable(GL_LIGHTING);
                setMultiColor();
            }
            glBegin(GL_POLYGON);
            glNormal3f(normal->at(0)[0], normal->at(0)[1], normal->at(0)[2]);
            glVertex3f(shape->at(0)[0], shape->at(0)[1], shape->at(0)[2]);
            glNormal3f(normal->at(1)[0], normal->at(1)[1], normal->at(1)[2]);
            glVertex3f(shape->at(1)[0], shape->at(1)[1], shape->at(1)[2]);
            glNormal3f(normal->at(2)[0], normal->at(2)[1], normal->at(2)[2]);
            glVertex3f(shape->at(2)[0], shape->at(2)[1], shape->at(2)[2]);
            if (normal->size() > 3) {
                glNormal3f(normal->at(3)[0], normal->at(3)[1], normal->at(3)[2]);
                glVertex3f(shape->at(3)[0], shape->at(3)[1], shape->at(3)[2]);
            }
            glEnd();
        }
    } else { // m has no nromals.
        for (int i = 0; i < shapes->size(); i++) {
            vector<glm::vec3>* shape = shapes->at(i);
            if (multiColor.at(modelNum)) {
                glDisable(GL_LIGHTING);
                setMultiColor();
            }
            glBegin(GL_POLYGON);
            glVertex3f(shape->at(0)[0], shape->at(0)[1], shape->at(0)[2]);
            glVertex3f(shape->at(1)[0], shape->at(1)[1], shape->at(1)[2]);
            glVertex3f(shape->at(2)[0], shape->at(2)[1], shape->at(2)[2]);
            if (shape->size() > 3) {
                glVertex3f(shape->at(3)[0], shape->at(3)[1], shape->at(3)[2]);
            }
            glEnd();
        }
    }
}

void myDisplay(int num) {
    if (LOGLEVEL > 6) {
        cout << "MY DISPLAY CALLED" << endl;
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0, viewport.w, 0, viewport.h);
    glLoadIdentity();

    glTranslatef(translation.at(num).x, translation.at(num).y, translation.at(num).z);
    glRotatef(rotation.at(num).x, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.at(num).y, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.at(num).z, 0.0f, 0.0f, 1.0f);

    if (useHiddenLineMode.at(num)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Color hidden lines as red
        COLOR_RED
        // add glDepthMask(GL_FALSE); before you render the hidden lines stage, but after the solid stage.
        // (don't forget to reverse this after the visible line stage or your rendering will be messed up)
    }

    drawObject(models.at(num));

    if (useHiddenLineMode.at(num)) {
        glDepthFunc(GL_LEQUAL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
        glEnable(GL_LIGHTING);

        drawObject(models.at(num));

        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_LIGHTING);
    }
    if (normalDisplay.at(num)) {
        drawNormals(models.at(num));
    }

    glFlush();
    glutSwapBuffers(); // swap buffers (we earlier set float buffer)
}


// OPTIONAL: Extra Credit...
void toggleHiddenLines() {
    useHiddenLineMode.at(modelNum) = !useHiddenLineMode.at(modelNum);
    if (!useHiddenLineMode.at(modelNum)) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
}

void displayAll() {

    for (int i = 0; i < models.size(); i += 1) {
        myDisplay(i);
    }
}

void toggleWireframe() {
    useWireframeMode.at(modelNum) = !useWireframeMode.at(modelNum);
    if (useWireframeMode.at(modelNum)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // Hiddenline and wireframes arent compatible.
    if (useHiddenLineMode.at(modelNum)) {
        toggleHiddenLines();
    }
}

void toggleShading() {
    useSmoothShading.at(modelNum) = !useSmoothShading.at(modelNum);
    // Shading model:
    // https://www.opengl.org/sdk/docs/man2/xhtml/glShadeModel.xml
    if (useSmoothShading.at(modelNum)) {
        glShadeModel(GL_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
    }
}

void changeZoom(float amt) {
    translation.at(modelNum).z += amt;
}

// 0: Left, 1: Up, 2: Right, 3: Down
void rotate(int dir) {
    if (!dir) {
        rotation.at(modelNum).x -= 5;
    } else if (dir == 1) {
        rotation.at(modelNum).y += 5;
    } else if (dir == 2) {
        rotation.at(modelNum).x += 5;
    } else if (dir == 3) {
        rotation.at(modelNum).y -= 5;
    } else if (dir == 4) {
        rotation.at(modelNum).z += 5;
    } else if (dir == 5) {
        rotation.at(modelNum).z -= 5;
    }
}

// 0: Left, 1: Up, 2: Right, 3: Down
void translate(int dir) {
    if (!dir) {
        translation.at(modelNum).x -= 0.1f;
    } else if (dir == 1) {
        translation.at(modelNum).y += 0.1f;
    } else if (dir == 2) {
        translation.at(modelNum).x += 0.1f;
    } else if (dir == 3) {
        translation.at(modelNum).y -= 0.1f;
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
        if (writeFile) {
            for (int i = 0; i < models.size(); i += 1) {
                writeObj(outputFile, models.at(i), inputFile);
            }
        }
        exit(0);
    } else if (key == 'w' or key == 'W') { // Filled or Wireframe
        toggleWireframe();
    } else if (key == 's' or key == 'S') { // Smooth or flat shading
        toggleShading();
    } else if (key == '-' or key == '_') { // zoom out
        changeZoom(-0.1f);
    } else if (key == '=' or key == '+') { // zoom in
        changeZoom(0.1f);
    } else if (key == 'z' or key == 'Z') { // rotate Z
        rotate(4);
    } else if (key == 'x' or key == 'X') { // rotate Z
        rotate(5);
    } else if (key == 'h' or key == 'H') { // OPTIONAL: toggleHiddenLines
        toggleHiddenLines();
    } else if (key == 'n' or key == 'N') {
        if (noNormal) {
            return;
        }
        normalDisplay.at(modelNum) = !normalDisplay.at(modelNum);
        if (!normalDisplay.at(modelNum)) {
            glEnable(GL_LIGHTING);
        }
    } else if (key == 'b' or key == 'B') {
        multiColor.at(modelNum) = !multiColor.at(modelNum);
        if (!multiColor.at(modelNum)) {
            glEnable(GL_LIGHTING);
        }
    } else if (key == 'j' or key == 'J') {
        modelNum -= 1;
        if (modelNum < 0) {
            modelNum = models.size() - 1;
        }
    } else if (key == 'k' or key == 'K') {
        modelNum += 1;
        if (modelNum == models.size()) {
            modelNum = 0;
        }
    }

    // Update display after circles change
    myDisplay(modelNum);
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
    myDisplay(modelNum);
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

    //This initializes glut
    glutInit(&argc, argv);

    // This tells glut to use a float-buffered window with red, green, and blue channels
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initalize theviewport size
    viewport.w = 800;
    viewport.h = 800;

    //The size and position of the window
    glutInitWindowSize(viewport.w, viewport.h);
    glutInitWindowPosition(0,0);
    glutCreateWindow(argv[0]);
    initScene(argc, argv);  // Parse command line args here.

    if (sceneFile) {
        vector<string> files = loadScene(inputFile);
        for (int i = 0; i < files.size(); i += 1) {
            string file = files.at(i);
            size_t obj = file.find(".obj");
            if (obj != string::npos) {
                loadobj(file);
                models.push_back(new SimpleModel(shapes, shapeNormals));
            } else {
                loadPatches(file);
                if (useAdaptiveMode) {
                    models.push_back(new AdaptiveModel(patches, errorParam));
                } else {
                    models.push_back(new UniformModel(patches, errorParam));
                }
            }
            patches.clear();
            shapes.clear();
            shapeNormals.clear();
            vertices.clear();
            normals.clear();
        }
    } else {
        // detect file type...
        if (objInput) {
            loadobj(inputFile);
        } else {
            loadPatches(inputFile);
        }
        // Create the Main Model
        if (useAdaptiveMode) {
            models.push_back(new AdaptiveModel(patches, errorParam));
        } else if (objInput) {
            models.push_back(new SimpleModel(shapes, shapeNormals));
        } else {
            models.push_back(new UniformModel(patches, errorParam));
        }
    }

    glutKeyboardFunc(keypress); // Detect key presses
    glutSpecialFunc(specialkeypress); // Detect SPECIAL (arrow) keys

    glutDisplayFunc(displayAll);

    glutReshapeFunc(myReshape);
    setupGlut();
    glutMainLoop();

    return 0;
}

