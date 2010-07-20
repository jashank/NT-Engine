#ifndef RECT_H
#define RECT_H

#include "Vector.h"
/**
 * Class Rect. A generic class that will be more heavily used in future
 * versions of the engine and may form part of a core:: namespace. It
 * will be used in the camera class. TODO make a template. 
 */

class Rect {
 public:
  /// Empty Constructor NOT IMPLEMENTED
  Rect() : topLeft( 0.0f,0.0f ), bottomRight( 0.0f,0.0f ) {}
  /// Four Variable  Constructor NOT IMPLEMENTED
  Rect( float left, float top, float right, float bottom ) 
        : topLeft( left, top ), bottomRight( right, bottom ) {}
  /// Two Variable Vector Constructor NOT IMPLEMENTED
  Rect( Vector<float> upperLeft, Vector<float> lowerRight)
        : topLeft( upperLeft ), bottomRight( lowerRight ) {}
  ~Rect() {}
  
  // Useful Methods
  // Getters
  float GetWidth() const;
  float GetHeight() const;
  Vector<float> GetSize() const;

  float GetArea() const;
  Vector<float> GetCenter() const;

  bool Contains( const Vector<float> pos ) const;
  bool Intersects( const Rect &other ) const;
  
  // Setters
  void SetCenter( Vector<float> centerPosition );
  void SetPosition( Vector<float> position );
  void SetScale( Vector<float> scale );

  // Overload Operators
  /*
  // Offset to the right
  Rect operator+( const Vector<float> &pos ) const;
  // Offest to the left
  Rect operator-( const Vector<float> &pos ) const;

  Rect operator+=( const Vector<float> &pos ) const;
  Rect operator-=( const Vector<float> &pos ) const;

   Have to take note of float rounding errors
  bool operator==( const Rect &other ) const;
  bool operator!=( const Rect &other ) const;
  bool operator<( const Rect &other ) const;
  bool operator>( const Rect &other ) const;
  bool operator<=( const Rect &other ) const;
  bool operator>=( const Rect &other ) const;  
  */

  // Public Members
  Vector<float> topLeft;
  Vector<float> bottomRight;

};

#endif // RECT_H
