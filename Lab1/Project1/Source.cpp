#include <GL/glut.h>      // (or others, depending on the system in use)

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

	glMatrixMode(GL_PROJECTION);       // Set projection parameters.
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.

	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);

	glVertex2i(30, 30);
	glVertex2i(30, 50);
	glVertex2i(50, 80);

	glVertex2i(100, 50);
	glVertex2i(100, 100);
	glVertex2i(150, 150);
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
	glutDisplayFunc(lineSegment);       // Send graphics to display window.
	glutMainLoop();                    // Display everything and wait.
}