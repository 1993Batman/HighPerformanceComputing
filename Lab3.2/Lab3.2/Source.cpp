#include <GL/glut.h>
#include<stdlib.h>
#include <vector>
#include<math.h>
#include<stdio.h>
#include <iostream>

using namespace std;

int R = 5;
GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.
GLint endPtCtr = 0;                   // Initialize line endpoint counter.
GLint start = 0;

class scrPt {
public:
	GLint x, y;
};
static scrPt endPt1, endPt2;

struct points
{
	int x;
	int y;
}point;

vector<points> storePoints;

void init(void)
{

	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

void winReshapeFcn(int newWidth, int newHeight)
{
	/*  Reset viewport and projection parameters  */
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

	/*  Reset display-window size parameters.  */
	winWidth = newWidth;
	winHeight = newHeight;
}

void drawLineSegment(scrPt endPt1, scrPt endPt2)
{

	glBegin(GL_LINES);
	glVertex2i(endPt1.x, endPt1.y);
	glVertex2i(endPt2.x, endPt2.y);
	glEnd();
}
void checkDistance(scrPt p1, scrPt p2) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	int dis = sqrt((dx*dx) + (dy*dy));
	int numCir = dis / (2 * R);
	printf("Distance : %d \n", dis);
	printf("Number of circles: %d \n", numCir);

}
void keyboard(unsigned char key, int x, int y)
{
	scrPt p1;
	p1.x = storePoints[0].x;
	p1.y = storePoints[0].y;
	scrPt p2;
	p2.x = storePoints[1].x;
	p2.y = storePoints[1].y;


	switch (key) {
	case '+':
		if (R == 20) {
			printf("The range of R is 0-20\n");
			printf("You can try to scale down\n");
		}
		else {
			R = R + 1;
			glutPostRedisplay();
		}
		break;
	case '-':
		if (R == 1) {
			printf("The range of R is 0-20\n");
			printf("You can try to scale up\n");
		}
		else {
			R = R - 1;
			glutPostRedisplay();

		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void drawCircle(float radius, float x, float y)
{
	float vectorY1 = y + radius;
	float vectorX1 = x;
	glBegin(GL_POLYGON);
	for (float angle = 0.0f; angle <= (2.0f*3.14159); angle += 0.01f)
	{
		float vectorX = x + (radius*(float)cos((double)angle));
		float vectorY = y + (radius*(float)sin((double)angle));
		glVertex2f(vectorX1, vectorY1);
		vectorY1 = vectorY;
		vectorX1 = vectorX;
	}
	glEnd();

}


void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (storePoints.size() >= 2) {

		scrPt p1;
		p1.x = storePoints[0].x;
		p1.y = storePoints[0].y;
		scrPt p2;
		p2.x = storePoints[1].x;
		p2.y = storePoints[1].y;

	

		int dx = p2.x - p1.x;
		int dy = p2.y - p1.y;

		int dis = sqrt((dx*dx) + (dy*dy));
		int numCir = dis / (R * 2);
		int j = 1;
		glColor3f(1.0, 0.0, 0.0);
		drawCircle((float)R, p1.x, p1.y);
		float xIn = ((2 * R)*dx / dis);
		float yIn = ((2 * R)*dy / dis);
		for (int i = 0; i < numCir; i++) {
			p1.x += xIn;
			p1.y += yIn;
			if (j == 0) {
				glColor3f(1.0, 0.0, 0.0);
				drawCircle((float)R, p1.x, p1.y);
				j++;
			}
			else if (j == 1) {
				glColor3f(0.0, 1.0, 0.0);
				drawCircle((float)R, p1.x, p1.y);
				j++;
			}
			else if (j == 2) {
				glColor3f(0.0, 0.0, 1.0);
				drawCircle((float)R, p1.x, p1.y);
				j = 0;
			}
			else {}

		}
	}
	glFlush();     // Process all OpenGL routines as quickly as possible.
}


void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		point.x = xMouse;
		point.y = winHeight - yMouse;
		storePoints.insert(storePoints.end(), point);
	}

	if (storePoints.size() == 4) {
		storePoints.erase(storePoints.begin());
		storePoints.erase(storePoints.begin());
	}
	glutPostRedisplay();
	glFlush();
}







int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Draw Interactive Polyline");

	init();
	glutDisplayFunc(lineSegment);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousePtPlot);

	glutMainLoop();
}