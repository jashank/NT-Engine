#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "BaseState.h"
#include "Utilities.h"
#include "BasicTypeDefs.h"
#include "tinyxml.h"

#include <vector>
#include <string>


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

  int m_numLevels;
  int m_currentLevel;
  int m_nextLevel;

  std::vector< std::string > m_levelPaths;

  // Loads the current stage
  bool LoadStage( std::string currentStage );
  void AddLevel( std::string levelPath );
};

#endif // WORLD_STATE_H