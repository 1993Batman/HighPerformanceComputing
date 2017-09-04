#include <GL/glut.h>      // (or others, depending on the system in use)
#include "math.h"
#include <vector>
#include <iostream>

using namespace std;

inline int roundPoint(const float a) { return int(a + 0.5); }

float xMove, yMove = 0.0f;
bool moveUp, moveDown, moveLeft, moveRight, moveDia, drawTri, drawSemCir = false;
bool drawDots = true;
int R = 5;
int i = 0;
struct points
{
	int x;
	int y;
}point;
vector<points> storePoints,semCirPoints;

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

void setPixel(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}


void circlePlotPoints(points circCtr, points circPt)
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(circCtr.x + circPt.x, circCtr.y + circPt.y);
	glVertex2i(circCtr.x - circPt.x, circCtr.y + circPt.y);
	glVertex2i(circCtr.x + circPt.x, circCtr.y - circPt.y);
	glVertex2i(circCtr.x - circPt.x, circCtr.y - circPt.y);
	glVertex2i(circCtr.x + circPt.y, circCtr.y + circPt.x);
	glVertex2i(circCtr.x - circPt.y, circCtr.y + circPt.x);
	glVertex2i(circCtr.x + circPt.y, circCtr.y - circPt.x);
	glVertex2i(circCtr.x - circPt.y, circCtr.y - circPt.x);
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

void drawTriangle() {
	lineDDA(storePoints[0].x, storePoints[0].y, storePoints[1].x, storePoints[1].y);
	lineDDA(storePoints[1].x, storePoints[1].y, storePoints[1].x, storePoints[0].y);
	lineDDA(storePoints[1].x, storePoints[0].y, storePoints[0].x, storePoints[0].y);
}


void circleMidpoint(points circCtr, GLint radius)
{
	points circPt;

	GLint p = 1 - radius;         // Initial value of midpoint parameter.

	circPt.x = 0;             // Set coordinates for top point of circle.
	circPt.y = radius;

	void circlePlotPoints(points, points);

	/* Plot the initial point in each circle quadrant. */
	circlePlotPoints(circCtr, circPt);

	/* Calculate next points and plot in each octant. */
	if (drawTri) {
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
	else if (drawSemCir) {
		while ((circPt.x) < (circPt.y)) {
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

}

void drawCircle(float radius, float x, float y)
{
	float vectorY1 = y + radius;
	float vectorX1 = x;
	glBegin(GL_POLYGON);
	for (float angle = 0.0f; (angle) <= (2.0f*3.14159); angle += 0.01f)
	{
		float vectorX = x + (radius*(float)cos((double)angle));
		float vectorY = y + (radius*(float)sin((double)angle));
		glVertex2f(vectorX1, vectorY1);
		vectorY1 = vectorY;
		vectorX1 = vectorX;
	}
	glEnd();
}



void storeSemiCircle(points p1, points p2)
{
	float x = (p2.x + p1.x) / 2;
	float y = (p2.y + p1.y) / 2;
	points center,temp;
	center.x = x;
	center.y = y;
	float radius = (sqrt(pow((p2.x-p1.x),2)+ pow((p2.y - p1.y), 2)))/2;
	float vectorY1 = y + radius;
	float vectorX1 = x;
	float PI = 3.14;
	if (semCirPoints.size() > 0) {
		semCirPoints.clear();
	}
		float step = 5.0;// How far is the next point i.e it should be small value
		for (float angle = 0.0f; angle <= 180; angle += step)
		{
			float rad = PI*angle / 180;
			x = center.x + radius*cos(rad);
			y = center.y + radius*sin(rad);
			temp.x = x;
			temp.y = y;
			semCirPoints.insert(semCirPoints.end(), temp);
		}

}

void drawSemiCircle()
{
	
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < semCirPoints.size(); j++)
	{
		glVertex2f(semCirPoints[j].x, semCirPoints[j].y);
	}
	glEnd();

}


void moveCircle(int dx, int dy) 
{
	xMove += dx*0.1;
	yMove += dy*0.1;
	glutPostRedisplay();
}

void moveTri(int n)     
{
	int dx = storePoints[1].x - storePoints[0].x;
	int dy = storePoints[1].y - storePoints[0].y;
	if (moveDia) {

		moveCircle(dx, dy);
		if (!(dx > 0)) {
			if (dx - xMove >= -1) {
				moveDia = false;
				if (dy >= 0) {
					moveDown = true;
				}
				else {
					moveUp = true;
				}

			}
		}else if (dx - xMove <= 1) {
			moveDia = false;
			if (dy >= 0) {
				moveDown = true;
			}
			else {
				moveUp = true;
			}

		}
	}
	else if (moveUp) {
		dx = storePoints[1].x - storePoints[1].x;
		dy = storePoints[0].y - storePoints[1].y;
		moveCircle(dx, dy);
		cout << "going up"<< endl;
		if (!(dy > 0)) {
			dy *= -1;
		}
		if (yMove >= 0) {
			moveUp = false;
			if (storePoints[1].x - storePoints[0].x >= 0) {
				moveLeft = true;
			}
			else {
				moveRight = true;
			}
		}
	}
	else if (moveDown) {
		dx = storePoints[1].x - storePoints[1].x;
		dy = storePoints[0].y - storePoints[1].y;
		moveCircle(dx, dy);
		cout << "going down" << endl;
		if (!(dy > 0)) {
			dy *= -1;
		}
		cout << storePoints[0].y << " : " << (yMove)  << " : "<< storePoints[0].y  - (yMove+ dy)<< endl;
		if (yMove <= 1) {
			moveDown = false;
			if (storePoints[1].x - storePoints[0].x >= 0) {
				moveLeft = true;
			}
			else {
			moveRight = true;
			}
		}
		
	}
	else if (moveLeft) {
		dx = storePoints[0].x - storePoints[1].x;
		dy = storePoints[1].y - storePoints[1].y;
		moveCircle(dx, dy);
		cout << "going left" << endl;
		cout << xMove - storePoints[0].x  << endl;
		if (!(dx > 0)) {
			dx *= -1;
		}
		if (xMove <=1) {
			moveLeft = false;
			xMove = 0;
			yMove = 0;
			dx = 0;
			dy = 0;
		}
	}
	else if (moveRight) {
		dx = storePoints[0].x - storePoints[1].x;
		dy = storePoints[1].y - storePoints[1].y;
		moveCircle(dx, dy);
		cout << "going right" << endl;
		if (!(dx > 0)) {
			dx *= -1;
		}
		if (xMove >= 1) {
			moveRight = false;
			xMove = 0;
			yMove = 0;
			dx = 0;
			dy = 0;
		}
	}
	glutTimerFunc(n, moveTri, n); // recursively call moveTri
}
void moveSemCir(int n)
{
	float dx,dy;
	points temp;
	if (moveLeft) {
		if (i < semCirPoints.size()) {
			dx = semCirPoints[i + 1].x - semCirPoints[i].x;
			dy = semCirPoints[i + 1].y - semCirPoints[i].y;
		}
		else {
			dx = semCirPoints[i].x - semCirPoints[0].x;
			dy = semCirPoints[i].y - semCirPoints[0].y;
		}
		moveCircle(dx, dy);
		temp.x = semCirPoints[i].x + xMove;
		temp.y = semCirPoints[i].y + yMove;
		cout << "Next pos :"<< semCirPoints[i].x << " : " << semCirPoints[i].y << endl;
		cout <<"Current Cir : "<< temp.x << " : " << temp.y << endl;
		if (i < semCirPoints.size()) {
			if ((temp.x - semCirPoints[i + 1].x) <= 1   &&  (temp.x -semCirPoints[i + 1].x) >= -1 &&
				(temp.y - semCirPoints[i + 1].y) <= 1 && (temp.y - semCirPoints[i + 1].y) >= -1) {
				i++;

			}
		}
		else {
			if ((semCirPoints[i].x + xMove) >= semCirPoints[0].x) {
				xMove = 0;
				yMove = 0;
				moveLeft = false;
				cout << "opps shouldnt be here" << endl;
			}
		}
	}

	glutTimerFunc(n, moveSemCir, n); // recursively call moveTri
}

void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		point.x = xMouse;
		point.y = winHeight - yMouse;
		storePoints.insert(storePoints.end(), point);
	}

	if (storePoints.size() == 4) {
		storePoints.erase(storePoints.begin());
		storePoints.erase(storePoints.begin());
	}

	if (storePoints.size() == 2) {
		cout << "i'm here" << endl;
		storeSemiCircle(storePoints[0], storePoints[1]);
		cout << semCirPoints.size() << endl;
	}

	glutPostRedisplay();
	glFlush();
}

