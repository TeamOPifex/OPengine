#include "./Pipeline/include/OPspriteSystem.h"
#include "./Core/include/Assert.h"

void OPspriteSystemEffectDefault(OPeffect* effect) {
	effect->Init("Common/OPspriteSheet.vert", "Common/OPspriteSheet.frag");
}

void OPspriteSystemInit(OPspriteSystem* system, OPsprite** sprites, OPint count, OPeffect* effect, OPspriteSystemAlign alignment) {
	if(effect == NULL) {
		system->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		OPspriteSystemEffectDefault(system->Effect);
		system->ExternalEffect = 0;
	} else {
		system->Effect = effect;
		system->ExternalEffect = 1;
	}
	system->SystemSprites = (OPspriteSystemSprite*)OPallocZero(sizeof(OPspriteSystemSprite)* count);
	system->Count = 0;
	system->Sprites = sprites;
	system->CountMax = count;
	system->FrameRate = 24.0f;

	switch (alignment) {
	case OPSPRITESYSTEMALIGN_CENTER:
		system->_mesh = OPquadCreate(0.5f, 0.5f, OPVEC2_ZERO, OPVEC2_ONE);
		break;
	case OPSPRITESYSTEMALIGN_BOTTOM_LEFT:
		system->_mesh = OPquadCreate(0.5f, 0.5f, OPvec2(0.5, 0.5));
		break;
	default:
		system->_mesh = OPquadCreate(0.5f, 0.5f, OPvec2(0.0, 0.5));
	}

	for (OPint i = 0; i < count; i++) {
		system->SystemSprites[i].Scale = OPVEC2_ONE;
		system->SystemSprites[i].Direction = 1;
	}
}

void OPspriteSystemDestroy(OPspriteSystem* system) {
	//OPeffectDestroy(system->Effect);
	OPfree(system->Effect);
	OPfree(system->SystemSprites);
	//OPmeshDestroy(system->_mesh);
}

OPspriteSystem* OPspriteSystemCreate(OPsprite** sprites, OPint count, OPeffect* effect, OPspriteSystemAlign alignment) {
	OPspriteSystem* result = (OPspriteSystem*)OPalloc(sizeof(OPspriteSystem));
	OPspriteSystemInit(result, sprites, count, effect, alignment);
	return result;
}

OPspriteSystemSprite* OPspriteSystemAdd(OPspriteSystem* system) {
	ASSERT(system->Count != system->CountMax, "Filled up the Sprite System");
	OPspriteSystemSprite* result = &system->SystemSprites[system->Count];
	system->Count++;
	return result;
}

void OPspriteSystemRemove(OPspriteSystem* system, OPspriteSystemSprite* sprite) {
	ASSERT(system->Count != 0, "Nothing to remove in Sprite System");
	if (system->Count > 1) {
		for (OPuint i = 0; i < system->Count - 1; i++) {
			if (&system->SystemSprites[i] == sprite) {
				system->SystemSprites[i] = system->SystemSprites[system->Count - 1];
				break;
			}
		}
	}
	system->Count--;
}

void OPspriteSystemUpdate(OPspriteSystem* system, OPtimer* timer) {
	OPspriteSystemSprite* sprite;
	for (OPuint i = 0; i < system->Count; i++) {
		sprite = &system->SystemSprites[i];

		sprite->CurrentElapsed += timer->Elapsed;
		if (sprite->CurrentElapsed > 1000 / system->FrameRate) {
			sprite->CurrentElapsed = 0;
			sprite->CurrentFrame++;
			if (sprite->CurrentFrame >= system->Sprites[sprite->CurrentSprite]->FrameCount) {
				sprite->CurrentFrame = 0;
			}
		}
	}
}

void OPspriteSystemRender(OPspriteSystem* system, OPcam* cam) {
	if (system->Count == 0) return;

	OPsprite* currentSprite;
	OPmat4 world, view;
	view = OPmat4Translate(cam->pos * -1);

	system->_mesh->Bind();
	system->Effect->Bind();

	//OPtexturePixelate();

	//OPtextureClearActive();
	OPeffectSet("uColorTexture", system->Sprites[0]->Sheet, 0);
	OPeffectSet("uProj", &cam->proj);
	OPeffectSet("uView", &cam->view);

	for (OPuint i = 0; i < system->Count; i++) {
		currentSprite = system->Sprites[system->SystemSprites[i].CurrentSprite];
		OPvec2 frameSize = currentSprite->FrameSize();

		//world = // OPmat4Translate((frameSize.x / 2.0) *system->SystemSprites[i].Scale.x, (frameSize.y / 2.0) *system->SystemSprites[i].Scale.y, 0);
		world = OPMAT4_IDENTITY;
		//world.Scl(system->SystemSprites[i].Scale.x, system->SystemSprites[i].Scale.y, 1.0f);
		//world.RotZ(system->SystemSprites[i].Rotation);


        world.Scl(250);
        //world.Scl(system->SystemSprites[i].Scale.x, system->SystemSprites[i].Scale.y, 1.0f);
		//world.Translate(system->SystemSprites[i].Position.x, system->SystemSprites[i].Position.y, 0);
		//world.Scl(frameSize.x * system->SystemSprites[i].Direction, frameSize.y, 0);


		OPeffectSet("uWorld", &world);
		OPeffectSet("uOffset", &currentSprite->Frames[system->SystemSprites[i].CurrentFrame].Offset);
		OPeffectSet("uSize", &currentSprite->Frames[system->SystemSprites[i].CurrentFrame].Size);

		OPrenderDrawBufferIndexed(0);
	}
}
