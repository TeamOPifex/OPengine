// TODO: (garrett) restructure
#pragma once

struct OPmaterial;
typedef struct OPmaterial OPmaterial;

struct OPmaterialInstance;
typedef struct OPmaterialInstance OPmaterialInstance;

#define OPMATERIAL_MAX_UNIFORMS 20

#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPmaterialParam.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"
struct OPmodel;

inline void OPmaterialClearParams(OPmaterial* material);
void OPmaterialInit(OPmaterial* material, OPeffect* effect);
inline OPmaterial* OPmaterialCreate(OPeffect* effect);
inline void OPmaterialAddParam(OPmaterial* material, OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count );
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtexture* data, ui32 slot);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtextureCube* data, ui32 slot);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec3* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec4* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data, ui8 count);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, bool* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data, ui8 count);
inline void OPmaterialBind(OPmaterial* material);

struct OPmaterial {
	OPeffect* effect;
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPuint paramIndex;
	ui64 id;
	bool depth;
	bool cull = true;
	OPcullFace cullFace = OPcullFace::BACK;
	bool visible = true;
	i8 alpha;

	OPmaterial() {

	}

	OPmaterial(OPeffect* effect) {
		Init(effect);
	}

	OPmaterialParam* GetParam(const OPchar* name) {
		for (OPuint i = 0; i < paramIndex; i++) {
			if (OPstringEquals(params[i].name, name)) {
				return &params[i];
			}
		}
		return NULL;
	}

	void SetDepth(bool val) {
		depth = val;
	}

	void SetCull(bool val) {
		cull = val;
	}

	void ClearParams() {
	    OPmaterialClearParams(this);
	}

	void Init(OPeffect* effect) {
		OPmaterialInit(this, effect);
	}

	void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
	    OPmaterialAddParam(this, paramType, name, data, count);
	}

	void AddParam(const OPchar* name, OPtexture* data, ui32 slot) {
	    OPmaterialAddParam(this, name, data, slot);
	}

	inline void AddParam(const OPchar* name, OPtextureCube* data, ui32 slot) {
		OPmaterialAddParam(this, name, data, slot);
	}

	void AddParam(const OPchar* name, OPvec3* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPvec4* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPmat4* data) {
	    OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
	    OPmaterialAddParam(this, name, data, count);
	}

	void AddParam(const OPchar* name, f32* data) {
		OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, bool* data) {
		OPmaterialAddParam(this, name, data);
	}

	void AddParam(const OPchar* name, f32* data, ui8 count) {
		OPmaterialAddParam(this, name, data, count);
	}

	void Bind() {
	    OPmaterialBind(this);
	}

	void Destroy();

	OPmaterialInstance** CreateInstances(OPmodel* model, bool materialPerMesh);
};

inline void OPmaterialClearParams(OPmaterial* material) {
	material->paramIndex = 0;
}

inline OPmaterial* OPmaterialCreate(OPeffect* effect) {
	OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
	OPmaterialInit(material, effect);
	return material;
}

