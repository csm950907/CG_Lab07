#define _CRT_SECURE_NO_WARNINGS
#include "SStar.h"
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>

SStar::SStar(){
	this->_light = { 0, };
	this->_position = Vector3(0, 0, 0);
	this->_size = 0;
	this->_enable = false;
}


SStar::~SStar(){
	Release();
}

void SStar::Initialize() {

}

void SStar::Release() {
	if (this->_texture != -1) glDeleteTextures(1, &this->_texture);
}

void SStar::Update() {
}

void SStar::Render() {
	float ambient[4] = { 0, };
	float diffuse[4] = { 0, };
	float specular[4] = { 0, };
	float position[3] = { 0, };

	float m_ambient[4] = { 0, };
	float m_diffuse[4] = { 0, };
	float m_specular[4] = { 0, };

	memcpy(m_ambient, &_material.ambient, sizeof(float) * 4);
	memcpy(m_diffuse, &_material.diffuse, sizeof(float) * 4);
	memcpy(m_specular, &_material.specular, sizeof(float) * 4);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, _material.shine);

	if (this->_enable && _light.lightIdx) {
		glEnable(_light.lightIdx);
		memcpy(ambient, &_light.ambient, sizeof(float) * 4);
		memcpy(diffuse, &_light.diffuse, sizeof(float) * 4);
		memcpy(specular, &_light.specular, sizeof(float) * 4);
		memcpy(position, &_position, sizeof(float) * 3);
		glLightfv(_light.lightIdx, GL_AMBIENT, ambient);
		glLightfv(_light.lightIdx, GL_DIFFUSE, diffuse);
		glLightfv(_light.lightIdx, GL_SPECULAR, specular);
		glLightfv(_light.lightIdx, GL_POSITION, position);

		glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glBindTexture(GL_TEXTURE_2D, this->_texture);
		glutSolidSphere(this->_size, 100, 100);
		glPopMatrix();
	}
}

void SStar::SetLight(float lightIdx, Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 attenuation) {
	this->_light.lightIdx = lightIdx;
	this->_light.ambient = ambient;
	this->_light.diffuse = diffuse;
	this->_light.specular = specular;
	this->_light.attenuation = attenuation;
}

void SStar::SetPosition(Vector3 & pos) {
	this->_position.x = pos.x;
	this->_position.y = pos.y;
	this->_position.z = pos.z;
}

void SStar::SetSize(float size) {
	this->_size = size;
}

void SStar::SetEnable(bool flag) {
	this->_enable = flag;
}

void SStar::SetMaterial(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shine) {
	this->_material.ambient = ambient;
	this->_material.diffuse = diffuse;
	this->_material.specular = specular;
	this->_material.shine = shine;
}

void SStar::TextureLoad(const char* filename) {
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

	GLubyte * pixels = new GLubyte[2048 * 1024 * 4];
	fread(pixels, 1, 2048 * 1024 * 4, fp);
	fclose(fp);

	if (this->_texture != -1) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBindTexture(GL_TEXTURE_2D, this->_texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1024, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

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
}