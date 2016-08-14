#pragma once

struct OPmaterial;
struct OPmaterialParam;

typedef struct OPmaterial OPmaterial;
typedef struct OPmaterialParam OPmaterialParam;

#define OPMATERIAL_MAX_UNIFORMS 20

#include "OPeffect.h"
#include "./Core/include/OPtypes.h"

enum OPmaterialParamType {
	MATERIAL_PARAM_TYPE_MATRIX4,
	MATERIAL_PARAM_TYPE_MATRIX4V,
	MATERIAL_PARAM_TYPE_TEXTURE,
	MATERIAL_PARAM_TYPE_VECTOR3,
	MATERIAL_PARAM_TYPE_VECTOR4,
	MATERIAL_PARAM_TYPE_FLOAT,
	MATERIAL_PARAM_TYPE_TEXTURE_CUBE
};

struct OPmaterialParam {
	OPmaterialParamType  type;
	const OPchar*        name;
	void*                data;
	ui8                  count;
};



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
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data);
inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data, ui8 count);
inline void OPmaterialBind(OPmaterial* material);
//inline void OPmaterialBind(OPmaterial* material, ui32 stride);

struct OPmaterial {
	OPeffect* effect;
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPuint paramIndex;
	ui64 id;
	bool depth;
	bool cull;
	i8 alpha;

	OPmaterial() {

	}

	OPmaterial(OPeffect* effect) {
		Init(effect);
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

	void AddParam(const OPchar* name, f32* data, ui8 count) {
		OPmaterialAddParam(this, name, data, count);
	}

	void Bind() {
	    OPmaterialBind(this);
	}
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
	OPlog("Name %s %p", name, data);
	material->params[material->paramIndex].type = paramType;
	material->params[material->paramIndex].name = name;
	material->params[material->paramIndex].data = data;
	material->params[material->paramIndex].count = count;
	material->paramIndex++;
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtexture* data, ui32 slot) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_TEXTURE, name, (void*)data, slot);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPtextureCube* data, ui32 slot) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_TEXTURE_CUBE, name, (void*)data, slot);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec3* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_VECTOR3, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPvec4* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_VECTOR4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_MATRIX4, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, OPmat4* data, ui8 count) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_MATRIX4V, name, (void*)data, count);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_FLOAT, name, (void*)data, 1);
}

inline void OPmaterialAddParam(OPmaterial* material, const OPchar* name, f32* data, ui8 count) {
	OPmaterialAddParam(material, MATERIAL_PARAM_TYPE_FLOAT, name, (void*)data, count);
}

inline void OPmaterialBind(OPmaterial* material) {
	material->effect->Bind();

	OPrenderDepth(material->depth);
	OPrenderCull(material->cull);

	//OPtextureClearActive();
	for(OPuint i = 0; i < material->paramIndex; i++) {

		switch(material->params[i].type) {
			case MATERIAL_PARAM_TYPE_TEXTURE: {
				material->effect->Set(material->params[i].name, (OPtexture*)material->params[i].data, material->params[i].count);
				break;
			}
			case MATERIAL_PARAM_TYPE_TEXTURE_CUBE: {
				material->effect->Set(material->params[i].name, (OPtextureCube*)material->params[i].data, material->params[i].count);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR3: {
				material->effect->Set(material->params[i].name, (OPvec3*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_VECTOR4: {
				material->effect->Set(material->params[i].name, (OPvec4*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_MATRIX4: {
				material->effect->Set(material->params[i].name, (OPmat4*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_MATRIX4V: {
				material->effect->Set(material->params[i].name, material->params[i].count, (OPmat4*)material->params[i].data);
				break;
			}
			case MATERIAL_PARAM_TYPE_FLOAT: {
				material->effect->Set(material->params[i].name, *(f32*)material->params[i].data);
				break;
			}
		}
	}
}




struct OPmaterialInstance {
	OPmaterial* rootMaterial;
	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
	OPuint paramIndex;
	ui64 id;

	OPmaterialInstance() { }
	OPmaterialInstance(OPmaterial* material) {
		Init(material);
	}

	void Init(OPmaterial* material) {
		rootMaterial = material;
		paramIndex = 0;
	}

	inline void ClearParams(OPmaterial* material) {
		material->paramIndex = 0;
	}

	inline static OPmaterialInstance* Create(OPmaterial* material) {
		OPmaterialInstance* materialInstance = OPNEW(OPmaterialInstance());
		materialInstance->Init(material);
		return materialInstance;
	}

	inline void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
		OPlog("Name %s %p", name, data);
		params[paramIndex].type = paramType;
		params[paramIndex].name = name;
		params[paramIndex].data = data;
		params[paramIndex].count = count;
		paramIndex++;
	}

	inline void AddParam(const OPchar* name, OPtexture* data, ui32 slot) {
		AddParam(MATERIAL_PARAM_TYPE_TEXTURE, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPtextureCube* data, ui32 slot) {
		AddParam(MATERIAL_PARAM_TYPE_TEXTURE_CUBE, name, (void*)data, slot);
	}

	inline void AddParam(const OPchar* name, OPvec3* data) {
		AddParam(MATERIAL_PARAM_TYPE_VECTOR3, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPvec4* data) {
		AddParam(MATERIAL_PARAM_TYPE_VECTOR4, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPmat4* data) {
		AddParam(MATERIAL_PARAM_TYPE_MATRIX4, name, (void*)data, 1);
	}

	inline void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
		AddParam(MATERIAL_PARAM_TYPE_MATRIX4V, name, (void*)data, count);
	}

	inline void AddParam(const OPchar* name, f32* data) {
		AddParam(MATERIAL_PARAM_TYPE_FLOAT, name, (void*)data, 1);
	}

	inline void Bind() {
		rootMaterial->Bind();

		for (OPuint i = 0; i < paramIndex; i++) {
			switch (params[i].type) {
				case MATERIAL_PARAM_TYPE_TEXTURE: {
					rootMaterial->effect->Set(params[i].name, (OPtexture*)params[i].data, params[i].count);
					break;
				}
				case MATERIAL_PARAM_TYPE_TEXTURE_CUBE: {
					rootMaterial->effect->Set(params[i].name, (OPtextureCube*)params[i].data, params[i].count);
					break;
				}
				case MATERIAL_PARAM_TYPE_VECTOR3: {
					rootMaterial->effect->Set(params[i].name, (OPvec3*)params[i].data);
					break;
				}
				case MATERIAL_PARAM_TYPE_VECTOR4: {
					rootMaterial->effect->Set(params[i].name, (OPvec4*)params[i].data);
					break;
				}
				case MATERIAL_PARAM_TYPE_MATRIX4: {
					rootMaterial->effect->Set(params[i].name, (OPmat4*)params[i].data);
					break;
				}
				case MATERIAL_PARAM_TYPE_MATRIX4V: {
					rootMaterial->effect->Set(params[i].name, params[i].count, (OPmat4*)params[i].data);
					break;
				}
				case MATERIAL_PARAM_TYPE_FLOAT: {
					rootMaterial->effect->Set(params[i].name, *(f32*)params[i].data);
					break;
				}
			}
		}
	}

};