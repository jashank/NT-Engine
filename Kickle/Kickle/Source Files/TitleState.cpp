#include "TitleState.h"
#include "Utilities.h"

TitleState *TitleState::m_instance = 0;

TitleState::TitleState() {

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
}

void TitleState::CleanUp() {
	SetInit( false );
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

}