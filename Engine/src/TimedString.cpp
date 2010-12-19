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

#include "TimedString.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window.hpp>

#include "FontManager.h"

namespace nt {

/******************
 * Constructor
 *****************/
TimedString::TimedString()
  : m_size( 30.0 ), m_printTime( 0.0 ) {}

/********************************
 * Public Methods
 *******************************/
void TimedString::BufferText( const std::string &text ) {
  m_buffer = text;
}


void TimedString::SetPrintTime( float time ) {
  m_printTime = time;
}


void TimedString::StartPrint() {
  if ( m_printTime == 0.f ) {
    m_output = m_buffer;
    m_buffer.clear();
    return;
  }

  m_timer.Reset();
}


void TimedString::UpdatePrint() {
  if ( !m_buffer.empty() && m_timer.GetElapsedTime() > m_printTime ) {
    m_output.append( m_buffer.substr( 0, 1 ));
    m_buffer.erase( 0, 1 );
    m_timer.Reset();
  }
}


void TimedString::SetFont( const std::string &fontName ) {
  m_font = FontManager::GetFont( fontName );
}


void TimedString::SetSize( float size ) {
  m_size = size;
}

/**************************
 * Private Methods
 *************************/
// Straight from SFML's sf::String::Render function with some small changes.
// See SFML documentation for details.
void TimedString::Render( sf::RenderTarget &target ) const {
  if ( m_output.empty() ) {
    return;
  }

  const sf::Font *font = NULL;
  if ( m_font ) {
    font = m_font.get();
  } else {
    font = &sf::Font::GetDefaultFont();
  }

  // Set scaling factor to get actual size
  float charSize = static_cast<float>( font->GetCharacterSize() );
  float factor = m_size / charSize;
  glScalef(factor, factor, 1.0);

  // Bind font to texture
  font->GetImage().Bind();

  // Initialize rendering coordinates
  float x = 0.0;
  float y = charSize;

  // Draw one quad for each character
  glBegin(GL_QUADS);
  for ( unsigned int i = 0; i < m_output.size(); ++i ) {
    // Get current character and corresponding glyph
    char curChar = m_output[i];
    const sf::Glyph &curGlyph = font->GetGlyph( curChar );
    int distNextChar = curGlyph.Advance;
    const sf::IntRect &glyphBounds = curGlyph.Rectangle;
    const sf::FloatRect &glyphCoords = curGlyph.TexCoords;

    // Handle special characters
    switch (curChar) {
      case ' ': {
        x += distNextChar;
        continue;
      }
      case '\n': {
        x = 0;
        y += charSize;
        continue;
      }
      case '\t': {
        x += distNextChar * 4;
        continue;
      }
      case '\v': {
        y += charSize * 4;
        continue;
      }
      default: {}
    }

    // Draw textured quad for current character
    glTexCoord2f( glyphCoords.Left, glyphCoords.Top );
    glVertex2f( x + glyphBounds.Left, y + glyphBounds.Top );

    glTexCoord2f( glyphCoords.Left, glyphCoords.Bottom );
    glVertex2f( x + glyphBounds.Left, y + glyphBounds.Bottom );

    glTexCoord2f( glyphCoords.Right, glyphCoords.Bottom );
    glVertex2f( x + glyphBounds.Right, y + glyphBounds.Bottom );

    glTexCoord2f( glyphCoords.Right, glyphCoords.Top );
    glVertex2f( x + glyphBounds.Right, y + glyphBounds.Top );

    // Advance to next character
    x += distNextChar;
  }
  glEnd();
}

} // namespace nt
