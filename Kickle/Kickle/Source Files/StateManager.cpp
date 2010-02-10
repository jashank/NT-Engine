#include "StateManager.h"
#include "BaseState.h"
#include "Utilities.h"

StateManager::StateManager() {
}

StateManager::~StateManager() {
	std::vector< BaseState* >::iterator i = m_states.end();
	for( ; i != m_states.begin(); ++i ) {
		(*i)->CleanUp();
		SAFEDELETE( *i );
	}
}

BaseState* StateManager::operator->() {
	return m_states.back();
}

//void StateManager::HandleEvents() {
//	//calls the top state's handleEvents function
//	m_states.back()->HandleEvents();
//}
//
//void StateManager::Render() {
//	//calls the top state's render function
//	m_states.back()->Render();
//}
//
//void StateManager::Update() {
//	//calls the top state's update function
//	m_states.back()->Update();
//}

void StateManager::ChangeState( BaseState *state ) {
	//If the states aren't empty and state is initialized, 
	//then cleanup the top state
	if( !m_states.empty() && m_states.back()->IsInit() ) {
		m_states.back()->CleanUp();
	}

	//Add the passed in state to the stack, 
	//and initialize it if necessary
	m_states.push_back( state );
	if( !m_states.back()->IsInit() ) {
		m_states.back()->Init();
	}
}
void StateManager::PushState( BaseState *state ) {
	//If the states aren't empty, pause the top state
	if ( !m_states.empty() )  {
		m_states.back()->Pause();
	}

	//Add the passed in state to the stack and initialize if necessary
	m_states.push_back( state );
	if( !m_states.back()->IsInit() ) {
		m_states.back()->Init();
	}
}

void StateManager::PopState() {
	//If the states aren't empty, 
	//cleanup the top state and take it off the stack
	if ( !m_states.empty() ) {
		if( m_states.back()->IsInit() ) {
			m_states.back()->CleanUp();
		}
		m_states.pop_back();
	}
	
	//Now, if states aren't empty, resume the top most state
	if ( !m_states.empty() ) {
		m_states.back()->Resume();
	}
}