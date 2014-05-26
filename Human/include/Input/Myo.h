#ifndef OPENGINE_HUMAN_INPUT_MYO
#define OPENGINE_HUMAN_INPUT_MYO

#include "./Core/include/Types.h"
#include "./Core/include/Timer.h"


typedef enum {
	Myo_Pose_None,
	Myo_Pose_Fist,
	Myo_Pose_Fingers_Spread,
	Myo_Pose_Wave_In,
	Myo_Pose_Wave_Out,
	Myo_Pose_Turn_In,
	Myo_Pose_Wave,
	Myo_Pose_Max
} OPmyoPoses;

void OPmyoConnect();
void OPmyoUpdate(OPtimer* timer);

f32 OPmyoRoll();
f32 OPmyoPitch();
f32 OPmyoYaw();
f32 OPmyoRollChange();
f32 OPmyoPitchChange();
f32 OPmyoYawChange();
OPint OPmyoPoseNow(OPmyoPoses pose);
OPint OPmyoPoseWas(OPmyoPoses pose);
OPint OPmyoPoseIs(OPmyoPoses pose);

#endif