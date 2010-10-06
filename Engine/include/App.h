#ifndef APP_H
#define APP_H

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>

// TEMPORARY
extern "C" {
  #include "lua.h"
}

#include "AnimData.h"
#include "KeyManager.h"

class State;
class AnimData;

/**
 * Singleton that Controls application. Initializes SFML,  
 * and StateManager. Provides access to time tracking, FPS info, 
 * event polling.
 */
class App {
 public:
  static App* CreateApp(
    std::string title,
    int width,
    int height,
    int framerate,
    std::string filePath
  );

  ~App();

  /// Destroys the instance of App
  static void DestroyApp();

  /// Draws any Drawable object to the screen
  void Draw( const sf::Drawable &object );

  /// Returns an instance of App, insuring that only one instance
  /// of App exists at a time
  static App* GetApp();

  /// Returns State app is currently running
  State* GetCurrentState() const;

  /// Returns the time since App was created.
  float GetTime() const;

  /// Returns the change in time since last frame
  float GetDeltaTime() const;

  /// Returns monitored Key matching with 'key'
  Key GetKey( sf::Key::Code key ) const;

  /// Returns event in app
  const sf::Event &GetEvent() const;

  /// Returns input in app.
  const sf::Input* GetInput() const;

  /// Contains the main game loop
  void Run();

  /// Registers a key for m_keyManager to monitor
  void RegisterKey( sf::Key::Code key );

  ///TEMPORARY - Sets the next state
  void SetNextState( const std::string &filepath );

  /// TEMPORARY - Returns lua state held by App
  lua_State* GetLuaState() const;

  /// Sets the screen's clear color
  void SetClearColor( const sf::Color& color );

 private:
  App(
    const std::string &title,
    int width,
    int height,
    int framerate,
  std::string filePath // Temporary
  );

  /// Restricts copy constructor and assignment.
  App( const App &app );
  App& operator=( const App &app );

  static App *m_instance; //Single instance of App

  float m_time; //Total time since app was first created.
  float m_deltaTime; //Time in seconds spent on last frame render
  float m_fps; //Frames per Second
  KeyManager m_keyManager; //Keeps track of how long registered keys are held

  sf::Color m_clearColor; //Color to clear the screen to
  sf::Event m_event; //holds the most current event
  sf::RenderWindow  m_window; //SFML's window

  // TEMPORARY
  bool m_nextStateSet;
  std::string m_nextStatePath;
  State *m_currentState;
  std::string m_filePath;
  lua_State *m_luaState;
};

#endif
