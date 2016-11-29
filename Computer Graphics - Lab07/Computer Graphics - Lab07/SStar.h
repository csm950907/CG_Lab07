#pragma once
#include "SMath.h"
typedef struct _light {
	int lightIdx;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector3 attenuation;
}Light;


class SStar
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
	Light _light;
	_material _material;
	float _size;
	bool _enable;

public:
	SStar();
	~SStar();
	
public:
	void Initialize();
	void Release();
	void Update();
	void Render();

public:
	void SetLight(float lightIdx, Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 attenuation);
	void SetMaterial(Vector4 ambient, Vector4 diffuse, Vector4 specular, float shine);
	void SetPosition(Vector3 & pos);
	void SetSize(float size);
	void SetEnable(bool flag);
};

