#include "./Human/include/Math/Matrix3.h"


void Matrix3::SetIdentity(){
	cols[0] = Vector3(1,0,0);
	cols[1] = Vector3(0,1,0);
	cols[2] = Vector3(0,0,1);
}