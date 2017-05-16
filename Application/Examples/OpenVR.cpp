#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>

#ifdef ADDON_openvr
#include "openvr.h"
#include "OPopenVR.h"
#endif

typedef struct {
#ifdef ADDON_openvr
	OPopenVR openVR;

	GLuint m_glSceneVertBuffer;
	GLuint m_unSceneVAO;
	GLuint m_unLensVAO;


	OPmat4 ProjectionCenter;

	OPmesh* mesh;
	OPeffect* modelEffect;
	OPtexture* texture;
#endif
} OpenVRExample;

OpenVRExample openVRExample;


void ExampleOpenVREnter(OPgameState* last) {

}

OPint ExampleOpenVRUpdate(OPtimer* time) {

	return false;
}

#ifdef ADDON_openvr


void RenderScene(vr::EVREye eye) {

}

void RenderStereoTargets() {



}

#endif

void ExampleOpenVRRender(OPfloat delta) {

}

OPint ExampleOpenVRExit(OPgameState* next) {
	return 0;
}

#ifdef ADDON_openvr
OPint GS_EXAMPLE_OPENVR_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_OPENVR_AVAILABLE = 0;
#endif

// This is the Game State for this IMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState* GS_EXAMPLE_OPENVR = NULL;