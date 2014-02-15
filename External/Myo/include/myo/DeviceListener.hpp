// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_DEVICELISTENER_HPP
#define MYO_DEVICELISTENER_HPP

#include <cstdint>

#include "Pose.hpp"

namespace myo {

class Myo;
template<typename T>
class Quaternion;
template<typename T>
class Vector3;

/// A DeviceListener receives events about a Myo.
/// @see Hub::addListener()
class DeviceListener {
public:
    virtual ~DeviceListener() {}

    /// Called when a Myo has been paired.
    virtual void onPair(Myo* myo, uint64_t timestamp) {}

    /// Called when a paired Myo has been connected.
    virtual void onConnect(Myo* myo, uint64_t timestamp) {}

    /// Called when a paired Myo has been disconnected.
    virtual void onDisconnect(Myo* myo, uint64_t timestamp) {}

    /// Called when a paired Myo has provided a new pose.
    virtual void onPose(Myo* myo, uint64_t timestamp, Pose pose) {}

    /// Called when a paired Myo has provided new orientation data.
    virtual void onOrientationData(Myo* myo, uint64_t timestamp, const Quaternion<float>& rotation) {}

    /// Called when a paired Myo has provided new accelerometer data in units of g.
    virtual void onAccelerometerData(Myo* myo, uint64_t timestamp, const Vector3<float>& accel) {}

};

} // namespace myo

#endif // MYO_DEVICELISTENER_HPP
