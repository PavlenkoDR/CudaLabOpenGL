#include <free_glut/glut.h>
#include <stdlib.h> 
#include "kernel.h"
#include "ParticleSystem.h"

auto particleSystem = ParticleSystemInstance::getInstanse();

static void resize(int width, int height)
{
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void display(void)
{
	/*while (true)
	{*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(2.5);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);

	auto offsetZ = -60;

	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);

	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);

	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);

	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.first.y, particleSystem->border.third.z + offsetZ);

	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.first.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);

	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.first.z + offsetZ);
	glVertex3f(particleSystem->border.second.x, particleSystem->border.second.y, particleSystem->border.third.z + offsetZ);

	glEnd();

	glColor3d(1, 0, 1);

	for (auto particle : particleSystem->particles)
	{
		glPushMatrix();
		glTranslated(particle.position.x, particle.position.y, particle.position.z + offsetZ);
		glutSolidSphere(particle.radius * 2, 50, 50);
		glPopMatrix();
	}

	glutSwapBuffers();
	//}
}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("CUDA");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	/////////////////////////////////////////////////////

	particleSystem->MoveParticlesByDeltaTimeAsync();

	/////////////////////////////////////////////////////

	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glutMainLoop();

	return EXIT_SUCCESS;
}