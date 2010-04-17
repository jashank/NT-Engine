#include "InfoPanel.h"

InfoPanel::InfoPanel() {
}

InfoPanel::InfoPanel( float x, float y, float width, float height,
                      sf::Color baseColor, float outline,
                      sf::Color outlineColor, float padding ) {
  SetX( x );
  SetY( y );
  SetWidth( width );
  SetHeight( height );
  SetBaseColor( baseColor );
  SetOutlineColor( outlineColor );
  SetOutlineSize( outline );
  SetPadding( padding );

}

InfoPanel::~InfoPanel() {
}

void InfoPanel::Render() {
  App::GetApp()->Draw( sf::Shape::Rectangle( GetX(), GetY(), GetX()+GetWidth(),
                       GetY()+GetHeight(), m_baseColor, m_outline,
                       m_outlineColor ) );
  App::GetApp()->Draw( m_sprite );
  App::GetApp()->Draw( m_info );
}

void InfoPanel::Update() {
}

void InfoPanel::HandleEvents() {
}

void InfoPanel::SetFont( sf::Font font ) {
}

void InfoPanel::SetThumbnail( std::string imagePath ) {
  m_image.LoadFromFile( imagePath );
  m_sprite.SetImage( m_image );
  m_sprite.SetPosition( GetX() + m_padding, GetY() + m_padding );
  m_sprite.Resize( GetWidth()- 2*m_padding, GetHeight()/2);
}

void InfoPanel::SetInfo( std::string info ) {
  m_info.SetText( info );
  m_info.SetPosition( GetX() + m_padding, GetY()+GetHeight()/2 + m_padding );
}

void  InfoPanel::SetPadding( float padding ) {
  m_padding = padding;
}