#include <GL/glut.h>
#include <stdlib.h>
//#include <unistd.h>

//Create states for robot
enum state {
	Basic,
	Complex
};

enum animation {
	None,
	Dance,
	Kick,
	Run,
	Wave
};
//Set state and Animation
state currState = Basic;
animation currAnimation = Wave;

//Basic State Movement
static int rShoulder, lShoulder, neck = 0;
//Rotate The Robot
static int xRotate,yRotate, zRotate = 0;
//Complex Shoulder Movement
static int rShX, rShY, rShZ, lShX, lShY, lShZ = 0;
//Complex Elbows Movement
static int rElX, rElY, lElX, lElY = 0;
//Complex Knees Movement
static int rKnX, rKnY, lKnX, lKnY = 0;
//Complex Hips Movement
static int rHiX = 0, rHiY = 0, rHiZ = 0, lHiX = 0, lHiY = 0, lHiZ = 0;
//Move robot around the screen
static int xMove, yMove, zMove = 0;
// Animation Frame and Length
int animationFrame, animationLen = 0;

GLfloat lightingPos[] = {1.0, 1.0, 1.0, 1.0 };
GLfloat lightingCol[] = { 1.0, 0.0, 0.0, 1.0 };
// setup opengl
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
}

// Reset values to orginal settings
void resetValues() {
	rShoulder = 0;
	lShoulder = 0;
	neck = 0;
	xRotate = 0;
	yRotate = 0;
	zRotate = 0;
	xMove = 0;
	yMove = -1;
	zMove = 0;
}

void resetAnimation()
{
	currAnimation = None;
	animationFrame = 0;
	animationLen = 0;
	neck = 0;
	rShX = 0;
	rShY = 0;
	rShZ = 0;
	lShX = 0;
	lShY = 0;
	lShZ = 0;
	rElX = 0;
	rElY = 0;
	lElX = 0;
	lElY = 0;
	rKnX = 0;
	rKnY = 0;
	lKnX = 0;
	lKnY = 0;
	rHiX = 0;
	rHiY = 0;
	rHiZ = 0;
	lHiX = 0;
	lHiY = 0;
	lHiZ = 0;
}

