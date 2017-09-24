#include <GL/glut.h>  
#include "math.h"
#include <vector>
#include <iostream>


using namespace std;

inline int roundPoint(const float a) { return int(a + 0.5); }
//set golbal paramaters 
float xMove, yMove = 0.0f;
bool moveUp, moveDown, moveLeft, moveRight, moveDia, drawTri, drawSemCir = false;
bool drawDots = true;
int R = 5;
int i = 0;

//create a struct to store in vector
struct points
{
	int x;
	int y;
}point;
vector<points> storePoints,semCirPoints;

GLsizei winWidth = 400, winHeight = 300;   // Initial display-window size.

// setup opengl 
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

//used to draw a pixel on the screen
void setPixel(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

//this is used to create a circle for drawing a circle.
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


// This is used to create points a line between two points
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

// This is used to draw a triangle when called
void drawTriangle() {
	lineDDA(storePoints[0].x, storePoints[0].y, storePoints[1].x, storePoints[1].y);
	lineDDA(storePoints[1].x, storePoints[1].y, storePoints[1].x, storePoints[0].y);
	lineDDA(storePoints[1].x, storePoints[0].y, storePoints[0].x, storePoints[0].y);
}

//This is used to generate a midpoint for a circle so we can get the middle of the circle on the line while traveling around the triangle.
void circleMidpoint(points circCtr, GLint radius)
{
	points circPt;

	GLint p = 1 - radius;         // Initial value of midpoint parameter.

	circPt.x = 0;             // Set coordinates for top point of circle.
	circPt.y = radius;

	void circlePlotPoints(points, points);

	/* Plot the initial point in each circle quadrant. */
	circlePlotPoints(circCtr, circPt);

	/* Calculate next points and plot in each octant.*/
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

//We use this to draw a circle for the line of circles
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


// When two points are generated we store the points for a semi circle.
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
	// make sure there are no vaules stored so we get the right points used
	if (semCirPoints.size() > 0) {
		semCirPoints.clear();
	}
		float step = 5.0;
		for (float angle = 0.0f; angle <= 180; angle += step)
		{
			float rad = PI*angle / 180;
			x = center.x + radius*cos(rad);
			y = center.y + radius*sin(rad);
			temp.x = roundPoint(x);
			temp.y = roundPoint(y);
			semCirPoints.insert(semCirPoints.end(), temp);
			cout << "x: " << x << " : y" << y << endl;
		}

}

// This is where we draw the semi circle. it is seperate so when you change tasks we can reset values if points change
void drawSemiCircle()
{
	
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < semCirPoints.size(); j++)
	{
		glVertex2f(semCirPoints[j].x, semCirPoints[j].y);
	}
	glEnd();

}

// we move the circle here
void moveCircle(int dx, int dy) 
{
	// we use 0.1 of dx so we don't see the circle doing massive jumps along the line
	xMove += dx*0.1;
	yMove += dy*0.1;
	glutPostRedisplay();
}


