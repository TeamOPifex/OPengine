// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#include "../Hub.hpp"

#include <algorithm>
#include <exception>

#include "../DeviceListener.hpp"
#include "../Myo.hpp"
#include "../Pose.hpp"
#include "../Quaternion.hpp"
#include "../Vector3.hpp"
#include "../detail/ThrowOnError.hpp"

namespace myo {

inline
Hub::Hub()
: _hub(0)
, _myos()
, _listeners()
{
    libmyo_init_hub(&_hub, ThrowOnError());
}

inline
Hub::~Hub()
{
    for (std::vector<Myo*>::iterator I = _myos.begin(), IE = _myos.end(); I != IE; ++I) {
        delete *I;
    }
    libmyo_shutdown_hub(_hub, 0);
}

inline
Myo* Hub::waitForAnyMyo(unsigned int timeout_ms)
{
    return waitForMyo(false, timeout_ms);
}

inline
Myo* Hub::waitForAdjacentMyo(unsigned int timeout_ms)
{
    return waitForMyo(true, timeout_ms);
}

inline
void Hub::pairWithAnyMyo()
{
    pairWithAnyMyos(1);
}

inline
void Hub::pairWithAnyMyos(unsigned int count)
{
    libmyo_pair_any(_hub, count, ThrowOnError());
}

inline
void Hub::pairWithAdjacentMyo()
{
    pairWithAdjacentMyos(1);
}

inline
void Hub::pairWithAdjacentMyos(unsigned int count)
{
    libmyo_pair_adjacent(_hub, count, ThrowOnError());
}

inline
void Hub::pairByMacAddress(uint64_t mac_address)
{
    libmyo_pair_by_mac_address(_hub, mac_address, ThrowOnError());
}

inline
void Hub::addListener(DeviceListener* listener)
{
    if (std::find(_listeners.begin(), _listeners.end(), listener) != _listeners.end()) {
        // Listener was already added.
        return;
    }
    _listeners.push_back(listener);
}

inline
void Hub::removeListener(DeviceListener* listener)
{
    std::vector<DeviceListener*>::iterator I = std::find(_listeners.begin(), _listeners.end(), listener);
    if (I == _listeners.end()) {
        // Don't have this listener.
        return;
    }

    _listeners.erase(I);
}

inline
void Hub::onDeviceEvent(libmyo_event_t event)
{
    libmyo_myo_t opaqueMyo = libmyo_event_get_myo(event);

    Myo* myo = 0;

    if (libmyo_event_get_type(event) == libmyo_event_paired) {
        myo = new Myo(opaqueMyo);
        _myos.push_back(myo);
    } else {
        myo = lookupMyo(opaqueMyo);
        if (!myo) {
            // Ignore events for Myos we don't know about.
            return;
        }
    }

    for (std::vector<DeviceListener*>::iterator I = _listeners.begin(), IE = _listeners.end(); I != IE; ++I) {
        DeviceListener* listener = *I;

        uint64_t time = libmyo_event_get_timestamp(event);

        switch (libmyo_event_get_type(event)) {
        case libmyo_event_paired:
            listener->onPair(myo, time);
            break;
        case libmyo_event_connected:
            listener->onConnect(myo, time);
            break;
        case libmyo_event_disconnected:
            listener->onDisconnect(myo, time);
            break;
        case libmyo_event_orientation:
            listener->onOrientationData(myo, time,
                                        Quaternion<float>(libmyo_event_get_orientation(event, libmyo_orientation_x),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_y),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_z),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_w)));
            listener->onAccelerometerData(myo, time,
                                          Vector3<float>(libmyo_event_get_accelerometer(event, 0),
                                                         libmyo_event_get_accelerometer(event, 1),
                                                         libmyo_event_get_accelerometer(event, 2)));

            listener->onGyroscopeData(myo, time,
                                      Vector3<float>(libmyo_event_get_gyroscope(event, 0),
                                                     libmyo_event_get_gyroscope(event, 1),
                                                     libmyo_event_get_gyroscope(event, 2)));

            break;
        case libmyo_event_pose:
            listener->onPose(myo, time, Pose(static_cast<Pose::Type>(libmyo_event_get_pose(event))));
            break;
        case libmyo_event_rssi:
            listener->onRssi(myo, time, libmyo_event_get_rssi(event));
            break;
        }
    }
}

inline
void Hub::run(unsigned int duration_ms)
{
    struct local {
        static libmyo_handler_result_t handler(void* user_data, libmyo_event_t event) {
            Hub* hub = static_cast<Hub*>(user_data);

            hub->onDeviceEvent(event);

            return libmyo_handler_continue;
        }
    };
    libmyo_run(_hub, duration_ms, &local::handler, this, ThrowOnError());
}

inline
void Hub::runOnce(unsigned int duration_ms)
{
    struct local {
        static libmyo_handler_result_t handler(void* user_data, libmyo_event_t event) {
            Hub* hub = static_cast<Hub*>(user_data);

            hub->onDeviceEvent(event);

            return libmyo_handler_stop;
        }
    };
    libmyo_run(_hub, duration_ms, &local::handler, this, ThrowOnError());
}

inline
libmyo_hub_t Hub::libmyoObject()
{
    return _hub;
}

inline
uint64_t Hub::now()
{
    return libmyo_now();
}

inline
Myo* Hub::lookupMyo(libmyo_myo_t opaqueMyo) const
{
    Myo* myo = 0;
    for (std::vector<Myo*>::const_iterator I = _myos.begin(), IE = _myos.end(); I != IE; ++I) {
        if ((*I)->libmyoObject() == opaqueMyo) {
            myo = *I;
            break;
        }
    }

    return myo;
}

inline
Myo* Hub::waitForMyo(bool adjacent, unsigned int timeout_ms)
{
    if (!_myos.empty()) {
        throw std::runtime_error("Already paired with a Myo");
    }

    if (adjacent) {
        pairWithAdjacentMyo();
    } else {
        pairWithAnyMyo();
    }
    struct local {
        static libmyo_handler_result_t handler(void* user_data, libmyo_event_t event) {
            Hub* hub = static_cast<Hub*>(user_data);

            libmyo_myo_t opaque_myo = libmyo_event_get_myo(event);

            switch (libmyo_event_get_type(event)) {
            case libmyo_event_paired:
                hub->_myos.push_back(new Myo(opaque_myo));
                return libmyo_handler_stop;
            default:
                break;
            }

            return libmyo_handler_continue;
        }
    };

    do {
        libmyo_run(_hub, timeout_ms ? timeout_ms : 1000, &local::handler, this, ThrowOnError());
    } while (!timeout_ms && _myos.empty());

    if (_myos.empty()) {
        return 0;
    }

    return _myos.front();
}

} // namespace myo
