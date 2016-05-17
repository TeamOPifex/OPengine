#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>

#ifdef ADDON_openvr
#include "openvr.h"
#endif

typedef struct {
	vr::IVRSystem* hmd;
	vr::IVRRenderModels* renderModels;
	bool showTrackedDevice[vr::k_unMaxTrackedDeviceCount];
	OPmesh* trackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];
	OPframeBuffer leftEyeDesc;
	OPframeBuffer rightEyeDesc;

	GLuint m_glSceneVertBuffer;
	GLuint m_unSceneVAO;
	GLuint m_unLensVAO;
	GLuint m_glIDVertBuffer;
	GLuint m_glIDIndexBuffer;
	unsigned int indexSize;
	int trackedControllerCount;
	int trackedControllerCount_Last;
	int validPoseCount;
	int validPoseCount_Last;
	vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	OPmat4 DevicePose[vr::k_unMaxTrackedDeviceCount];

	std::string strPoseClasses;                            // what classes we saw poses for this frame
	char DevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	OPmat4 HMDPose;
	OPmat4 eyePosLeft;
	OPmat4 eyePosRight;
	GLuint lensVAO;
	GLuint lensProgramID;

	OPeffect lensEffect;
	OPmat4 ProjectionCenter;
	OPmat4 ProjectionLeft;
	OPmat4 ProjectionRight;

	OPmesh* mesh;
	OPeffect modelEffect;
	OPtexture* texture;
} OpenVRExample;

OpenVRExample openVRExample;

char* GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	return pchBuffer;
}

struct VertexDataLens
{
	OPvec2 position;
	OPvec2 texCoordRed;
	OPvec2 texCoordGreen;
	OPvec2 texCoordBlue;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void SetupDistortion()
{
	if (!openVRExample.hmd)
		return;

	GLushort m_iLensGridSegmentCountH = 43;
	GLushort m_iLensGridSegmentCountV = 43;

	float w = (float)(1.0 / float(m_iLensGridSegmentCountH - 1));
	float h = (float)(1.0 / float(m_iLensGridSegmentCountV - 1));

	float u, v = 0;

	std::vector<VertexDataLens> vVerts(0);
	VertexDataLens vert;

	//left eye distortion verts
	float Xoffset = -1;
	for (int y = 0; y<m_iLensGridSegmentCountV; y++)
	{
		for (int x = 0; x<m_iLensGridSegmentCountH; x++)
		{
			u = x*w; v = 1 - y*h;
			vert.position = OPvec2Create(Xoffset + u, -1 + 2 * y*h);

			vr::DistortionCoordinates_t dc0 = openVRExample.hmd->ComputeDistortion(vr::Eye_Left, u, v);

			vert.texCoordRed = OPvec2Create(dc0.rfRed[0], 1 - dc0.rfRed[1]);
			vert.texCoordGreen = OPvec2Create(dc0.rfGreen[0], 1 - dc0.rfGreen[1]);
			vert.texCoordBlue = OPvec2Create(dc0.rfBlue[0], 1 - dc0.rfBlue[1]);

			vVerts.push_back(vert);
		}
	}

	//right eye distortion verts
	Xoffset = 0;
	for (int y = 0; y<m_iLensGridSegmentCountV; y++)
	{
		for (int x = 0; x<m_iLensGridSegmentCountH; x++)
		{
			u = x*w; v = 1 - y*h;
			vert.position = OPvec2Create(Xoffset + u, -1 + 2 * y*h);

			vr::DistortionCoordinates_t dc0 = openVRExample.hmd->ComputeDistortion(vr::Eye_Right, u, v);

			vert.texCoordRed = OPvec2Create(dc0.rfRed[0], 1 - dc0.rfRed[1]);
			vert.texCoordGreen = OPvec2Create(dc0.rfGreen[0], 1 - dc0.rfGreen[1]);
			vert.texCoordBlue = OPvec2Create(dc0.rfBlue[0], 1 - dc0.rfBlue[1]);

			vVerts.push_back(vert);
		}
	}

	std::vector<GLushort> vIndices;
	GLushort a, b, c, d;

	GLushort offset = 0;
	for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
	{
		for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
		{
			a = m_iLensGridSegmentCountH*y + x + offset;
			b = m_iLensGridSegmentCountH*y + x + 1 + offset;
			c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
			d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
			vIndices.push_back(a);
			vIndices.push_back(b);
			vIndices.push_back(c);

			vIndices.push_back(a);
			vIndices.push_back(c);
			vIndices.push_back(d);
		}
	}

	offset = (m_iLensGridSegmentCountH)*(m_iLensGridSegmentCountV);
	for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
	{
		for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
		{
			a = m_iLensGridSegmentCountH*y + x + offset;
			b = m_iLensGridSegmentCountH*y + x + 1 + offset;
			c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
			d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
			vIndices.push_back(a);
			vIndices.push_back(b);
			vIndices.push_back(c);

			vIndices.push_back(a);
			vIndices.push_back(c);
			vIndices.push_back(d);
		}
	}

	openVRExample.indexSize = vIndices.size();

	glGenVertexArrays(1, &openVRExample.m_unLensVAO);
	glBindVertexArray(openVRExample.m_unLensVAO);

	glGenBuffers(1, &openVRExample.m_glIDVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, openVRExample.m_glIDVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataLens), &vVerts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &openVRExample.m_glIDIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openVRExample.m_glIDIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordRed));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordGreen));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordBlue));

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OPmat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!openVRExample.hmd)
		return OPMAT4_IDENTITY;

	vr::HmdMatrix44_t mat = openVRExample.hmd->GetProjectionMatrix(nEye, 0.01, 1000.0f, vr::API_OpenGL);

	return OPmat4Create(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

OPmat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!openVRExample.hmd)
		return OPMAT4_IDENTITY;

	vr::HmdMatrix34_t matEyeRight = openVRExample.hmd->GetEyeToHeadTransform(nEye);
	OPmat4 matrixObj = OPmat4Create(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	OPmat4Inverse(&matrixObj, matrixObj);

	return matrixObj;
}

