#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Utilities/Errors.h"

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
	CheckError("OPtextureCreate::Error 0");
	glGenTextures(1, &tex.Handle);
	CheckError("OPtextureCreate::Error 1");
	glBindTexture(GL_TEXTURE_2D, tex.Handle);
	CheckError("OPtextureCreate::Error 2");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.MagFilter);
	CheckError("OPtextureCreate::Error 3");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.MinFilter);
	CheckError("OPtextureCreate::Error 4");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.WrapX);
	CheckError("OPtextureCreate::Error 5");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.WrapY);
	CheckError("OPtextureCreate::Error 6");
#ifndef OPIFEX_ANDROID
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#else
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_HINT, GL_TRUE);
#endif
	CheckError("OPtextureCreate::Error 7");

	return tex;
}

//-----------------------------------------------------------------------------
void OPtextureDestroy(OPtexture* tex){
	glDeleteTextures(1, &tex->Handle);
}
//-----------------------------------------------------------------------------
void OPtextureBind(OPtexture* tex){
	OPRENDER_CURR_TEX = tex;
	glActiveTexture(GL_TEXTURE0 + tex->Handle);
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
		OPRENDER_CURR_TEX->Description.Format,
		OPRENDER_CURR_TEX->Description.DataType,
		data
	);
}
