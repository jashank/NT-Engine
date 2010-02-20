#include "Button.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "App.h"

/********************************************************
Constructors
********************************************************/
Button::Button( 
  const Action &action, 
  const sf::Vector2f &position,
  const sf::IntRect &clip
) 
  : m_action( action ), m_text( "" ) {
  Sprite::SetPosition( position );
  SetSubRect( clip );
}


Button::Button(
  const Action &action,
  const sf::Vector2f &position,
  const sf::IntRect &clip,
  const sf::Image &image
)
  : m_action( action ), m_text( "" ) {
  Sprite::SetPosition( position );
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


void Button::SetText( const sf::String &text ) {
  m_text = text;

  float buttonCenterX = GetSubRect().GetWidth()  / 2.f;
  float buttonCenterY = GetSubRect().GetHeight() / 2.f;
  SetCenter( buttonCenterX, buttonCenterY );

  float textCenterX = m_text.GetRect().GetWidth() / 2.f;
  float textCenterY = m_text.GetRect().GetHeight() / 2.f;
  m_text.SetCenter( textCenterX, textCenterY );

  m_text.SetPosition( GetPosition() );
}


/********************************************************
Protected Member Functions
********************************************************/
void Button::Render( sf::RenderTarget &Target ) const {
  Sprite::Render( Target );
  Target.Draw( m_text );
}
  