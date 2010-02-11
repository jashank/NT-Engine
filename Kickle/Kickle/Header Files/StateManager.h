
#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

#include <stack>


class BaseState;

/************************************************
StateManager
-Handles switching states
************************************************/
class StateManager {
public:
	StateManager( BaseState* initialState );

	/************************************************
	HandleStateChange
	-Applies state change post to the active state
	-Should not be called in the middle of accessing
	 methods of the currently active state.
	************************************************/
	void HandleStateChange();

	/************************************************
	operator->
	-returns the active state
	************************************************/
	BaseState* operator->();

	/************************************************
	PostStateChange
	-Sets the next state to switch to upon the
	 next call to HandleStateChange()
	************************************************/
	void PostStateChange( BaseState *state );

private:
	//Restrict copying and assignment
	StateManager( const StateManager &stateMan );
	StateManager& operator=( const StateManager &stateMan );


	/************************************************
	CloseActiveState
	-Pauses and cleans up the active state
	************************************************/
	void CloseActiveState();

	/************************************************
	SetActiveState
	-Assigns a new active state, initializes it, and
	 resumes it
	************************************************/
	void SetActiveState( BaseState* state );

	BaseState* m_activeState;			
	BaseState* m_nextState;			
};

#endif
