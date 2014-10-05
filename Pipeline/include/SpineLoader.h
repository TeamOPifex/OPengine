#ifndef OPIFEX_PIPELINE_SPINELOADER
#define OPIFEX_PIPELINE_SPINELOADER

#include "./Core/include/Types.h"
#include "./Core/include/Timer.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/Camera.h"


#ifdef OPIFEX_SPINE
	#include <spine/spine.h>
	#include <spine/extension.h>
	#include <spine/SkeletonJson.h>
	#include <spine/SkeletonData.h>
#endif

typedef struct {

#ifdef OPIFEX_SPINE
	spSkeletonBounds* bounds;
	spSkeleton* skeleton;
	spAnimationState* state;
	spAnimationStateData* stateData;
#endif

} Spine;

void SpineAddLoader();
void SpineInitialize();

void SpineSetMix(Spine* spine, const OPchar* from, const OPchar* to, f32 duration);
void SpineSetAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop);
void SpineAddAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop, OPfloat delay);
void SpineBuildMixingState(Spine* spine);
i32 SpineLoad(const OPchar* filename, Spine** spine);
void SpineRender(Spine* spine, OPmat4* world, OPeffect* effect, OPcam* camera);
void SpineUpdate(Spine* spine, OPtimer* timer);
i32 SpineUnload(void* spine);

#endif