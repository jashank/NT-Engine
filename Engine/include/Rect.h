#ifndef RECT_H
#define RECT_H

#include "Vector.h"

namespace nt {
namespace core {

/**
 * Rectangle with (x,y) coordinates for top left and bottom right members.
 */ 
template< typename T >
struct Rect {
  Rect() {}
  /**
   * @param left leftmost x-coordinate.
   * @param top topmost y-coordinate.
   * @param right rightmost x-coordinate.
   * @param bottom bottommost y-coordinate.
   */
  Rect( T left, T top, T right, T bottom ) 
        : topLeft( left, top ), bottomRight( right, bottom ) {}

  /**
   * @param topLeft 2D Vector with coordinates for top left of rectangle.
   * @param bottomRight 2D Vector with coordinates for bottom right of rectangle.
   */
  Rect( nt::core::Vector<T> topLeftCorner, 
        nt::core::Vector<T> bottomRightCorner )
        : topLeft( topLeftCorner ), bottomRight( bottomRightCorner ) {}

  ~Rect() {}
  
  /**
   * @return Distance in pixels between leftmost x and rightmost x.
   */
  T GetWidth() const {
    return ( bottomRight.x - topLeft.x );
  }

  /**
   * @return Distance in pixels between topmost y and bottommost y.
   */
  T GetHeight() const {
    return ( bottomRight.y - topLeft.y );
  }

  /**
   * @return Vector with (width,height) of rect.
   */
  nt::core::Vector<T> GetSize() const {
    return nt::core::Vector<T>( GetWidth(), GetHeight() );
  }

  /**
   * @return Area in pixels of the rectangle (width*height).
   */
  T GetArea() const {
    return ( GetWidth() * GetHeight() );
  } 

  /**
   * @return Vector containing coordinates (x,y,z) of center of rectangle.
   */
  nt::core::Vector<T> GetCenter() const {
    T centerX = ( topLeft.x + bottomRight.x ) / 2;
    T centerY = ( topLeft.y + bottomRight.y ) / 2;
    return nt::core::Vector<T>( centerX, centerY );
  }

  /**
   * @param x x-coordinate to check.
   * @param y y-coordinate to check.
   * @return True if coordinate is inside Rect. Considered inside if on edge.
   */
  bool Contains( T x, T y ) const {
    return ( topLeft.x <= x &&
             topLeft.y <= y &&
             bottomRight.x >= x &&
             bottomRight.y >= y );
  }
  
  /**
   * Moves Rect by values passed in x and y directions.
   * @param x amount to move in x direction.
   * @param y amount to move in y direction.
   */
  void Offset( T x, T y ) {
    topLeft.x += x;
    topLeft.y += y;
    bottomRight.x += x;
    bottomRight.y += y;
  }

  /**
   * Sets new center for this Rect, adjusting position of other coordinates
   * accordingly (size of Rect is unchanged).
   * @param x x-coordinate for new center.
   * @param y y-coordinate for new center.
   */
  void SetCenter( T x, T y ) {
    topLeft.x = x - GetWidth() / 2;
    topLeft.y = y - GetHeight() / 2;
    bottomRight.x = x + GetWidth() / 2;
    bottomRight.y = y + GetHeight() / 2;
  }

  /**
   * Sets new position for this Rect. Position is the coordinates for the
   * Rect's top left corner. Other coordinates are adjusted accordingly (width
   * and height remain unchanged).
   * @param x x-coordinate for new position.
   * @param y y-coordinate for new position.
   */
  void SetPosition( T x, T y ) {
    topLeft.x = x;
    topLeft.y = y;
    bottomRight.x = topLeft.x + GetWidth();
    bottomRight.y = topLeft.y + GetHeight();
  }

  /**
   * Scales this Rect to new size. For example, passing a Vector of (3,5) will
   * make the Rect have a width of 3 and height of 5. Position of Rect doesn't
   * change.
   * @param x amount to scale width wise.
   * @param y amount to scale height wise.
   */
  void Scale( T x, T y ) {
    bottomRight.x = topLeft.x + x;
    bottomRight.y = topLeft.y + y;
  }

  Vector<T> topLeft;
  Vector<T> bottomRight;
};

//@{
/**
 * Common Rect types.
 */
typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;
//@}


/**
 * Returns true if two rectangles passed intersect. To intersect means that
 * part of one rectangle crosses at least one unit over the part of another
 * rectangle. Exact overlapping is not considered an intersection.
 */
template< typename T >
bool Intersect( const Rect<T> &a, const Rect<T> &b ) const {
  return ( a.topLeft.x < b.bottomRight.x &&
           a.topLeft.y < b.bottomRight.y &&
           a.bottomRight.x > b.topLeft.x &&
           a.bottomRight.y > b.topLeft.y );
}


/**
 * Compare equality of top left and bottom right corners of rectangles.
 */
bool operator==( const IntVec &a, const IntVec &b ) {
  return ( a.topLeft == b.topLeft && a.bottomRight == b.bottomRight );
}


/**
 * Compare inequality of top left and bottom right corners of rectangles.
 */
bool operator!=( const IntVec &a, const IntVec &b ) {
  return !( a == b );
}


} // namespace core
} // namespace nt

#endif // RECT_H

