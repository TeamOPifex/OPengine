#ifndef OPENGINE_PIPELINE_TEXTURE2D
#define OPENGINE_PIPELINE_TEXTURE2D

#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"

struct OPtexture2DOLD {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPtextureOLD* Texture;
	OPeffect* Effect;
};

OPtexture2DOLD* OPtexture2DCreate(OPtextureOLD* texture, OPeffect* effect, OPvec2 uvStart, OPvec2 uvEnd);
OPtexture2DOLD* OPtexture2DCreate(OPtextureOLD* texture, OPeffect* effect);
void OPtexture2DDestroy(OPtexture2DOLD* character);
void OPtexture2DPrepRender(OPtexture2DOLD* tex2d);
void OPtexture2DRender(OPtexture2DOLD* tex2d);
void OPtexture2DUnloadGlobals();
inline OPtexture2DOLD* OPtexture2DCreate(OPtextureOLD* texture) {
	return OPtexture2DCreate(texture, NULL);
}

#endif
