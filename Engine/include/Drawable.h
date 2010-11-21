#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics/Drawable.hpp>

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
    * Derived classes should define this.
    */
   virtual void Render( float alpha ) const = 0;
};

} // graphics
} // nt

#endif // DRAWABLE_H