//This is where we move the. we work out which way the circle is traveling and go in that driection. we use the timer function to recall the method
void moveTri(int n)     
{
	int dx = storePoints[1].x - storePoints[0].x;
	int dy = storePoints[1].y - storePoints[0].y;
	//when traviling in a diagnal
	if (moveDia) {
		moveCircle(dx, dy);
		// we check to se if dx is a positive value
		if (!(dx > 0)) {
			// we see if dx greater can -1
			if (dx - xMove >= -1) {
				moveDia = false;
				// if dy > 0 we travel down else we travel up
				if (dy >= 0) {
					moveDown = true;
				}
				else {
					moveUp = true;
				}

			}
		} else if (dx > 0) {
			// we see if dx less than 1
			if (dx - xMove <= 1) {
				moveDia = false;
				// if dy > 0 we travel down else we travel up
				if (dy >= 0) {
					moveDown = true;
				}
				else {
					moveUp = true;
				}

			}
		}
	}
	// this is for when we move up the triangle
	else if (moveUp) {
		dx = storePoints[1].x - storePoints[1].x;
		dy = storePoints[0].y - storePoints[1].y;
		moveCircle(dx, dy);
		//we see when yMove is >=-1 
		if (yMove >= -1) {
			moveUp = false;
			// check to see if the distance between point 1 and 2 x values return a positive number
			if (storePoints[1].x - storePoints[0].x >= 0) {
				moveLeft = true;
			}
			else {
				moveRight = true;
			}
		}
	}
	// when we need to move down the triangle
	else if (moveDown) {
		dx = storePoints[1].x - storePoints[1].x;
		dy = storePoints[0].y - storePoints[1].y;
		moveCircle(dx, dy);
		//we see if yMove is <=1
		if (yMove <= 1) {
			moveDown = false;	
			// check to see if the distance between point 1 and 2 x values return a positive number
			if (storePoints[1].x - storePoints[0].x >= 0) {
				moveLeft = true;
			}
			else {
			moveRight = true;
			}
		}
		
	}
	//when we move left on the triangle
	else if (moveLeft) {
		dx = storePoints[0].x - storePoints[1].x;
		dy = storePoints[1].y - storePoints[1].y;
		moveCircle(dx, dy);
		// we see if xMove  is <= 1. then we reset all the values
		if (xMove <=1) {
			moveLeft = false;
			xMove = 0;
			yMove = 0;
			dx = 0;
			dy = 0;
		}
	}
	//when we move right on the triangle
	else if (moveRight) {
		dx = storePoints[0].x - storePoints[1].x;
		dy = storePoints[1].y - storePoints[1].y;
		moveCircle(dx, dy);
		// we see if xMove  is >= 1. then we reset all the values
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


// This is to move around a semi circle... This doesn't work as I didnt know how to fix the issue in time
void moveSemCir(int n)
{
	int dx,dy;
	points temp;
	temp.x = semCirPoints[i].x;
	temp.y = semCirPoints[i].y;
	if (moveLeft) {
		if (i < semCirPoints.size()) {
			dx = semCirPoints[i + 1].x - temp.x;
			dy = semCirPoints[i + 1].y - temp.y;
		}
		else {
			dx = semCirPoints[i].x - semCirPoints[0].x;
			dy = semCirPoints[i].y - semCirPoints[0].y;
		}

		
		cout <<dx << ": "<< dy << endl;
		cout << "Next pos :"<< semCirPoints[i].x << " : " << semCirPoints[i].y << endl;
		cout <<"curr pos : "<< temp.x << " : " <<  temp.y << endl;
		moveCircle(dx, dy);
		if (i < semCirPoints.size()) {
			if (semCirPoints[i + 1].x == temp.x + dx  &&  semCirPoints[i + 1].y == temp.y + dy) {
				i++;

			}
			else {
				
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

	glutTimerFunc(n, moveSemCir, n); // recursively call moveSemCir
}


// This is used to get the different points on the screen
void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	// we clear the screen to make sure we don't get any glitches
	glClear(GL_COLOR_BUFFER_BIT);
	//we add a point to the vector
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		point.x = xMouse;
		point.y = winHeight - yMouse;
		storePoints.insert(storePoints.end(), point);
	}
	// when the vector size is 4 we reduce it back to 2
	if (storePoints.size() == 4) {
		storePoints.erase(storePoints.begin());
		storePoints.erase(storePoints.begin());
	}
	// when we get to points we store the points to generate a semi circle. Then it make draw dots true to draw a line of circles
	if (storePoints.size() == 2) {
		storeSemiCircle(storePoints[0], storePoints[1]);
		drawDots = true;
		drawTri = false;
		drawSemCir = false;
	}
	glutPostRedisplay();
	glFlush();
}

//setup keyboard controls
void keyboardown(unsigned char key, int x, int y)
{
	switch (key) {
	// when you press a circle will move around a triangle
	case 'a':
		glClear(GL_COLOR_BUFFER_BIT);
		if (!drawTri) {
			xMove = 0;
			yMove = 0;
		}
		drawTri = true;
		drawDots = false;
		drawSemCir = false;
		moveDia = true;
		moveTri(1000);
		break;
	// increase the size of a circle
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
	// decrease the size of a circle
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
	// draws a line of circles
	case 'x':
		drawDots = true;
		drawTri = false;
		drawSemCir = false;
		xMove = 0;
		yMove = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		break;
		// moves the circle around a semi circle
	case 'c':
		if (!drawSemCir) {
			xMove = 0;
			yMove = 0;
		}
		drawDots = false;
		drawTri = false;
		drawSemCir = true;
		moveLeft = true;
		moveSemCir(100);
		glClear(GL_COLOR_BUFFER_BIT);		
		break;
		// Escape key exits the program
	case 27: 
		glutDestroyWindow(1);
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// this is used to draw all objects
void lineSegment(void)
{
	// create variables and setup opengl to use a maxtrix to move the circle
	points circPt;
	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	//if we have enough points and draw dots is true
	if (storePoints.size() >= 2 && drawDots) {
		//create temp values
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
		//we draw starting circle here
		glColor3f(1.0, 0.0, 0.0);
		drawCircle((float)R, p1.x, p1.y);
		float xIn = ((2 * R)*dx / dis);
		//we draw  the of the circles here
		float yIn = ((2 * R)*dy / dis);
		for (int i = 0; i < numCir; i++) {
			p1.x += xIn;
			p1.y += yIn;
			// j is 0 we have a red circle
			if (j == 0) {
				glColor3f(1.0, 0.0, 0.0);
				drawCircle((float)R, p1.x, p1.y);
				j++;
			}
			// j is 1 we have a green circle
			else if (j == 1) {
				glColor3f(0.0, 1.0, 0.0);
				drawCircle((float)R, p1.x, p1.y);
				j++;
			}
			// j is 2 we have a blue circle
			else if (j == 2) {
				glColor3f(0.0, 0.0, 1.0);
				drawCircle((float)R, p1.x, p1.y);
				j = 0;
			}
			else {}

		}
	}
	//if we have enough points and draw triangle is true
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
	//if we have enough points and draw sdemi circle is true
	if (semCirPoints.size() >= 0 && drawSemCir) {

		points p1,p2;
		p1.x = storePoints[0].x;
		p1.y = storePoints[0].y;
		drawSemiCircle();
		glTranslatef(xMove, yMove, 0);
		circleMidpoint(semCirPoints[0], 8);
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