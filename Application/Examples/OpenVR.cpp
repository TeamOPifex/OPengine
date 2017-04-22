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


	GLuint lensVAO;
	GLuint lensProgramID;

	OPeffect* lensEffect;
	OPmat4 ProjectionCenter;

	OPmesh* mesh;
	OPeffect* modelEffect;
	OPtexture* texture;
#endif
} OpenVRExample;

OpenVRExample openVRExample;


void ExampleOpenVREnter(OPgameState* last) {
#ifdef ADDON_openvr

	OPshader* vert = (OPshader*)OPCMAN.LoadGet("Common/OpenVR.vert");
	OPshader* frag = (OPshader*)OPCMAN.LoadGet("Common/OpenVR.frag");
	openVRExample.lensEffect = OPeffect::Create(vert, frag);

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add("position", OPattributeTypes::OPATTR_TYPE_FLOAT, 4);
	builder.Add("v2UVredIn", OPattributeTypes::OPATTR_TYPE_FLOAT, 2);
	builder.Add("v2UVGreenIn", OPattributeTypes::OPATTR_TYPE_FLOAT, 2);
	builder.Add("v2UVblueIn", OPattributeTypes::OPATTR_TYPE_FLOAT, 2);
	OPvertexLayout layout = builder.Build();
	openVRExample.lensEffect->UpdateVertexLayout(&layout);


	openVRExample.mesh = (OPmesh*)OPCMAN.LoadGet("wolf.opm");
	openVRExample.texture = (OPtexture*)OPCMAN.LoadGet("wolf.png");

	OPshaderAttribute attribs2[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	vert = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadowMVP.vert");
	frag = (OPshader*)OPCMAN.LoadGet("Common/TexturedShadowMVP.frag");	
	openVRExample.modelEffect = OPeffect::Create(vert, frag);

	builder.Init();
	builder.Add("aPosition", OPattributeTypes::OPATTR_TYPE_FLOAT, 3);
	builder.Add("aNormal", OPattributeTypes::OPATTR_TYPE_FLOAT, 3);
	builder.Add("aUV", OPattributeTypes::OPATTR_TYPE_FLOAT, 2);
	layout = builder.Build();
	openVRExample.modelEffect->UpdateVertexLayout(&layout);



	



#endif
}

#ifdef ADDON_openvr

#endif

OPint ExampleOpenVRUpdate(OPtimer* time) {
	openVRExample.openVR.Update();
	return false;
}

#ifdef ADDON_openvr


void RenderScene(vr::EVREye eye) {
	OPrenderClear(0, 0, 0);
	OPrenderDepth(1);
	//OPmat4 mvp = GetCurrentViewProjectionMatrix(eye);
	OPmat4 proj = openVRExample.openVR.GetCurrentProjectionMatrix(eye);
	OPmat4 view = openVRExample.openVR.GetCurrentViewMatrix(eye);
	OPfloat scl = 0.05;
	OPmat4 world = OPmat4Scl(scl) * OPmat4Translate(0, -1, 0);

	//glBindVertexArray(OPRENDER_VAO);
	openVRExample.mesh->Bind();
	openVRExample.modelEffect.Bind();

	OPtextureClearActive();
	OPeffectParamBindTex("uColorTexture", openVRExample.texture);
	OPeffectParamMat4("uWorld", &world);
	OPeffectParamMat4("uView", &view);
	OPeffectParamMat4("uProj", &proj);
	OPmeshRender();
}

void RenderStereoTargets() {

	glClearColor(0.15f, 0.15f, 0.18f, 1.0f); // nice background color, but not black
	glEnable(GL_MULTISAMPLE);

	// Left Eye
	glBindFramebuffer(GL_FRAMEBUFFER, openVRExample.leftEyeDesc.RenderFramebufferId);
	glViewport(0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT);
	RenderScene(vr::Eye_Left);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, openVRExample.leftEyeDesc.RenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, openVRExample.leftEyeDesc.ResolveFramebufferId);

	glBlitFramebuffer(0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT, 0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glEnable(GL_MULTISAMPLE);

	// Right Eye
	glBindFramebuffer(GL_FRAMEBUFFER, openVRExample.rightEyeDesc.RenderFramebufferId);
	glViewport(0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT);
	RenderScene(vr::Eye_Right);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, openVRExample.rightEyeDesc.RenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, openVRExample.rightEyeDesc.ResolveFramebufferId);

	glBlitFramebuffer(0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT, 0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RenderDistortion() {
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, OPRENDER_WIDTH, OPRENDER_HEIGHT);

	//glBindVertexArray(openVRExample.lensVAO);
	glUseProgram(openVRExample.lensProgramID);

	//render left lens (first half of index array )
	glBindTexture(GL_TEXTURE_2D, openVRExample.leftEyeDesc.ResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawElements(GL_TRIANGLES, openVRExample.indexSize / 2, GL_UNSIGNED_SHORT, 0);

	//render right lens (second half of index array )
	glBindTexture(GL_TEXTURE_2D, openVRExample.rightEyeDesc.ResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawElements(GL_TRIANGLES, openVRExample.indexSize / 2, GL_UNSIGNED_SHORT, (const void *)(openVRExample.indexSize));

	//glBindVertexArray(0);
	glUseProgram(0);
}


#endif

void ExampleOpenVRRender(OPfloat delta) {
#ifdef ADDON_openvr
	// for now as fast as possible
	if (openVRExample.hmd)
	{
		//DrawControllers();
		RenderStereoTargets();
		RenderDistortion();

		vr::Texture_t leftEyeTexture = { (void*)openVRExample.leftEyeDesc.ResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::Texture_t rightEyeTexture = { (void*)openVRExample.rightEyeDesc.ResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	}

	//if (m_bVblank && m_bGlFinishHack)
	//{
	//	//$ HACKHACK. From gpuview profiling, it looks like there is a bug where two renders and a present
	//	// happen right before and after the vsync causing all kinds of jittering issues. This glFinish()
	//	// appears to clear that up. Temporary fix while I try to get nvidia to investigate this problem.
	//	// 1/29/2014 mikesart
		glFinish();
	//}

	// SwapWindow
	{
		OPrenderPresent();
		//SDL_GL_SwapWindow(m_pWindow);
	}

	// Clear
	{
		// We want to make sure the glFinish waits for the entire present to complete, not just the submission
		// of the command. So, we do a clear here right here so the glFinish will wait fully for the swap.
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Flush and wait for swap.
	//if (m_bVblank)
	{
		glFlush();
		glFinish();
	}

	// Spew out the controller and pose count whenever they change.
	if (openVRExample.trackedControllerCount != openVRExample.trackedControllerCount_Last || openVRExample.validPoseCount != openVRExample.validPoseCount_Last)
	{
		openVRExample.validPoseCount_Last = openVRExample.validPoseCount;
		openVRExample.trackedControllerCount_Last = openVRExample.trackedControllerCount;

		//OPlog("PoseCount:%d(%s) Controllers:%d\n", openVRExample.validPoseCount, m_strPoseClasses, openVRExample.trackedControllerCount);
	}

	UpdateHMDMatrixPose();
	
	#else

	//OPrenderClear(0, 0, 0);

	//OPrenderPresent();
	#endif
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