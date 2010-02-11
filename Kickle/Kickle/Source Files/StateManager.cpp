#include "StateManager.h"
#include "BaseState.h"
#include "Utilities.h"

StateManager::StateManager() {
}

StateManager::~StateManager() {
	CleanUp();
}

BaseState* StateManager::operator->() {
	return m_states.top();
}

void StateManager::ChangeState( BaseState *state ) {
	//If m_states isn't empty and state is initialized, 
	//then cleanup the top state and pop it off
	if( !m_states.empty() && m_states.top()->IsInit() ) {
		m_states.top()->CleanUp();
		m_states.pop();
	}

	//Add the passed in state to the stack, 
	//and initialize it if necessary
	m_states.push( state );
	if( !m_states.top()->IsInit() ) {
		m_states.top()->Init();
	}
}
void StateManager::PushState( BaseState *state ) {
	//If the m_states isn't empty, pause the top state
	if ( !m_states.empty() )  {
		m_states.top()->Pause();
		if( m_states.top()->IsInit() ) {
			m_states.top()->CleanUp();
		}
	}

	//Add the passed in state to the stack and initialize if necessary
	m_states.push( state );
	if( !m_states.top()->IsInit() ) {
		m_states.top()->Init();
	}
}

void StateManager::PopState() {
	//If m_states isn't empty, 
	//cleanup the top state and take it off the stack
	if ( !m_states.empty() ) {
		if( m_states.top()->IsInit() ) {
			m_states.top()->CleanUp();
		}
		m_states.pop();
	}
	
	//Now, if m_states isn't empty, resume the top most state
	if ( !m_states.empty() ) {
		if( !m_states.top()->IsInit() ) {
			m_states.top()->Init();
		}
		m_states.top()->Resume();
	}
}

void StateManager::CleanUp() {
	while( !m_states.empty() ) {
		m_states.top()->CleanUp();
		m_states.pop();
	}
}