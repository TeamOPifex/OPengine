
#include "./Human/include/Platform/opengles/OPshaderAttributeAPIGLES.h"

#ifdef OPIFEX_OPENGL_ES_2

#include "./Human/include/Platform/opengles/OPeffectAPIGLES.h"
#include "./Human/include/Platform/opengles/OPcommonGLES.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/OPmemory.h"

void OPshaderAttributeGLESSetOffset(OPshaderAttribute* shaderAttribute, OPeffect* effect) {
	OPeffectGLES* effectGL = (OPeffectGLES*)effect->internalPtr;
	OPGLFN(shaderAttribute->Location = glGetAttribLocation(effectGL->Handle, shaderAttribute->Name));
}

void OPshaderAttributeAPIGLESInit(OPshaderAttributeAPI* shaderAttribute) {
	shaderAttribute->SetOffset = OPshaderAttributeGLESSetOffset;
}

#endif