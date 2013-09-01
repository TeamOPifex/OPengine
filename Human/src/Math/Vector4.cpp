#include "./Human/include/Math/Vector4.h"
#include "./Core/include/MathHelpers.h"

Vector4* Vector4::normalize(){
		OPfloat length = sqrt((x * x) + (y * y) + (z * z) + (w * w));
		x /= length;
		y /= length;
		z /= length;
		w /= length;
		return this;
	}

Vector4& Vector4::normalize(Vector4& source){
	OPfloat length = sqrt((source.x * source.x) + (source.y * source.y) + (source.z * source.z) + (source.w * source.w));
	source.x /= length;
	source.y /= length;
	source.z /= length;
	source.w /= length;
	return source;
}

OPfloat& Vector4::operator[](int idx){
	switch(idx){
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		return w;
		break;
	}
}


Vector4* Vector4::operator*=(const Vector4 &rhs) {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return this;
}
