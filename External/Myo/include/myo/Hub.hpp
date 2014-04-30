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
class Hub {
public:
    /// Construct a hub.
    /// It is not safe to concurrently construct two Hub instances on two different threads.
    /// Throws an exception of type std::runtime_error if another Hub instance exists.
    Hub();

    /// Deallocate any resources associated with a Hub.
    /// This will cause all Myo instances retrieved from this Hub to become invalid.
    ~Hub();

    /// Find a nearby Myo and pair with it, or time out after \a timeout_ms milliseconds if provided.
    /// If \a timeout_ms is zero, this function blocks until a Myo is found.
    /// This function must not be run concurrently with run() or runOnce().
    Myo* waitForAnyMyo(unsigned int timeout_ms = 0);

    /// Wait until a Myo is physically very near to the Bluetooth radio and pair with it, or time out after
    /// \a timeout_ms milliseconds if provided.
    /// If \a timeout_ms is zero, this function blocks until a Myo is found.
    /// This function must not be run concurrently with run() or runOnce().
    Myo* waitForAdjacentMyo(unsigned int timeout_ms = 0);

    /// Initiate pairing with any nearby Myo.
    /// Once pairing is initiated, run() must be called until a pairing event is received (via DeviceListener::onPair).
    void pairWithAnyMyo();

    /// Initiate pairing with the provided number of nearby Myos.
    /// Once pairing is initiated, run() must be called until a pairing event is received (via DeviceListener::onPair).
    void pairWithAnyMyos(unsigned int count);

    /// Initiate pairing with a Myo that is physically very near to the Bluetooth radio.
    /// Once pairing is initiated, run() must be called until a pairing event is received (via DeviceListener::onPair).
    void pairWithAdjacentMyo();

    /// Initiate pairing with one or more Myos that are physically very near to the Bluetooth radio.
    /// Each Myo can be brought close to the Bluetooth radio one at a time.
    /// Once pairing is initiated, run() must be called until a pairing event is received (via DeviceListener::onPair).
    void pairWithAdjacentMyos(unsigned int count);

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

    /// Run the event loop until a single event occurs, or the specified duration (in milliseconds) has elapsed.
    void runOnce(unsigned int duration_ms);

    /// Retrieve the current timestamp.
    uint64_t now();

    /// @cond MYO_INTERNALS

    /// Return the internal libmyo object corresponding to this hub.
    libmyo_hub_t libmyoObject();

protected:
    void onDeviceEvent(libmyo_event_t event);

    Myo* lookupMyo(libmyo_myo_t opaqueMyo) const;

    /// @endcond

private:
    /// Wait for a Myo and pair with it, timing out if \a timeout_ms is set.
    /// If \a timeout_ms is 0, block until paired with a Myo.
    /// If \a adjacent is true, pair with a physically adjacent, rather than any, Myo.
    Myo* waitForMyo(bool adjacent, unsigned int timeout_ms);

    bool _withoutTrainingProfile;
    libmyo_hub_t _hub;
    std::vector<Myo*> _myos;
    std::vector<DeviceListener*> _listeners;

    // Not implemented
    Hub(const Hub&);
    Hub& operator=(const Hub&);
};

/// @example hello-myo.cpp
/// @example multiple-myos.cpp

} // namespace myo

#include "impl/Hub_impl.hpp"

#endif // MYO_HUB_HPP
