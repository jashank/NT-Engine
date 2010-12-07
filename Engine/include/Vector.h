#ifndef VECTOR_H
#define VECTOR_H

namespace nt {

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
  Vector<T> &operator=( const Vector<T>& other ) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  /** Vector addition. */
  Vector<T> operator+( const Vector<T> &other ) const {
    return Vector<T>(x+other.x, y+other.y, z+other.z);
  }

  /** Scalar addition. */
  Vector<T> operator+( const T &val ) const {
    return Vector<T>(x+val, y+val, z+val);
  }

  /** Vector subtraction. */
  Vector<T> operator-( const Vector<T> &other ) const {
    return Vector<T>(x-other.x, y-other.y, z-other.z);
  }

  /** Scalar subtraction. */
  Vector<T> operator-( const T &val ) const {
    return Vector<T>(x-val, y-val, z-val);
  }

  /** Negate vector. */
  Vector<T> operator-() const {
    return Vector<T>(-x, -y, -z);
  }

  /** Vector multiplication. */
  Vector<T> operator*( const Vector<T> &other ) const {
    return Vector<T>(x*other.x, y*other.y, z*other.z);
  }

  /** Scalar multiplication. */
  Vector<T> operator*( const T &val ) const {
    return Vector<T>(x*val, y*val, z*val);
  }

  /** Vector division. */
  Vector<T> operator/( const Vector<T> &other ) const {
    return Vector<T>(x / other.x, y / other.y, z / other.z );
  }

  /** Scalar division. */
  Vector<T> operator/( const T &val ) const {
    return Vector<T>(x / val, y / val, z / val );
  }

  /** Vector addition assignment. */
  Vector<T> &operator+=( const Vector<T> &other ) {
    *this = *this + other;
    return *this;
  }

  /** Scalar addition assignment. */
  Vector<T> &operator+=( const T &val ) {
    *this = *this + val;
    return *this;
  }

  /** Vector subtraction assignment. */
  Vector<T> &operator-=( const Vector<T> &other ) {
    *this = *this - other;
    return *this;
  }

  /** Scalar subtraction assignment. */
  Vector<T> &operator-=( const T &val ) {
    *this = *this - val;
    return *this;
  }

  /** Vector multiplication assignment. */
  Vector<T> &operator*=( const Vector<T> &other ) {
    *this = *this * other;
    return *this;
  }

  /** Scalar multiplication assignment. */
  Vector<T> &operator*=( const T &val ) {
    *this = *this * val;
    return *this;
  }

  /** Vector division assignment. */
  Vector<T> &operator/=( const Vector &other ) {
    *this = *this / other;
    return *this;
  }

  /** Scalar division assignment. */
  Vector<T> &operator/=( const T &val ) {
    *this = *this / val;
    return *this;
  }

  /**
   * Return true if x, y, and z are equal to other Vector's members.
   */
  bool operator==( const Vector<T> &other ) const {
    return ( x == other.x && y == other.y && z == other.z );
  }

  /**
   * Return true if x, y, or z are not equal to other Vector's members.
   */
  bool operator!=( const Vector<T> &other ) const {
    return !( *this == other ); 
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
 

} // namespace nt

#endif // VECTOR_H

