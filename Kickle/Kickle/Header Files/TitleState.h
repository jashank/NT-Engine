#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "BaseState.h"
#include "Container.h"

#include <SFML/Graphics/String.hpp>
class sf::Font;

#include "Button.h"
#include "BasicTypeDefs.h"

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

  // Loads the resources required by TitleState
  virtual void Init();

  // Releases the resources acquired by TitleState
  virtual void CleanUp();

  // Pauses the TitleState
  virtual void Pause();

  // Unpauses the TitleState
  virtual void Resume();

  // Handle TitleState's user input events
  virtual void HandleEvents();

  // Update TitleState's frame
  virtual void Update();

  // Render TitleState's frame
  virtual void Render();

 private:	
  // Restricts construction, copy construction, and assignment
  TitleState();
  TitleState( const TitleState &title );
  TitleState& operator=( const TitleState &title );

  // Given the path to an xml file, opens it and determines information
  // needed for the TitleState
	bool SetTitle( const std::string &filePath );

  // Function assigned to Play button. Puts player into game.
  static void Play();

  // Creates the buttons for the Title Menu
  void CreateButtons();
  
  Button m_play; // Button to play game
  sf::Font *m_font; // Font for the title menu

  sf::Sprite m_snowflakes[50]; // A vector of snowflakes!
  float m_snowflakesSpeed[50]; // Each flake has a speed.
  sf::Image m_snowflakeImage; // The snow flake image

  Container m_container; // Main Container;
  static const Uint m_numFlakes = 50; // Number of snow flakes to be generated
  Uint m_snowflakeBuffer; // Screen width - width of a snow flake.

  static TitleState *m_instance; // TitleState has one instance
};

#endif