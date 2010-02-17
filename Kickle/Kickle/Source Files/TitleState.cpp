#include "TitleState.h"

#include <SFML/Graphics/Font.hpp>

#include "App.h"
#include "Utilities.h"

TitleState *TitleState::m_instance = 0;

TitleState::TitleState()
  : m_title( "Kickle Cubicle" ),
    m_play( "Play" ) {
  m_title.SetSize( 60 );
  m_play.SetSize( 48 );

  m_title.Move( 256.f, 10.f );
  m_play.Move( 384.f, 110.f );
}

TitleState::~TitleState() {
	CleanUp();
}

BaseState* TitleState::GetInstance() {
	if( !m_instance ) {
		m_instance = new TitleState();
	}
	return m_instance;
}

void TitleState::DestroyInstance() {
	SAFEDELETE( m_instance );
}

void TitleState::Init() {
	SetInit( true );
  App::GetApp()->SetClearColor( sf::Color(255,0,0) );

  m_font = new sf::Font();
  m_font->LoadFromFile( "Resources\\Fonts\\MICKEY.TTF" );
  
  m_title.SetFont( *m_font );
  m_play.SetFont( *m_font );

}

void TitleState::CleanUp() {
	SetInit( false );
  App::GetApp()->SetClearColor( sf::Color() );

  
  SAFEDELETE( m_font );
}

void TitleState::Pause() {

}

void TitleState::Resume() {

}

void TitleState::HandleEvents() {

}

void TitleState::Update() {

}

void TitleState::Render() {
  App::GetApp()->Draw( m_title );
  App::GetApp()->Draw( m_play );
}