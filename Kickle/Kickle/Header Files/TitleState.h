#ifndef TITLESTATE_H
#define TITLESTATE_H
#include "BaseState.h"

/************************************************
	TitleState
	-Inherits from BaseState
	-Contains all TitleState Content
*************************************************/
class TitleState : public BaseState {
public:
	virtual ~TitleState();

	static BaseState* GetInstance();
	static void DestroyInstance();

	virtual void Init();
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:	
	TitleState();
	TitleState( const TitleState &title );
	TitleState& operator=( const TitleState &title );

	static TitleState *m_instance;
};

#endif