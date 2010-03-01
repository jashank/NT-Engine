#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "BaseState.h"
#include "Level.h"

#include <SFML/Graphics/String.hpp>
class sf::Font;

#include "Button.h"
#include "BasicTypeDefs.h"

/************************************************
EditorState
-Inherits from BaseState
-Contains all EditorState Content
************************************************/
class EditorState : public BaseState {
 public:
  virtual ~EditorState();

  static BaseState* GetInstance();
  static void DestroyInstance();

  /************************************************
  Init
  -Loads the resources required by EditorState
  ************************************************/
  virtual void Init();

  /************************************************
  CleanUp
  -Releases the resources acquired by EditorState
  ************************************************/
  virtual void CleanUp();

  /************************************************
  Pause
  -Pauses the EditorState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the EditorState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle EditorState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update EditorState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render EditorState's frame
  ************************************************/
  virtual void Render();

 private:	
  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  EditorState();
  EditorState( const EditorState &title );
  EditorState& operator=( const EditorState &title );

  static EditorState *m_instance; // EditorState has one instance

  Level m_level;
};

#endif