// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
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

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  // glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
}

//***************************************************
// functions that I wrote
//***************************************************
void drawRect(float p1[], float p2[], float p3[], float p4[]) {
    glBegin(GL_POLYGON);            // draw rectangle
    glVertex3f(p1[0], p1[1], 0.0f); // bottom left corner of rectangle
    glVertex3f(p2[0], p2[1], 0.0f); // top left corner of rectangle
    glVertex3f(p3[0], p3[1], 0.0f); // top right corner of rectangle
    glVertex3f(p4[0], p4[1], 0.0f); // bottom right corner of rectangle
    glEnd();
}

void drawSquareDirection(float x, float y, float size, float angle) {
    // Draw a rotated square.
    // Angle is in degrees
    float radiusC = (size) * cos(angle*(180/PI));
    float radiusS = (size) * sin(angle*(180/PI));

    float p1[2]   = {x - radiusC, y - radiusS};
    float p2[2]   = {x - radiusC, y + radiusS};
    float p3[2]   = {x + radiusC, y + radiusS};
    float p4[2]   = {x + radiusC, y - radiusS};

    drawRect(p1, p2, p3, p4);
}

void drawSquare(float x, float y, float size) {
    drawSquareDirection(x, y, size, 45.0f);
}

void drawSquareCorner(float x, float y, float size) {
    // FROM THE LOWER LEFT CORNER
    float p1[2] = {x, y};
    float p2[2] = {x, y + size};
    float p3[2] = {x + size, y + size};
    float p4[2] = {x + size, y};
    drawRect(p1, p2, p3, p4);
}

// Normalize 0-255 RGB values to opengl floats
void rgb(float r, float g, float b) { glColor3f(r/255, g/255, b/255); }

void grey() { glColor3f(0.5f, 0.5f, 0.5f); }

void black() { glColor3f(0.0f, 0.0f, 0.0f); }

void white() { glColor3f(1.0f, 1.0f, 1.0f); }

void red() { glColor3f(1.0f, 0.0f, 0.0f); }

void gold() { rgb(255.0f, 215.0f, 0.0f); }

void mostlyOriginalCode(float tip) {
    // Rectangle Code
    glColor3f(1.0f,0.0f,0.0f);    // setting the color to pure red
    glBegin(GL_POLYGON);  // draw rectangle
    glVertex3f(-0.8f, 0.0f, 0.0f);    // bottom left corner of rectangle
    glVertex3f(-0.8f, 0.5f, 0.0f);    // top left corner of rectangle
    glVertex3f( 0.0f, 0.5f, 0.0f);    // top right corner of rectangle
    glVertex3f( 0.0f, 0.0f, 0.0f);    // bottom right corner of rectangle
    glEnd();
    // Triangle Code
    glColor3f(1.0f,0.5f,0.0f); // setting the color to orange for the triangle

    float basey = -sqrt(0.48f); // height of triangle = sqrt(.8^2-.4^2)
    glBegin(GL_POLYGON);
    glVertex3f(tip,  0.0f, 0.0f); // top tip of triangle
    glVertex3f(0.1f, basey, 0.0f);    // lower left corner of triangle
    glVertex3f(0.9f, basey, 0.0f);    // lower right corner of triangle
    glEnd();

    glColor3f(0, 1.0f, 0.0f);
    //glutWireCone(2, 15, 50,  2);
    glutWireDodecahedron();

    glColor3f(1.0f, 1.0f, 0.0f);
    drawSquare(0.0f, 0.0f, .5f);
}


void checkerBoard() {
    // Draw an 8x8 checkerboard from top left to bottom right going a
    float startX = -1.0f + 0.125; // Who's Idea was it to draw from the center
    float startY = 1.0f - 0.125; // of the square?! DUMB... oh wait, that's me.
    float size = 0.25f; // 1/8 window size (Which is 2, -1..1 )

    int box = 0;
    int color = 0;
    black();
    for(float j = startY; j > -1; j -= size) {
        for(float i = startX; i < 1; i += size) {
            drawSquare(i, j, size-.1f);
            box += 1;
            if (box % 8 != 0) {
                if (not color) {
                    color=1;
                    red();
                } else {
                    color=0;
                    black();
                }
            }
        }
    }
}

void tree(int lvl, int maxLvl, float length) {
    float startX = 0.0f;
    float startY = -1.0f;
    float width = 0.1f;

    if (lvl < maxLvl) {

    }
}

void drawLine(float x, float y, float width, float len, float dir) {
    for(float i = 0; i < len; i += width) {
        //drawSquare()
    }
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

  //----------------------- ----------------------- -----------------------
  // This is a quick hack to add a little bit of animation.
  static float tip = 0.5f;
  const  float stp = 0.001f;
  const  float beg = 0.1f;
  const  float end = 0.9f;

  tip += stp;
  if (tip>end) tip = beg;
  //----------------------- ----------------------- -----------------------

  glClear(GL_COLOR_BUFFER_BIT);   // clear the color buffer

  glMatrixMode(GL_MODELVIEW); // indicate we are specifying camera transforms
  glLoadIdentity();           // make sure transformation is "zero'd"
  //----------------------- code to draw objects --------------------------

  gold();
  drawSquareCorner(-1.0f, 1.0f, 2.0f);
  checkerBoard();

  //-----------------------------------------------------------------------
  glFlush();
  glutSwapBuffers();  // swap buffers (we earlier set double buffer)
}

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  // This tells glut to use a double-buffered window with red, green, and blue channels
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Lets Play!");

  initScene();                                 // quick function to set up scene

  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








