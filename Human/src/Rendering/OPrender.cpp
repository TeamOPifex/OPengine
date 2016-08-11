#include "./Human/include/Rendering/OPrender.h"

OPrenderAPI* OPRENDERER_ACTIVE = NULL;

#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL 1
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif

#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Platform/opengl/OPrendererGL.h"
#include "./Core/include/Assert.h"


OPint OPrenderSetup() {
#ifdef OPIFEX_OPENGL
	OPRENDERER_ACTIVE = OPrendererGL();
#else
#endif
	return 0;
}

OPint OPrenderSetup(OPrendererType renderer) {
	switch (renderer) {
		case OPrendererType::OPENGL: {
			OPRENDERER_ACTIVE = OPrendererGL();
			break;
		}
	}

    return 0;
}

OPint OPrenderInit(OPwindow* window) {
	return OPRENDERER_ACTIVE->Init(window);
}

OPint OPrenderGetWidth() {
    return OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width;
}

OPint OPrenderGetHeight() {
    return OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height;
}

OPfloat OPrenderGetAspectRatio() {
    ASSERT(OPrenderGetHeight() > 0, "Height was not greater than 0, there was problem getting width and height");
    return OPrenderGetWidth() / (OPfloat)OPrenderGetHeight();
}

OPfloat OPrenderGetWidthAspectRatio() {
    OPfloat aspect = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width;
    return aspect > 1.0f ? 1.0f : aspect;
}

OPfloat OPrenderGetHeightAspectRatio() {
    OPfloat aspect = OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height;
    return aspect > 1.0f ? 1.0f : aspect;
}
