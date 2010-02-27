#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BaseState.h"
#include "GameObject.h"
#include "Level.h"

/************************************************
PlayState
-Inherits from BaseState
-Contains all TitleState Content
************************************************/
class PlayState : public BaseState {
 public:
  virtual ~PlayState();

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
  -Pauses the PlayState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the PlayState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle PlayState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update PlayState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render PlayState's frame
  ************************************************/
  virtual void Render();

 private:	
  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  PlayState();
  PlayState( const PlayState &title );
  PlayState& operator=( const PlayState &title );
  
  static PlayState *m_instance; // PlayState has one instance
  Level m_level;

  //TESTCODE
  GameObject m_kickle;
  GameObject m_slime;
};

#endif