#include <GL/glut.h>      // (or others, depending on the system in use)
#include <math.h>
#include <ostream>
using namespace std;

int winHeight;
bool isSet = false;
float xMove, yMove = 0.0f;
bool moveUp, moveDown,moveLeft = false;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

	glMatrixMode(GL_PROJECTION);       // Set projection parameters.
	glLoadIdentity();
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
	winHeight = 300.0;
}

class scrPt {
public:
	GLint x, y;
};
scrPt point;
void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void circlePlotPoints(scrPt circCtr, scrPt circPt)
{
	setPixel(circCtr.x + circPt.x, circCtr.y + circPt.y);
	setPixel(circCtr.x - circPt.x, circCtr.y + circPt.y);
	setPixel(circCtr.x + circPt.x, circCtr.y - circPt.y);
	setPixel(circCtr.x - circPt.x, circCtr.y - circPt.y);
	setPixel(circCtr.x + circPt.y, circCtr.y + circPt.x);
	setPixel(circCtr.x - circPt.y, circCtr.y + circPt.x);
	setPixel(circCtr.x + circPt.y, circCtr.y - circPt.x);
	setPixel(circCtr.x - circPt.y, circCtr.y - circPt.x);
}

void circleMidpoint(scrPt circCtr, GLint radius)
{
	scrPt circPt;

	GLint p = 1 - radius;         // Initial value of midpoint parameter.

	circPt.x = 0;             // Set coordinates for top point of circle.
	circPt.y = radius;

	void circlePlotPoints(scrPt, scrPt);

	/* Plot the initial point in each circle quadrant. */
	circlePlotPoints(circCtr, circPt);

	/* Calculate next points and plot in each octant. */
	while (circPt.x < circPt.y) {
		circPt.x++;
		if (p < 0)
			p += 2 * circPt.x + 1;
		else {
			circPt.y--;
			p += 2 * (circPt.x - circPt.y) + 1;
		}
		circlePlotPoints(circCtr, circPt);
	}
}

void lineSegment(void)
{
	scrPt circPt;
	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glColor3f(0.0, 0.0, 1.0);      // Set line segment color to red.
	glBegin(GL_LINES);
	glVertex2i(280, 250);       // Specify line-segment geometry.
	glVertex2i(80, 50);
	glVertex2i(280, 250);
	glVertex2i(280, 50);
	glVertex2i(80, 50);
	glVertex2i(280, 50);
	glEnd();
	

	glTranslatef(xMove, yMove, 0);
	glColor3f(0.0, 1.0, 0.0);
	circPt.x = 80;
	circPt.y = 50;
	circleMidpoint(circPt, 8);
	glPopMatrix();
	glFlush();     // Process all OpenGL routines as quickly as possible.
}

void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{

	
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		point.x = xMouse;
		point.y = winHeight - yMouse;
		isSet = true;
		
	}

	if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP && isSet) {
		circleMidpoint(point, 8);
	}
	glFlush();
}


void moveUpDisplay()          // ORIGINAL FUNCTION
{
	xMove += 2.0f;
	yMove += 2.0f;
	glutPostRedisplay();
}

void moveDownDisplay()          // ORIGINAL FUNCTION
{
	yMove -= 2.0f;
	glutPostRedisplay();
}

void moveLeftDisplay()          // ORIGINAL FUNCTION
{
	xMove -= 2.0f;
	glutPostRedisplay();
}

void timer_func(int n)           // NEW FUNCTION
{
	// Update the object positions, etc.
	if (moveUp) {
		moveUpDisplay();
		if (200 - xMove == 0){
			moveUp = false;
			moveDown = true;

			
		}
	}
	else if (moveDown) {
		moveDownDisplay();
		if (200 - yMove == 200) {
			moveDown = false;
			moveLeft = true;
		}
	}
	else if(moveLeft) {
		moveLeftDisplay();
		if (200 - xMove == 200) {
			moveLeft = false;
		}
	}

	glutTimerFunc(n, timer_func, n); // recursively call timer_func
}

void keyboardown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		moveUp = true;
		timer_func(100);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowPosition(50, 100);   // Set top-left display-window position.
	glutInitWindowSize(400, 300);      // Set display-window width and height.
	glutCreateWindow("An Example OpenGL Program"); // Create display window.

	init();                            // Execute initialization procedure.
	glutDisplayFunc(lineSegment);       // Send graphics to display window.
	glutMouseFunc(mousePtPlot);// Send graphics to display window.
	glutSpecialFunc(keyboardown);
	glutMainLoop();                    // Display everything and wait.
}
