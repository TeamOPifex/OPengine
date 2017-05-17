#pragma once

#include "./Math/include/OPvec3.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"

struct OPtexture3D {
	OPvec3 position;
	OPvec3 rotation;
	OPvec3 scale;
	OPtexture* texture;
	OPeffect* effect;

	void Init(OPtexture* texture);
	void Init(OPtexture* texture, OPeffect* effect);
    void PrepRender(OPcam* camera);
    void Render(OPcam* camera);
    void Destroy();

    static OPtexture3D* Create(OPtexture* texture);
    static OPtexture3D* Create(OPtexture* texture, OPeffect* effect);
    static void Shutdown();

    inline void Free() {
        Destroy();
        OPfree(this);
    }
};
