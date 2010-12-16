#ifndef RECT_H
#define RECT_H

#include "Point.h"

namespace nt {

/**
 * Rectangle with (x,y) coordinates for top left and bottom right members.
 */ 
template<typename T>
struct Rect {
  Rect() {}
  /**
   * @param left leftmost x-coordinate.
   * @param top topmost y-coordinate.
   * @param right rightmost x-coordinate.
   * @param bottom bottommost y-coordinate.
   */
  Rect( T left, T top, T right, T bottom ) 
        : topLeft( left, top, 0 ), bottomRight( right, bottom, 0 ) {}

  /**
   * @param topLeft Point with coordinates for top left of rectangle.
   * @param bottomRight Point with coordinates for bottom right of rectangle.
   */
  Rect( const Point<T> &topLeft, const Point<T> &bottomRight )
        : topLeft( topLeft ), bottomRight( bottomRight ) {}

  ~Rect() {}
  
  /**
   * @return Distance between leftmost x and rightmost x.
   */
  T GetWidth() const {
    return ( bottomRight.x - topLeft.x );
  }

  /**
   * @return Distance between topmost y and bottommost y.
   */
  T GetHeight() const {
    return ( bottomRight.y - topLeft.y );
  }
  /**
   * @return Area in pixels of the rectangle (width*height).
   */
  T GetArea() const {
    return ( GetWidth() * GetHeight() );
  } 

  /**
   * @return Point containing coordinates of center of rectangle.
   */
  Point<T> GetCenter() const {
    T centerX = ( topLeft.x + bottomRight.x ) / 2;
    T centerY = ( topLeft.y + bottomRight.y ) / 2;
    return Point<T>( centerX, centerY, 0 );
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
   * Returns true if two rectangles passed intersect. To intersect means that
   * part of one rectangle crosses at least one unit over the part of another
   * rectangle. Exact overlapping is not considered an intersection.
   */
  bool Intersects( const Rect<T> &other ) const {
    return ( topLeft.x < other.bottomRight.x &&
             topLeft.y < other.bottomRight.y &&
             bottomRight.x > other.topLeft.x &&
             bottomRight.y > other.topLeft.y );
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
   * Scales this Rect to new size. For example, passing (3,5) will
   * make the Rect have a width of 3 and height of 5. Position of Rect doesn't
   * change.
   * @param x amount to scale width wise.
   * @param y amount to scale height wise.
   */
  void Scale( T x, T y ) {
    bottomRight.x = topLeft.x + x;
    bottomRight.y = topLeft.y + y;
  }

  /**
   * Returns true if Rects are at the same position and have the
   * same dimensions.
   */
  bool operator==( const Rect<T> &other ) const {
    return ( topLeft == other.topLeft && bottomRight == other.bottomRight );
  }

  /**
   * Return true if topLeft and bottomRight are not equal to other 
   * Rect's members.
   */
  bool operator!=( const Rect<T> &other ) const {
    return !( *this == other );
  }

  Point<T> topLeft;
  Point<T> bottomRight;
};

//@{
/**
 * Common Rect types.
 */
typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;
//@}

/**
 * Fits 'b' into 'a' by trimming any sides off of 'b' that extend outside of
 * 'a'. For example, if a's coordinates were (0, 0, 5, 5) and b's coordinates
 * were (0, 0, 5, 6), b would be changed to (0, 0, 5, 5).
 */
template<typename T>
void FitRect( const Rect<T> &a, Rect<T> &b ) {
  if ( b.topLeft.x < a.topLeft.x ) {
    b.topLeft.x = a.topLeft.x;
  }
  if ( b.topLeft.y < a.topLeft.y ) {
    b.topLeft.y = a.topLeft.y;
  }
  if ( b.bottomRight.x > a.bottomRight.x ) {
    b.bottomRight.x = a.bottomRight.x;
  }
  if ( b.bottomRight.y > a.bottomRight.y ) {
    b.bottomRight.y = a.bottomRight.y;
  }
}

} // namespace nt

#endif // RECT_H

