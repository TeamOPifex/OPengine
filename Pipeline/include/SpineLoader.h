#ifndef OPIFEX_PIPELINE_SPINELOADER
#define OPIFEX_PIPELINE_SPINELOADER

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPtimer.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPcam.h"


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

EXPORT_METHOD void SpineAddLoader();
EXPORT_METHOD void SpineInitialize();

EXPORT_METHOD void SpineSetMix(Spine* spine, const OPchar* from, const OPchar* to, f32 duration);
EXPORT_METHOD void SpineSetAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop);
EXPORT_METHOD void SpineAddAnim(Spine* spine, i32 track, const OPchar* anim, OPint loop, OPfloat delay);
EXPORT_METHOD void SpineBuildMixingState(Spine* spine);
EXPORT_METHOD i32 SpineLoad(const OPchar* filename, Spine** spine);
EXPORT_METHOD void SpineRender(Spine* spine, OPmat4* world, OPeffect* effect, OPcam* camera);
EXPORT_METHOD void SpineUpdate(Spine* spine, OPtimer* timer);
EXPORT_METHOD i32 SpineUnload(void* spine);

#endif