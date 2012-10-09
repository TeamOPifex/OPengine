#include "external\Phsyx.3.2.1\Include\foundation\PxMat44.h"
#include "Core\include\Types.h"

using namespace physx;

class MathHelper {
public:
	MathHelper(){	
		_mat44 = new PxMat44();
	}
	PxMat44 Identity(){
		return _mat44->createIdentity();
	}
	void Translate(f32 x, f32 y, f32 z){
		_mat44->setPosition(PxVec3(x, y, z));
	}
private:
	PxMat44* _mat44;
};
