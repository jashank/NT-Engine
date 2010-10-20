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
 */
class Camera {
 public:
  Camera();

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
   * adjustments made regarding arguments passed.
   */
  nt::core::IntRect GetAdjustedFocus( int x, int y ) const;

  //@{
  /**
   * Lua-related functions. See State API documentation for details. Note
   * that these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaOffset( lua_State *L );

  int LuaSetCenter( lua_State *L );

  int LuaSetSpeed( lua_State *L );

  int LuaSpeedUp( lua_State *L );

  int LuaSlowDown( lua_State *L );
  //@}

 private:
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

