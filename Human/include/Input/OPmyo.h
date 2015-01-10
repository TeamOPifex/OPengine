#ifndef OPENGINE_HUMAN_INPUT_MYO
#define OPENGINE_HUMAN_INPUT_MYO

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

EXPORT_METHOD void OPmyoConnect();
EXPORT_METHOD void OPmyoUpdate(OPtimer* timer);

EXPORT_METHOD f32 OPmyoRoll();
EXPORT_METHOD f32 OPmyoPitch();
EXPORT_METHOD f32 OPmyoYaw();
EXPORT_METHOD f32 OPmyoRollChange();
EXPORT_METHOD f32 OPmyoPitchChange();
EXPORT_METHOD f32 OPmyoYawChange();
EXPORT_METHOD void OPmyoSetVibration(OPmyoVibrations vibration);
EXPORT_METHOD OPint OPmyoPoseNow(OPmyoPoses pose);
EXPORT_METHOD OPint OPmyoPoseWas(OPmyoPoses pose);
EXPORT_METHOD OPint OPmyoPoseIs(OPmyoPoses pose);
EXPORT_METHOD OPint OPmyoLock();
EXPORT_METHOD OPint OPmyoUnlock(OPint state);

#endif