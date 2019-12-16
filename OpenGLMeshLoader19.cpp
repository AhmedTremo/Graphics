#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include "Camera.h"

int WIDTH = 1280;
int HEIGHT = 720;
double xPlayer = 0;
double jump = 0;
double zPlayer = 0;
double zWeapon = 70;
double depth = 0;
boolean jumpUp = true;;
int swap = 0;
boolean resetLevel = false;
int score = 0;
int max_score = 100;
bool l0, l1, l2;
int light = 0;
GLboolean level2flag = false;
GLdouble enemyZ = 0;
int lightZ = 70;
float lightx = -3.0f;
GLboolean lightxf = false;
float lights = 1.0f;
GLboolean l = false;
float scalef = 0.0;
GLboolean scaleflag = false;
double rotang = 0;
bool hit = false;
double zqube = 0;

GLuint tex;
GLuint texMarple;
char title[] = "The Great Hall Escape";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

int cameraZoom = 0;

Camera camera = Camera(0.5f, 2.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f);

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 12, 90);
Vector At(0, -20, 0);
Vector Up(0, 1, 0);


// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS player;
Model_3DS weapon;
Model_3DS model_car;

// Textures
GLTexture tex_ground;
GLTexture tex_wood;
GLTexture tex_marple;

void InitLightSource()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	GLfloat l0Diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat l0Spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l0Ambient[] = { .1f, 0.1f, 0.1f, 1.f };
	GLfloat l0Position[] = { 0.0f + lightx, 5.0f, 72.0f, 1 };
	GLfloat l0Direction[] = { -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0Spec);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	GLfloat l1Diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.1f, .1f, 0.1f, 1.0f };
	GLfloat l1Spec[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 10.0f, 0.0f, l1 };
	GLfloat l1Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l1Spec);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);

	GLfloat l2Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l2Ambient[] = { 0.0f, 0.0f, .1f, 1.0f };
	GLfloat l2Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l2Position[] = { 0.0f + lightx, 5.0f, 72.0f, 1 };
	GLfloat l2Direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l2Spec);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);
}

void lightAnim(int time)
{
	// l0 = !l0;
	if (light == 0) {
		l0 = 1;
		l1 = 0;
		l2 = 0;
	}
	if (light == 1) {
		l0 = 0;
		l1 = 1;
		l2 = 0;
	}
	if (light == 2) {
		l0 = 0;
		l1 = 0;
		l2 = 1;
	}
	light++;
	light %= 3;
	glutTimerFunc(1500, lightAnim, 0);
}

void print(int x, int y, char *string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, 300 / 300, 0.1f, 300.0f);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	if (!level2flag) {
		glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_wood.texture[0]);	// Bind the ground texture
	}

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(10, 0, -1200-depth);
	glTexCoord2f(100, 0);
	glVertex3f(10, 0, 100-depth);
	glTexCoord2f(100, 100);
	glVertex3f(-10, 0, 100-depth);
	glTexCoord2f(0, 100);
	glVertex3f(-10, 0, -1200-depth);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myInit();

	// Draw Ground

	RenderGround();

	//Draw Sky
	glDisable(GL_LIGHTING);
	glPushMatrix();
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	if(!level2flag)
		glBindTexture(GL_TEXTURE_2D, tex);
	else
		glBindTexture(GL_TEXTURE_2D, texMarple);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 120, 120, 120);
	gluDeleteQuadric(qobj);
	glPopMatrix();
	glEnable(GL_LIGHTING);


	// Draw Player
	
	glPushMatrix();
	glTranslated(1 + xPlayer, 2,70);
	glRotatef(-180.0, 0, 1, 0);
	//glScaled(1, 1, 1);
	player.Draw();
	glPopMatrix();
	

	if (level2flag) {


		//Draw enemy
		glPushMatrix();
		glTranslated(xPlayer, 2, 79 - enemyZ);
		glScaled(0.5f, 0.5f, 0.5f);
		model_car.Draw();
		glPopMatrix();

	}

	// Draw Weapon
	glDisable(GL_LIGHTING);
	int xWeapon = -3;
	if (!resetLevel) {
		for (int i = -20; i > -1000; i = i - 15)
		{

			glPushMatrix();
			glTranslated(xWeapon, 2, i - depth);
			
			
			if (!level2flag) {
				glRotatef(-90, 1, 0, 0);
				glScaled(0.1, 0.1, 0.1);
				weapon.Draw();
			}
			else {
				glScaled(0.3, 0.4, 0.1);
				glColor3f(0.0f, 0.5f, 0.0f);
				model_tree.Draw();
			}
			glPopMatrix();

			if (xPlayer+0.4 > xWeapon && xWeapon > xPlayer -0.4 && (int(i-depth)) == 69) {
				if (!level2flag) {
					depth = 0;
					score = 0;
					break;
				}
				else {
					enemyZ += 1;
					if (enemyZ == 3) {
						level2flag = false;
						max_score = 200;
						depth = 0;
						score = 0;
						enemyZ = 0;
						break;


					}
				}
			}
			else if (int(i - depth) == 69) {
				if (!level2flag) {
					score += 1;
				}
				else {
					score += 10;
				}
			}
			if (score == max_score) {
				if(level2flag == true)
					exit(EXIT_SUCCESS);


				level2flag = !level2flag;
				score = 0;
				max_score = 400;
			}

			switch (xWeapon) {
			case -3: xWeapon = 0;break;
			case 0: xWeapon = 3;break;
			case 3: xWeapon = -3;break;
			}

		}
	}
	glEnable(GL_LIGHTING);
	// Display Score
	char* strScore[20];
	glPushMatrix();
	glColor3f(0, 0, 0);	// Dim the ground texture a bit
	sprintf((char *)strScore, "Score = %d/%d", score, max_score);
	print(20, 5, (char *)strScore);
	glPopMatrix();



	glPushMatrix();
	glTranslated(0, 5, 65+zqube);
	if(!hit)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 1, 0);
	
	glutSolidCube(1);
	glPopMatrix();

	glutSwapBuffers();

	
}

