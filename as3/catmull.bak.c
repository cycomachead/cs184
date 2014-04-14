/* THIS IS AN EXAMPLE FILE AND NOT PART OF OUR ASSIGNMENT. 
 * It comes from here: 
 * http://rosettacode.org/wiki/Catmull–Clark_subdivision_surface/C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* --------- GL stuff ----------*/
int gwin;
GLfloat rot[] = { 20, 40, 0 };
GLfloat rotx = 0, roty = 1;
GLfloat ambient[] = { .5f, .5f, .5f, 1.f };
GLfloat diffuse[] = { .5f, .5f, .5f, .6f };
GLfloat litepos[] = { 0, 2, 3, 1 };
GLfloat litepos2[] = { 0, -2, 5, 1 };
GLfloat color[] = { .3, 1, .6 };
GLfloat hole[] = { .7, .4, 0 };
GLfloat color2[] = { .5, .5, .5 };
GLfloat hole2[] = { .5, .2, 0 };
GLfloat red[] = {1, 0, 0};
GLfloat zpos = -6;

int max_depth = 7;
int show_parent = 1;
int wireframe_mode = 1;
int face_mode = 1;
int model_idx = 0;
int interp_norm = 0;

/* ------ global data stuff ------ */
list models = 0;
int model_pos = 0;

void draw_model(model m)
{
	int i, j;
	face f;
	vertex v;
	foreach(i, f, m->f) {
		glBegin(GL_POLYGON);
		if (!interp_norm) glNormal3fv(&(f->norm.x));
		foreach(j, v, f->v) {
			if (interp_norm)
				glNormal3fv(&(v->avg_norm.x));
			glVertex3fv(&(v->pos.x));
		}
		glEnd();
	}
}

void draw_wireframe(model m, GLfloat *color, GLfloat *hole_color)
{
	int i;
	edge e;

	glDisable(GL_LIGHTING);
	foreach(i, e, m->e) {
		if (e->f->n != 2) glColor3fv(hole_color);
		else		  glColor3fv(color);

		glBegin(GL_LINES);
		glVertex3fv(&(e->v[0]->pos.x));
		glVertex3fv(&(e->v[1]->pos.x));
		glEnd();
	}
}

void draw_faces(model m)
{
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

	if (wireframe_mode) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);
	}
	draw_model(m);
	if (wireframe_mode)
		glDisable(GL_POLYGON_OFFSET_FILL);
}

void keyspecial(int k, int x, int y)
{
	switch(k) {
	case GLUT_KEY_UP:
		rotx --; break;
	case GLUT_KEY_DOWN:
		rotx ++; break;
	case GLUT_KEY_LEFT:
		roty --; break;
	case GLUT_KEY_RIGHT:
		roty ++; break;
	}
}

void set_model()
{
	int i;
	void *p;

	model_pos = 1;
	model_idx = (model_idx + 1) % 3;

	foreach(i, p, models) model_del(p);

	len(models) = 0;

	switch(model_idx) {
	case 0:
		list_push(models, cube()); break;
	case 1:
		list_push(models, donut()); break;
	case 2:
		list_push(models, star()); break;
	}
}

void keypress(unsigned char key, int x, int y)
{
	switch(key) {
	case 27: case 'q':
		glFinish();
		glutDestroyWindow(gwin);
		return;
	case 'w':
		wireframe_mode = (wireframe_mode + 1) % 3;
		return;
	case 'l':
		diffuse[0] += .1;
		diffuse[1] += .1;
		diffuse[2] += .1;
		return;
	case 'L':
		diffuse[0] -= .1;
		diffuse[1] -= .1;
		diffuse[2] -= .1;
		return;
	case ' ':
		rotx = roty = 0;
		return;
	case 'z':
		zpos ++;
		return;
	case 'a':
		zpos --;
		return;
	case 's':
		interp_norm = !interp_norm;
		break;
	case 'p':
		show_parent = (show_parent + 1) % 3;
		break;

	case '.': case '>':
		if (++model_pos >= max_depth) model_pos = max_depth;
		return;
	case ',': case '<':
		if (--model_pos < 0) model_pos = 0;
		return;

	case 'm':
		set_model();
		break;
	}
}

void render()
{
	if (!len(models)) return;
	while (model_pos >= len(models))
		list_push(models, catmull(elem(models, len(models) - 1)));

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(.0f, 0.f, zpos);

	rot[0] += rotx / 8.;
	rot[1] += roty / 8.;

	if (rot[0] > 360) rot[0] -= 360;
	if (rot[0] < 0) rot[0] += 360;
	if (rot[1] > 360) rot[1] -= 360;
	if (rot[1] < 0) rot[1] += 360;

	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);

	GLfloat *pcolor = color2;
	if (model_pos && show_parent) {
		if (show_parent == 2) pcolor = red;
		draw_wireframe(elem(models, model_pos - 1), pcolor, hole2);
	}

	model m = elem(models, model_pos);
	if (wireframe_mode) draw_faces(m);
	if (wireframe_mode < 2) draw_wireframe(m, color, hole);

	glFlush();
	glFinish();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	printf("size %d %d\n", w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, (GLfloat)w / h, .1f, 100.f);
	glMatrixMode(GL_MODELVIEW);
}

void init_gfx(int *c, char **v) {
	glutInit(c, v);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);

	gwin = glutCreateWindow("Catmull-Clark");
	glutReshapeFunc(resize);

	glClearColor(.0f, .0f, .0f, .0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glutKeyboardFunc(keypress);
	glutSpecialFunc(keyspecial);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutMainLoop();
}

int main(int c, char **v)
{
	int i;
	void *p;

	models = list_new();
	list_push(models, cube());
	model_pos = 1;

	init_gfx(&c, v);

	foreach(i, p, models) model_del(p);
	list_del(models);

	return 0;
}