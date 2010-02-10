#ifndef BASESTATE_H
#define BASESTATE_H
//#include"StateManager.h"

/********************************************************
	BaseState
	-An abstract base class declarations for states
	-Will be inherited from create game states
	-All game states will be managed by the StateManager
*********************************************************/
class BaseState {
public:
	virtual ~BaseState();

	virtual void Init() = 0;
	virtual void CleanUp() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool IsInit() const;

protected:
	BaseState();
	void SetInit( bool initialized );

private:
	bool	m_initialized;
};

#endif
