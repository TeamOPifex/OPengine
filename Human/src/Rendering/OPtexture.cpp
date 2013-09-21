#include "./Human/include/Rendering/OPtexture.h"

OPtexture* OPRENDER_CURR_TEX;

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

	glGenTextures(1, &tex.Handle);
	glBindTexture(GL_TEXTURE_2D, tex.Handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.MagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.MinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.WrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.WrapY);

	return tex;
}

//-----------------------------------------------------------------------------
void OPtextureDestroy(OPtexture* tex){
	glDeleteTextures(1, &tex->Handle);
}
//-----------------------------------------------------------------------------
void OPtextureBind(OPtexture* tex, OPint slot){
	OPRENDER_CURR_TEX = tex;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex->Handle);
}
//-----------------------------------------------------------------------------
void OPtextureSetData(void* data){
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		OPRENDER_CURR_TEX->Description.Format,
		OPRENDER_CURR_TEX->Description.Width,
		OPRENDER_CURR_TEX->Description.Height,
		0,
		GL_RGBA,
		OPRENDER_CURR_TEX->Description.DataType,
		data
	);
}
