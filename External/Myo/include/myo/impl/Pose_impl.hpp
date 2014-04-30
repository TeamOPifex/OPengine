// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_IMPL_POSE_IMPL_HPP
#define MYO_IMPL_POSE_IMPL_HPP

#include "../Pose.hpp"

#include <iostream>

namespace myo {

inline
Pose::Pose()
: _type(none)
{
}

inline
Pose::Pose(Pose::Type type)
: _type(type)
{
}

inline
bool Pose::operator==(Pose other) const
{
    return _type == other._type;
}

inline
bool Pose::operator!=(Pose other) const
{
    return !(*this == other);
}

inline
Pose::Type Pose::type() const
{
    return _type;
}

inline
bool operator==(Pose pose, Pose::Type type)
{
    return pose.type() == type;
}

inline
bool operator==(Pose::Type type, Pose pose)
{
    return pose == type;
}

inline
bool operator!=(Pose pose, Pose::Type type)
{
    return !(pose == type);
}

inline
bool operator!=(Pose::Type type, Pose pose)
{
    return !(type == pose);
}

inline
std::string Pose::toString() const
{
    switch (type()) {
    case Pose::none:
        return "none";
    case Pose::fist:
        return "fist";
    case Pose::wave_in:
        return "wave_in";
    case Pose::wave_out:
        return "wave_out";
    case Pose::fingers_spread:
        return "fingers_spread";
    case Pose::twist_in:
        return "twist_in";
    }

    return "<invalid>";
}

inline
std::ostream& operator<<(std::ostream& out, const Pose& pose)
{
    return out << pose.toString();
}

} // namespace myo

#endif // MYO_IMPL_POSE_IMPL_HPP
