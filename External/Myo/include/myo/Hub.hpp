// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_HUB_HPP
#define MYO_HUB_HPP

#include <vector>

#include <libmyo.h>

namespace myo {

class Myo;
class DeviceListener;

/// @brief A Hub provides access to one or more Myo instances.
class Hub
{
  public:
    /// Construct a hub.
    /// Throws an exception of type std::runtime_error if another Hub instance exists.
    /// Throws an exception of type std::runtime_error if no training profile could be found.
    Hub();
    /// Deallocates any resources associated with a Hub.
    /// This will cause all Myo instances retrieved from this Hub to become invalid.
    ~Hub();

    /// Find a nearby Myo and pair with it, or time out after timeout_ms milliseconds if provided.
    /// If timeout_ms is zero, this function blocks until a Myo is found.
    /// This function must not be run concurrently with run() or runOnce().
    Myo* waitForAnyMyo(unsigned int timeout_ms = 0);

    /// Initiate pairing with any nearby Myo device.
    /// Once pairing is initiated, run() must be called until a pairing event is received (via DeviceListener::onPair).
    void pairWithAnyMyo();

    /// Initiate pairing with a Myo that has the given MAC address.
    /// This is primarily useful for testing in an environment with multiple Myos, where you wish to connect to a
    /// specific Myo.
    void pairByMacAddress(uint64_t mac_address);

    /// Register a listener to be called when device events occur.
    void addListener(DeviceListener* listener);

    /// Remove a previously registered listener.
    void removeListener(DeviceListener* listener);

    /// Run the event loop for the specified duration (in milliseconds).
    void run(unsigned int duration_ms);

    /// Run the event loop for the specified duration (in milliseconds) until a single event occurred.
    void runOnce(unsigned int duration_ms);

    /// Retrieve the current timestamp.
    uint64_t now();

    /// @cond MYO_INTERNALS

    enum without_training_profile_t { without_training_profile };

    /// Construct a Hub without requiring a training profile
    Hub(without_training_profile_t);

    /// Return the internal libmyo object corresponding to this hub.
    libmyo_hub_t libmyoObject();

  protected:

    /// Return the Myo associated with this hub.
    /// Note: this function will disappear when more than one Myo is supported.
    Myo* onlyMyo();

    void onDeviceEvent(libmyo_event_t event);    

    /// @endcond

  private:

    bool _withoutTrainingProfile;
    libmyo_hub_t _hub;
    Myo* _myo;
    bool _connected;
    std::vector<DeviceListener*> _listeners;

    // Not implemented
    Hub(const Hub&);
    Hub& operator=(const Hub&);
};

/// @example hello-myo.cpp

} // namespace myo

#include "impl/Hub_impl.hpp"

#endif // MYO_HUB_HPP
