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
  
  App::GetApp()->SetClearColor( sf::Color(255,0,255) );
}


void EditorState::CleanUp() {
	SetInit( false );
  App::GetApp()->SetClearColor( sf::Color() );
}


void EditorState::Pause() {

}


void EditorState::Resume() {

}


void EditorState::HandleEvents() {

}


void EditorState::Update() {

}


void EditorState::Render() {
}

