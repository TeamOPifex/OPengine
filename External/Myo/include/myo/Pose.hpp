// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_POSE_HPP
#define MYO_POSE_HPP

#include <iosfwd>
#include <string>

#include <libmyo.h>

namespace myo {

/// A pose represents a detected configuration of the user's hand.
class Pose {
public:
    /// Types of poses supported by the SDK.
    enum Type {
        none = libmyo_pose_none,
        fist = libmyo_pose_fist,
        wave_in = libmyo_pose_wave_in,
        wave_out = libmyo_pose_wave_out,
        fingers_spread = libmyo_pose_fingers_spread,
    };

    /// Construct a pose of type Pose::none.
    Pose();

    /// Construct a pose with the given type.
    Pose(Type type);

    /// Returns true if and only if the two poses are of the same type.
    bool operator==(Pose other) const;

    /// Equivalent to `!(*this == other)`.
    bool operator!=(Pose other) const;

    /// Returns the type of this pose.
    Type type() const;

    /// Return a human-readable string representation of the pose.
    std::string toString() const;

private:
    Type _type;
};

/// @relates Pose
/// Returns true if and only if the type of pose is the same as the provided type.
bool operator==(Pose pose, Pose::Type t);

/// @relates Pose
/// Equivalent to `pose == type`.
bool operator==(Pose::Type type, Pose pose);

/// @relates Pose
/// Equivalent to `!(pose == type)`.
bool operator!=(Pose pose, Pose::Type type);

/// @relates Pose
/// Equivalent to `!(type == pose)`.
bool operator!=(Pose::Type type, Pose pose);

/// @relates Pose
/// Write the name of the provided pose to the provided output stream.
std::ostream& operator<<(std::ostream& out, const Pose& pose);

} // namespace myo

#include "impl/Pose_impl.hpp"

#endif // MYO_POSE_HPP
