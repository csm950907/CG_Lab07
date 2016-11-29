#include "SStar.h"
#include <gl/glut.h>
#include <string.h>


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