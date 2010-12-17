#ifndef POINT_H
#define POINT_H

namespace nt {

/**
 * A 3 coordinate point...zzzz.
 */
template<typename T>
class Point {
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

#endif // POINT_H

