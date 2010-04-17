#ifndef STAGE_STATE_H
#define STAGE_STATE_H

#include "BaseState.h"
#include "Utilities.h"
#include "Button.h"
#include "BasicTypeDefs.h"
#include "tinyxml.h"

#include <iostream>
#include <vector>
#include <string>

#include "ListBox.h"
#include "InfoPanel.h"

#include <SFML/Graphics.hpp>

class StageState : public BaseState {
 public:
  virtual ~StageState();

  static StageState* GetInstance();
  static void DestroyInstance();

  // Loads the resources required by StageState
  virtual void Init();

  // Releases the resources acquired by StageState
  virtual void CleanUp();

  // Pauses the StageState
  virtual void Pause();

  // Unpauses the StageState
  virtual void Resume();

  // Handle StageState's user input events
  virtual void HandleEvents();

  // Update StageState's frame
  virtual void Update();

  // Render StageState's frame
  virtual void Render();

 private:
   StageState();
   static StageState *m_instance;
   
   //This allows a lot of the data from this state to be loaded in
   // instead of hard coded.
   bool SetStateData( std::string dataPath );

   // Loads the stage from the stage registery, plan to make this
   // automatically find the stages in the directory later on.
   bool LoadStages( std::string stageRegisteryPath );

   // Adds the stage to the vectors below, used in LoadStage
   void AddStage( std::string stageName, std::string stagePath, 
                  std::string stageThumb, std::string stageInfo );

   
   Container m_container;
   ListBox m_listbox;
   InfoPanel m_infoPanel;

   Uint m_previousSelection;
   static const float BUFFER_SIZE;
   static const float OUTLINE_SIZE;
   static const float MENU_BUFFER;

   sf::String m_playButtonText;
   sf::String m_exitButtonText;

   std::vector<std::string> m_stagePaths;
   std::vector<std::string> m_thumbnailPaths;
   std::vector<std::string> m_info;
};

#endif // STAGE_STATE_H