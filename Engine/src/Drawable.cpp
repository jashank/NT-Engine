#include "Drawable.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp> // For opengl

namespace nt {
namespace graphics {

/**************************
 * Constructors
 *************************/
Drawable::Drawable( float posX, float posY, float lastPosX, float lastPosY )
  : m_lerpBlend( 0.0 ), m_lastPos( lastPosX, lastPosY ) {
  SetPosition( posX, posY );
}

Drawable::Drawable( 
  const nt::core::FloatVec &pos,
  const nt::core::FloatVec &lastPos 
) : m_lerpBlend( 0.0 ), m_lastPos( lastPos ) {
  SetPosition( pos.x, pos.y );
}

/***************************
 * Public Methods
 **************************/
// Almost exactly the same as SFML draw, just less checking because
// of features not used and has lerping. See SFML docs for more info.
void Drawable::Draw( sf::RenderTarget &target ) {
  // Save logical position before lerping so can go back to it after
  // rendering
  const sf::Vector2f logicPos = GetPosition();
  Lerp( m_lerpBlend );

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glMultMatrixf( GetMatrix().Get4x4Elements() );
  
  sf::Blend::Mode blendMode = GetBlendMode();
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

  Render();

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  SetPosition( logicPos );
  m_lastPos.x = logicPos.x;
  m_lastPos.y = logicPos.y;
}


void Drawable::SetLerpBlend( float alpha ) {
  m_lerpBlend = alpha;
}


void Drawable::SetStartingPos( float x, float y ) {
  m_lastPos.x = x;
  m_lastPos.y = y;
  SetPosition( x, y );
}

/*************************
 * Private Methods
 ************************/
void Drawable::Lerp( float alpha ) {
  sf::Vector2f current;
  current.x = GetPosition().x * alpha;
  current.y = GetPosition().y * alpha;

  sf::Vector2f last;
  float diff = 1.0 - alpha;
  last.x = m_lastPos.x * ( diff );
  last.y = m_lastPos.y * ( diff );

  SetPosition( current.x + last.x, current.y + last.y );
}

} // graphics
} // nt
