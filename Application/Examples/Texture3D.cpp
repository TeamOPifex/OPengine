#include "./ExampleSelectorState.h"

#include "./Pipeline/include/OPtexture3D.h"

class Texture3DExample : public OPgameState {
	OPtexture3D* tex;
    OPcam camera;

    void Init(OPgameState* state) {
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

    OPint Exit(OPgameState* state) {
        return 0;
    }

};


OPint GS_EXAMPLE_TEXTURE3D_AVAILABLE = 1;
Texture3DExample _GS_EXAMPLE_TEXTURE3D;
OPgameState* GS_EXAMPLE_TEXTURE3D = &_GS_EXAMPLE_TEXTURE3D;