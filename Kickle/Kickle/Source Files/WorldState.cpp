#include "WorldState.h"
#include "Utilities.h"

WorldState *WorldState::m_instance = 0;

WorldState::WorldState() {
}

WorldState::~WorldState() {
  CleanUp();
}

WorldState* WorldState::GetInstance() {
  if( !m_instance ) {
		m_instance = new WorldState();
	}
	return m_instance;
}

void WorldState::DestroyInstance() {
  SAFEDELETE( m_instance );
}

// Loads the resources required by WorldState
void WorldState::Init() {
  SetInit( true );
}

// Releases the resources acquired by WorldState
void WorldState::CleanUp() {
  SetInit( false );
}

// Pauses the WorldState
void WorldState::Pause() {
}

// Unpauses the WorldState
void WorldState::Resume() {
}

// Handle WorldState's user input events
void WorldState::HandleEvents() {
}

// Update WorldState's frame
void WorldState::Update() {
}

// Render WorldState's frame
void WorldState::Render() {
}


