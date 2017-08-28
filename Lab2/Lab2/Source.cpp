#include <GL/glut.h>      // (or others, depending on the system in use)
#include "math.h"
#include <vector>
#include <iostream>

using namespace std;

inline int roundPoint(const float a) { return int(a + 0.5); }

struct points
{
	int x;
	int y;
}point;
vector<points> storePoints;

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

void setPixel(int x, int y) {
	glBegin(GL_POINTS);
	glColor3f(0, 0, 1);
	glVertex2i(x, y);
	glEnd();
}


void lineDDA(int x0, int y0, int xEnd, int yEnd)
{
	int dx = xEnd - x0, dy = yEnd - y0, steps, k;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);
	setPixel(roundPoint(x), roundPoint(y));
	for (k = 0; k < steps; k++) {
		x += xIncrement;
		y += yIncrement;
		setPixel(roundPoint(x), roundPoint(y));
	}
}
void lineBres(int x0, int y0, int xEnd, int yEnd) {
	int dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
	int d = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
	int x, y;
	/* Determine which endpoint to use as start position. */
	if (x0 > xEnd) {
		x = xEnd; y = yEnd; xEnd = x0;
	}
	else {
		x = x0; y = y0;
	}
	setPixel(x, y);
	while (x < xEnd) {
		x++;
		if (d < 0)
			d += twoDy;
		else {
			y++; d += twoDyMinusDx;
		}
		setPixel(x, y);
	}
}

void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		point.x = xMouse;
		point.y = winHeight - yMouse;
		storePoints.insert(storePoints.end(), point);
	}

	if (storePoints.size() == 3) {
		storePoints.erase(storePoints.begin());
	}

	if (storePoints.size() == 2) {
		lineDDA(storePoints[0].x, storePoints[0].y, storePoints[1].x, storePoints[1].y);
		//lineBres(storePoints[0].x, storePoints[0].y, storePoints[1].x, storePoints[1].y);
	}
	glFlush();
}


void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.

	glColor3f(1.0, 0.0, 0.0);      // Set line segment color to red.
	glBegin(GL_TRIANGLES);
	glVertex2i(180, 15);       // Specify line-segment geometry.
	glVertex2i(10, 145);
	glVertex2i(100, 145);
	glEnd();
	glFlush();     // Process all OpenGL routines as quickly as possible.
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowPosition(50, 100);   // Set top-left display-window position.
	glutInitWindowSize(400, 300);      // Set display-window width and height.
	glutCreateWindow("An Example OpenGL Program"); // Create display window.

	init();                            // Execute initialization procedure.
	glutDisplayFunc(lineSegment);  
	glutMouseFunc(mousePtPlot);// Send graphics to display window.
	glutMainLoop();                    // Display everything and wait.
}