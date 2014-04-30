// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef MYO_VECTOR3_HPP
#define MYO_VECTOR3_HPP

namespace myo {

/// A vector of three components.
/// This type provides only very basic functionality to store three dimensional vector that's sufficient to retrieve
/// the data to be placed in a full featured vector type. For example, this type does not support additions or dot
/// products.
template<typename T>
class Vector3 {
  public:
    /// Construct a vector of all zeroes.
    Vector3()
    {
        _data[0] = 0;
        _data[1] = 0;
        _data[2] = 0;
    }

    /// Construct a vector with the three provided components.
    Vector3(T x, T y, T z)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
    }

    /// Construct a vector with the same components as \a other.
    Vector3(const Vector3& other)
    {
        *this = other;
    }

    /// Set the components of this vector to be the same as \a other.
    Vector3& operator=(const Vector3& other)
    {
        _data[0] = other._data[0];
        _data[1] = other._data[1];
        _data[2] = other._data[2];

        return *this;
    }

    /// Return a copy of the component of this vector at \a index, which should be 0, 1, or 2.
    T operator[](unsigned int index) const
    {
        return _data[index];
    }

    /// Return the x-component of this vector.
    T x() const { return _data[0]; }

    /// Return the y-component of this vector.
    T y() const { return _data[1]; }

    /// Return the z-component of this vector.
    T z() const { return _data[2]; }

  private:
    T _data[3];
};

} // namespace myo

#endif // MYO_VECTOR3_HPP
