#ifndef RECT_H
#define RECT_H

#include "Vector.h"
/**
 * Class Rect. A generic class that will be more heavily used in future
 * versions of the engine and may form part of a core:: namespace. It
 * will be used in the camera class. TODO make a template. 
 */

namespace nt {
namespace core {

class Rect {
 public:
  /// Empty Constructor NOT IMPLEMENTED
  Rect() : topLeft( 0.0f,0.0f ), bottomRight( 0.0f,0.0f ) {}
  /// Four Variable  Constructor NOT IMPLEMENTED
  Rect( float left, float top, float right, float bottom ) 
        : topLeft( left, top ), bottomRight( right, bottom ) {}
  /// Two Variable Vector Constructor NOT IMPLEMENTED
  Rect( nt::core::Vector<float> upperLeft, 
        nt::core::Vector<float> lowerRight)
        : topLeft( upperLeft ), bottomRight( lowerRight ) {}
  ~Rect() {}
  
  // Useful Methods

  /// Returns topLeft.x
  float GetX() const;

  /// Sets topLeft.x to x if x <= bottomRight.x
  void SetX( float x );

  /// Returns topLeft.y
  float GetY() const;
  
  /// Sets topLeft.y to y if y <= bottomRight.x
  void SetY( float y );

  /// Returns bottomRight.x
  float GetX2() const;

  /// Sets bottomRight.x if x2 >= topLeft.x
  void SetX2( float x2 );

  /// Gets bottomRight.y
  float GetY2() const;

  /// Sets bottomRight.y to y2 if >= topLeft.y
  void SetY2( float y2 );

  /// Returns bottomRight.x-topLeft.x
  float GetWidth() const;

  /// Returns bottomRight.y-topLeft.y
  float GetHeight() const;

  /// Returns a vector containing width and height of the Rect
  nt::core::Vector<float> GetSize() const;

  /// Returns the widths times the height. Just a helpful method
  float GetArea() const;

  /// Returns a vector with the X and Y (potentially Z) coords of the
  /// center of the rectangle.
  nt::core::Vector<float> GetCenter() const;

  /// Returns true if the vector position is within the rectangle.
  bool Contains( const nt::core::Vector<float> pos ) const;
  
  /// Returns true if the rectangles intersect.
  bool Intersects( const Rect &other ) const;
  
  // Setters
  /// Returns a vector(topLeft.x+width/2, topLeft.y+height/2)
  /// This is particularly useful for focusing on game objects
  /// for RPG like / top down games
  void SetCenter( nt::core::Vector<float> centerPosition );

  /// Sets the topLeft position
  void SetPosition( nt::core::Vector<float> position );

  /// Sets the bottomRight position
  void SetScale( nt::core::Vector<float> scale );

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

 private:
  nt::core::Vector<float> topLeft;
  nt::core::Vector<float> bottomRight;

};

} // namespace core
} // namespace nt

#endif // RECT_H

