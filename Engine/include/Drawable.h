#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

namespace nt {
namespace graphics {

/**
 * Overrides sf::Drawable's Draw function to allow for interpolation via
 * an alpha value and remove the unnecessary need for a RenderTarget.
 */
class Drawable : public sf::Drawable {
 public:
  virtual ~Drawable() {}

  /**
   * Same functionality as sf::Drawable but calls Render with alpha value
   * for interpolation.
   * @param alpha blending factor between previous frame and current frame.
   * Should be between [0:1].
   */
  void Draw( float alpha );

  protected:
   /**
    * Derived classes should define this. Called by Draw after matrix OpenGL
    * matrix is set up and position is interpolated.
    */
   virtual void Render() const = 0;

  private:
   sf::Vector2f 
};

} // graphics
} // nt

#endif // DRAWABLE_H

