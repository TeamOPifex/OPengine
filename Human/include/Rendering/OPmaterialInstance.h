#pragma once

#include "./Human/include/Rendering/OPmaterial.h"
//
//class OPmaterialInstance;
//
//class OPmaterialInstance : public OPmaterial {
//private:
//
//public:
//	OPmaterial* rootMaterial = NULL;
//	OPmaterialParam params[OPMATERIAL_MAX_UNIFORMS];
//	OPuint paramIndex;
//	ui64 id;
//	bool visible = true;
//
//	OPmaterialInstance() { }
//	OPmaterialInstance(OPmaterial* material) {
//		Init(material);
//	}
//
//	void Init(OPmaterial* material) {
//		rootMaterial = material;
//		paramIndex = 0;
//		id = OPMATERIALINSTANCE_GLOBAL_ID++;
//	}
//
//	inline void ClearParams(OPmaterial* material) {
//		material->paramIndex = 0;
//	}
//
//	inline static OPmaterialInstance* Create(OPmaterial* material) {
//		OPmaterialInstance* materialInstance = OPNEW(OPmaterialInstance());
//		materialInstance->Init(material);
//		return materialInstance;
//	}
//
//	inline OPmaterialParam* GetParam(const OPchar* name) {
//		for (OPuint i = 0; i < paramIndex; i++) {
//			if (OPstringEquals(params[i].name, name)) {
//				return &params[i];
//			}
//		}
//		return NULL;
//	}
//
//	inline bool SetParam(const OPchar* name, void* ptr) {
//		for (OPuint i = 0; i < paramIndex; i++) {
//			if (OPstringEquals(params[i].name, name)) {
//				params[i].data = ptr;
//				return true;
//			}
//		}
//		return false;
//	}
//
//	inline void AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
//		//OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
//		if (SetParam(name, data)) return;
//		params[paramIndex].type = paramType;
//		params[paramIndex].name = name;
//		params[paramIndex].data = data;
//		params[paramIndex].count = count;
//		paramIndex++;
//	}
//
//	inline void AddParam(const OPchar* name, OPtexture* data, ui32 slot) {
//		AddParam(OPmaterialParamType::TEXTURE, name, (void*)data, slot);
//	}
//
//	inline void AddParam(const OPchar* name, OPtextureCube* data, ui32 slot) {
//		AddParam(OPmaterialParamType::TEXTURE_CUBE, name, (void*)data, slot);
//	}
//
//	inline void AddParam(const OPchar* name, OPvec3* data) {
//		AddParam(OPmaterialParamType::VECTOR3, name, (void*)data, 1);
//	}
//
//	inline void AddParam(const OPchar* name, OPvec4* data) {
//		AddParam(OPmaterialParamType::VECTOR4, name, (void*)data, 1);
//	}
//
//	inline void AddParam(const OPchar* name, OPmat4* data) {
//		AddParam(OPmaterialParamType::MATRIX4, name, (void*)data, 1);
//	}
//
//	inline void AddParam(const OPchar* name, OPmat4* data, ui8 count) {
//		AddParam(OPmaterialParamType::MATRIX4V, name, (void*)data, count);
//	}
//
//	inline void AddParam(const OPchar* name, f32* data) {
//		AddParam(OPmaterialParamType::FLOAT, name, (void*)data, 1);
//	}
//
//	inline void AddBones(OPskeleton* skeleton) {
//		AddParam("uBones", skeleton->skinned, (ui8)skeleton->hierarchyCount);
//	}
//
//	inline void Bind() {
//		rootMaterial->Bind();
//
//		for (OPuint i = 0; i < paramIndex; i++) {
//			switch (params[i].type) {
//			case OPmaterialParamType::TEXTURE: {
//				rootMaterial->effect->Set(params[i].name, (OPtexture*)params[i].data, params[i].count);
//				break;
//			}
//			case OPmaterialParamType::TEXTURE_CUBE: {
//				rootMaterial->effect->Set(params[i].name, (OPtextureCube*)params[i].data, params[i].count);
//				break;
//			}
//			case OPmaterialParamType::VECTOR3: {
//				rootMaterial->effect->Set(params[i].name, (OPvec3*)params[i].data);
//				break;
//			}
//			case OPmaterialParamType::VECTOR4: {
//				rootMaterial->effect->Set(params[i].name, (OPvec4*)params[i].data);
//				break;
//			}
//			case OPmaterialParamType::MATRIX4: {
//				rootMaterial->effect->Set(params[i].name, (OPmat4*)params[i].data);
//				break;
//			}
//			case OPmaterialParamType::MATRIX4V: {
//				rootMaterial->effect->Set(params[i].name, params[i].count, (OPmat4*)params[i].data);
//				break;
//			}
//			case OPmaterialParamType::FLOAT: {
//				rootMaterial->effect->Set(params[i].name, *(f32*)params[i].data);
//				break;
//			}
//			}
//		}
//	}
//
//	void Destroy();
//};