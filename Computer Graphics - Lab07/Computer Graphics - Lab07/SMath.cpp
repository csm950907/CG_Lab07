#include "SMath.h"
#include <cmath>

_vector3::_vector3(){ }
_vector3::_vector3(float x, float y, float z) : x(x), y(y), z(z) {}

_vector3 _vector3::operator+(_vector3 & v) { return _vector3(x + v.x, y + v.y, z + v.z); }
_vector3 _vector3::operator-(_vector3 & v) { return _vector3(x - v.x, y - v.y, z - v.z); }
_vector3 _vector3::operator*(_vector3 & v) { return _vector3(x * v.x, y * v.y, z * v.z); }
_vector3 _vector3::operator/(_vector3 & v) { return _vector3(x / v.x, y / v.y, z / v.z); }

float _vector3::dotProduct(_vector3 & v) { return (x * v.x + y * v.y + z * v.z); }
_vector3 _vector3::crossProduct(_vector3 & v) { return _vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

_vector4::_vector4() { }
_vector4::_vector4(_vector3 & v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
_vector4::_vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

_vector4 _vector4::operator+(_vector4 & v) { return _vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
_vector4 _vector4::operator-(_vector4 & v) { return _vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
_vector4 _vector4::operator*(_vector4 & v) { return _vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
_vector4 _vector4::operator/(_vector4 & v) { return _vector4(x / v.x, y / v.y, z / v.z, w / v.w); }

float _vector4::dotProduct(_vector4 & v) { return x * v.x + y * v.y + z * v.z + w * v.w; }
