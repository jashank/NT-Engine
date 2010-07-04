#ifndef VECTOR_H
#define VECTOR_H

template< typename T >
class Vector {
 public:
  // Constructors and Destructors
  Vector() : x( 0 ), y( 0 ), z( 0 ) {};
  Vector( T xPos, T yPos ) : x( xPos ), y( yPos ), z( 0 ) {};
  Vector( T xPos, T yPos, T zPos ) : x( xPos ), y( yPos ), z( zPos ){};
  Vector( const Vector<T> &other) 
          : x( other.x ), y( other.y ), z( other.z ) {};
  ~Vector() {};

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
  // Overload Operators
  // Asignment Overloading
  Vector<T>& operator=( const Vector<T>& other ) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
  }

  Vector<T>& operator+=( const Vector<T>& other ) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vector<T>& operator-=( const Vector<T>& other ) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Vector<T>& operator*=( const Vector<T>& other ) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }

  Vector<T>& operator/=( const Vector* other ) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  // Mathematical Overloading
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

#endif // VECTOR_H
