#pragma once

typedef struct _vector3 {
public:
	float x;
	float y;
	float z;

public:
	_vector3();
	_vector3(float x, float y, float z);

public:
	_vector3 operator+(_vector3 & v);
	_vector3 operator-(_vector3 & v);
	_vector3 operator*(_vector3 & v);
	_vector3 operator/(_vector3 & v);

public:
	float dotProduct(_vector3 & v);
	_vector3 crossProduct(_vector3 & v);

}Vector3;

typedef struct _vector4 {
public:
	float x;
	float y;
	float z;
	float w;

public:
	_vector4();
	_vector4(float x, float y, float z, float w);
	_vector4(_vector3 & v, float w);

public:
	_vector4 operator+(_vector4 & v);
	_vector4 operator-(_vector4 & v);
	_vector4 operator*(_vector4 & v);
	_vector4 operator/(_vector4 & v);

public:
	float dotProduct(_vector4 & v);

}Vector4;

