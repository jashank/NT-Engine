#include "StateManager.h"
#include "BaseState.h"
#include "Utilities.h"

/************************************************
Public Methods
************************************************/
StateManager::StateManager( BaseState* initialState )
 : m_activeState(0), 
   m_nextState(0) {
	   SetActiveState( initialState );
}

void StateManager::HandleStateChange() {
	//If a state change has been posted
	if ( m_nextState )  {
		CloseActiveState();
		SetActiveState( m_nextState );

		//Posted state change has been handled so clear the next state
		m_nextState = 0;
	}
}

BaseState* StateManager::operator->() {
	return m_activeState;
}

void StateManager::PostStateChange( BaseState *state ) {
	m_nextState = state;
}

/************************************************
Private Methods
************************************************/
void StateManager::CloseActiveState() {
	//Pause active state and clean it up
	m_activeState->Pause();
	if( m_activeState->IsInit() ) {
		m_activeState->CleanUp();
	}
}

void StateManager::SetActiveState( BaseState* state ) {
	//Assign the new state, initialize it, and resume the state
	m_activeState = state;
	if( !m_activeState->IsInit() ) {
		m_activeState->Init();
	}
}
