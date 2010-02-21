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
PlayState::PlayState() {

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

}


void PlayState::Update() {

}


void PlayState::Render() {
	// Render the map.
	m_level.Render();
}