inline void OPmaterialAddParam(OPmaterial* material, OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count ) {
	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
	material->params[material->paramIndex].type = paramType;
	material->params[material->paramIndex].name = name;
	material->params[material->paramIndex].data = data;
	material->params[material->paramIndex].count = count;
	material->paramIndex++;
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtexture* data, ui32 slot) {
	OPmaterialAddParam(material, OPmaterialParamType::TEXTURE, name, (void*)data, slot);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtextureCube* data, ui32 slot) {
	OPmaterialAddParam(material, OPmaterialParamType::TEXTURE_CUBE, name, (void*)data, slot);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec3* data) {
	OPmaterialAddParam(material, OPmaterialParamType::VECTOR3, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec4* data) {
	OPmaterialAddParam(material, OPmaterialParamType::VECTOR4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data) {
	OPmaterialAddParam(material, OPmaterialParamType::MATRIX4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data, ui8 count) {
	OPmaterialAddParam(material, OPmaterialParamType::MATRIX4V, name, (void*)data, count);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data) {
	OPmaterialAddParam(material, OPmaterialParamType::FLOAT, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, bool* data) {
	OPmaterialAddParam(material, OPmaterialParamType::BOOL, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data, ui8 count) {
	OPmaterialAddParam(material, OPmaterialParamType::FLOAT, name, (void*)data, count);
}

inline void OPmaterialBind(OPmaterial* material) {
	material->effect->Bind();

	OPrenderDepth(material->depth);
	OPrenderCull(material->cull);
	if (material->cull) {
		OPrenderCullMode(material->cullFace);
	}

	for(OPuint i = 0; i < material->paramIndex; i++) {

		switch(material->params[i].type) {
			case OPmaterialParamType::TEXTURE: {
				material->effect->Set(material->params[i].name, (OPtexture*)material->params[i].data, material->params[i].count);
				break;
			}
			case OPmaterialParamType::TEXTURE_CUBE: {
				material->effect->Set(material->params[i].name, (OPtextureCube*)material->params[i].data, material->params[i].count);
				break;
			}
			case OPmaterialParamType::VECTOR3: {
				material->effect->Set(material->params[i].name, (OPvec3*)material->params[i].data);
				break;
			}
			case OPmaterialParamType::VECTOR4: {
				material->effect->Set(material->params[i].name, (OPvec4*)material->params[i].data);
				break;
			}
			case OPmaterialParamType::MATRIX4: {
				material->effect->Set(material->params[i].name, (OPmat4*)material->params[i].data);
				break;
			}
			case OPmaterialParamType::MATRIX4V: {
				material->effect->Set(material->params[i].name, material->params[i].count, (OPmat4*)material->params[i].data);
				break;
			}
			case OPmaterialParamType::FLOAT: {
				material->effect->Set(material->params[i].name, *(f32*)material->params[i].data);
				break;
			}
			case OPmaterialParamType::BOOL: {
				material->effect->Set(material->params[i].name, *(bool*)material->params[i].data);
				break;
			}
		}
	}
}

extern OPuint OPMATERIALINSTANCE_GLOBAL_ID;
struct OPmaterialInstance {
	OPmaterial* rootMaterial = NULL;
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPuint paramIndex;
	ui64 id;
	bool visible = true;

	OPmaterialInstance() { }
	OPmaterialInstance(OPmaterial* material) {
		Init(material);
	}

	void Init(OPmaterial* material) {
		rootMaterial = material;
		paramIndex = 0;
		id = OPMATERIALINSTANCE_GLOBAL_ID++;
	}

	inline void ClearParams(OPmaterial* material) {
		material->paramIndex = 0;
	}

	inline static OPmaterialInstance* Create(OPmaterial* material) {
		OPmaterialInstance* materialInstance = OPNEW(OPmaterialInstance());
		materialInstance->Init(material);
		return materialInstance;
	}

	inline OPmaterialParam* GetParam(const OPchar* name) {
		for (OPuint i = 0; i < paramIndex; i++) {
			if (OPstringEquals(params[i].name, name)) {
				return &params[i];
			}
		}
		return NULL;
	}

	inline bool SetParam(const OPchar* name, void* ptr) {
		for (OPuint i = 0; i < paramIndex; i++) {
			if (OPstringEquals(params[i].name, name)) {
				params[i].data = ptr;
				return true;
			}
		}
		return false;
	}

	inline void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
		//OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
		params[paramIndex].type = paramType;
		params[paramIndex].name = name;
		params[paramIndex].data = data;
		params[paramIndex].count = count;
		paramIndex++;
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

	inline void AddBones(OPskeleton* skeleton) {
		AddParam("uBones", skeleton->skinned, (ui8)skeleton->hierarchyCount);
	}

	inline void Bind() {
		rootMaterial->Bind();

		for (OPuint i = 0; i < paramIndex; i++) {
			switch (params[i].type) {
				case OPmaterialParamType::TEXTURE: {
					rootMaterial->effect->Set(params[i].name, (OPtexture*)params[i].data, params[i].count);
					break;
				}
				case OPmaterialParamType::TEXTURE_CUBE: {
					rootMaterial->effect->Set(params[i].name, (OPtextureCube*)params[i].data, params[i].count);
					break;
				}
				case OPmaterialParamType::VECTOR3: {
					rootMaterial->effect->Set(params[i].name, (OPvec3*)params[i].data);
					break;
				}
				case OPmaterialParamType::VECTOR4: {
					rootMaterial->effect->Set(params[i].name, (OPvec4*)params[i].data);
					break;
				}
				case OPmaterialParamType::MATRIX4: {
					rootMaterial->effect->Set(params[i].name, (OPmat4*)params[i].data);
					break;
				}
				case OPmaterialParamType::MATRIX4V: {
					rootMaterial->effect->Set(params[i].name, params[i].count, (OPmat4*)params[i].data);
					break;
				}
				case OPmaterialParamType::FLOAT: {
					rootMaterial->effect->Set(params[i].name, *(f32*)params[i].data);
					break;
				}
			}
		}
	}

	void Destroy();
};