
#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

#include<vector>



class BaseState;

/************************************************
	StateManager
	-Handles switching states
*************************************************/
class StateManager {
public:
	StateManager();
	~StateManager();


	void ChangeState( BaseState *state );
	void PushState( BaseState *state );
	void PopState();

	BaseState* operator->();

	//void HandleEvents();
	//void Update();
	//void Render();

private:
	StateManager( const StateManager &stateMan );
	StateManager& operator=( const StateManager &stateMan );

	std::vector< BaseState* > m_states;
};

#endif
