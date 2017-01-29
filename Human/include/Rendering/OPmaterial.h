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
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPeffect* effect = NULL;
	OPmaterial* rootMaterial = NULL;
	OPuint paramIndex = 0;
	bool depth = true;
	bool cull = true;
	bool visible = true;
	bool alpha = 1;
	OPcullFace cullFace = OPcullFace::BACK;

	OPmaterial() { }

	OPmaterial(OPmaterial* base) {
		Init(base);
	}

	OPmaterial(OPeffect* effect) {
		Init(effect);
	}

	virtual OPmaterial* Init(OPeffect* effect);
	virtual OPmaterial* Init(OPmaterial* base);
	virtual void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count);
	virtual void Bind(bool onlyParams = false);
	virtual OPmaterialParam* GetParam(const OPchar* name);
	virtual bool SetParam(const OPchar* name, void* ptr);
	virtual void Destroy();
	virtual OPmaterial* CreateInstances(OPmodel* model, bool materialPerMesh);
	virtual OPmaterial* CreateInstances(OPrendererEntity* model);
	static void OPmaterial::SetMeta(OPrendererEntity* rendererEntity);
	virtual void AddParam(OPskeleton* skeleton);

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
		AddParam(OPmaterialParamType::TEXTURE, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPtextureCube* data, ui32 slot) {
		AddParam(OPmaterialParamType::TEXTURE_CUBE, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPvec3* data) {
		AddParam(OPmaterialParamType::VECTOR3, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPvec4* data) {
		AddParam(OPmaterialParamType::VECTOR4, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPmat4* data) {
		AddParam(OPmaterialParamType::MATRIX4, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
		AddParam(OPmaterialParamType::MATRIX4V, name, (void*)data, count);
	}

	inline void AddParam(const OPchar* name, f32* data) {
		AddParam(OPmaterialParamType::FLOAT, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, i32* data) {
		AddParam(OPmaterialParamType::INT, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, bool* data) {
		AddParam(OPmaterialParamType::BOOL, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, f32* data, ui8 count) {
		AddParam(OPmaterialParamType::FLOAT, name, (void*)data, count);
	}

	static OPmaterial* Create(OPeffect* effect);
};

