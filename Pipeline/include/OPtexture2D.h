#ifndef OPENGINE_PIPELINE_TEXTURE2D
#define OPENGINE_PIPELINE_TEXTURE2D

#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"

struct OPtexture2D {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPtexture* Texture;
	OPeffect* Effect;
};

OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect, OPvec2 uvStart, OPvec2 uvEnd);
OPtexture2D* OPtexture2DCreate(OPtexture* texture, OPeffect* effect);
void OPtexture2DDestroy(OPtexture2D* character);
void OPtexture2DPrepRender(OPtexture2D* tex2d);
void OPtexture2DRender(OPtexture2D* tex2d);
void OPtexture2DUnloadGlobals();
inline OPtexture2D* OPtexture2DCreate(OPtexture* texture) {
	return OPtexture2DCreate(texture, NULL);
}

#endif
