#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics/Drawable.hpp>

#include "Vector.h"

namespace sf {
  class RenderTarget;
}

namespace nt {
namespace graphics {

/**
 * Overrides sf::Drawable's Draw function to allow for lerping via an alpha
 * value.
 */
class Drawable : public sf::Drawable {
 public:
  Drawable() {}
  Drawable( 
    float posX, 
    float posY, 
    float lastPosX = 0.0, 
    float lastPosY = 0.0 
  );
  Drawable( const nt::core::FloatVec &pos, 
            const nt::core::FloatVec &lastPos = 
              nt::core::FloatVec( 0.0, 0.0 ));

  virtual ~Drawable() {}

  /**
   * Same functionality as sf::Drawable but lerps before calling Render.
   */
  virtual void Draw( sf::RenderTarget &target );

  /**
   * Sets blending factor for lerping this drawable object. Should be from
   * [0:1].
   */
  virtual void SetLerpBlend( float alpha );

  /**
   * If Drawable thing wasn't initialized with its starting position, then
   * should call this method to do so to allow proper lerping starting out.
   */
  void SetStartingPos( float x, float y );

 protected:
  /**
   * Derived classes should define this. Called by Draw after matrix OpenGL
   * matrix is set up and position is lerped.
   */
  virtual void Render() const = 0;

 private:
  /**
   * Sets current position to linearly interpolated position based on
   * current position, last position, and alpha value passed.
   */
  void Lerp( float alpha );

  /**
   * Get rid of unneeded pure Render function from sf::Drawable.
   */
  void Render( sf::RenderTarget &target ) const {}

  /** Current blending factor for linear interpolation. */
  float m_lerpBlend;

  /** Last (x,y) position drawable thing was in. */
  nt::core::FloatVec m_lastPos;
};

} // graphics
} // nt

#endif // DRAWABLE_H

