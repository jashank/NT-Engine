#include "PlayState.h"

#include "App.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
PlayState *PlayState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
PlayState::PlayState()
 : m_kickle( "guttersluts.com" ) {
  //Test code
  m_kickle.SetAnimation( 1 );
  m_kickle.SetPosition( 336.0f, 372.0f );
  m_kickle.Play();
}


PlayState::~PlayState() {
	CleanUp();
}


BaseState* PlayState::GetInstance() {
	if( !m_instance ) {
		m_instance = new PlayState();
	}
	return m_instance;
}


void PlayState::DestroyInstance() {
	SAFEDELETE( m_instance );
}


void PlayState::Init() {
	SetInit( true );
	App::GetApp()->SetClearColor( sf::Color(0,49,139) );
	// Create a level with a place holder path.
	m_level.SetLevel( "levelPath.lua" );
}


void PlayState::CleanUp() {
	SetInit( false );
}


void PlayState::Pause() {

}


void PlayState::Resume() {

}


void PlayState::HandleEvents() {
  //TESTCODE
  static float speed = 1.0f;
  static int direction = 1; // 0 = Up, 1 = Down, 2 = Left, 3 = Right
  const sf::Input& inp = App::GetApp()->GetInput();
  if( inp.IsKeyDown( sf::Key::Up ) ) {
    direction = 0;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( 0.0f, -speed );
  }
  else if( inp.IsKeyDown( sf::Key::Down ) ) {
    direction = 1;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( 0.0f, speed );
  }
  else if( inp.IsKeyDown( sf::Key::Left ) ) {
    direction = 2;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( -speed, 0.0f );
    
  }
  else if( inp.IsKeyDown( sf::Key::Right ) ) {
    direction = 3;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( speed, 0.0f );
  }
  else if( inp.IsKeyDown( sf::Key::Z ) ) {
    m_kickle.SetAnimation( direction+8 );
  }
  else {
    m_kickle.SetAnimation( direction );
  }
}


void PlayState::Update() {
  //TESTCODE
  m_kickle.Update();
}


void PlayState::Render() {
	// Render the map.
	m_level.Render();
  App::GetApp()->Draw( m_kickle );
}