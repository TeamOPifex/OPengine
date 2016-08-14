#pragma once

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPtimer.h"


typedef enum {
	Myo_Pose_None,
	Myo_Pose_Rest,
	Myo_Pose_Fist,
	Myo_Pose_Fingers_Spread,
	Myo_Pose_Wave_In,
	Myo_Pose_Wave_Out,
	Myo_Pose_Double_Tap,
	Myo_Pose_Turn_In,
	Myo_Pose_Wave,
	Myo_Pose_ThumbToPinky,
	Myo_Pose_Max
} OPmyoPoses;

typedef enum {
	Myo_Vibration_Short,
	Myo_Vibration_Medium,
	Myo_Vibration_Long
} OPmyoVibrations;

void OPmyoConnect();
void OPmyoUpdate(OPtimer* timer);

f32 OPmyoRoll();
f32 OPmyoPitch();
f32 OPmyoYaw();
f32 OPmyoRollChange();
f32 OPmyoPitchChange();
f32 OPmyoYawChange();
void OPmyoSetVibration(OPmyoVibrations vibration);
OPint OPmyoPoseNow(OPmyoPoses pose);
OPint OPmyoPoseWas(OPmyoPoses pose);
OPint OPmyoPoseIs(OPmyoPoses pose);
OPint OPmyoLock();
OPint OPmyoUnlock(OPint state);