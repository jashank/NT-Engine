#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "BaseState.h"

/************************************************
TitleState
-Inherits from BaseState
-Contains all TitleState Content
************************************************/
class TitleState : public BaseState {
 public:
  virtual ~TitleState();

  static BaseState* GetInstance();
  static void DestroyInstance();

  /************************************************
  Init
  -Loads the resources required by TitleState
  ************************************************/
  virtual void Init();

  /************************************************
  CleanUp
  -Releases the resources acquired by TitleState
  ************************************************/
  virtual void CleanUp();

  /************************************************
  Pause
  -Pauses the TitleState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the TitleState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle TitleState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update TitleState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render TitleState's frame
  ************************************************/
  virtual void Render();

 private:	
  TitleState();
  TitleState( const TitleState &title );
  TitleState& operator=( const TitleState &title );

  static TitleState *m_instance;
};

#endif