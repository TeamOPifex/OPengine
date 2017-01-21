#pragma once

struct OPrenderer;
typedef struct OPrenderer OPrenderer;

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPlightSpot.h"
#include "./Human/include/Rendering/OPrendererEntity.h"

struct OPrenderer {
	void* internalPtr;
	OPcam** camera;

	void(*_Init)(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) = 0;
	OPmaterial*(*_GetMaterial)(OPrenderer* renderer, ui32 pass) = 0;
	void(*_SetMaterial)(OPrenderer* renderer, OPmaterial* material, ui32 pass) = 0;
	void(*_SetMaterialEffect)(OPrenderer* renderer, OPeffect* effect, ui32 pass) = 0;
	void(*_SetMaterials)(OPrenderer* renderer, OPrendererEntity* entity) = 0;
	void(*_SetMaterialsSkeleton)(OPrenderer* renderer, OPrendererEntity* entity, OPskeleton* skeleton) = 0;
	OPmaterial*(*_CreateMaterial)(OPrenderer* renderer, ui32 pass) = 0;
	void(*_Begin)(OPrenderer* renderer) = 0;
	void(*_SubmitModel)(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) = 0;
	void(*_SubmitModelMaterial)(OPrenderer* renderer, OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) = 0;
	void(*_SubmitMeshMaterial)(OPrenderer* renderer, OPmesh* mesh, OPmat4* world, bool shadowed, OPmaterial* material) = 0;
	void(*_SubmitRendererEntity)(OPrenderer* renderer, OPrendererEntity* rendererEntity) = 0;
	void(*_SubmitLight)(OPrenderer* renderer, OPlightSpot* light) = 0;
	void(*_End)(OPrenderer* renderer) = 0;
	void(*_Present)(OPrenderer* renderer) = 0;

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { _Init(this, camera, maxCalls, maxLights); }
	inline OPmaterial* GetMaterial(ui32 pass) { return _GetMaterial(this, pass); }
	inline void SetMaterial(OPmaterial* material, ui32 pass) { _SetMaterial(this, material, pass); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { _SetMaterialEffect(this, effect, pass); }
	inline void SetMaterials(OPrendererEntity* entity) { _SetMaterials(this, entity); }
	inline void SetMaterials(OPrendererEntity* entity, OPskeleton* skeleton) { _SetMaterialsSkeleton(this, entity, skeleton); }
	inline OPmaterial* CreateMaterial(ui32 pass = 0) { return _CreateMaterial(this, pass); }
	inline void Begin() { _Begin(this); }
	//inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { _SubmitModel(this, model, world, shadowed, material); }
	inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { _SubmitModelMaterial(this, model, world, shadowed, material); }
	inline void Submit(OPmesh* mesh, OPmat4* world, bool shadowed, OPmaterial* material) { _SubmitMeshMaterial(this, mesh, world, shadowed, material); }
	inline void Submit(OPrendererEntity* rendererEntity) { _SubmitRendererEntity(this, rendererEntity); }
	inline void Submit(OPlightSpot* light) { _SubmitLight(this, light); }
	inline void End() { _End(this); }
	inline void Present() { _Present(this); }
};

enum struct OPmaterialType {
	DEFAULT = 0x01,
	ANIMATED = 0x02,
	SHADOWED = 0x04
};

class OPrendererPass {
public:
	OPmaterial* materials;
	OPrenderCommandBucket renderBucket;
	OPcam** camera;
	OPcam** shadowCamera;

public:
	OPrendererPass() {  }
	virtual void Init(OPcam** cam) {
		camera = cam;
	}
	virtual void Begin() = 0;
	virtual void Submit(OPrendererEntity* rendererEntity) = 0;
	virtual void End() = 0;
	virtual void Destroy() = 0;
	virtual OPmaterial* GetMaterial(ui32 materialType) = 0;
	void SetCamera(OPcam** cam) {
		camera = cam;
		renderBucket.camera = cam;
	}
};

class OPrenderer2 {
protected:
	OPrendererPass** passes;

public:
	virtual void Init(OPcam** cam, OPcam** shadowCam) = 0;
	virtual void Begin() = 0;
	virtual void Submit(OPrendererEntity* rendererEntity) = 0;
	virtual OPmaterial* GetMaterial(ui32 pass = 0, ui32 materialType = (ui32)OPmaterialType::DEFAULT) = 0;
	virtual void End() = 0;
	virtual void Present() = 0;
	virtual void Destroy() = 0;
	virtual void SetCamera(OPcam** camera) = 0;
	virtual void SetCamera(OPcam** camera, ui32 pass) = 0;
};