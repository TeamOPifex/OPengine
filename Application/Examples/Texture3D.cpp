#include "./ExampleSelectorState.h"

#include "./Pipeline/include/OPtexture3D.h"

typedef struct {
	OPtexture3D* tex;
    OPcam camera;

    void Init() {
        tex = OPtexture3D::Create((OPtexture*)OPCMAN.LoadGet("TetrisYellow.png"));
        camera.SetPerspective(OPvec3(0,5,5), OPVEC3_ZERO);
        tex->rotation.x = OPpi_2;
    }

    OPint Update(OPtimer* timer) {
        tex->rotation.y += timer->Elapsed * 0.001f;
        return 0;
    }

    void Render(OPfloat delta) {
        OPrenderClear(0,0,0);

        // Draw the OPtexture3D
        tex->Render(&camera);

        OPrenderPresent();
    }

    OPint Exit() {
        return 0;
    }

} Texture3DExample;

Texture3DExample texture3DExample;



void ExampleTexture3DEnter(OPgameState* last) {
    texture3DExample.Init();
}
OPint ExampleTexture3DUpdate(OPtimer* time) {
    return texture3DExample.Update(time);
}
void ExampleTexture3DRender(OPfloat delta) {
    texture3DExample.Render(delta);
}
OPint ExampleTexture3DExit(OPgameState* next) {
    return texture3DExample.Exit();
}
OPint GS_EXAMPLE_TEXTURE3D_AVAILABLE = 1;
OPgameState GS_EXAMPLE_TEXTURE3D = {
	ExampleTexture3DEnter,
	ExampleTexture3DUpdate,
	ExampleTexture3DRender,
	ExampleTexture3DExit
};
