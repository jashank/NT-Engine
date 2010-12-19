/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_POINT_H
#define NT_POINT_H

namespace nt {

/**
 * A 3 coordinate point...zzzz.
 */
template<typename T>
struct Point {
  // Make sure members are initialized on default construction.
  Point()
    :x(T()), y(T()), z(T()) {}

  Point( T x, T y, T z )
    :x(x), y(y), z(z) {}

  /**
   * Assign coordinates of rhs to this Point.
   */
  Point<T> &operator=( const Point<T> &rhs ) {
    if ( this != &rhs ) {
      x = rhs.x; y = rhs.y; z = rhs.z;
    }
    return *this;
  }

  /** Adds val to each coordinate of this Point. */
  Point<T> &operator+=( T val ) {
    x += val; y += val; z += val;
    return *this;
  }

  /** Subtracts val from each coordinate of this Point. */
  Point<T> &operator-=( T val ) {
    x -= val; y -= val; z -= val;
    return *this;
  }

  /** Multiplies val by each coordinate of this Point. */
  Point<T> &operator*=( T val ) {
    x *= val; y *= val; z *= val;
    return *this;
  }

  /** Divides each coordinate by val, unless val = 0. */
  Point<T> &operator/=( T val ) {
    if ( val != 0 ) {
      x /= val; y /= val; z /= val;
    }
    return *this;
  }

  // Return new Points.
  const Point<T> operator+( T val ) const {
    return Point<T>(*this) += val;
  }

  const Point<T> operator-( T val ) const {
    return Point<T>(*this) -= val;
  }

  const Point<T> operator*( T val ) const {
    return Point<T>(*this) *= val;
  }

  const Point<T> operator/( T val ) const {
    return Point<T>(*this) /= val;
  }

  /**
   * Returns true if all coordinates are equal. Not meant for floating point.
   */
  bool operator==( const Point<T> &rhs ) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  /**
   * Returns true if any of the coordinates are not equal.
   * Not meant for floating point.
   */
  bool operator!=( const Point<T> &rhs ) const {
    return !( *this == rhs );
  }

  T x;
  T y;
  T z;
};

typedef Point<int> IntPoint;
typedef Point<float> FloatPoint;

} // namespace nt

#endif // NT_POINT_H

