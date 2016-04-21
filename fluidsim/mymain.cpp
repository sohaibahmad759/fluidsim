#include "fluid.h"
#include <iostream>

static int year = 0;
static int day = 0;
GLUquadricObj* quad;
static pSystem particles;		// initialized in main()

float cam_xrot=0, cam_yrot=0, cam_zrot=0;

void initLight(void)
{
   GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
   GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.8, 1.0 };
   GLfloat local_view[] = { 0.0 };

   glClearColor(0.0, 0.1, 0.1, 0.0);
   //glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

void init(void)
{
	initLight();
	quad = gluNewQuadric();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glColor3f(0.3, 0.3, 0.3);
	GLfloat color[] = {0.2, 0.3, 0.1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslatef(1.0, 2.5, 1.5);
	glScalef(2.0, 5.0, 2.0);
	glutSolidCube(1.0);
	glPopMatrix();

	particles.updateSystem();
	vector<Particle*>::const_iterator traverseIter = particles.getBegin();
	vector<Particle*>::const_iterator endIter = particles.getEnd();
	
	while (traverseIter != endIter) {
		double* color = (*traverseIter)->getRGB();
		//glColor3f(color[0], color[1], color[2]);
		GLfloat colorMatrix[3];
		colorMatrix[0] = color[0];
		colorMatrix[1] = color[1];
		colorMatrix[2] = color[2];
		glMaterialfv(GL_FRONT, GL_AMBIENT, colorMatrix);
		glPushMatrix();
			glTranslatef((GLfloat)(*traverseIter)->getX(), (GLfloat)(*traverseIter)->getY(), (GLfloat)(*traverseIter)->getZ());
			gluSphere(quad, (*traverseIter)->getRadius(), 32, 32);
		glPopMatrix();

		traverseIter++;
	}
	glutSwapBuffers();
}

void camera(){
   glLoadIdentity();
   glRotatef(cam_xrot,1,0,0);
   glRotatef(cam_yrot,0,1,0);
   glRotatef(cam_zrot,0,0,1);
   glTranslatef(-1, -0.5, -0.5);
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	gluLookAt(0.0, 0.0, 3.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();
}


void timer(int i)
{
	year = (year + 2) % 360;
	day = (day + 10) % 360;
	glutPostRedisplay();
	glutTimerFunc(50, timer, 1);
}

int main(int argc, char** argv)
{
	// Initializing the particle system	
	// height is defined here (in the range 0.0-3.0)
	double height = -1.0;

	while (!(height >= 0 && height <= 3.0)) {
		cout << "Please specify height of the fluid to drop from (in the range 0.0 to 3.0):" << endl;
		cin >> height;
	}

	int surfaceT = -1;
	while (surfaceT != 0 && surfaceT != 1 && surfaceT != 2) {
		cout << "Please specify intermolecular force constant for surface tension\n(0 for free flow, 1 for water-like flow, 2 for high surface tension - drop formation):" << endl;
		cin >> surfaceT;
	}

	for (double i=0; i<1.2; i=i+0.2) {
		for (double j=0+height; j<1.2+height; j=j+0.2) {
			for (double k=0; k<1.2; k=k+0.2) {
				Particle* myParticle = new Particle(i,j,k,1,surfaceT);
				particles.addParticle(myParticle);
			}
		}
	}
	
	/*Particle* myParticle = new Particle;
	particles.addParticle(myParticle);
	for (int i=0; i<10; i++) {
		Particle* myParticle = new Particle(0-(i/100.0*1), 0.1, 0+(i/100.0*0.5));
		particles.addParticle(myParticle);
	}
	for (int i=0; i<10; i++) {
		Particle* myParticle = new Particle(0+(i/100.0*0.5), 0, 0+(i/100.0*1));
		particles.addParticle(myParticle);
	}*/

	// OpenGL stuff
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Fluid Simulation");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(50, timer, year);
	glutMainLoop();
	return 0;
}