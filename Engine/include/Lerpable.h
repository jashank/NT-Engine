#ifndef LERPABLE_H
#define LERPABLE_H

#include <SFML/Graphics/Drawable.hpp>

#include "Vector.h"

namespace sf {
  class RenderTarget;
}

namespace nt {
namespace graphics {

/**
 * An sf::Drawable object that can be linearly interpolated before
 * being drawn. If this is done, it is the user's responsbility to undo
 * the linear interpolation afterwards because it affects the object's
 * logical position.
 */
class Lerpable : public sf::Drawable {
 public:
  Lerpable() {}
  Lerpable( 
    float posX, 
    float posY, 
    float lastPosX = 0.0, 
    float lastPosY = 0.0 
  );
  virtual ~Lerpable() {}

  /**
   * If Drawable thing wasn't initialized with its starting position, then
   * should call this method to do so to allow proper lerping starting out.
   */
  void SetStartingPos( float x, float y );

  /**
   * Sets current position to linearly interpolated position based on
   * current position, last position (since last lerp), and
   * alpha value passed.
   */
  void Lerp( float alpha );

  /**
   * Undoes lerp performed. Should be called after object has been drawn.
   */
  void Unlerp();

 protected:
  /**
   * Derived classes should define this. Called by Draw after matrix OpenGL
   * matrix is set up and position is lerped.
   */
  virtual void Render( sf::RenderTarget &target ) const = 0;

 private:
  /** 
   * Holds actual logical position while object's position is lerped for
   * rendering.
   */
  nt::core::FloatVec m_logicPos;

  /** Last (x,y) position drawable thing was in. */
  nt::core::FloatVec m_lastPos;
};

} // graphics
} // nt

#endif // DRAWABLE_H

