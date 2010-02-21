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
	App::GetApp()->SetClearColor( sf::Color(0,0,0) );
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
	m_level.Render();
}