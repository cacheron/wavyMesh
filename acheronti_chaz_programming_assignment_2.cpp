#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <random>
using namespace std;

/* Chaz Acheronti */

int camera_speed = 3.0f;
float camera_focus[3]{ 480.0f, 30.0f, 500.0f }; // x, y, z coordinates of camera focus
float camera_position[3]{ 480.0f,65.0f, 1000.0f }; // x, y, z cooredinates of camera in world space

float elapsedFrames = 0.0f;

float MAX_HEIGHT = 3.0f;
float vertex_heights[100][100];

// initialize mesh heights to a random % times max height
void initializeMesh() {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			vertex_heights[i][j] = ((float)rand() / (float)RAND_MAX) * MAX_HEIGHT;
		}
	}
}

// draw triangle meshes in view of the camera with a cos scale
void drawMesh() {
	elapsedFrames += 0.5;
	if (elapsedFrames >= numeric_limits<float>::max() - 10.0f) elapsedFrames = 0.0f;
	float ratio = elapsedFrames / 75.0f;
	for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 99; j++) {
			// prevent drawing of triangles behind the camera
			if (j * 10.0f <= camera_position[2]) {
				glBegin(GL_POLYGON); // first triangle
				glVertex3f(i * 10.0f, MAX_HEIGHT * cos(vertex_heights[i][j] * ratio), j * 10.0f);
				glVertex3f((i + 1) * 10.0f, MAX_HEIGHT * cos(vertex_heights[i + 1][j] * ratio), j * 10.0f);
				glVertex3f((i + 1) * 10.0f, MAX_HEIGHT * cos(vertex_heights[i + 1][j + 1] * ratio), (j + 1) * 10.0f);
				glEnd();

				glBegin(GL_POLYGON); // second triangle
				glVertex3f(i * 10.0f, MAX_HEIGHT * cos(vertex_heights[i][j] * ratio), j * 10.0f);
				glVertex3f(i * 10.0f, MAX_HEIGHT * cos(vertex_heights[i][j + 1] * ratio), (j + 1) * 10.0f);
				glVertex3f((i + 1) * 10.0f, MAX_HEIGHT * cos(vertex_heights[i + 1][j + 1] * ratio), (j + 1) * 10.0f);
				glEnd();
			}
		}
	}
}

void init(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { -1.0, 1.0, 0.5, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);
	glEnable(GL_LIGHT1);
	GLfloat lightAmbient1[4] = { 0.65, 0.2,  1.0, 0.0};
	GLfloat lightPos1[4] = { 1.0, 0.0, -0.2, 0.0 };
	GLfloat lightDiffuse1[4] = { 1.0, 0.8,  0.5, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // set polygon mode to unfilled
}

void display(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(camera_position[0], camera_position[1], camera_position[2], camera_focus[0], camera_focus[1], camera_focus[2], 0.0f, 1.0f, 0.0f);

	// draw triangles at distances of 10 in the x and z plane with unique y height
	drawMesh();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
	{
		gluPerspective(80, (float)w, 1.0, 5000.0);
	}
	else
	{
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// keyboard event handlers
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			camera_position[1] += camera_speed;
			camera_focus[1] += camera_speed;
			break;
		case 's':
			camera_position[1] -= camera_speed;
			camera_focus[1] -= camera_speed;
			break;
		default:
			break;
	}
	if (camera_position[1] < 0) { camera_position[1] = 0; camera_focus[1] = -35; } // camera bounds y height
	cout << "Focus (x,y,x) =  (" << camera_focus[0] << ", " << camera_focus[1] << ", " << camera_focus[2] << ")"
		<< "\nCamera (x,y,z) = (" << camera_position[0] << ", " << camera_position[1] << ", " << camera_position[2] << ")\n";
}
// arrow key event handlers
void arrow_keys(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			camera_position[2] -= camera_speed;
			camera_focus[2] -= camera_speed;
			break;
		case GLUT_KEY_DOWN:
			camera_position[2] += camera_speed;
			camera_focus[2] += camera_speed;
			break;
		case GLUT_KEY_LEFT:
			camera_position[0] -= camera_speed;
			camera_focus[0] -= camera_speed;
			break;
		case GLUT_KEY_RIGHT:
			camera_position[0] += camera_speed;
			camera_focus[0] += camera_speed;
			break;
		default:
			break;
	}
	if (camera_position[0] > 1000) { camera_position[0] = 1000; camera_focus[0] = 1000; } // camera bounds x axis
	if (camera_position[0] < 0) { camera_position[0] = 0; camera_focus[0] = 0; }
	if (camera_position[2] > 1000) { camera_position[2] = 1000; camera_focus[2] = 500; } // camera forward bounds
	if (camera_position[2] < 0) { camera_position[2] = 0; camera_focus[2] = -500; }

	cout << "Focus (x,y,x) =  (" << camera_focus[0] << ", " << camera_focus[1] << ", " << camera_focus[2] << ")"
		<< "\nCamera (x,y,z) = (" << camera_position[0] << ", " << camera_position[1] << ", " << camera_position[2] << ")\n";
}

int main(int argc, char *argv[])
{
	int window;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(160, 160);
	window = glutCreateWindow("T H E  G R I D      C h a z  A c h e r o n t i");
	init();

	initializeMesh();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	glutMainLoop();
}