void selectAnimation() {
	// Wave animation
	if (currAnimation == Wave) {
		animationLen = 161;
		// Move shoulder into position
		if (animationFrame < 30)
		{
			rShZ -= 3;
			animationFrame++;
		}
		else if (animationFrame == 30)
		{
			rShY += 90;
			animationFrame++;
		}
		//Move elbow and then create a wave like animation
		else if (animationFrame <= 60)
		{
			rElX += 3;
			animationFrame++;
		}
		else if (animationFrame <= 70)
		{
			rElX -= 4;
			animationFrame++;
		}
		else if (animationFrame <= 80)
		{
			rElX += 4;
			animationFrame++;
		}
		else if (animationFrame <= 90)
		{
			rElX -= 4;
			animationFrame++;
		}
		else if (animationFrame <= 100)
		{
			rElX += 4;
			animationFrame++;
		}
		else if (animationFrame <= 130)
		{
			rElX -= 3;
			animationFrame++;
		}
		else if (animationFrame == 131)
		{
			rShY -= 90;
			animationFrame++;
		}
		else if (animationFrame <= 161)
		{
			rShZ += 3;
			animationFrame++;
		}
		//when finsihed it resets the animation to None
		if (animationFrame > animationLen)
		{
			animationFrame = 0;
			glutPostRedisplay();
			resetAnimation();
		}
		Sleep(16.6);
		glutPostRedisplay();

	}

	if (currAnimation == Run) {
		animationLen = 64;
		//Set up arm movement
		if (animationFrame <= 9)
		{
			//Move both arms in a flowing motion using your shoulder and elbows
			rShX += 5;
			rElX -= 10;
			lShX -= 5;
			lElX -= 10;

			// Extend the left leg in running like motion
			rHiX -= 5;
			lHiX += 5;
			lKnX += 10;
			animationFrame++;
		}	else if (animationFrame <= 18){
			//Swing legs back into poistion
			rHiX += 5;
			lHiX -= 5;
			
			//Swing arms back into postion
			rShX -= 5;
			lShX += 5;
			animationFrame++;
		}	else if (animationFrame <= 27){
			//Move right leg forward and bring left leg back
			rHiX += 5;
			lHiX -= 5;
			lKnX -= 10;
			
			//Bring left arm backward right arm forward
			rShX -= 5;
			lShX += 5;
			animationFrame++;
		}	else if (animationFrame <= 36){
			//Bend right leg and bring left leg forward
			rHiX -= 5;
			rKnX += 10;
			lHiX += 5;

			//Swing arms back into postion
			rShX += 5;
			lShX -= 5;
			animationFrame++;
		}	else if (animationFrame <= 45){
			//Bring right leg forward and left leg back
			rHiX -= 5;
			rKnX -= 10;
			lHiX += 5;

			//Swing arms back into postion
			rShX += 5;
			lShX -= 5;
			animationFrame++;
		}	else if (animationFrame <= 54){
			//Bring right leg back and left leg forward
			lHiX -= 5;
			lKnX += 10;
			rHiX += 5;

			//Swing arms back into postion
			rShX -= 5;
			lShX += 5;
			animationFrame++;
		}	else if (animationFrame <= 63){
			//Bring right leg back and left leg forward
			lHiX -= 5;
			lKnX -= 10;
			rHiX += 5;

			//Swing arms back into postion
			rShX -= 5;
			lShX += 5;
			animationFrame++;
		}
		// Reset the frame so we can get a fluid running motion
		else if (animationFrame == 64)
		{
			animationFrame = 28;
		}
		Sleep(16.6);
		glutPostRedisplay();
	}

	if (currAnimation == Kick) {

		animationLen = 100;
		if (animationFrame <= 10)
		{
			lHiX -= 9;
			lKnX += 9;
			animationFrame++;
		}	else if (animationFrame <= 20)
		{
			lKnX -= 9;
			animationFrame++;
		}	else if (animationFrame <= 30)
		{
			lKnX += 9;
			animationFrame++;
		}	else if (animationFrame <= 40)
		{
			lHiX += 9;
			lKnX -= 9;
			animationFrame++;
		}
		//Reset animation
		if (animationFrame > animationLen)
		{
			animationFrame = 0;
			glutPostRedisplay();
			resetAnimation();
		}
		Sleep(16.6);
		glutPostRedisplay();
	}

	if (currAnimation == Dance) {
		animationLen = 587;
		if (animationFrame <= 90)
		{
			//Bring arms up
			rShX -= 2.5;
			rShY += 2;
			lShX -= 2;
			lShY += 2;
			animationFrame++;
		}	else if (animationFrame <= 120) {
			//Rotate Shoulder on z axis
			rShZ -= 3;
			animationFrame++;
		}	else if (animationFrame <= 135){
			//Bend arm slightly
			rElX -= 5;
			animationFrame++;
		}	else if (animationFrame <= 165) {
			//Bring right leg up
			rKnX += 3;
			rHiY -= 3;
			rHiX -= 3;
			animationFrame++;
		}	else if (animationFrame <= 255) {
			//Spin around
			xRotate -= 4;
			animationFrame++;
		}	else if (animationFrame <= 285) {
			//Bring right leg down
			rKnX -= 3;
			rHiY += 3;
			rHiX += 3;
			animationFrame++;
		}	else if (animationFrame <= 300) {
			//Bend arm back
			rElX += 5;
			animationFrame++;
		}	else if (animationFrame <= 330) {
			//Rotate Shoulder on z axis
			rShZ += 3;
			animationFrame++;
		}	else if (animationFrame <= 420) {
			//Rotate Shoulder on z axis
			rShX -= 2.5;
			rShY += 2;
			lShX -= 2;
			lShY += 2;
			animationFrame++;
		}	else if (animationFrame <= 440) {
			//Move left leg forward and righ leg back
			lKnX += 2;
			lHiX -= 2;
			rKnX += 2;
			rHiX += 2;
			animationFrame++;
		}	else if (animationFrame <= 470) {
			//Bring both arms up
			rShZ -= 3;
			lShZ += 3;
			animationFrame++;
		}	else if (animationFrame <= 500) {
			//Dab it
			rShZ -= 3;
			rShX += 3;
			rShY -= 4;
			rElX -= 4;
			animationFrame++;
		}	else if (animationFrame <= 530) {
			//Move left leg forward and righ leg back
			rShZ += 3;
			rShX -= 3;
			rShY += 4;
			rElX += 4;
			animationFrame++;
		}	else if (animationFrame <= 560) {
			//Bring both arms down
			rShZ += 3;
			lShZ -= 3;
			animationFrame++;
		}	else if (animationFrame <= 580) {
			//Move legs back in place
			lKnX -= 2;
			lHiX += 2;
			rKnX -= 2;
			rHiX -= 2;
			animationFrame++;
		}
		//Reset animation
		else if (animationFrame > animationLen)
		{
			animationFrame = 0;
			glutPostRedisplay();
			resetAnimation();
		}
		Sleep(16.6);
		glutPostRedisplay();
	}
}


