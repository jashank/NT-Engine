#include "Button.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "App.h"

/********************************************************
Constructors
********************************************************/
Button::Button( 
  const ButtonAction &action, 
  const sf::Vector2f &position,
  const sf::IntRect &clip,
  const sf::String &text
) 
  : m_action( action ), m_text( text ) {
  SetPosition( position );
  SetSubRect( clip );
}


Button::Button(
  const ButtonAction &action,
  const sf::Vector2f &position,
  const sf::IntRect &clip,
  const sf::String &text,
  const sf::Image &image
)
  : m_action( action ), m_text( text ) {
  SetPosition( position );
  SetSubRect( clip );
  SetImage( image );
}


/********************************************************
Public Member Functions
********************************************************/
bool Button::ContainsCursor( int cursorX, int cursorY ) const {
  float buttonLeft = GetPosition().x;
  float buttonRight = buttonLeft + GetSubRect().GetWidth();
  float buttonTop = GetPosition().y;
  float buttonBottom = buttonTop + GetSubRect().GetHeight();
  
  return (( cursorX > buttonLeft ) && ( cursorX < buttonRight ) &&
          ( cursorY > buttonTop ) && ( cursorY < buttonBottom ));
}


void Button::Activate() {
  m_action();
}


/********************************************************
Protected Member Functions
********************************************************/
void Button::Render( sf::RenderTarget &Target ) const {
  Sprite::Render( Target );
  Target.Draw( m_text );
}
  