#include "PlayState.h"

#include "App.h"
#include "Utilities.h"
#include "Configuration.h"

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
	m_level.SetLevel( "Content\\Core\\Levels\\TestLevel.txt" );
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
  sf::Vector2f kicklePos = m_kickle.GetPosition();

  if( inp.IsKeyDown( sf::Key::Up ) && !m_level.IsTileSolid(sf::Vector2f(kicklePos.x, kicklePos.y-1)) ) {
    direction = 0;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( 0.0f, -speed );
  }
  else if( inp.IsKeyDown( sf::Key::Down ) && !m_level.IsTileSolid(sf::Vector2f(kicklePos.x, kicklePos.y+1)) ) {
    direction = 1;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( 0.0f, speed );
  }
  else if( inp.IsKeyDown( sf::Key::Left )  && !m_level.IsTileSolid(sf::Vector2f(kicklePos.x-1, kicklePos.y)) ) {
    direction = 2;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( -speed, 0.0f );
    
  }
  else if( inp.IsKeyDown( sf::Key::Right ) && !m_level.IsTileSolid(sf::Vector2f(kicklePos.x+1, kicklePos.y)) ) {
    direction = 3;
    m_kickle.SetAnimation( direction+4 );
    m_kickle.Move( speed, 0.0f );
  }
  else if( inp.IsKeyDown( sf::Key::Z ) ) {
    m_kickle.SetAnimation( direction+8 );

    int x_pos = ((int)kicklePos.x+24-Config::X_PAD)/48;
    int y_pos = ((int)kicklePos.y+48-Config::Y_PAD)/48;

    switch ( direction ) {
      case 0:
        y_pos--;
        break;
      case 1:
        y_pos++;
        break;
      case 2:
        x_pos--;
        break;
      case 3:
        x_pos++;
        break;
      default:
        break;
    }
    if ( m_level.GetTile(x_pos, y_pos) == 0 ) {
      m_level.SetTile( x_pos , y_pos , 1, 0 );
    }
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