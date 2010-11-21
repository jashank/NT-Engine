#include "Drawable.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp> // For opengl

namespace nt {
namespace graphics {

// Almost exactly the same as SFML draw, just less checking because
// of features not used and calling Render with alpha value. See SFML
// docs for more information.
void Drawable::Draw( float alpha ) {
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glMultMatrix( GetMatrix().Get4x4Elements() );
  
  sf::Blend blendMode = GetBlendMode();
  if ( blendMode  == sf::Blend::None ) {
    glDisable( GL_BLEND );
  } else {
    glEnable( GL_BLEND );

    switch( blendMode ) {
      case sf::Blend::Alpha: {
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        break;
      }
      case sf::Blend::Add: {
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        break;
      }
      case sf::Blend::Multiply: {
        glBlendFunc( GL_DST_COLOR, GL_ZERO );
        break;
      }
      default: {}
    }
  }

  const sf::Color &color = GetColor();
  glColor4f( color.r / 255.0, color.g / 255.0, 
             color.b / 255.0, color.a / 255.0 );

  Render( alpha );

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}

} // graphics
} // nt
