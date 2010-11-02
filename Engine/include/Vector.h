#ifndef VECTOR_H
#define VECTOR_H

namespace nt {
namespace core {

/**
 * 3D Vector supporting various operations.
 */
template< typename T >
struct Vector {
  Vector() : x( 0 ), y( 0 ), z( 0 ) {}

  /**
   * Constructor for 2D Vector.
   */
  Vector( T xPos, T yPos ) : x( xPos ), y( yPos ), z( 0 ) {}

  /**
   * Constructor for 3D Vector.
   */
  Vector( T xPos, T yPos, T zPos ) : x( xPos ), y( yPos ), z( zPos ){}

  Vector( const Vector<T> &other) 
          : x( other.x ), y( other.y ), z( other.z ) {}

  ~Vector() {}

  /**
   * @return This Vector with x,y,z assigned to by other's x,y,z.
   */
  Vector<T>& operator=( const Vector<T>& other ) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  /**
   * @return This vector with other's x,y,z added to this' x,y,z.
   */
  Vector<T>& operator+=( const Vector<T>& other ) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /**
   * @return This vector with other's x,y,z subtracted from this' x,y,z.
   */
  Vector<T>& operator-=( const Vector<T>& other ) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  /**
   * @return This vector with x,y,z multipled by other's x,y,z.
   */
  Vector<T>& operator*=( const Vector<T>& other ) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  /**
   * @return This vector with x,y,z divided by other's x,y,z. 
   */
  Vector<T>& operator/=( const Vector* other ) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  /**
   * @return Vector with x,y,z of this and other added.
   */
  Vector<T> operator+( const Vector<T> &other ) const {
    return Vector<T>(x+other.x, y+other.y, z+other.z);
  }

  /**
   * @return Vector with x,y,z of other subtracted from this.
   */
  Vector<T> operator-( const Vector<T> &other ) const {
    return Vector<T>(x-other.x, y-other.y, z-other.z);
  }

  /**
   * @return Vector with this' members negated.
   */
  Vector<T> operator-() const {
    return Vector<T>(-x, -y, -z);
  }

  /**
   * @return Vector with x,y,z of other multiplied by this.
   */
  Vector<T> operator*( const Vector<T> &other ) const {
    return Vector<T>(x*other.x, y*other.y, z*other.z);
  }

  /**
   * @return Vector with x,y,z of other dividied by this.
   */
  Vector<T> operator/( const Vector<T> &other ) const {
    return Vector<T>(x / other.x, y / other.y, z / other.z );
  }

  T x; /** x-coordinate of Vector. */
  T y; /** y-coordinate of Vector. */
  T z; /** z-coordinate of Vector. */
};

//@{
/**
 * Common Vector types.
 */
typedef Vector<int> IntVec;
typedef Vector<float> FloatVec;
//@}
 

/**
 * Compare equality x, y and z.
 */
bool operator==( const IntVec &a, const IntVec &b ) {
  return ( a.x == b.x && a.y == b.y && a.z == b.z );
}

/**
 * Compare inequality of x, y and z.
 */
bool operator!=( const IntVec &a, const IntVec &b ) {
  return !( a == b ); 
}

} // namespace core
} // namespace nt

#endif // VECTOR_H

