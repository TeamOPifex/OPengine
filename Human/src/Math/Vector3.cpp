#include "./Human/include/Math/Vector3.h"
#include "./Core/include/MathHelpers.h"

float Vector3::Length(){
	return sqrt(_x * _x + _y * _y + _z * _z);
}

Vector3* Vector3::Normalize(){
	OPfloat length = Length();
	_x /= length;
	_y /= length;
	_z /= length;
	return this;
}

Vector3* Vector3::Cross(Vector3 right){
	_x = _y * right._z - _z * right._y;
	_y = _z * right._x - _x * right._z;
	_z = _x * right._y - _y * right._x;
	return this;
}

Vector3* Vector3::Subtract(Vector3 right){
	_x = _x - right._x;
	_y = _y - right._y;
	_z = _z - right._z;
	return this;
}

Vector3* Vector3::Add(Vector3 right){
	_x = _x + right._x;
	_y = _y + right._y;
	_z = _z + right._z;
	return this;
}

Vector3* Vector3::Multiply(OPfloat scaler)
{
	_x = _x * scaler;
	_y = _y * scaler;
	_z = _z * scaler;

	return this;
}


Vector3 Vector3::Normalize(Vector3 left){
	OPfloat length = left.Length();
	return Vector3(left._x / length, left._y / length, left._z / length);
}

Vector3 Vector3::Cross(Vector3 left, Vector3 right){
	return Vector3(
		left._y * right._z - left._z * right._y,
		left._z * right._x - left._x * right._z,
		left._x * right._y - left._y * right._x
		);
}

Vector3 Vector3::Subtract(Vector3 left, Vector3 right){
	return Vector3(
		left._x - right._x,
		left._y - right._y,
		left._z - right._z
		);
}

Vector3 Vector3::Add(Vector3 left, Vector3 right){
	return Vector3(
		left._x + right._x,
		left._y + right._y,
		left._z + right._z
		);
}

Vector3 Vector3::Multiply(Vector3 left, OPfloat scaler)
{
	return Vector3(
		left._x * scaler,
		left._y * scaler,
		left._z * scaler
		);
}

Vector3 Vector3::Multiply(Vector3 left, Vector3 right)
{
	return Vector3(
		left._x * right._x,
		left._y * right._y,
		left._z * right._z
		);
}

OPfloat Vector3::Distance(Vector3& left, Vector3& right){
	return Vector3(left._x - right._x, left._y - right._y, left._z - right._z).Length();
}

OPfloat Vector3::Dot(Vector3 left, Vector3 right){
	return left._x * right._x + left._y * right._y + left._z * right._z;
}

Vector3* Vector3::operator-=(Vector3 &rhs) {
	Subtract(rhs);
	return this;
}

Vector3* Vector3::operator+=(Vector3 &rhs) {
	Add(rhs);
	return this;
}
		
OPfloat& Vector3::operator[](int idx){		
	switch(idx){
	case 0:
		return _x;
		break;
	case 1:
		return _y;
		break;
	default:
		return _z;
		break;
	}
}
