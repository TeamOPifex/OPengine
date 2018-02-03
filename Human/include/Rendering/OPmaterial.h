#pragma once

class OPmaterial;

#define OPMATERIAL_MAX_UNIFORMS 20

#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPmaterialParam.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Core/include/Assert.h"
struct OPmodel;
struct OPrendererEntity;


extern OPuint OPMATERIAL_GLOBAL_ID;

class OPmaterial {
public:
	ui64 id = 0;

	OPeffect* effect = NULL;
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPmaterialUniformBufferParam paramsUnformBuffer[OPMATERIAL_MAX_UNIFORMS];
	OPuint paramIndex = 0;
	OPuint paramUniformBufferIndex = 0;
	ui8 textureSlot = 0;

	OPmaterial* rootMaterial = NULL;

	bool depth = true;
	bool cull = true;
	bool visible = true;
	bool alpha = true;
	OPcullFace cullFace = OPcullFace::BACK;

	OPshaderUniform* worldUniform = NULL;
	OPshaderUniform* viewUniform = NULL;
	OPshaderUniform* projUniform = NULL;

	OPshaderUniformBuffer* worldBuffer = NULL;
	OPshaderUniformBufferUniform* worldBufferUniform = NULL;
	OPshaderUniformBuffer* viewProjBuffer = NULL;
	OPshaderUniformBufferUniform* viewBufferUniform = NULL;
	OPshaderUniformBufferUniform* projBufferUniform = NULL;

	OPmaterial() { }

	OPmaterial(OPmaterial* base) {
		Init(base);
	}

	OPmaterial(OPeffect* effect) {
		Init(effect);
	}

	virtual OPmaterial* Init(OPeffect* effect);
	virtual OPmaterial* Init(OPmaterial* base);
	void SetupWVP();
	virtual void AddParam(const OPchar* ubo, const OPchar* name, void* data, ui32 loc);
	virtual void AddParam(OPskeleton* skeleton);
	ui32 NextTextureSlot();
		
	virtual void SetWorld(OPmat4* world);
	virtual void SetCamera(OPcam* camera);
	virtual void Bind(bool onlyParams = false); 
	virtual OPmaterialParam* GetParam(const OPchar* name);
	virtual OPmaterialUniformBufferParam* GetParam(const OPchar* ubo, const OPchar* name);
	virtual bool SetParam(const OPchar* ubo, const OPchar* name, void* ptr, ui32 loc);
	virtual void Destroy();
	virtual OPmaterial* CreateInstances(OPmodel* model, bool materialPerMesh);
	virtual OPmaterial* CreateInstances(OPrendererEntity* model);
	static void SetMeta(OPrendererEntity* rendererEntity);


	inline void AddParam(const OPchar* name, void* data) {
		AddParam(NULL, name, data, 0);
	}
	inline bool SetParam(const OPchar* name, void* ptr) {
		return SetParam(NULL, name, ptr, 0);
	}

	inline void SetDepth(bool val) {
		depth = val;
	}

	inline void SetCull(bool val) {
		cull = val;
	}

	inline void ClearParams() {
		paramIndex = 0;
	}

	inline void AddParam(const OPchar* name, OPtexture* data, ui32 slot) {
		AddParam(NULL, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPtextureCube* data, ui32 slot) {
		AddParam(NULL, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPvec3* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, OPvec4* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, OPmat4* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, f32* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, i32* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	inline void AddParam(const OPchar* name, bool* data) {
		AddParam(NULL, name, (void*)data, 0);
	}

	static OPmaterial* Create(OPeffect* effect);
};
