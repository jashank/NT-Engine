
#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

class BaseState;

/************************************************
StateManager
-Handles switching states
************************************************/
class StateManager {
 public:
	StateManager();

	/************************************************
	operator->
	-returns the active state
	************************************************/
	BaseState* operator->();

	/************************************************
	RefreshState
	-Applies the state change
	-Should not be called in the middle of accessing
	 methods of the currently active state.
	************************************************/
	void RefreshState();

	/************************************************
	SetNextState
	-Sets the next state to switch to upon the
	 next call to HandleStateChange()
	************************************************/
	void SetNextState( BaseState *state );

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
	void SetActiveState( BaseState *state );

	BaseState *m_activeState;			
	BaseState *m_nextState;			
};

#endif
