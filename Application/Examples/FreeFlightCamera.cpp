#include "./ExampleSelectorState.h"
#include "./OPengine.h"

class FreeFlightExample  : public OPgameState{
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcamFreeFlight Camera;
	ui32 Rotation;
	OPtexture* Texture;


	void Init(OPgameState* last) {
		OPCMAN.Load("PuzzleBlock.opm");
		OPCMAN.Load("Common/Texture.frag");
		OPCMAN.Load("Common/Texture3D.vert");
		OPCMAN.Load("TetrisBroken.png");

		Mesh = (OPmesh*)OPCMAN.Get("PuzzleBlock.opm");
		Texture = (OPtexture*)OPCMAN.Get("TetrisBroken.png");
		Rotation = 0;

		OPshaderAttribute attribs[] = {
			{ "aPosition", OPshaderElementType::FLOAT, 3 },
			{ "aNormal", OPshaderElementType::FLOAT, 3 },
			{ "aUV", OPshaderElementType::FLOAT, 2 }
		};

		Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
		OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
		Effect->Init(vert, frag);

		Camera.Init(3.0f, 3.0f, OPVEC3_ONE);
	}

	OPint Update(OPtimer* timer) {
		Camera.Update(timer);
		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation++; }

		return false;
	}

	void Render(OPfloat delta) {
		OPrenderDepth(1);
		OPrenderClear(0, 0, 0);

		Mesh->Bind();
		Effect->Bind();

		OPmat4 world;
		world = OPmat4RotY(Rotation / 100.0f);


		OPeffectSet("uColorTexture", Texture, 0);

		OPeffectSet("uWorld", &world);
		OPeffectSet("uProj", &Camera.Camera.proj);
		OPeffectSet("uView", &Camera.Camera.view);

		OPvec3 light = OPvec3Create(0, 1, 0);
		OPeffectSet("vLightDirection", &light);

		OPrenderDrawBufferIndexed(0);

		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		Effect->Destroy();
		OPfree(Effect);

		return 0;
	}
};



OPint GS_EXAMPLE_FREEFLIGHT_AVAILABLE = 1;
FreeFlightExample _GS_EXAMPLE_FREEFLIGHT;
OPgameState* GS_EXAMPLE_FREEFLIGHT = &_GS_EXAMPLE_FREEFLIGHT;