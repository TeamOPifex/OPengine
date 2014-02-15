// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#include "../Myo.hpp"
#include "../detail/ThrowOnError.hpp"

#include <stdexcept>

namespace myo {

inline
uint64_t Myo::macAddress() const
{
    return libmyo_get_mac_address(_myo);
}

inline
libmyo_myo_t Myo::libmyoObject() const
{
    return _myo;
}

inline
void Myo::vibrate(VibrationType type)
{
    libmyo_vibrate(_myo, static_cast<libmyo_vibration_type_t>(type), ThrowOnError());
}

inline
Myo::Myo(libmyo_myo_t myo, bool withoutTrainingProfile)
: _myo(myo)
{
    if (!_myo) {
        throw std::invalid_argument("Cannot construct Myo instance with null pointer");
    }

    if (!withoutTrainingProfile) {
        if (libmyo_training_load_profile(_myo, nullptr, ThrowOnError()) != libmyo_success) {
            std::cerr << "Warning: No training profile found. Please run the training application.";
        }
    }
}

inline
Myo::~Myo()
{
}

} // namespace myo
