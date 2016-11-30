#pragma once
#include "SMath.h"
#include <vector>
#include <gl/glut.h>

struct _material;

class SPlanet
{
private:
	typedef struct _material {
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		float shine;
	}Material;

private:
	Vector3 _position;
	_material _material;
	float _size;
	float _velocity;
	float _rotateAngle;
	float _rotateAxis;
	float _revolutionAxis;
	std::vector<SPlanet *> _satellites;
	float _circleInner;
	float _circleOuter;
	GLUquadricObj *_disk;
	GLUquadricObj *_texQuad;
	GLuint _texture;
	GLuint _ringTexture;
private:

public:
	SPlanet();
	~SPlanet();

public:
	void Initialize();
	void Release();
	void Update();
	void Render();

public:
	void SetPosition(Vector3 & pos);
	void SetMaterial(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shine);
	void SetSize(float size);
	void SetVelocity(float velocity);
	void SetRotateAxis(float axis);
	void SetRevolutionAxis(float axis);
	void SetCircle(float inner, float outer);
	void AddSatellite(SPlanet *satellite);
	void TextureLoad(const char* filename, int mode);
};

