#define _CRT_SECURE_NO_WARNINGS
#include "SPlanet.h"
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>

SPlanet::SPlanet()
{
	this->_material = { };
	this->_position = Vector3(0, 0, 0);
	this->_size = 0;
	this->_velocity = 0;
	this->_rotateAngle = 0;
	this->_rotateAxis = 0;
	this->_revolutionAxis = 0;
	this->_circleInner = 0;
	this->_circleOuter = 0;
	this->_disk = gluNewQuadric();
	this->_texQuad = gluNewQuadric();
	this->_texture = -1;
	this->_ringTexture = -1;
	this->Initialize();
}


SPlanet::~SPlanet()
{
	Release();
}

void SPlanet::Initialize() {
}

void SPlanet::Release() {
	for (std::vector<SPlanet*>::iterator it = this->_satellites.begin(); it!= this->_satellites.end(); it++)
	{
		delete (*it);
	}
	gluDeleteQuadric(this->_disk);
	gluDeleteQuadric(this->_texQuad);
	if(this->_texture != -1) glDeleteTextures(1, &this->_texture);
	if(this->_ringTexture != -1) glDeleteTextures(1, &this->_ringTexture);
	this->_satellites.clear();
}

void SPlanet::Update() {
	for (int i = 0; i < this->_satellites.size(); i++) {
		this->_satellites[i]->Update();
	}

	this->_rotateAngle += (this->_velocity * 0.5);
}

void SPlanet::Render() {
	float ambient[4] = { 0, };
	float diffuse[4] = { 0, };
	float specular[4] = { 0, };

	memcpy(ambient, &_material.ambient, sizeof(float) * 4);
	memcpy(diffuse, &_material.diffuse, sizeof(float) * 4);
	memcpy(specular, &_material.specular, sizeof(float) * 4);

	glPushMatrix();

	glRotatef(this->_revolutionAxis, 0, 0, 1);
	glRotatef(this->_rotateAngle * 0.1f, 0, 1, 0);
	glTranslatef(this->_position.x, this->_position.y, this->_position.z);
	for (int i = 0; i < this->_satellites.size(); i++) {
		this->_satellites[i]->Render();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();
	glTranslatef(this->_rotateAngle / 100, 0, 0);
	glRotatef(this->_rotateAxis, 0, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);

	glRotatef(this->_rotateAxis, 0, 0, 1);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, _material.shine);
	glBindTexture(GL_TEXTURE_2D, this->_texture);
	glutSolidSphere(this->_size, 36, 72);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	if (this->_ringTexture != -1) {
		gluQuadricTexture(this->_disk, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, this->_ringTexture);
		gluDisk(this->_disk, this->_circleInner, this->_circleOuter, 100, 100);
	}
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void SPlanet::SetPosition(Vector3 & pos) {
	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;
}

void SPlanet::SetMaterial(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shine) {
	this->_material.ambient = ambient;
	this->_material.diffuse = diffuse;
	this->_material.specular = specular;
	this->_material.shine = shine;
}

void SPlanet::SetSize(float size) {
	this->_size = size;
}

void SPlanet::SetVelocity(float velocity) {
	this->_velocity = velocity;
}

void SPlanet::SetRotateAxis(float axis) {
	this->_rotateAxis = axis;
}

void SPlanet::SetRevolutionAxis(float axis) {
	this->_revolutionAxis = axis;
}

void SPlanet::SetCircle(float inner, float outer) {
	this->_circleInner = inner;
	this->_circleOuter = outer;
}

void SPlanet::AddSatellite(SPlanet* satellite) {
	this->_satellites.push_back(satellite);
}

void SPlanet::TextureLoad(const char* filename, int mode) {
	if (mode == 0) {
		glGenTextures(1, &this->_texture);
		FILE * fp = fopen(filename, "rb");
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;

		fread(&fileHeader, 1, sizeof(BITMAPFILEHEADER), fp);

		if (fileHeader.bfType != 0x4D42 || fileHeader.bfReserved1 != 0 || fileHeader.bfReserved2 != 0) {
			glDeleteTextures(1, &this->_texture);
		}
		
		fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), fp);

		if (infoHeader.biPlanes != 1) {
			glDeleteTextures(1, &this->_texture);
		}

		fseek(fp, fileHeader.bfOffBits, SEEK_SET);

		GLubyte * pixels = new GLubyte[infoHeader.biWidth * infoHeader.biHeight * 4];
		fread(pixels, 1, infoHeader.biWidth * infoHeader.biHeight * 4, fp);
		fclose(fp);

		if (this->_texture != -1) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glBindTexture(GL_TEXTURE_2D, this->_texture);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.biWidth, infoHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

			GLfloat plane_coef_s[] = { 1, 0, 0, 1 };
			GLfloat plane_coef_t[] = { 0, 1, 0, 1 };

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenfv(GL_S, GL_OBJECT_PLANE, plane_coef_s);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, plane_coef_t);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		delete[] pixels;
	}
	else {
		glGenTextures(1, &this->_ringTexture);
		FILE * fp = fopen(filename, "rb");
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;

		fread(&fileHeader, 1, sizeof(BITMAPFILEHEADER), fp);

		if (fileHeader.bfType != 0x4D42 || fileHeader.bfReserved1 != 0 || fileHeader.bfReserved2 != 0) {
			glDeleteTextures(1, &this->_ringTexture);
		}

		fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), fp);

		if (infoHeader.biPlanes != 1) {
			glDeleteTextures(1, &this->_ringTexture);
		}

		fseek(fp, fileHeader.bfOffBits, SEEK_SET);

		GLubyte * pixels = new GLubyte[infoHeader.biWidth * infoHeader.biHeight * 4];
		fread(pixels, 1, infoHeader.biWidth * infoHeader.biHeight * 4, fp);
		fclose(fp);

		if (this->_texture != -1) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glBindTexture(GL_TEXTURE_2D, this->_ringTexture);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.biWidth, infoHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

			GLfloat plane_coef_s[] = { 1, 0, 0, 1 };
			GLfloat plane_coef_t[] = { 0, 1, 0, 1 };

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenfv(GL_S, GL_OBJECT_PLANE, plane_coef_s);
			glTexGenfv(GL_T, GL_OBJECT_PLANE, plane_coef_t);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		delete[] pixels;
	}
}