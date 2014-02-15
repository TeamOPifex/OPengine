// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#include "../Hub.hpp"

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
: _withoutTrainingProfile(false)
, _hub(0)
, _myo(0)
, _connected(false)
, _listeners()
{
    libmyo_init_hub(&_hub, ThrowOnError());
}

inline
Hub::Hub(Hub::without_training_profile_t)
: _withoutTrainingProfile(true)
, _hub(0)
, _myo(0)
, _connected(false)
, _listeners()
{
    libmyo_init_hub(&_hub, ThrowOnError());
}

inline
Hub::~Hub()
{
    delete _myo;
    libmyo_shutdown_hub(_hub, nullptr);
}

inline
Myo* Hub::waitForAnyMyo(unsigned int timeout_ms)
{
    if (_myo) {
        throw std::runtime_error("Already paired with a Myo");
    }

    pairWithAnyMyo();
    struct local {
        static libmyo_handler_result_t handler(void* user_data, libmyo_event_t event) {
            Hub* hub = static_cast<Hub*>(user_data);

            libmyo_myo_t opaque_myo = libmyo_event_get_myo(event);

            if (hub->_myo && hub->_myo->libmyoObject() != opaque_myo) {
                return libmyo_handler_continue;
            }

            switch (libmyo_event_get_type(event)) {
            case libmyo_event_paired:
                hub->_myo = new Myo(opaque_myo, hub->_withoutTrainingProfile);
                break;
            case libmyo_event_connected:
                hub->_connected = true;
                return libmyo_handler_stop;
            default:
                break;
            }

            return libmyo_handler_continue;
        }
    };

    do {
        libmyo_run(_hub, timeout_ms ? timeout_ms : 1000, &local::handler, this, ThrowOnError());
    } while (!timeout_ms);

    if (!_connected) {
        return 0;
    }

    return _myo;
}

inline
void Hub::pairWithAnyMyo()
{
    libmyo_pair_any(_hub, 1, ThrowOnError());
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
    libmyo_myo_t myo_opq = libmyo_event_get_myo(event);

    if (!_myo) {
        // Still pairing.
        if (libmyo_event_get_type(event) == libmyo_event_paired) {
            _myo = new Myo(myo_opq, _withoutTrainingProfile);
        }
    }

    if (_myo->libmyoObject() != myo_opq) {
        // Ignore events not coming from the Myo we know about.
        return;
    }

    for (std::vector<DeviceListener*>::iterator I = _listeners.begin(), IE = _listeners.end(); I != IE; ++I) {
        DeviceListener* listener = *I;

        uint64_t time = libmyo_event_get_timestamp(event);

        switch (libmyo_event_get_type(event)) {
        case libmyo_event_paired:
            listener->onPair(_myo, time);
            break;
        case libmyo_event_connected:
            listener->onConnect(_myo, time);
            break;
        case libmyo_event_disconnected:
            listener->onDisconnect(_myo, time);
            break;
        case libmyo_event_orientation:
            listener->onOrientationData(_myo, time,
                                        Quaternion<float>(libmyo_event_get_orientation(event, libmyo_orientation_x),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_y),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_z),
                                                          libmyo_event_get_orientation(event, libmyo_orientation_w)));
            listener->onAccelerometerData(_myo, time,
                                          Vector3<float>(libmyo_event_get_accelerometer(event, 0),
                                                         libmyo_event_get_accelerometer(event, 1),
                                                         libmyo_event_get_accelerometer(event, 2)));
            break;
        case libmyo_event_pose:
            listener->onPose(_myo, time, Pose(static_cast<Pose::Type>(libmyo_event_get_pose(event))));
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
Myo* Hub::onlyMyo()
{
    return _myo;
}

inline
uint64_t Hub::now()
{
    return libmyo_now();
}

} // namespace myo
