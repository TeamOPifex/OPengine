#include "./ExampleSelectorState.h"
#include "./OPengine.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcamFreeFlight Camera;
	ui32 Rotation;
	OPtexture* Texture;
} FreeFlightExample;

FreeFlightExample freeFlightExample;

void ExampleFreeFlightEnter(OPgameState* last) {
	OPCMAN.Load("PuzzleBlock.opm");
	OPCMAN.Load("Common/Texture.frag");
	OPCMAN.Load("Common/Texture3D.vert");
	OPCMAN.Load("TetrisBroken.png");

	freeFlightExample.Mesh = (OPmesh*)OPCMAN.Get("PuzzleBlock.opm");
	freeFlightExample.Texture = (OPtexture*)OPCMAN.Get("TetrisBroken.png");
	freeFlightExample.Rotation = 0;

	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	freeFlightExample.Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
	freeFlightExample.Effect->Init(vert, frag);

	OPcamFreeFlightInit(&freeFlightExample.Camera, 3.0f, 3.0f, OPVEC3_ONE);
}

OPint ExampleFreeFlightUpdate(OPtimer* timer) {
	OPcamFreeFlightUpdate(&freeFlightExample.Camera, timer);
	if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { freeFlightExample.Rotation++; }

	return false;
}

void ExampleFreeFlightRender(OPfloat delta) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	freeFlightExample.Mesh->Bind();
	freeFlightExample.Effect->Bind();

	OPmat4 world;
	world = OPmat4RotY(freeFlightExample.Rotation / 100.0f);


	OPeffectSet("uColorTexture", freeFlightExample.Texture, 0);

	OPeffectSet("uWorld", &world);
	OPeffectSet("uProj", &freeFlightExample.Camera.Camera.proj);
	OPeffectSet("uView", &freeFlightExample.Camera.Camera.view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectSet("vLightDirection", &light);

	OPmeshRender();

	OPrenderPresent();
}

OPint ExampleFreeFlightExit(OPgameState* next) {
	freeFlightExample.Effect->Destroy();
	OPfree(freeFlightExample.Effect);

	return 0;
}

OPint GS_EXAMPLE_FREEFLIGHT_AVAILABLE = 1;
OPgameState GS_EXAMPLE_FREEFLIGHT = {
	ExampleFreeFlightEnter,
	ExampleFreeFlightUpdate,
	ExampleFreeFlightRender,
	ExampleFreeFlightExit
};
