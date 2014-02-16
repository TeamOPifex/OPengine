#include "./Human/include/Input/Myo.h"

#ifdef OPIFEX_MYO
#include <myo.hpp>
#include "./Core/include/MathHelpers.h"
#else
#include "./Core/include/Log.h"
#endif

f32 roll = 0;
f32 pitch = 0;
f32 yaw = 0;
ui32 pose = 0;

f32 OPmyoRoll() {
	return roll;
}
f32 OPmyoPitch() {
	return pitch;
}
f32 OPmyoYaw() {
	return yaw;
}

ui32 OPmyoPose() {
	return pose;
}


#ifdef OPIFEX_MYO
class DataCollector : public myo::DeviceListener {
public:
	DataCollector()
		: currentPose()
	{
	}
	// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	// as a quaternion.
	void onOrientationData(myo::Myo* myo, ui64 timestamp, const myo::Quaternion<f32>& quat)
	{
		// Calculate the normalized quaternion.
		f32 norm = OPsqrt(quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z() + quat.w() * quat.w());
		myo::Quaternion<f32> normalized(quat.x() / norm, quat.y() / norm, quat.z() / norm, quat.w() / norm);

		// Calculate Euler angles (roll, pitch, and yaw) from the normalized quaternion.
		f32 tmpRoll = OPatan2(2.0f * (normalized.w() * normalized.x() + normalized.y() * normalized.z()),
			1.0f - 2.0f * (normalized.x() * normalized.x() + normalized.y() * normalized.y()));
		f32 tmpPitch = OPsin(2.0f * (normalized.w() * normalized.y() - normalized.z() * normalized.x()));
		f32 tmpYaw = OPatan2(2.0f * (normalized.w() * normalized.z() + normalized.x() * normalized.y()),
			1.0f - 2.0f * (normalized.y() * normalized.y() + normalized.z() * normalized.z()));

		// Convert the floating point angles in radians to a scale from 0 to 20.
		roll = tmpRoll; //static_cast<int>((tmpRoll + (float)OPpi) / (OPpi * 2.0f) * 18.0);
		pitch = tmpPitch; // static_cast<int>((tmpPitch + (float)OPpi / 2.0f) / OPpi * 18.0);
		yaw = tmpYaw; // static_cast<int>((tmpYaw + (float)OPpi) / (OPpi * 2.0f) * 18.0);
	}

	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
	void onPose(myo::Myo* myo, ui64 timestamp, myo::Pose detectedPose)
	{
		currentPose = detectedPose;

		// Vibrate the Myo whenever we've detected that the user has made a fist.
		if (detectedPose == myo::Pose::fist) {
			pose = myo::Pose::fist;
			myo->vibrate(myo::Myo::VibrationMedium);
		}

		if (detectedPose == myo::Pose::fingers_spread) {
			pose = myo::Pose::fingers_spread;
		}

		if (detectedPose == myo::Pose::wave_in) {
			pose = myo::Pose::wave_in;
		}

		if (detectedPose == myo::Pose::wave_out) {
			pose = myo::Pose::wave_out;
		}
	}

	// These values are set by onOrientationData() and onPose() above.
	myo::Pose currentPose;
};

myo::Hub* hub;
DataCollector* collector;
#endif

void OPmyoConnect() {
	#ifdef OPIFEX_MYO
		 hub = new myo::Hub();
		 myo::Myo* myoDevice = hub->waitForAnyMyo(10000);
		 if (!myoDevice) {
			 throw std::runtime_error("Unable to find a Myo!");
		 }

		 // Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		 collector = new DataCollector();

		 // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
		 // Hub::run() to send events to all registered device listeners.
		 hub->addListener(collector);
	#else
		 OPlog("Myo Feature is not enabled");
	#endif
 }

void OPmyoUpdate() {
	#ifdef OPIFEX_MYO
		if (hub) {
			hub->run(1);
		}
	#endif
}