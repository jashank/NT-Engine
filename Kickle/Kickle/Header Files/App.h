#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "ResourceManager.h"
#include "StateManager.h"

class StateManager;

//Template specialization to handle sf::Music's OpenFromFile() 
//instead of the typical LoadFromFile()
template<>
sf::Music* ResourceLoader<sf::Music>::Load( const std::string& filename );


/************************************************
App
-Initializes SFML
-Initializes ResoureManagers
-Initializes StateManager
-Calculates DeltaTime
-Calculates FPS
-Polls for events
-Loads Resources
************************************************/
class App {
 public:
  static App* CreateApp( 
    const std::string &title, 
    unsigned int width, 
    unsigned int height, 
    unsigned int framerate 
  );
  ~App();

  /************************************************
  DestroyApp
  -Destroys the instance of App
  ************************************************/
  static void DestroyApp();

  /************************************************
  Draw
  -Draws any Drawable object to the screen
  ************************************************/
  void Draw( const sf::Drawable &object );

  /************************************************
  GetApp
  -Returns an instance of App
  -Insures that only one instance of App exists
   at a time
  ************************************************/
  static App* GetApp();

  /************************************************
  GetDeltaTime
  -Returns the change in time since last frame
  ************************************************/
  float GetDeltaTime() const;

  /************************************************
  GetMouseX, GetMouseY
  - Returns mouse's current x position.
  - Returns mouse's current y position.
  ************************************************/
  int GetMouseX() const;
  int GetMouseY() const;
  
  /************************************************
  LoadImage
  -Returns an Image given a filename
  -Insures that no duplicate Sound is loaded into
  memory
  ************************************************/
  sf::Image& LoadImage( const std::string &filename );\

  /************************************************
  LoadSound
  -Returns an Sound given a filename
  -Insures that no duplicate Sound is loaded into
  memory
  ************************************************/
  sf::SoundBuffer& LoadSound( const std::string &filename );

  /************************************************
  LoadMusic
  -Returns an Music given a filename
  -Insures that no duplicate Sound is loaded into
  memory
  ************************************************/
  sf::Music& LoadMusic( const std::string &filename );

  /************************************************
  Run
  -Contains the main game loop
  ************************************************/
  void Run();

  /************************************************
  SetClearColor
  -Sets the screen's clear color
  ************************************************/
  void SetClearColor( const sf::Color& color );

 private:
  App( 
    const std::string &title, 
    unsigned int width, 
    unsigned int height, 
    unsigned int framerate 
  );
  App( const App &app );

  static App *m_instance;

  char m_fpsStrBuff[50];
  sf::Event	m_event;
  float	m_deltaTime;
  float	m_fps;

  //Resource Managers
  ResourceManager< sf::Image > m_images;
  ResourceManager< sf::SoundBuffer > m_sounds;
  ResourceManager< sf::Music > m_music;

  sf::RenderWindow	m_window;
  sf::Color m_clearColor;


  StateManager m_stateManager;
};

#endif