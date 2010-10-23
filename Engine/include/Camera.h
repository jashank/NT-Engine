#ifndef CAMERA_H
#define CAMERA_H

#include "Rect.h"
#include "Vector.h"

class lua_State;

/**
 * Camera is useful (if not necessary) for States larger than the window
 * size. It supplies various functions to do camera like things (speed
 * adjustment, moving area of focus) and allows clients to know where its
 * focus is on the State map. Any dimensions retrieved by Camera are
 * guaranteed to be in range on the map.
 * Camera's speed starts off as instantaneous.
 */
class Camera {
 public:
  Camera();

  /** Resets the window view. */
  ~Camera();

  /**
   * Move if needs to move.
   * @param dt delta time - amount of time to step forward
   */
  void Update( float dt );

  /**
   * Returns a modified rectangle representation of the Camera's current
   * view. Parameters passed allow client to "extend" the rectangle in
   * both dimensions. So if the camera's leftmost tile was at 3, and its
   * rightmost tile was at 12, and the client passed an argument of 2 to the
   * x parameter, the rectangle returned would have a top left x of 1 and
   * bottom right x of 14. However, the dimensions returned will stay in
   * range of the state, so if the leftmost tile was 0, the returned
   * rectangle's leftmost parameter would still be 0.
   * @param x number of tiles to extend in -x and +x directions
   * @param y number of tiles to extend in -y and +y directions
   * @return Rectangle representation of Camera's current view with
   * adjustments made regarding arguments passed. Units in tiles.
   */
  nt::core::IntRect GetAdjustedFocus( int x, int y ) const;

  /**
   * Given a FloatRect containing points on the screen (in pixels), returns
   * tiles on Camera that the rectangle covers.
   * @return Rectangle representation of Camera's current view, units in
   * tiles.
   */
  nt::core::IntRect GetTileOverlap( const nt::core::FloatRect &rect ) const;

  /**
   * Spans camera to view number of tiles passed in each direction.
   * For example, passing (5, 5) would have the camera view 5 tiles from 
   * the current leftmost tile in the x direction and 5 tiles from the
   * current topmost tile in the y direction.
   * If span exceeds dimensions of State map then will automatically adjust
   * to fit in maximum bounds.
   */
  void Span( int xSpan, int ySpan );

  //@{
  /**
   * Lua-related functions. See State API documentation for details. Note
   * that these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaSpan( lua_State *L );

  int LuaOffset( lua_State *L );

  int LuaSetCenter( lua_State *L );

  int LuaSetSpeed( lua_State *L );

  int LuaSpeedUp( lua_State *L );

  int LuaSlowDown( lua_State *L );
  //@}

 private:
  /**
   * This function should be used to set an offset for the camera when the
   * user calls for it to be moved. It checks to make sure that the offset
   * position will still be in bounds, and if it won't be, it adjusts
   * the offset to move as far as possible while still being in bounds.
   * @param x x-wise offset in tiles.
   * @param y y-wise offset in tiles.
   */
  void SetOffset( int x, int y );

  /** Whether camera is moving to a new position. */
  bool m_moving;

  /** Speed in pixels per second at which the camera is traveling. */
  float m_speed;

  /** Holds tile coordinates of top left along with width/height of view. */
  nt::core::IntRect m_view;

  /** Holds offset values in pixels when user moves camera. */
  nt::core::IntVec m_offset;

  /** Distance camera has traveled horizontally and vertically in pixels. */
  nt::core::FloatVec m_distance;

};

#endif // CAMERA_H

