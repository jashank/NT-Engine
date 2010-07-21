#ifndef VECTOR_H
#define VECTOR_H

namespace nt {
namespace core {

template< typename T >
/**
 * Vector Class. The vector is a generic all purpose type to be used 
 * throught the engine. This is still a work in progress and will play
 * a much larger role in the engine in later builds. Part of namespace
 * core.
 */

class Vector {
 public:
  /// Empty Constructor
  Vector() : x( 0 ), y( 0 ), z( 0 ) {}
  /// 2-Dimensional Constructor
  Vector( T xPos, T yPos ) : x( xPos ), y( yPos ), z( 0 ) {}
  /// 3-Dimensional Constructor
  Vector( T xPos, T yPos, T zPos ) : x( xPos ), y( yPos ), z( zPos ){}
  /// Vector Constructor - TODO Remove.
  Vector( const Vector<T> &other) 
          : x( other.x ), y( other.y ), z( other.z ) {}
  /// Empty Destructor
  ~Vector() {}

  // Overload Operators
  // Asignment Overloading
  /*Vector<T>& operator=( const Vector<T>& other );
  Vector<T>& operator+=( const Vector<T>& other );
  Vector<T>& operator-=( const Vector<T>& other );
  Vector<T>& operator*=( const Vector<T>& other );
  Vector<T>& operator/=( const Vector* other );

  // Mathematical Overloading
  Vector<T> operator+( const Vector<T> &other ) const;
  Vector<T> operator-( const Vector<T> &other ) const;
  Vector<T> operator-() const; // Make Negative
  Vector<T> operator*( const Vector<T> &other ) const ;
  Vector<T> operator/( const Vector<T> &other ) const;
  
  // Boolean Overloading
  To be implemented later on.
  bool operator==( const Vector<T> &other ) const;
  bool operator!=( const Vector<T> &other ) const;
  bool operator<( const Vector<T> &other ) const;
  bool operator>( const Vector<T> &other ) const;
  bool operator<=( const Vector<T> &other ) const;
  bool operator>=( const Vector<T> &other ) const;
  */
  // Data
  T x;
  T y;
  T z;

  /***********************************************
  Implementation
  ************************************************/
  /// Simply sets x,y,z to that of others x,y,z data members.
  Vector<T>& operator=( const Vector<T>& other ) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  /// += others data members x,y,z to self's x,y,z
  Vector<T>& operator+=( const Vector<T>& other ) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  /// -= others data members x,y,z to self's x,y,z
  Vector<T>& operator-=( const Vector<T>& other ) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  /// *= others data members x,y,z to self's x,y,z
  Vector<T>& operator*=( const Vector<T>& other ) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  /// /= others data memebers x,y,z,to self's x,y,z
  Vector<T>& operator/=( const Vector* other ) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  /// Mathematical Overloading
  Vector<T> operator+( const Vector<T> &other ) const {
    return Vector<T>(x+other.x, y+other.y, z+other.z);
  }

  Vector<T> operator-( const Vector<T> &other ) const {
    return Vector<T>(x-other.x, y-other.y, z-other.z);
  }

  Vector<T> operator-() const {
    return Vector<T>(-x, -y, -z);
  }

  Vector<T> operator*( const Vector<T> &other ) const {
    return Vector<T>(x*other.x, y*other.y, z*other.z);
  }

  Vector<T> operator/( const Vector<T> &other ) const {
    return Vector<T>(x / other.x, y / other.y, z / other.z );
  }
  
  // Boolean Overloading
  /* need to implement for rounding errors on floats
  bool operator==( const Vector<T> &other ) const {
  
  }

  bool operator!=( const Vector<T> &other ) const {
  }

  bool operator<( const Vector<T> &other ) const {
  }

  bool operator>( const Vector<T> &other ) const {
  }

  bool operator<=( const Vector<T> &other ) const {
  }

  bool operator>=( const Vector<T> &other ) const {
  }
  */
};

} // namespace core.
} // namespace nt.

#endif // VECTOR_H

