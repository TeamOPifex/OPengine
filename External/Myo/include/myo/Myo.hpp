// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_MYO_HPP
#define MYO_MYO_HPP

#include <cstdint>

#include <libmyo.h>

namespace myo {

/// Represents a Myo device with a specific MAC address.
/// This class can not be instantiated directly; instead, use Hub to get access to a Myo.
/// There is only one Myo instance corresponding to each device; thus, if the addresses of two Myo instances compare
/// equal, they refer to the same device.
class Myo
{
  public:
    /// Return the MAC address associated with this device.
    uint64_t macAddress() const;

    /// Types of vibration supported by the Myo.
    enum VibrationType {
        VibrationShort  = libmyo_vibration_short,
        VibrationMedium = libmyo_vibration_medium,
        VibrationLong   = libmyo_vibration_long
    };

    /// Vibrate the Myo.
    void vibrate(VibrationType type);

    /// @cond MYO_INTERNALS

    /// Return the internal libmyo object corresponding to this device.
    libmyo_myo_t libmyoObject() const;

    /// @endcond

  private:
    Myo(libmyo_myo_t myo, bool withoutTrainingProfile);
    ~Myo();

    libmyo_myo_t _myo;

    // Not implemented.
    Myo(const Myo&);
    Myo& operator=(const Myo&);

    friend class Hub;
};

} // namespace myo

#include "impl/Myo_impl.hpp"

#endif // MYO_MYO_HPP
