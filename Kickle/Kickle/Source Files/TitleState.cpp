#include "TitleState.h"

#include <SFML/Graphics/Font.hpp>

#include "App.h"
#include "PlayState.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
TitleState *TitleState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
TitleState::TitleState()
  : m_title( "Kickle Cubicle" ) {
  CreateButtons();

  m_title.SetSize( 72 );
  m_title.SetPosition( 190.f, 10.f );
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
  
  App::GetApp()->SetClearColor( sf::Color(0,0,0) );

  m_font = new sf::Font();
  m_font->LoadFromFile( "Content/Core/Fonts/MICKEY.TTF" );
  
  m_title.SetFont( *m_font );
  
  sf::String buttonText( "Play", *m_font, 60 );
  m_play.SetText( buttonText );
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
  if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left )) {
    if ( m_play.ContainsCursor( 
          App::GetApp()->GetInput().GetMouseX(), 
          App::GetApp()->GetInput().GetMouseY()
         )) {
      m_play.Activate();
    }
  }
}


void TitleState::Update() {

}


void TitleState::Render() {
  App::GetApp()->Draw( m_title );
  App::GetApp()->Draw( m_play );
}


/************************************************
Private Member Functions
************************************************/

void TitleState::Play() {
  App::GetApp()->SetNextState( PlayState::GetInstance() );
}


void TitleState::CreateButtons() {
  const float PLAY_WIDTH = 200.f;
  const float PLAY_HEIGHT = 75.f;
  sf::Vector2f playPosition( 400.f, 250.f );
  sf::IntRect playRect( 
    static_cast< int >( playPosition.x ), 
    static_cast< int >( playPosition.y ), 
    static_cast< int >( playPosition.x + PLAY_WIDTH ), 
    static_cast< int >( playPosition.y + PLAY_HEIGHT )
  );
  m_play = Button( Play, playPosition, playRect );
}