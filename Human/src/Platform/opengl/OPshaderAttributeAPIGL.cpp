#include "./Human/include/Platform/opengl/OPshaderAttributeAPIGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/OPmemory.h"

void OPshaderAttributeGLSetOffset(OPshaderAttribute* shaderAttribute, OPeffect* effect) {
	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;
	OPGLFN(shaderAttribute->Location = glGetAttribLocation(effectGL->Handle, shaderAttribute->Name));
}

void OPshaderAttributeAPIGLInit(OPshaderAttributeAPI* shaderAttribute) {
	shaderAttribute->SetOffset = OPshaderAttributeGLSetOffset;
}