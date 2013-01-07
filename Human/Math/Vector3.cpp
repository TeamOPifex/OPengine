#include "Vector3.h"
#include "Core\include\MathHelpers.h"

void Vector3::normalize(){
		OPfloat length = sqrt((_x * _x) + (_y * _y) + (_z * _z));
		_x /= length;
		_y /= length;
		_z /= length;
	}

Vector3& Vector3::normalize(Vector3& source){
	OPfloat length = sqrt((source._x * source._x) + (source._y * source._y) + (source._z * source._z));
	source._x /= length;
	source._y /= length;
	source._z /= length;
	return source;
}

void Vector3::cross(Vector3& rhs){
		Vector3 tmp;
		tmp._x = _y * rhs._z - _z * rhs._y;
		tmp._y = _z * rhs._x - _x * rhs._z;
		tmp._z = _x * rhs._y - _y * rhs._x;
		_x = tmp._x;
		_y = tmp._y;
		_z = tmp._z;
	}

Vector3 Vector3::cross(Vector3& left, Vector3& right){
	Vector3 returnValue;
	returnValue._x = left._y * right._z - left._z * right._y;
	returnValue._y = left._z * right._x - left._x * right._z;
	returnValue._z = left._x * right._y - left._y * right._x;
	return returnValue;
}

Vector3 Vector3::sub(Vector3& left, Vector3& right){
	Vector3 returnValue;
	returnValue._x = left._x - right._x;
	returnValue._y = left._y - right._y;
	returnValue._z = left._z - right._z;
	return returnValue;
}

//Vector3& Vector3::operator-(const Vector3 &rhs) {
//	return Vector3(this->_x - rhs._x, this->_y - rhs._y, this->_z - rhs._z);
//}
//
//Vector3& Vector3::operator+(const Vector3 &rhs) {
//	return Vector3(_x + rhs._x, _y + rhs._y, _z + rhs._z);
//}
		
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