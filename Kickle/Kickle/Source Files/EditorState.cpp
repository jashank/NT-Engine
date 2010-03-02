#include "EditorState.h"

#include <SFML/Graphics/Font.hpp>

#include "App.h"
#include "PlayState.h"
#include "Utilities.h"

/************************************************
Data Members
************************************************/
EditorState *EditorState::m_instance = 0;


/************************************************
Public Member Functions
************************************************/
EditorState::EditorState() {
  App::GetApp()->GetConfig()->SetXPad( 0 );
  App::GetApp()->GetConfig()->SetYPad( (App::GetApp()->GetConfig()->GetScreenHeight() - 
                                        App::GetApp()->GetConfig()->GetMapSize()*
                                        App::GetApp()->GetConfig()->GetTileSize()));

  CreateButtons();
  CreateSidePanel();
}


EditorState::~EditorState() {
	CleanUp();
}


BaseState* EditorState::GetInstance() {
	if( !m_instance ) {
		m_instance = new EditorState();
	}
	return m_instance;
}


void EditorState::DestroyInstance() {
	SAFEDELETE( m_instance );
}


void EditorState::Init() {
	SetInit( true );
  App::GetApp()->SetClearColor( sf::Color(0,49,139) );
  
  m_font = new sf::Font();
  m_font->LoadFromFile( "Content/Core/Fonts/MICKEY.TTF" );

  m_saveMap.SetText( sf::String( "Save Map", *m_font, 60 ) );
  m_loadMap.SetText( sf::String( "Load Map", *m_font, 60 ) );
}


void EditorState::CleanUp() {
	SetInit( false );
  App::GetApp()->SetClearColor( sf::Color() );
  SAFEDELETE( m_font );
  /*
  Return the values to their orignal state.
  */
  App::GetApp()->GetConfig()->SetXPad( (App::GetApp()->GetConfig()->GetScreenWidth() - 
                                      App::GetApp()->GetConfig()->GetMapSize()*
                                      App::GetApp()->GetConfig()->GetTileSize())/2 );
  App::GetApp()->GetConfig()->SetYPad( (App::GetApp()->GetConfig()->GetScreenHeight() - 
                                      App::GetApp()->GetConfig()->GetMapSize()*
                                      App::GetApp()->GetConfig()->GetTileSize())/2);
}


void EditorState::Pause() {

}


void EditorState::Resume() {

}


void EditorState::HandleEvents() {
   if ( App::GetApp()->GetInput().IsMouseButtonDown( sf::Mouse::Left ) ) {
   }
}


void EditorState::Update() {
  m_level.Update();
}


void EditorState::Render() {
  m_level.Render();
  DrawGrid();
  DrawToolbar();
  DrawSideBar();
}

void EditorState::CreateButtons() {
}

void EditorState::CreateSidePanel() {

}

void EditorState::DrawGrid() {
    for ( float i = 0; i <= Config::MAP_SIZE; i++ ) {
    App::GetApp()->Draw(sf::Shape::Line( i*Config::TILE_SIZE, 
                                        (float)App::GetApp()->GetConfig()->GetYPad(), 
                                        i*Config::TILE_SIZE, 
                                        (float)Config::MAP_SIZE*Config::TILE_SIZE+(float)App::GetApp()->GetConfig()->GetYPad(), 
                                        1 ,sf::Color::Black ));
    App::GetApp()->Draw(sf::Shape::Line( 0, i*Config::TILE_SIZE+(float)App::GetApp()->GetConfig()->GetYPad(),
                                        (float)Config::MAP_SIZE*Config::TILE_SIZE,
                                        i*Config::TILE_SIZE+(float)App::GetApp()->GetConfig()->GetYPad(), 
                                        1, sf::Color::Black ));
  }
}

void EditorState::DrawSideBar() {
  App::GetApp()->Draw(sf::Shape::Rectangle((float)App::GetApp()->GetConfig()->GetTileSize()*App::GetApp()->GetConfig()->GetMapSize(),
                                          (float)App::GetApp()->GetConfig()->GetYPad()+5.0f, // The outline size.
                                          (float)App::GetApp()->GetConfig()->GetScreenWidth(), 
                                          (float)App::GetApp()->GetConfig()->GetScreenHeight(),
                                          sf::Color(153,180,209), 5.0f, sf::Color::Black));
}

void EditorState::DrawToolbar() {
    App::GetApp()->Draw(sf::Shape::Rectangle(0.0f, 0.0f,
                                          (float)App::GetApp()->GetConfig()->GetScreenWidth(), 
                                          (float)App::GetApp()->GetConfig()->GetYPad(),
                                          sf::Color(153,180,209), 5.0f, sf::Color::Black));
}
