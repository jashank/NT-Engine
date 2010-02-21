#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "BaseState.h"

#include <SFML/Graphics/String.hpp>
class sf::Font;

#include "Button.h"

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

  /************************************************
  Init
  -Loads the resources required by TitleState
  ************************************************/
  virtual void Init();

  /************************************************
  CleanUp
  -Releases the resources acquired by TitleState
  ************************************************/
  virtual void CleanUp();

  /************************************************
  Pause
  -Pauses the TitleState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the TitleState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle TitleState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update TitleState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render TitleState's frame
  ************************************************/
  virtual void Render();

 private:	
  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  TitleState();
  TitleState( const TitleState &title );
  TitleState& operator=( const TitleState &title );

  /************************************************
  Play
  - Function called when player clicks play.
  ************************************************/
  static void Play();
  
  Button m_play; // Button to play game
  sf::Font *m_font; // Font for the title menu
  sf::String m_title; // Title string

  static TitleState *m_instance; // TitleState has one instance
};

#endif