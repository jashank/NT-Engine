#include "TitleState.h"

#include <SFML/Graphics/Font.hpp>

#include "App.h"
#include "Utilities.h"
#include "Configuration.h"
#include "StageState.h"
#include "LevelState.h"

/************************************************
Data Members
************************************************/
TitleState *TitleState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
TitleState::TitleState()
  : m_snowflakeBuffer( Configuration::GetScreenWidth()
                      -Configuration::GetTileSize() ),
    m_container( 0.0f, 0.0f, (float)Configuration::GetScreenWidth(),
                (float)Configuration::GetScreenHeight() ),
    m_frontalElements( 0.0f, 0.0f, (float)Configuration::GetScreenWidth(),
                      (float)Configuration::GetScreenHeight() ) {
  CreateButtons();
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
  SetTitle( "Content/Core/States/Title.xml" );
  
  StageState::DestroyInstance();
  LevelState::DestroyInstance();

  App::GetApp()->SetClearColor( sf::Color(0,0,0) );
  
  sf::String buttonText( "new game", *m_font, 60 );
  m_play.SetText( buttonText );

  sf::Clock Clock;

  m_snowflakeImage.LoadFromFile("Content/Core/Sheets/snowflake_48x48.png");
  m_snowflakeImage.CreateMaskFromColor( sf::Color( 4, 28, 90 ) );
  srand ( (unsigned int)Clock.GetElapsedTime() );

  for ( int i = 0; i < m_numFlakes; i++ ) {
    m_snowflakes[i] = sf::Sprite( m_snowflakeImage, 
                      sf::Vector2f( (float)(rand()%m_snowflakeBuffer), 
                                    (float)(-rand()%(8*48)-(48*2)) ) );
    m_snowflakes[i].SetSubRect( sf::IntRect( (i%5)*48, (i%2)*48, (i%5)*48+48, (i%2)*48+48 ) );
    float scale = (float)(1 + (float)i/m_numFlakes );
    m_snowflakes[i].SetScale( scale, scale );
    m_snowflakesSpeed[i] = scale;

  }

  m_container.SetImage( "Content/Core/Sheets/title_background.png" );
  m_frontalElements.SetImage( "Content/Core/Sheets/frontalElements.png" );
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
  } else if ( App::GetApp()->GetInput().IsKeyDown( sf::Key::Return ) ) {
    m_play.Activate();
  }
}


void TitleState::Update() {
  for ( int i = 0; i < m_numFlakes; i++ ) {
    if ( m_snowflakes[i].GetPosition().y > Configuration::GetScreenHeight() ) {
      m_snowflakes[i].SetPosition( (float)(rand()%m_snowflakeBuffer), 
                                   (float)(-rand()%(2*48)-(48*2)) );
    } else {
      m_snowflakes[i].Move( 0.2f*(-i%2), m_snowflakesSpeed[i] );
    }
  }
}


void TitleState::Render() {
  m_container.Render();

  for ( int i = 0; i < m_numFlakes; i++ )  {
    App::GetApp()->Draw( m_snowflakes[i] );
  }

  App::GetApp()->Draw( m_play );

  m_frontalElements.Render();
}


/************************************************
Private Member Functions
************************************************/

bool TitleState::SetTitle( const std::string &filePath ) {
  TiXmlDocument doc ( filePath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "media" );

  m_font = new sf::Font();
  m_font->LoadFromFile( root->FirstChildElement( "font" )->Attribute( "path" ) );

  return true;
}
  

void TitleState::Play() {
  App::GetApp()->SetNextState( StageState::GetInstance() );
}


void TitleState::CreateButtons() {
  const float PLAY_WIDTH = 350.f;
  const float PLAY_HEIGHT = 75.f;
  sf::Vector2f playPosition( 3*Configuration::GetScreenWidth()/5.0f, 
                             Configuration::GetScreenHeight()-450.0f );
  sf::IntRect playRect( 
    static_cast< int >( playPosition.x ), 
    static_cast< int >( playPosition.y ), 
    static_cast< int >( playPosition.x + PLAY_WIDTH ), 
    static_cast< int >( playPosition.y + PLAY_HEIGHT )
  );

  sf::Image buttonImage;
  m_play = Button( Play, playPosition, playRect );
}