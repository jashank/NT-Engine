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

  /**
   * Adds 
  void operator+( const Point<T> &rhs ) const {

  }

  /**
   * Multiples all coordinates by value passed.
   */
  Point<T> operator*( T rhs ) {
    return Point<T>( x * rhs.x, y * rhs.y, z * rhs.z );
  }

  T x;
  T y;
  T z;
};

typedef Point<int> IntPoint;
typedef Point<float> FloatPoint;

} // namespace nt

#endif // POINT_H
