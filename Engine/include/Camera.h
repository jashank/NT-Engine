#ifndef CAMERA_H
#define CAMERA_H

#include "Rect.h"

/**
 * Camera is useful (if not necessary) for States larger than the window
 * size. It supplies various functions to do camera like things (speed
 * adjustment, moving area of focus) and allows clients to know where its
 * focus is on the State map.
 */
class Camera {
 public:
  /**
   * Move if needs to move.
   * @param dt delta time - amount of time to step forward
   */
  void Update( float dt );

  /**
   * Sets a tile destination for to move to.
   */
  void SetDestination( int x, int y );

  /**
   * Sets speed (pixels per second) that camera moves.
   */
  void SetSpeed( float speed );

  /**
   * Increases camera's speed (pixels per second) by amount passed.
   */
  void SpeedUp( float speed );

  /**
   * Decreases camera's speed (pixels per second) by amount passed.
   * Camera's speed cannot be < 0.
   */
  void SlowDown( float speed );

  /**
   * Returns rectangle containing Camera's current focus. x and y members
   * are the top left tiles, while width and height are the width of its
   * focus in tiles.
   */
  const nt::core::IntRect &GetFocus();

 private:
  /** Speed in pixels per second at which the camera is traveling. */
  float m_speed;

  /** Holds tile coordinates of top left along with width/height of view. */
  nt::core::IntRect m_view;
};

#endif // CAMERA_H

