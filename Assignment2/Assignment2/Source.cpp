#include <GL/glut.h>
#include <stdlib.h>

static int rightShoulder, leftShoulder, rightElbow, leftElbow = 0;
int angle = 0;
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	glRotatef((GLfloat)angle, 0.0, 1.0, 0.0);


	glTranslatef(-0.25, 0.0, 0.0);
	glRotatef((GLfloat)rightShoulder, 0.0, 0.0, 1.0);
	glTranslatef(0.25, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.5, 0.1, 0.25);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(0.25, 0.0, 0.0);
	glRotatef((GLfloat)rightElbow, 0.0, 0.0, 1.0);
	glTranslatef(0.25, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.5, 0.1, 0.25);
	glutWireCube(1.0);
	glPopMatrix();


	
	
	glTranslatef(0.75, 0.0, 0.0);
	glRotatef((GLfloat)leftShoulder, 0.0, 0.0, 1.0);
	glTranslatef(0.75, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.5, 0.1, 0.25);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(0.75, 0.0, 0.0);
	glRotatef((GLfloat)leftElbow, 0.0, 0.0, 1.0);
	glTranslatef(0.75, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.5, 0.1, 0.25);
	glutWireCube(1.0);
	glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
		rightShoulder = (rightShoulder + 5) % 360;
		glutPostRedisplay();
		break;
	case 'S':
		rightShoulder = (rightShoulder - 5) % 360;
		glutPostRedisplay();
		break;
	case 'e':
		rightElbow = (rightElbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'E':
		rightElbow = (rightElbow - 5) % 360;
		glutPostRedisplay();
		break;
	case 'q':
		leftShoulder = (leftShoulder + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Q':
		leftShoulder = (leftShoulder - 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		leftElbow = (leftElbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'W':
		leftElbow = (leftElbow - 5) % 360;
		glutPostRedisplay();
		break;
	case 'l':
		angle = (angle + 5) % 360;
		glutPostRedisplay();
		break;
	case 'r':
		angle = (angle - 5) % 360;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
