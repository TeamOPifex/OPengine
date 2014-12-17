#include "./Human/include/Input/OPmyo.h"

#ifdef OPIFEX_MYO
#include <myo/myo.hpp>
#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#endif
#include "./Core/include/OPlog.h"

f32 roll = 0;
f32 rolld = 0;
f32 pitch = 0;
f32 pitchd = 0;
f32 yaw = 0;
f32 yawd = 0;
ui32 pose = 0;

OPint prevPoses[Myo_Pose_Max];
OPint poses[Myo_Pose_Max];

f32 OPmyoRoll() {
	return roll;
}
f32 OPmyoPitch() {
	return pitch;
}
f32 OPmyoYaw() {
	return yaw;
}
f32 OPmyoRollChange() {
	return rolld;
}
f32 OPmyoPitchChange() {
	return pitchd;
}
f32 OPmyoYawChange() {
	return yawd;
}

OPint OPmyoPoseNow(OPmyoPoses pose) {
	return prevPoses[pose] == 0 && poses[pose] == 1;
}

OPint OPmyoPoseWas(OPmyoPoses pose) {
	return prevPoses[pose] == 1 && poses[pose] == 0;
}

OPint OPmyoPoseIs(OPmyoPoses pose) {
	return poses[pose] == 1;
}


#ifdef OPIFEX_MYO
class DataCollector : public myo::DeviceListener {
public:
	DataCollector()
		: currentPose()
	{
	}
	//// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	//// as a unit quaternion.
	//void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
	//{
	//	// Calculate the normalized quaternion.
	//	f32 norm = OPsqrt(quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z() + quat.w() * quat.w());
	//	myo::Quaternion<f32> normalized(quat.x() / norm, quat.y() / norm, quat.z() / norm, quat.w() / norm);
	//	
	//	// Calculate Euler angles (roll, pitch, and yaw) from the normalized quaternion.
	//	//f32 tmpRoll = OPatan2(2.0f * (normalized.w() * normalized.x() + normalized.y() * normalized.z()),
	//	//	1.0f - 2.0f * (normalized.x() * normalized.x() + normalized.y() * normalized.y()));
	//	//f32 tmpPitch = OPsin(2.0f * (normalized.w() * normalized.y() - normalized.z() * normalized.x()));
	//	//f32 tmpYaw = OPatan2(2.0f * (normalized.w() * normalized.z() + normalized.x() * normalized.y()),
	//	//	1.0f - 2.0f * (normalized.y() * normalized.y() + normalized.z() * normalized.z()));

	//	f32 tmpRoll = OPatan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
	//		1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
	//	f32 tmpPitch = OPsin(2.0f * (quat.w() * quat.y() - quat.z() * quat.x()));
	//	f32 tmpYaw = OPatan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
	//		1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

	//	rolld = tmpRoll - roll;
	//	pitchd = tmpPitch - pitch;
	//	yawd = tmpYaw - yaw;

	//	// Convert the floating point angles in radians to a scale from 0 to 20.
	//	roll = tmpRoll; //static_cast<int>((tmpRoll + (float)OPpi) / (OPpi * 2.0f) * 18.0);
	//	pitch = tmpPitch; // static_cast<int>((tmpPitch + (float)OPpi / 2.0f) / OPpi * 18.0);
	//	yaw = tmpYaw; // static_cast<int>((tmpYaw + (float)OPpi) / (OPpi * 2.0f) * 18.0);	

	//}

	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
	{
		using std::atan2;
		using std::asin;
		using std::sqrt;
		using std::max;
		using std::min;

		// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
		float _roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
			1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		float _pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		float _yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
			1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
	


		rolld = _roll - roll;
		pitchd = _pitch - pitch;
		yawd = _yaw - yaw;

		// Convert the floating point angles in radians to a scale from 0 to 18.
		roll = _roll;
		pitch = _pitch;
		yaw = _yaw;
	}

	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose detectedPose)
	{
		OPbzero(&poses, sizeof(OPint)* Myo_Pose_Max);

		// Vibrate the Myo whenever we've detected that the user has made a fist.
		if (detectedPose == myo::Pose::fist) {
			poses[Myo_Pose_Fist] = 1;
		} else
		
		if (detectedPose == myo::Pose::fingersSpread) {
			poses[Myo_Pose_Fingers_Spread] = 1;
		} else
		
		if (detectedPose == myo::Pose::waveIn) {
			poses[Myo_Pose_Turn_In] = 1;
		} else

		if (detectedPose == myo::Pose::waveOut) {
			poses[Myo_Pose_Wave_Out] = 1;
		}
		else

		if (detectedPose == myo::Pose::rest) {
			poses[Myo_Pose_Rest] = 1;
		}
		else
		{
			poses[Myo_Pose_None] = 1;
		}
	}

	// These values are set by onOrientationData() and onPose() above.
	myo::Pose currentPose;
};



