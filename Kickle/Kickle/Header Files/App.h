#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

extern "C" {
#include "lua.h"
}

#include "ResourceManager.h"
#include "StateManager.h"
#include "BaseState.h"
#include "AnimData.h"

#include "BasicTypeDefs.h"

class Input;
class StateManager;
class AnimData;

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
    Uint width, 
    Uint height, 
    Uint framerate 
  );


  ~App();


  // Destroys the instance of App
  static void DestroyApp();

  // Draws any Drawable object to the screen
  void Draw( const sf::Drawable &object );

  // Returns an instance of App, insuring that only one instance
  // of App exists at a time
  static App* GetApp();

  // Returns the change in time since last frame
  float GetDeltaTime() const;

  // Returns event in app
  const sf::Event &GetEvent() const;  

  // Returns input in app.
  const sf::Input &GetInput() const;

  // Returns an Image given a filename, insuring that no 
  // duplicate Image is loaded into memory
  sf::Image& LoadImage( const std::string &filename );

  // Returns a Sound given a filename, insuring that no
  // duplicate Sound is loaded into memory
  sf::SoundBuffer& LoadSound( const std::string &filename );

  // Returns a Music given a filename, insuring that no
  // duplicate Music is loaded into memory
  sf::Music& LoadMusic( const std::string &filename );

  // Returns an AnimData given a filename, insuring that no 
  // duplicate AnimData is loaded into memory
  AnimData& LoadAnim( const std::string &filename );

  // Contains the main game loop
  void Run();

  // Sets the screen's clear color
  void SetClearColor( const sf::Color& color );

  // Wrapper for StateManager SetNextState
  void SetNextState( BaseState *state );


 private:
  App( 
    const std::string &title, 
    Uint width, 
    Uint height, 
    Uint framerate 
  );


  App( const App &app );


  static App *m_instance; //Single instance of App

  char m_fpsStrBuff[50]; //String for displaying current FPS
  float	m_deltaTime; //Time in seconds spent on last frame render
  float	m_fps; //Frames per Second


  //Resource Managers
  ResourceManager< sf::Image > m_images;
  ResourceManager< sf::SoundBuffer > m_sounds;
  ResourceManager< sf::Music > m_music;
  ResourceManager< AnimData > m_anims;


  sf::Color m_clearColor; //Color to clear the screen to
  sf::Event	m_event; //holds the most current event
  sf::RenderWindow	m_window; //SFML's window

  StateManager m_stateManager; //Keeps track of current state
};

#endif