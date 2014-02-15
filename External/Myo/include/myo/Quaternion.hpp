// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_QUATERNION_HPP
#define MYO_QUATERNION_HPP

namespace myo {

/// A quaternion that can be used to represent a rotation.
/// This type provides only very basic functionality to store quaternions that's sufficient to retrieve the data to
/// be placed in a full featured quaternion type.
template<typename T>
class Quaternion {
  public:
    /// Constructs a quaternion with the multiplicative identity.
    Quaternion()
    : _x(0)
    , _y(0)
    , _z(0)
    , _w(1)
    {
    }

    /// Constructs a quaternion with the provided components.
    Quaternion(T x, T y, T z, T w)
    : _x(x)
    , _y(y)
    , _z(z)
    , _w(w)
    {
    }

    /// Sets the components of this quaternion to be those of the other.
    Quaternion& operator=(const Quaternion other)
    {
        _x = other._x;
        _y = other._y;
        _z = other._z;
        _w = other._w;

        return *this;
    }

    /// Return the first component of the quaternion's vector.
    T x() const { return _x; }
    /// Return the second component of the quaternion's vector.
    T y() const { return _y; }
    /// Return the third component of the quaternion's vector.
    T z() const { return _z; }
    /// Return the scalar component of the quaternion.
    T w() const { return _w; }

  private:
    T _x, _y, _z, _w;
};

} // namespace myo;

#endif
