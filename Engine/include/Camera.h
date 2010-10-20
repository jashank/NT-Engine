#ifndef CAMERA_H
#define CAMERA_H

#include "Rect.h"
#include "Vector.h"

class lua_State;

/**
 * Camera is useful (if not necessary) for States larger than the window
 * size. It supplies various functions to do camera like things (speed
 * adjustment, moving area of focus) and allows clients to know where its
 * focus is on the State map.
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
   * Returns rectangle containing Camera's current focus. x and y members
   * are the top left tiles, while width and height are the width of its
   * focus in tiles.
   */
  const nt::core::IntRect &GetFocus();

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

