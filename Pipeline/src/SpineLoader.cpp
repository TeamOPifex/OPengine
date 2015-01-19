#include "./Pipeline/include/SpineLoader.h"

#ifdef OPIFEX_OPTION_SPINE

	#include "./Pipeline/include/Rendering.h"
	#include "./Human/include/Utilities/ImagePNG.h"
	#include "./Core/include/OPlog.h"
	#include "./Data/include/OPcman.h"
	#include "./Human/include/Rendering/OPmesh.h"
	#include "./Human/include/Rendering/OPMvertex.h"
	#include "./Human/include/Rendering/Primitives/OPquad.h"
	#include "./Core/include/Assert.h"

	void _spAtlasPage_createTexture(spAtlasPage* self, const char* path){
		OPlog("Spine Atlas Texture to load: %s", path);
		OPtexture* image;
		OPimagePNGLoad(path, &image);
		self->rendererObject = image;
		self->width = image->Description.Width;
		self->height = image->Description.Height;
	}

	void _spAtlasPage_disposeTexture(spAtlasPage* self) {
		OPtextureDestroy((OPtexture*)self->rendererObject);
	}

	char* _spUtil_readFile(const char* path, int* length) {
		return _readFile(path, length);
	}

	OPmesh OP_SPINE_QUAD;
	OPint OP_SPINE_INITIALIZED = 0;
#endif

void SpineAddLoader() {

#ifdef OPIFEX_OPTION_SPINE
	OPassetLoader loader = {
		".atlas",
		"Spine/",
		sizeof(Spine),
		(OPint(*)(const OPchar*, void**))SpineLoad,
		(OPint(*)(void*))SpineUnload,
		NULL
	};
	OPcmanAddLoader(&loader);
#endif
}

void SpineInitialize() {
#ifdef OPIFEX_OPTION_SPINE
	if (!OP_SPINE_INITIALIZED) {
		OP_SPINE_INITIALIZED = 1;
		OP_SPINE_QUAD = OPquadCreate();
	}
#endif
}

void SpineSetMix(Spine* spine, const OPchar* from, const OPchar* to, f32 duration) {
#ifdef OPIFEX_OPTION_SPINE
	spAnimationStateData_setMixByName(spine->stateData, from, to, duration);
#endif
}

void SpineSetAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop) {
#ifdef OPIFEX_OPTION_SPINE
	ASSERT(spine->state != NULL, "Spine State has not been created yet.");
	spAnimationState_setAnimationByName(spine->state, track, anim, loop);
#endif
}
void SpineAddAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop, OPfloat delay) {
#ifdef OPIFEX_OPTION_SPINE
	ASSERT(spine->state != NULL, "Spine State has not been created yet.");
	spAnimationState_addAnimationByName(spine->state, track, anim, loop, delay);
#endif
}

void SpineBuildMixingState(Spine* spine) {
#ifdef OPIFEX_OPTION_SPINE
	spine->state = spAnimationState_create(spine->stateData);
#endif
}

i32 SpineLoad(const OPchar* filename, Spine** spine) {

#ifdef OPIFEX_OPTION_SPINE

	Spine* result = (Spine*)OPalloc(sizeof(Spine));

	spAtlas* atlas = spAtlas_createFromFile(filename, NULL);
	spSkeletonJson* json = spSkeletonJson_create(atlas);
	json->scale = 0.6f;
	spSkeletonData *skeletonData = spSkeletonJson_readSkeletonDataFile(json, "C:/_Repos/OPifex/OPifex.Hg/OPifex.Engine/Assets/Spine/spineboy.json");

	spSkeletonJson_dispose(json);
	result->bounds = spSkeletonBounds_create();

	result->skeleton = spSkeleton_create(skeletonData);
	result->skeleton->flipX = false;
	result->skeleton->flipY = false;
	spSkeleton_setToSetupPose(result->skeleton);
	spSkeleton_updateWorldTransform(result->skeleton);

	result->stateData = spAnimationStateData_create(skeletonData);

	*spine = result;
#endif

	return 1;
}