void myKeyboard(unsigned char button, int x, int y)
{
	if (button == 'a' && xPlayer > -2.8) {
		xPlayer -= 0.2;
	}
	if (button == 'd' && xPlayer < 2.8) {
		xPlayer += 0.2;
	}
	if (button == 'w')
		Eye.y += 1;
	if (button == 's')
		Eye.y -= 1;
	if (button == 'f') {
		Eye.y = 5;
		Eye.z = 70;
		Eye.x = xPlayer;
	}
	if (button == 'g') {
		Eye.y = 12;
		Eye.z = 90;
		Eye.x = 0;
	}

	glutPostRedisplay();

}


void myMotion(int x, int y)
{	
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
		Eye.y += -0.1;
	else
		Eye.y += 0.1;

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	
	glutPostRedisplay();	//Re-draw scene 
}


void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
		if(x > 700 && xPlayer < 2.8)
			xPlayer += 0.2;
		if (x < 600 && xPlayer > -2.8) 
			xPlayer -= 0.2;
	} 
}

void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

void Anim() {
	//if (l) {
	//	light -= 0.01;
	//	

	//}
	//else {
	//	light += 0.01;
	//}
	//printf("%f/d\n", light);
	//if (light >= 1) {
	//	l = true;
	//}
	//else {
	//	if (light <= 0) {
	//		l = false;
	//	}
	//}
	zqube += 0.1;
	if (zqube >= 5)
		hit = true;
	zPlayer -= 0.01;
	if (!level2flag) {
		depth -= 0.1;
	}
	else {
		depth -= 1.5;
	}
	if (!level2flag) {
		if (lightxf) {
			lightx -= 0.01;

		}
		else {
			lightx += 0.01;
			/*printf("%f\n", lightx);*/
		}
}
	else {
		if (lightxf) {
			lightx -= 0.15;

		}
		else {
			lightx += 0.15;
			printf("%f\n", lightx);
		}

	}
	if (lightx >= 3) {
		lightxf = true;
	}
	else {
		if (lightx <= -3) {
			lightxf = false;
		}
	}
	zWeapon -= 2;
	Eye.x = xPlayer;
	if (scaleflag) {
		
		scalef += 0.01;
	}
	else {
		scalef -= 0.01;
	}
	if (scalef >= 2) {
		scaleflag = false;
	}
	else {
		if (scalef <= 0) {
			scaleflag = true;
	} }
	glutPostRedisplay();
	
	

	
	

	


}

void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	player.Load("Models/human man/human_man_1.2.3ds");
	weapon.Load("Models/weapon/Artorias_Sword.3ds");
	model_car.Load("Models/car/ausfb.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_wood.Load("Textures/wood.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&texMarple, "Textures/marple.bmp", true);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(50, 50);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutIdleFunc(Anim);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutTimerFunc(0, lightAnim, 0);

	glutReshapeFunc(myReshape);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}