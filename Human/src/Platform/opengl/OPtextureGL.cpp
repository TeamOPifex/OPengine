#include "./Human/include/Platform/opengl/OPtextureGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

void OPtextureGLBind(void* ptr, ui32 slot) {
	OPtextureGL* texture = (OPtextureGL*)ptr;
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	OPGLFN(glBindTexture(GL_TEXTURE_2D, texture->Handle));
}

void OPtextureGLUnbind(void* ptr, ui32 slot) {
	OPGLFN(glActiveTexture(GL_TEXTURE0 + slot));
	OPGLFN(glBindTexture(GL_TEXTURE_2D, 0));
}

void OPtextureGLInit(OPtexture* texture) {
	texture->Bind = OPtextureGLBind;
	texture->Unbind = OPtextureGLUnbind;
}