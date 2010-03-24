#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "BaseState.h"

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

  // Function assigned to Play button. Puts player into game.
  static void Play();

  // Function assigned to Edit button. Puts player into editor.
  static void Edit();

  // Creates the buttons for the Title Menu
  void CreateButtons();
  
  Button m_play; // Button to play game
  Button m_editor; // Button to enter editor
  sf::Font *m_font; // Font for the title menu
  sf::String m_title; // Title string

  static TitleState *m_instance; // TitleState has one instance
};

#endif