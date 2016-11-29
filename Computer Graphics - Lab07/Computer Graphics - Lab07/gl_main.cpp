/*
* Computer Graphics - Lab04
* Shcho@cnu.ac.kr
* Form : [CG]Lab#4_201402441_√÷ºº∏Ò
*/

#include <gl/glut.h>
#include <math.h>
#include "SMath.h"
#include "SStar.h"
#include "SPlanet.h"
typedef enum _planet{
	MERCURY,
	VENUS,
	EARTH,
	MARS,
	JUPITER,
	SATURN,
	URANUS,
	NEPTUNE
}Planet;

SStar* star;
SPlanet* planet[8];
bool isKeyDown[256] = { 0, };
float camera_x, camera_y, camera_z;
float center_x, center_y, center_z;

void init();
void display();
void reshape(int w, int h);
void update();
void releaseAllObject();
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("test");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();
	releaseAllObject();
}

void init() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(+1.0, +1.0, +1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	camera_x = 0; camera_y = 0; camera_z = 10;

	star = new SStar();
	star->SetLight(GL_LIGHT0, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	star->SetMaterial(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);

	star->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	star->SetSize(1.5);
	star->SetEnable(true);

	for (int i = 0; i < 8; i++) {
		planet[i] = new SPlanet();
	}

	planet[MERCURY]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[MERCURY]->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
	planet[MERCURY]->SetSize(0.4);
	planet[MERCURY]->SetVelocity(1.0f);
	planet[MERCURY]->TextureLoad("../Texture/2k_mercury.bmp");

	planet[VENUS]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[VENUS]->SetPosition(Vector3(5.0f, 0.0f, 0.0f));
	planet[VENUS]->SetSize(0.5);
	planet[VENUS]->SetVelocity(0.9f);
	planet[VENUS]->TextureLoad("../Texture/2k_venus_surface.bmp");

	planet[EARTH]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[EARTH]->SetPosition(Vector3(7.0f, 0.0f, 0.0f));
	planet[EARTH]->SetSize(0.7);
	planet[EARTH]->SetVelocity(0.8f);
	planet[EARTH]->SetRotateAxis(23.5);
	planet[EARTH]->TextureLoad("../Texture/2k_earth_daymap.bmp");

	planet[MARS]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[MARS]->SetPosition(Vector3(9.0f, 0.0f, 0.0f));
	planet[MARS]->SetSize(0.6);
	planet[MARS]->SetVelocity(0.7f);
	planet[MARS]->TextureLoad("../Texture/2k_mars.bmp");

	planet[JUPITER]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[JUPITER]->SetPosition(Vector3(12.0f, 0.0f, 0.0f));
	planet[JUPITER]->SetSize(1.4);
	planet[JUPITER]->SetVelocity(0.6f);
	planet[JUPITER]->TextureLoad("../Texture/2k_jupiter.bmp");

	planet[SATURN]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[SATURN]->SetPosition(Vector3(16.0f, 0.0f, 0.0f));
	planet[SATURN]->SetSize(1.2);
	planet[SATURN]->SetVelocity(0.5f);
	planet[SATURN]->SetCircle(1.4f, 2.0f);
	planet[SATURN]->SetRotateAxis(30);
	planet[SATURN]->TextureLoad("../Texture/2k_saturn.bmp");

	planet[URANUS]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[URANUS]->SetPosition(Vector3(19.0f, 0.0f, 0.0f));
	planet[URANUS]->SetSize(1.0);
	planet[URANUS]->SetVelocity(0.4f);
	planet[URANUS]->TextureLoad("../Texture/2k_uranus.bmp");

	planet[NEPTUNE]->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	planet[NEPTUNE]->SetPosition(Vector3(22.0f, 0.0f, 0.0f));
	planet[NEPTUNE]->SetSize(0.8);
	planet[NEPTUNE]->SetVelocity(0.3f);
	planet[NEPTUNE]->TextureLoad("../Texture/2k_neptune.bmp");

	SPlanet* moon = new SPlanet();
	moon->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	moon->SetPosition(Vector3(1.5f, 0.0f, 0.0f));
	moon->SetSize(0.2);
	moon->SetVelocity(2.4f);
	planet[EARTH]->AddSatellite(moon);

	SPlanet* europa = new SPlanet();
	europa->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	europa->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	europa->SetSize(0.4);
	europa->SetVelocity(3.3f);
	europa->SetRevolutionAxis(-20.0f);
	planet[JUPITER]->AddSatellite(europa);

	SPlanet* ganimede = new SPlanet();
	ganimede->SetMaterial(Vector4(0.1f, 0.1f, 0.1f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1);
	ganimede->SetPosition(Vector3(2.5f, 0.0f, 0.0f));
	ganimede->SetSize(0.3f);
	ganimede->SetVelocity(4.0f);
	ganimede->SetRevolutionAxis(20.0f);
	planet[JUPITER]->AddSatellite(ganimede);
}	

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z, center_x, center_y, center_z, 0, 1, 0);

	star->Render();

	for (int i = 0; i < 8; i++) {
		planet[i]->Render();
	}

	//planet[2]->Render();
	glFlush();
}

void reshape(int w, int h) {
	float ratio = w / (float)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 10, 100);
}

void update() {
	star->Update();

	for (int i = 0; i < 8; i++) {
		planet[i]->Update();
	}
	
	if (isKeyDown['w']) camera_z -= 0.1f, center_z -= 0.1f;
	if (isKeyDown['s']) camera_z += 0.1f, center_z += 0.1f;
	if (isKeyDown['a']) camera_x -= 0.1f, center_x -= 0.1f;
	if (isKeyDown['d']) camera_x += 0.1f, center_x += 0.1f;
	if (isKeyDown['r']) camera_y += 0.1f, center_y += 0.1f;
	if (isKeyDown['f']) camera_y -= 0.1f, center_y -= 0.1f;

	printf("Current LookAt : %f, %f, %f\n", camera_x, camera_y, camera_z);
	glutPostRedisplay();
}

void releaseAllObject() {
	star->Release();

	for (int i = 0; i < 8; i++) {
		planet[i]->Release();
		delete planet[i];
	}

	delete star;
}

void keyDown(unsigned char key, int x, int y) {
	isKeyDown[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	isKeyDown[key] = false;
}