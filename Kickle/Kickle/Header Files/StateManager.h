
#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

//#include<vector>
#include <stack>


class BaseState;

/************************************************
StateManager
-Handles switching states
************************************************/
class StateManager {
public:
	StateManager();
	~StateManager();

	/************************************************
	ChangeState
	-Replaces the top of the stack
	************************************************/
	void ChangeState( BaseState *state );

	/************************************************
	PushState
	-Adds new active state, pausing last active state
		*Pauses the active state
		*Cleans Up active state
		*Then adds a new active to top of m_states
		*Initializes the new active state
	************************************************/
	void PushState( BaseState *state );

	/************************************************
	PopState
	-Removes top state resuming state prior on stack
		*Cleans up active state
		*Removes it from top of stack
		*ReInitializes the new active state
		*Unpauses the new active state
	************************************************/
	void PopState();


	/************************************************
	operator->
	-returns a pointer to the active state
	************************************************/
	BaseState* operator->();


	/************************************************
	CleanUp()
	-Cleans up all states on stack then pops them off
	************************************************/
	void CleanUp();

private:
	//Restrict copying and assignment
	StateManager( const StateManager &stateMan );
	StateManager& operator=( const StateManager &stateMan );


	//Stack that contains the states 
	//The active state is always the top
	std::stack< BaseState* > m_states;			
};

#endif
