#include <include/Rendering/OPtexture.h>
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

OPtexture* OPRENDER_CURR_TEX;
ui32 OPTEXTURE_ACTIVE = 0;

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPtexture OPtextureCreate(OPtextureDescription desc){
	OPtexture tex = {
		desc,
		0
	};
	OPglError("OPtextureCreate::Error 0");
	glGenTextures(1, &tex.Handle);
	OPglError("OPtextureCreate::Error 1");
	glBindTexture(GL_TEXTURE_2D, tex.Handle);
	OPglError("OPtextureCreate::Error 2");
	glTexImage2D(GL_TEXTURE_2D, 0, desc.InternalFormat, desc.Width, desc.Height, 0, desc.Format, desc.DataType, NULL);
	if (OPglError("OPtextureCreate::Error 2.5")) {
		OPlog("Desc Used: %d, %d", desc.InternalFormat, desc.Format);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.MagFilter);
	OPglError("OPtextureCreate::Error 3");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.MinFilter);
	OPglError("OPtextureCreate::Error 4");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.WrapX);
	OPglError("OPtextureCreate::Error 5");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.WrapY);
#ifndef OPIFEX_IOS
    // TODO: (garrett) This should still work on iOS
	OPglError("OPtextureCreate::Error 6");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, desc.CompareFunc);
	OPglError("OPtextureCreate::Error 7");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, desc.CompareMode);
	OPglError("OPtextureCreate::Error 8");
#endif

	return tex;
}

//-----------------------------------------------------------------------------
void OPtextureDestroy(OPtexture* tex){
	glDeleteTextures(1, &tex->Handle);
}
//-----------------------------------------------------------------------------
ui32 OPtextureBind(OPtexture* tex){
	ASSERT(OPTEXTURE_ACTIVE < 16, "Exceeded Active Texture Slots");
	OPglError("OPtextureBind::Error 0");
	OPRENDER_CURR_TEX = tex;
	glActiveTexture(GL_TEXTURE0 + OPTEXTURE_ACTIVE);
	if (OPglError("OPtextureBind::Error 1")) {
		OPlog("FAILED to make active texture: %d", tex->Handle);
	}
	glBindTexture(GL_TEXTURE_2D, tex->Handle);
	OPglError("OPtextureBind::Error 2");

	return OPTEXTURE_ACTIVE++;
}

void OPtextureClearActive() {
	OPTEXTURE_ACTIVE = 0;
}

//-----------------------------------------------------------------------------
void OPtextureSetData(void* data){
	OPglError("OPtextureSetData::Error 0");
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		OPRENDER_CURR_TEX->Description.InternalFormat,
		OPRENDER_CURR_TEX->Description.Width,
		OPRENDER_CURR_TEX->Description.Height,
		0,
		OPRENDER_CURR_TEX->Description.Format,
		OPRENDER_CURR_TEX->Description.DataType,
		data
		);
	OPglError("OPtextureSetData::Error 1");
}
//-----------------------------------------------------------------------------
void  OPtexturePixelate(){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
//-----------------------------------------------------------------------------
void OPtextureSmooth(){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

OPvec4 OPtextureReadPixel(OPtexture* tex, OPint x, OPint y) {
	OPvec4 result;

#if !defined(OPIFEX_ANDROID) && !defined(OPIFEX_IOS)
	glPixelStoref(GL_PACK_ALIGNMENT, 1);
#endif

	unsigned char pixel[4] = { 0 };
	glReadPixels((GLint)x, (GLint)y, 1, 1, tex->Description.Format, tex->Description.DataType, &pixel);
	result.x = pixel[0];
	result.y = pixel[1];
	result.z = pixel[2];
	result.w = pixel[3];

	return result;
}

#include "./Data/include/OPcman.h"
void OPtexture::Load(OPchar* asset) {
    OPtexture* result = (OPtexture*)OPcmanLoadGet(asset);
    this->Handle = result->Handle;
    this->Description = result->Description;
}

OPtexture* OPtextureLoad(const OPchar* asset) {
    return (OPtexture*)OPcmanLoadGet(asset);
}
