#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>

#include "AnimData.h"
#include "KeyManager.h"
#include "ResourceManager.h"

#undef LoadImage // Undefines LoadImage macro from WinUser

class GameState;
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
    unsigned int width, 
    unsigned int height, 
    unsigned int framerate,
    std::string filePath
  );


  ~App();


  // Destroys the instance of App
  static void DestroyApp();

  // Draws any Drawable object to the screen
  void Draw( const sf::Drawable &object );

  // Returns an instance of App, insuring that only one instance
  // of App exists at a time
  static App* GetApp();

  // Returns GameState app is currently running
  GameState* GetCurrentState() const;

  // Returns the time since App was created.
  float GetTime() const;

  // Returns the change in time since last frame
  float GetDeltaTime() const;

  // Returns monitored Key matching with 'key'
  Key GetKey( sf::Key::Code key ) const;

  // Returns event in app
  const sf::Event &GetEvent() const;  

  // Returns input in app.
  const sf::Input &GetInput() const;

  // Returns an Image given a filename, insuring that no 
  // duplicate Image is loaded into memory
  sf::Image* LoadImage( const std::string &filename );

  // Returns a Sound given a filename, insuring that no
  // duplicate Sound is loaded into memory
  sf::SoundBuffer* LoadSound( const std::string &filename );

  // Returns a Music given a filename, insuring that no
  // duplicate Music is loaded into memory
  sf::Music* LoadMusic( const std::string &filename );

  // Returns an AnimData given a filename, insuring that no 
  // duplicate AnimData is loaded into memory
  AnimData* LoadAnim( const std::string &filename );

  // Contains the main game loop
  void Run();

  // Registers a key for m_keyManager to monitor
  void RegisterKey( sf::Key::Code key );

  //TEMPORARY - Sets the next state
  void SetNextState( const std::string &filepath );

  // Sets the screen's clear color
  void SetClearColor( const sf::Color& color );

 private:
  App( 
    const std::string &title, 
    unsigned int width, 
    unsigned int height, 
    unsigned int framerate,
    std::string filePath 
  );


  App( const App &app );


  static App *m_instance; //Single instance of App

  char m_fpsStrBuff[50]; //String for displaying current FPS
  float m_time; //Total time since app was first created.
  float	m_deltaTime; //Time in seconds spent on last frame render
  float	m_fps; //Frames per Second
  std::string m_filePath; // Path to current level
  KeyManager m_keyManager; //Keeps track of how long registered keys are held
  
  //Resource Managers
  ResourceManager< sf::Image > m_images;
  ResourceManager< sf::SoundBuffer > m_sounds;
  ResourceManager< sf::Music > m_music;
  ResourceManager< AnimData > m_anims;

  sf::Color m_clearColor; //Color to clear the screen to
  sf::Event	m_event; //holds the most current event
  sf::RenderWindow	m_window; //SFML's window
  
  // TEMPORARY
  bool m_nextStateSet;
  std::string m_nextStatePath;

  GameState *m_currentState;
};

#endif