void SpineUpdate(Spine* spine, OPtimer* timer) {
#ifdef OPIFEX_OPTION_SPINE
	f32 dt = timer->Elapsed / 1000.0f;
	spSkeletonBounds_update(spine->bounds, spine->skeleton, true);
	spSkeleton_update(spine->skeleton, dt);
	spAnimationState_update(spine->state, dt * 1.0f);
	spAnimationState_apply(spine->state, spine->skeleton);
	spSkeleton_updateWorldTransform(spine->skeleton);
#endif
}

void SpineRender(Spine* spine, OPmat4* world, OPeffect* effect, OPcam* camera) {

#ifdef OPIFEX_OPTION_SPINE
	for (int i = 0; i < spine->skeleton->slotsCount; ++i) {

		spSlot* slot = spine->skeleton->drawOrder[i];
		spAttachment* attachment = slot->attachment;
		if (!attachment) continue;

		OPtexture* texture = NULL;
		OPvertexColor vertices[4];
		f32 worldVertices[1000];

		if (attachment->type == SP_ATTACHMENT_REGION) {

			//OPlog("	* Region");
			spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
			texture = (OPtexture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
			spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

			ui8 r = static_cast<ui8>(spine->skeleton->r * slot->r * 255);
			ui8 g = static_cast<ui8>(spine->skeleton->g * slot->g * 255);
			ui8 b = static_cast<ui8>(spine->skeleton->b * slot->b * 255);
			ui8 a = static_cast<ui8>(spine->skeleton->a * slot->a * 255);

			f32 width = texture->Description.Width;
			f32 height = texture->Description.Height;

			vertices[0].Position.x = worldVertices[SP_VERTEX_X1];
			vertices[0].Position.y = worldVertices[SP_VERTEX_Y1];
			vertices[0].Position.z = i * 0.05f;
			vertices[0].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
			vertices[0].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y1];

			vertices[1].Position.x = worldVertices[SP_VERTEX_X2];
			vertices[1].Position.y = worldVertices[SP_VERTEX_Y2];
			vertices[1].Position.z = i * 0.05f;
			vertices[1].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
			vertices[1].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y2];

			vertices[2].Position.x = worldVertices[SP_VERTEX_X3];
			vertices[2].Position.y = worldVertices[SP_VERTEX_Y3];
			vertices[2].Position.z = i * 0.05f;
			vertices[2].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
			vertices[2].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y3];

			vertices[3].Position.x = worldVertices[SP_VERTEX_X4];
			vertices[3].Position.y = worldVertices[SP_VERTEX_Y4];
			vertices[3].Position.z = i * 0.05f;
			vertices[3].TexCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
			vertices[3].TexCoord.y = regionAttachment->uvs[SP_VERTEX_Y4];

			OPrenderSetBufferSubData(&OP_SPINE_QUAD.VertexBuffer, sizeof(OPvertexColor), 0, 4, vertices);

			OPbindMeshEffectWorldCam(&OP_SPINE_QUAD, effect, world, camera);
			if (texture) {
				OPtextureClearActive();
				ui32 handle = OPtextureBind(texture);
				OPeffectParami("uColorTexture", handle);
			}
			OPmeshRender();
		}
		else if (attachment->type == SP_ATTACHMENT_MESH) {
			OPlog("	* Mesh");
		}
		else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
			OPlog("	* Skinned Mesh");
		}

		if (texture) {

		}
	}
#endif
}

i32 SpineUnload(void* spine) {
#ifdef OPIFEX_OPTION_SPINE
	Spine* ent = (Spine*)spine;
	spSkeletonBounds_dispose(ent->bounds);
	spSkeleton_dispose(ent->skeleton);
	spAnimationState_dispose(ent->state);
	OPfree(ent);
#endif
	return 1;
}