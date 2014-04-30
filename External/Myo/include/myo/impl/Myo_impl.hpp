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
std::string Myo::macAddressAsString() const
{
    libmyo_string_t libmyoStr = libmyo_mac_address_to_string(macAddress());
    std::string mac(libmyo_string_c_str(libmyoStr));
    libmyo_string_free(libmyoStr);

    return mac;
}

inline
bool Myo::isTrained() const
{
    return _isTrained;
}

inline
void Myo::vibrate(VibrationType type)
{
    libmyo_vibrate(_myo, static_cast<libmyo_vibration_type_t>(type), ThrowOnError());
}

inline
void Myo::requestRssi() const
{
    libmyo_request_rssi(_myo, ThrowOnError());
}

inline
libmyo_myo_t Myo::libmyoObject() const
{
    return _myo;
}

inline
Myo::Myo(libmyo_myo_t myo)
: _myo(myo)
, _isTrained(false)
{
    if (!_myo) {
        throw std::invalid_argument("Cannot construct Myo instance with null pointer");
    }

    _isTrained = libmyo_training_load_profile(_myo, 0, 0) == libmyo_success;
}

inline
Myo::~Myo()
{
}

} // namespace myo
