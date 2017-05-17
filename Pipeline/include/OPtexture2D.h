#pragma once

#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"

struct OPtexture2DOLD {
	OPvec2 Position;
	OPfloat Rotation;
	OPvec2 Scale;
	OPvec4 UVScale;
	OPtexture* Texture;
	OPeffect* Effect;

	void Destroy();
	inline void Free() {
		Destroy();
		OPfree(this);
	}
};

OPtexture2DOLD* OPtexture2DCreate(OPtexture* texture, OPeffect* effect, OPvec2 uvStart, OPvec2 uvEnd);
OPtexture2DOLD* OPtexture2DCreate(OPtexture* texture, OPeffect* effect);
void OPtexture2DDestroy(OPtexture2DOLD* character);
void OPtexture2DPrepRender(OPtexture2DOLD* tex2d);
void OPtexture2DRender(OPtexture2DOLD* tex2d);
void OPtexture2DUnloadGlobals();
inline OPtexture2DOLD* OPtexture2DCreate(OPtexture* texture) {
	return OPtexture2DCreate(texture, NULL);
}