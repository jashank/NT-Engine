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
   
   // Loads the stage from the stage registery, plan to make this
   // automatically find the stages in the directory later on.
   bool LoadStages( std::string stageRegisteryPath );

   // Adds the stage to the vectors below, used in LoadStage
   void AddStage( std::string stageName, std::string stagePath, 
                  std::string stageThumb, std::string stageInfo );

   

   sf::String m_playButtonText;
   sf::String m_exitButtonText;

   // We use these data structures to store all the information
   // needed on the stage selection screen.
   std::vector<std::string> m_stageNames;
   std::vector<std::string> m_stagePaths;
   std::vector<std::string> m_thumbnailPaths;
   std::vector<std::string> m_info;

   // The current image / signature to be loaded
   sf::Image m_currentImage;
   sf::Sprite m_thumbSprite;

   sf::Font *m_font;
   sf::String m_currentStageName;
   sf::String m_currentInfo;

   int m_previousStage;
   int m_currentStage;
   int m_numStages;


   sf::Shape m_listPanel;
   sf::Shape m_infoPanel;
   // This is used to calculate the position of the selector and is used
   // to index the 15 current selectable items from the list.
   int m_currentSelection;

   // A bunch of constants for the graphical user interface

   // Only tries to draw the image if it is loaded, also 
   // allows us to set the sprite to a NO-IMAGE or default image.
   bool m_imageLoaded;
};

#endif // STAGE_STATE_H