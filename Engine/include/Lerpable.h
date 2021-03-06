/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_LERPABLE_H
#define NT_LERPABLE_H

#include <SFML/Graphics/Drawable.hpp>

#include "Point.h"

namespace sf {
  class RenderTarget;
}

namespace nt {

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
  FloatPoint m_logicPos;

  /** Last (x,y) position drawable thing was in. */
  FloatPoint m_lastPos;
};

} // namespace nt

#endif // NT_LERPABLE_H