void keyboardown(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		glClear(GL_COLOR_BUFFER_BIT);
		drawTri = true;
		drawDots = false;
		drawSemCir = false;
		moveDia = true;
		moveTri(500);
		break;
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
	case 'x':
		drawDots = true;
		drawTri = false;
		drawSemCir = false;
		xMove = 0;
		yMove = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case 'z':
		drawDots = false;
		drawTri = false;
		drawSemCir = true;
		moveLeft = true;
		moveSemCir(10);
		glClear(GL_COLOR_BUFFER_BIT);
		
		break;
		
	default:
		break;
	}
	glutPostRedisplay();
}
void lineSegment(void)
{
	points circPt;
	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	if (storePoints.size() >= 2 && drawDots) {

		points p1;
		p1.x = storePoints[0].x;
		p1.y = storePoints[0].y;
		points p2;
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

	if (storePoints.size() == 2 && drawTri) {
		glColor3f(0.0, 0.0, 0.0);
		drawTriangle();
		glTranslatef(xMove, yMove, 0);
		glColor3f(0.0, 1.0, 0.0);
		circPt.x = storePoints[0].x;
		circPt.y = storePoints[0].y;
		circleMidpoint(circPt, 8);
		glPopMatrix();
		
	}
	if (semCirPoints.size() >= 0 && drawSemCir) {

		points p1,p2;
		p1.x = storePoints[0].x;
		p1.y = storePoints[0].y;
		p2.x = storePoints[1].x;
		p2.y = storePoints[1].y;
		glColor3f(0.0, 0.0, 0.0);
		drawSemiCircle();
		float radius = (sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2))) / 2;
				
		glTranslatef(xMove, yMove, 0);
		circleMidpoint(semCirPoints[0], R);
		glPopMatrix();

		for (int j = 0; j < semCirPoints.size();j++) {
			
		}
	}

	glFlush();
      // Process all OpenGL routines as quickly as possible.
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
	glutKeyboardFunc(keyboardown);
	glutMainLoop();                    // Display everything and wait.
}