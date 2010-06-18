#include "Container.h"


Container::Container() 
: m_outline( 0.0f ),
  m_baseColor( 0, 0, 0, 0 ),
  m_outlineColor( 0, 0, 0, 0 )  {
}

Container::Container( float x, float y, float width, float height ) 
: m_outline( 0.0f ),
  m_baseColor( 0, 0, 0, 0 ),
  m_outlineColor( 0, 0, 0, 0 ) {
  
    SetX( x*Config::GetScreenWidth() );
    SetY( y*Config::GetScreenHeight() );
    SetWidth( width*Config::GetScreenWidth() );
    SetHeight( height* Config::GetScreenHeight() );
}

Container::~Container() {
}

void Container::Render() {
  App::GetApp()->Draw( sf::Shape::Rectangle( GetX(), GetY(), GetX()+GetWidth(),
                       GetY()+GetHeight(), m_baseColor, m_outline,
                       m_outlineColor ) );

  App::GetApp()->Draw( m_sprite );
}

void Container::Update() {
  // Do on event

}

void Container::HandleEvents() {
}

void Container::SetBaseColor( sf::Color color ) {
  m_baseColor = color;
}

void Container::SetOutlineColor( sf::Color color ) {
  m_outlineColor = color;
}

void Container::SetOutlineSize( float outline ) {
  if ( outline >= 0.0f ) {
    m_outline = outline;
  }
}

void Container::SetImage( std::string imagePath ) {
  if ( !m_image.LoadFromFile( imagePath ) ) {
    LogErr( "Could Not Load Image: " + imagePath );
  } else {
    m_sprite.SetImage( m_image );
    m_sprite.SetPosition( GetX(), GetY() );
    m_sprite.Resize( GetWidth(), GetHeight() );
  }

}

void Container::AddController( Control *controller ) {

}

void Container::SetTheme( std::string themePath ) {
}