myo::Hub* hub = NULL;
myo::Myo* myoDevice = NULL;
DataCollector* collector;
#endif




void OPmyoSetVibration(OPmyoVibrations vibration) {

#ifdef OPIFEX_MYO
	if (myoDevice == NULL) return;
	switch (vibration) {
	case Myo_Vibration_Short:
		myoDevice->vibrate(myo::Myo::vibrationShort);
		break;
	case Myo_Vibration_Medium:
		myoDevice->vibrate(myo::Myo::vibrationMedium);
		break;
	case Myo_Vibration_Long:
		myoDevice->vibrate(myo::Myo::vibrationLong);
		break;
	}
#endif
}

void OPmyoConnect() {
	#ifdef OPIFEX_MYO
		OPbzero(&poses, sizeof(OPint)* Myo_Pose_Max);

		hub = new myo::Hub("com.teamopifex.engine");
		myoDevice = hub->waitForMyo(10000);

		if (!myoDevice) {
			throw std::runtime_error("Unable to find a Myo!");
		}

		OPlog("Myo Device Connected");

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		collector = new DataCollector();
		// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		// Hub::run() to send events to all registered device listeners.
		hub->addListener(collector);

	#else
		 OPlog("Myo Feature is not enabled");
	#endif
 }

#ifdef OPIFEX_MYO

OPint wavingState = 0;
OPint lastWaved = 0;

#endif

void OPmyoProcess(OPtimer* timer) {

#ifdef OPIFEX_MYO
	if (OPmyoPitch() < -0.3f) { // Arm is upward
		if (wavingState == 0) {
			if (OPmyoYaw() < 2) {
				wavingState = 1; // Left Side
			}
			else {
				wavingState = 2; // Right Side
			}
		}
		else if (wavingState == 1) { // Left Side to Right Side
			if (OPmyoYaw() > 2) {
				wavingState = 3;
				lastWaved = 1000;
			}
		}
		else if (wavingState == 2) { // Right Side to Left Side
			if (OPmyoYaw() < 2) {
				wavingState = 4;
				lastWaved = 1000;
			}
		}
		else if (wavingState == 3) { // Left Side to Right Side
			if (OPmyoYaw() < 2) {
				wavingState = 1;
				poses[Myo_Pose_Wave] = 1;
				lastWaved = 1000;
			}
		}
		else if (wavingState == 4) { // Right Side to Left Side
			if (OPmyoYaw() > 2) {
				wavingState = 2;
				poses[Myo_Pose_Wave] = 1;
				lastWaved = 1000;
			}
		}
	}
	else {
		wavingState = 0;
		poses[Myo_Pose_Wave] = 0;
	}
	lastWaved -= timer->Elapsed;
	if (poses[Myo_Pose_Wave] && lastWaved < 0) {
		poses[Myo_Pose_Wave] = 0;
	}

	#endif
}

void OPmyoUpdate(OPtimer* timer) {
	#ifdef OPIFEX_MYO
	if (hub) {
		OPmemcpy(&prevPoses, &poses, sizeof(OPint)* Myo_Pose_Max);
			hub->run(10);
			// Process
			OPmyoProcess(timer);
		}
	#endif
}

OPint OPmyoLock() {
	#ifdef OPIFEX_MYO
		myoDevice->lock();
	#endif
}

OPint OPmyoUnlock(OPint state) {
	#ifdef OPIFEX_MYO
	if(myoDevice) {
		if(state == 1) {
			myoDevice->unlock(myo::Myo::unlockHold);
		} else {
			myoDevice->unlock(myo::Myo::unlockTimed);
		}
	}
	#endif
}