void ExampleOpenVREnter(OPgameState* last) {
#ifdef ADDON_openvr

	OPshaderAttribute attribs[] = {
		{ "position", GL_FLOAT, 4 },
		{ "v2UVredIn", GL_FLOAT, 2 },
		{ "v2UVGreenIn", GL_FLOAT, 2 },
		{ "v2UVblueIn", GL_FLOAT, 2 }
	};
	OPshader* vert = (OPshader*)OPcmanLoadGet("Common/OpenVR.vert");
	OPshader* frag = (OPshader*)OPcmanLoadGet("Common/OpenVR.frag");
	openVRExample.lensEffect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		4,
		"Textured Effect",
		sizeof(GL_FLOAT) * ( 4 + 2 + 2 + 2)
	);

	openVRExample.mesh = (OPmesh*)OPcmanLoadGet("environment_04.opm");

	OPshaderAttribute attribs2[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	vert = (OPshader*)OPcmanLoadGet("Common/Texture3DMVP.vert");
	frag = (OPshader*)OPcmanLoadGet("Common/Texture.frag");
	openVRExample.modelEffect = OPeffectCreate(
		*vert,
		*frag,
		attribs2,
		3,
		"Textured Effect",
		openVRExample.mesh->vertexLayout.stride
	);	
	openVRExample.texture = (OPtexture*)OPcmanLoadGet("Environments.png");
	

	vr::EVRInitError eError = vr::VRInitError_None;
	openVRExample.hmd = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		openVRExample.hmd = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		OPlog("VR_Init Failed: %s", buf);
		return;
	}

	openVRExample.renderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!openVRExample.renderModels)
	{
		openVRExample.hmd = NULL;
		vr::VR_Shutdown();

		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		OPlog("VR_Init Failed: %s", buf);
		return;
	}

	char* strDriver = GetTrackedDeviceString(openVRExample.hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	char* strDisplay = GetTrackedDeviceString(openVRExample.hmd, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);
	OPlog("%s %s", strDriver, strDisplay);

	openVRExample.leftEyeDesc = OPframeBufferCreateVR(OPRENDER_WIDTH, OPRENDER_HEIGHT);
	openVRExample.rightEyeDesc = OPframeBufferCreateVR(OPRENDER_WIDTH, OPRENDER_HEIGHT);

	openVRExample.ProjectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
	openVRExample.ProjectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
	openVRExample.eyePosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	openVRExample.eyePosRight = GetHMDMatrixPoseEye(vr::Eye_Right);
#endif
}

void SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex)
{
	if (unTrackedDeviceIndex >= vr::k_unMaxTrackedDeviceCount)
		return;

	// try to find a model we've already set up
	char* sRenderModelName = GetTrackedDeviceString(openVRExample.hmd, unTrackedDeviceIndex, vr::Prop_RenderModelName_String);
	
	OPmesh* pRenderModel = NULL;// FindOrLoadRenderModel(sRenderModelName);
	if (!pRenderModel)
	{
		char* sTrackingSystemName = GetTrackedDeviceString(openVRExample.hmd, unTrackedDeviceIndex, vr::Prop_TrackingSystemName_String);
		OPlog("Unable to load render model for tracked device %d (%s.%s)", unTrackedDeviceIndex, sTrackingSystemName, sRenderModelName);
		delete sTrackingSystemName;
	}
	else
	{
		openVRExample.trackedDeviceToRenderModel[unTrackedDeviceIndex] = pRenderModel;
		openVRExample.showTrackedDevice[unTrackedDeviceIndex] = true;
	}

	delete sRenderModelName;
}

void ProcessVREvent(const vr::VREvent_t & event)
{
	switch (event.eventType)
	{
	case vr::VREvent_TrackedDeviceActivated:
	{
		SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
		OPlog("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		OPlog ("Device %u detached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		OPlog("Device %u updated.\n", event.trackedDeviceIndex);
	}
	break;
	}
}

OPint ExampleOpenVRUpdate(OPtimer* time) {

	vr::VREvent_t event;
	while (openVRExample.hmd->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event);
	}

	// Process SteamVR controller state
	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
		vr::VRControllerState_t state;
		if (openVRExample.hmd->GetControllerState(unDevice, &state))
		{
			openVRExample.showTrackedDevice[unDevice] = state.ulButtonPressed == 0;
		}
	}

	return false;
}

OPmat4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	OPmat4 matMVP;
	if (nEye == vr::Eye_Left)
	{
		matMVP = openVRExample.ProjectionLeft * openVRExample.eyePosLeft * openVRExample.HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = openVRExample.ProjectionRight * openVRExample.eyePosRight *  openVRExample.HMDPose;
	}

	return matMVP;
}

void RenderScene(vr::EVREye eye) {
	OPrenderClear(0, 0, 0);
	OPrenderDepth(1);
	OPmat4 mvp = GetCurrentViewProjectionMatrix(eye);
	OPmat4 scaled = mvp * OPmat4Scl(0.1, 0.1, 0.1);

	OPmeshBind(openVRExample.mesh);
	OPeffectBind(&openVRExample.modelEffect);
	OPtextureClearActive();
	OPeffectParamBindTex("uColorTexture", openVRExample.texture);
	OPeffectParamMat4("uMVP", &scaled);
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

	glBindVertexArray(openVRExample.lensVAO);
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

	glBindVertexArray(0);
	glUseProgram(0);
}

OPmat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	OPmat4 matrixObj = OPmat4Create(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}

void UpdateHMDMatrixPose()
{
	if (!openVRExample.hmd)
		return;

	vr::VRCompositor()->WaitGetPoses(openVRExample.trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	openVRExample.validPoseCount = 0;
	openVRExample.strPoseClasses = "";
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (openVRExample.trackedDevicePose[nDevice].bPoseIsValid)
		{
			openVRExample.validPoseCount++;
			openVRExample.DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(openVRExample.trackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (openVRExample.DevClassChar[nDevice] == 0)
			{
				switch (openVRExample.hmd->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        openVRExample.DevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               openVRExample.DevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           openVRExample.DevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_Other:             openVRExample.DevClassChar[nDevice] = 'O'; break;
				case vr::TrackedDeviceClass_TrackingReference: openVRExample.DevClassChar[nDevice] = 'T'; break;
				default:                                       openVRExample.DevClassChar[nDevice] = '?'; break;
				}
			}
			openVRExample.strPoseClasses += openVRExample.DevClassChar[nDevice];
		}
	}

	if (openVRExample.trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		//openVRExample.HMDPose = 
			OPmat4Inverse(&openVRExample.HMDPose, openVRExample.DevicePose[vr::k_unTrackedDeviceIndex_Hmd]);// .invert();
	}
}

void ExampleOpenVRRender(OPfloat delta) {
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


	OPrenderClear(0, 0, 0);

	OPrenderPresent();
}

OPint ExampleOpenVRExit(OPgameState* next) {
	return 0;
}

#ifdef ADDON_openvr
OPint GS_EXAMPLE_OPENVR_AVAILABLE = 1;
#else
OPint GS_EXAMPLE_OPENVR_AVAILABLE = 0;
#endif

// This is the Game State for this IMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_OPENVR = {
	ExampleOpenVREnter,
	ExampleOpenVRUpdate,
	ExampleOpenVRRender,
	ExampleOpenVRExit
};
