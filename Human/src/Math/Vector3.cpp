#include "./Human/include/Math/Vector3.h"
#include "./Core/include/MathHelpers.h"

float Vector3::Length(){
	return sqrt(x * x + y * y + z * z);
}

Vector3* Vector3::Normalize(){
	OPfloat length = Length();
	x /= length;
	y /= length;
	z /= length;
	return this;
}

Vector3* Vector3::Cross(Vector3 right){
	x = y * right.z - z * right.y;
	y = z * right.x - x * right.z;
	z = x * right.y - y * right.x;
	return this;
}

Vector3* Vector3::Subtract(Vector3 right){
	x = x - right.x;
	y = y - right.y;
	z = z - right.z;
	return this;
}

Vector3* Vector3::Add(Vector3 right){
	x = x + right.x;
	y = y + right.y;
	z = z + right.z;
	return this;
}

Vector3* Vector3::Multiply(OPfloat scaler)
{
	x = x * scaler;
	y = y * scaler;
	z = z * scaler;

	return this;
}


Vector3 Vector3::Normalize(Vector3 left){
	OPfloat length = left.Length();
	return Vector3(left.x / length, left.y / length, left.z / length);
}

Vector3 Vector3::Cross(Vector3 left, Vector3 right){
	return Vector3(
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x
		);
}

Vector3 Vector3::Subtract(Vector3 left, Vector3 right){
	return Vector3(
		left.x - right.x,
		left.y - right.y,
		left.z - right.z
		);
}

Vector3 Vector3::Add(Vector3 left, Vector3 right){
	return Vector3(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z
		);
}

Vector3 Vector3::Multiply(Vector3 left, OPfloat scaler)
{
	return Vector3(
		left.x * scaler,
		left.y * scaler,
		left.z * scaler
		);
}

Vector3 Vector3::Multiply(Vector3 left, Vector3 right)
{
	return Vector3(
		left.x * right.x,
		left.y * right.y,
		left.z * right.z
		);
}

OPfloat Vector3::Distance(Vector3& left, Vector3& right){
	return Vector3(left.x - right.x, left.y - right.y, left.z - right.z).Length();
}

OPfloat Vector3::Dot(Vector3 left, Vector3 right){
	return left.x * right.x + left.y * right.y + left.z * right.z;
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
		return x;
		break;
	case 1:
		return y;
		break;
	default:
		return z;
		break;
	}
}
