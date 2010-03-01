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
}


void EditorState::CleanUp() {
	SetInit( false );
  App::GetApp()->SetClearColor( sf::Color() );
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

}


void EditorState::Update() {
  m_level.Update();
}


void EditorState::Render() {
  /*
  Draw a grid.
  */
  m_level.Render();
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

