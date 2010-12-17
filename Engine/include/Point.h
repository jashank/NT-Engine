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
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
    }
    return *this;
  }

  /**
   * Add/assign coordintes of rhs to this Point.
   */
  Point<T> &operator+=( const Point<T> &rhs ) {
    if ( this != &rhs ) {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
    }
    return *this;
  } 

  /**
   * Subtract/assign coordinates of rhs from this Point.
   */
  Point<T> &operator-=( const Point<T> &rhs ) {
    if ( this != &rhs ) {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
    }
    return *this;
  }

  /**
   * Multiply/assign coordinates of rhs by this Point.
   */
  Point<T> &operator*=( const Point<T> &rhs ) {
    if ( this != &rhs ) {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
    }
    return *this;
  }

  /**
   * Divide/assign coordinates of rhs by this Point.
   * Checks for divide by 0, if one is to occur then the division for that
   * coordinate is not performed (but the others still are).
   */
  Point<T> &operator/=( const Point<T> &rhs ) {
    if ( this != &rhs ) {
      if ( rhs.x != 0 ) x /= rhs.x;
      if ( rhs.y != 0 ) y /= rhs.y;
      if ( rhs.z != 0 ) z /= rhs.z;
    }
    return *this;
  }

  // Return new Points.
  const Point<T> operator+( const Point<T> &rhs ) const {
    return Point<T>(*this) += rhs;
  }

  const Point<T> operator-( const Point<T> &rhs ) const {
    return Point<T>(*this) -= rhs;
  }

  const Point<T> operator*( const Point<T> &rhs ) const {
    return Point<T>(*this) *= rhs;
  }

  const Point<T> operator/( const Point<T> &rhs ) const {
    return Point<T>(*this) /= rhs;
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

