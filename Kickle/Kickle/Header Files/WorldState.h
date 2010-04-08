#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "BaseState.h"

class WorldState : public BaseState{
 public:
  virtual ~WorldState();

  static WorldState* GetInstance();
  static void DestroyInstance();

  // Loads the resources required by WorldState
  virtual void Init();

  // Releases the resources acquired by WorldState
  virtual void CleanUp();

  // Pauses the WorldState
  virtual void Pause();

  // Unpauses the WorldState
  virtual void Resume();

  // Handle WorldState's user input events
  virtual void HandleEvents();

  // Update WorldState's frame
  virtual void Update();

  // Render WorldState's frame
  virtual void Render();

 private:
   WorldState();
   static WorldState *m_instance;
};

#endif // WORLD_STATE_H