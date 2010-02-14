#include "StateManager.h"
#include "BaseState.h"
#include "Utilities.h"

/************************************************
Public Methods
************************************************/
StateManager::StateManager()
 : m_activeState(0), 
   m_nextState(0) {
}

BaseState* StateManager::operator->() {
	return m_activeState;
}

void StateManager::RefreshState() {
	//If a state change is needed
	if ( m_nextState )  {
		CloseActiveState();
		SetActiveState( m_nextState );

		//State change has been handled so clear the next state
		m_nextState = 0;
	}
}

void StateManager::SetNextState( BaseState *state ) {
	m_nextState = state;
}

/************************************************
Private Methods
************************************************/
void StateManager::CloseActiveState() {
	if( m_activeState ) {
		//Pause active state and clean it up
		m_activeState->Pause();
		if( m_activeState->IsInit() ) {
			m_activeState->CleanUp();
		}
	}
}

void StateManager::SetActiveState( BaseState* state ) {
	//Assign the new state, initialize it, and resume the state
	m_activeState = state;
	if( !m_activeState->IsInit() ) {
		m_activeState->Init();
	}
}