//Create the robot
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//Lighting
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightingCol);
	glLightfv(GL_LIGHT0, GL_POSITION, lightingPos);



	glPushMatrix();
	// Rotate and Move robot around the screen
	glTranslatef(yMove, xMove, zMove);
	glRotatef((GLfloat)xRotate, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)yRotate, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)zRotate, 0.0, 0.0, 1.0);
	
	// check state if you want a basic or complex robot
	// Make a basic Robot
	if (currState == Basic) {
		//disable lighting
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		//Draw the Body
		glPushMatrix();
		glScalef(1.5, 1.5, 0.375);
		glutWireCube(1.0);
		glPopMatrix();

		//Draw the Head
		glPushMatrix();
		glTranslatef(0.0, 0.75, 0.0);
		glRotatef((GLfloat)neck, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.25, 0.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glutWireCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw the Left Arm
		glPushMatrix();
		glTranslatef(0.9, 0.6, 0.0);
		glRotatef((GLfloat)lShoulder, 1.0, 0.0, 0.0);
		glTranslatef(0.0, -0.6, 0.0);
		glPushMatrix();
		glScalef(0.3, 1.2, 0.3);
		glutWireCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw the Right Arm
		glPushMatrix();
		glTranslatef(-0.9, 0.6, 0.0);
		glRotatef((GLfloat)rShoulder, 1.0, 0.0, 0.0);
		glTranslatef(0.0, -0.6, 0.0);
		glPushMatrix();
		glScalef(0.3, 1.2, 0.3);
		glutWireCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw the Left Leg
		glPushMatrix();
		glTranslatef(0.375, -1.35, 0.0);
		glPushMatrix();
		glScalef(0.3, 1.2, 0.3);
		glutWireCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw the Right Leg
		glPushMatrix();
		glTranslatef(-0.375, -1.35, 0.0);
		glPushMatrix();
		glScalef(0.3, 1.2, 0.3);
		glutWireCube(1.0);
		glPopMatrix();
		glPopMatrix();
	}

	//Make a Complex Robot
	if (currState == Complex) {
		//Enable Lighting
		glEnable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		
		//Start your Animation
		selectAnimation();

		//Draw Upper Body
		glPushMatrix();
		glScalef(1.0, 0.5, 0.5);
		glutSolidCube(1.0);
		glPopMatrix();

		//Draw Waist Upper
		glPushMatrix();
		glTranslatef(0.0, -0.375, 0.0);
		glPushMatrix();
		glScalef(0.5, 0.25, 0.25);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Lower Body
		glPushMatrix();
		glTranslatef(0.0, -0.625, 0.0);
		glPushMatrix();
		glScalef(0.75, 0.25, 0.4);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Head
		glPushMatrix();
		glTranslatef(0.0, 0.45, 0.0);
		glPushMatrix();
		glScalef(0.4, 0.4, 0.4);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Left Shoulder
		glPushMatrix();
		glTranslatef(0.625, 0.125, 0.0);
		glPushMatrix();
		glScalef(0.125, 0.125, 0.125);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		
		//Draw Left Upper Arm and Rotate the Shoulder
		glRotatef((GLfloat)lShX, 1.0, 0.0, 0.0);
		glRotatef((GLfloat)lShZ, 0.0, 0.0, 1.0);
		glRotatef((GLfloat)lShY, 0.0, 1.0, 0.0); 
		glPushMatrix();
		glTranslatef(0.0, -0.475, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.7, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		
		//Draw Left Elbow
		glPushMatrix();
		glTranslatef(0, -0.8125, 0.0);
		glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Left Lower Arm and Rotate the Elbow
		glRotatef((GLfloat)lElX, 1.0, 0, 0); 
		glRotatef((GLfloat)lElY, 0, 1.0, 0); 
		glPushMatrix();
		glTranslatef(0.0, -0.3, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.6, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		//Draw Right Shoulder
		glPushMatrix();
		glTranslatef(-0.625, 0.125, 0.0);
		glPushMatrix();
		glScalef(0.125, 0.125, 0.125);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Right Upper Arm and Rotate the Shoulder
		glRotatef((GLfloat)rShX, 1.0, 0.0, 0.0);
		glRotatef((GLfloat)rShZ, 0.0, 0.0, 1.0); 
		glRotatef((GLfloat)rShY, 0.0, 1.0, 0.0); 
		glPushMatrix();
		glTranslatef(0.0, -0.475, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.7, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Right Elbow
		glPushMatrix();
		glTranslatef(0, -0.8125, 0.0);
		glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Right Lower Arm and Rotate the Elbow
		glRotatef((GLfloat)rElX, 1.0, 0, 0); 
		glRotatef((GLfloat)rElY, 0, 1.0, 0);
		glPushMatrix();
		glTranslatef(0.0, -0.3, 0.0);
		glPushMatrix();
		glScalef(0.2, 0.6, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		
		//Draw Right Hip
		glPushMatrix();
		glTranslatef(0.25, -0.875, 0.0);
		glPushMatrix();
		glScalef(0.125, 0.125, 0.125);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();
		
		
		//Draw Left Upper Leg and Rotate the Hip
		glRotatef((GLfloat)lHiX, 1.0, 0, 0);
		glRotatef((GLfloat)lHiY, 0, 1.0, 0); 
		glRotatef((GLfloat)lHiZ, 0, 0, 1.0); 
		glPushMatrix();
		glTranslatef(0.0, -0.675, 0.0);
		glPushMatrix();
		glScalef(0.2, 1.1, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		
		//Draw Left Knee
		glPushMatrix();
		glTranslatef(0, -1.225, 0.0);
		glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Left Lower Leg and Rotate the Knee
		glRotatef((GLfloat)lKnX, 1.0, 0, 0); 
		glRotatef((GLfloat)lKnY, 0, 1.0, 0);
		glPushMatrix();
		glTranslatef(0.0, -0.5, 0.0);
		glPushMatrix();
		glScalef(0.2, 1, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Left Foot
		glPushMatrix();
		glTranslatef(0.0, -1.0625, 0.1);
		glPushMatrix();
		glScalef(0.2, 0.125, 0.4);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		//Draw Right Hip
		glPushMatrix();
		glTranslatef(-0.25, -0.875, 0.0);
		glPushMatrix();
		glScalef(0.125, 0.125, 0.125);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Right Upper Leg and Rotate the Hip
		glRotatef((GLfloat)rHiX, 1.0, 0, 0); 
		glRotatef((GLfloat)rHiY, 0, 1.0, 0); 
		glRotatef((GLfloat)rHiZ, 0, 0, 1.0); 
		glPushMatrix();
		glTranslatef(0.0, -0.675, 0.0);
		glPushMatrix();
		glScalef(0.2, 1.1, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Right Knee
		glPushMatrix();
		glTranslatef(0, -1.225, 0.0);
		glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glutSolidSphere(1.0, 10, 8);
		glPopMatrix();

		//Draw Right Lower Leg and Rotate the Knee
		glRotatef((GLfloat)rKnX, 1.0, 0, 0); 
		glRotatef((GLfloat)rKnY, 0, 1.0, 0); 
		glPushMatrix();
		glTranslatef(0.0, -0.5, 0.0);
		glPushMatrix();
		glScalef(0.2, 1, 0.2);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();

		//Draw Right Foot
		glPushMatrix();
		glTranslatef(0.0, -1.0625, 0.1);
		glPushMatrix();
		glScalef(0.2, 0.125, 0.4);
		glutSolidCube(1.0);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}


//When changing the screen size you update the models to scale
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 6.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


// Add keyboard functionality for basic and complex robot
void keyboard(unsigned char key, int x, int y)
{
	// Keys used for the Basic Robot
	if (currState == Basic) {
		switch (key) {
		case 'a':
			neck = (neck + 5) % 360;
			glutPostRedisplay();
			break;
		case 'A':
			neck = (neck - 5) % 360;
			glutPostRedisplay();
			break;
		case 's':
			rShoulder = (rShoulder + 5) % 360;
			glutPostRedisplay();
			break;
		case 'S':
			rShoulder = (rShoulder - 5) % 360;
			glutPostRedisplay();
			break;
		case 'd':
			lShoulder = (lShoulder + 5) % 360;
			glutPostRedisplay();
			break;
		case 'D':
			lShoulder = (lShoulder - 5) % 360;
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}

	// Keys used for the Complex Robot
	if (currState == Complex) {
		switch (key) {
		case '1':
			resetAnimation();
			currAnimation = Dance;
			animationFrame = 1;
			glutPostRedisplay();
			break;
		case '2':
			resetAnimation();
			currAnimation = Kick;
			animationFrame = 1;
			glutPostRedisplay();
			break;
		case '3':
			resetAnimation();
			currAnimation = Run;
			animationFrame = 1;
			glutPostRedisplay();
			break;
		case '4':
			resetAnimation();
			currAnimation = Wave;
			animationFrame = 1;
			glutPostRedisplay();
			break;
		case '5':
			resetAnimation();
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}

	// Common Keys used between both states
	switch (key) {
	case 'q':
		zRotate = (zRotate + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Q':
		zRotate = (zRotate - 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		xMove += 1;;
		glutPostRedisplay();
		break;
	case 'W':
		xMove -= 1;
		glutPostRedisplay();
		break;
	case 'e':
		yMove += 1;
		glutPostRedisplay();
		break;
	case 'E':
		yMove -= 1;
		glutPostRedisplay();
		break;
	case 'r':
		zMove += 1;
		glutPostRedisplay();
		break;
	case 'R':
		zMove -= 1;
		glutPostRedisplay();
		break;
	case 'c':
		if (currState == Basic) {
			currState = Complex;	
		}
		else {
			currState = Basic;
			currAnimation = Wave;
		}
		
		glutPostRedisplay();
		break;
	case '0':
		resetValues();
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void rotate(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		xRotate = (xRotate + 5) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		xRotate = (xRotate - 5) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		yRotate = (yRotate + 5) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		yRotate = (yRotate - 5) % 360;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	resetValues();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(rotate);
	glutMainLoop();
	glDisable(GL_CULL_FACE);
	return 0;
}
