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

  //GUI

  // Side Panel
  void CreateSidePanel();
  // Buttons.
  void CreateButtons();
  // Button Actions
  static void SaveFile();
  static void SaveAsFile();
  static void LoadFile();
  static void LoadTileSheet();
  static void TileLayer();
  static void CollisionLayer();
  static void GameObjectLayer();

 private:
   void DrawGrid();
   void DrawSideBar();
   void DrawToolbar();
  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  EditorState();
  EditorState( const EditorState &title );
  EditorState& operator=( const EditorState &title );

  static EditorState *m_instance; // EditorState has one instance

  Level m_level;

  Button m_back; // Go back to title screen.
  Button m_saveMap; // Button to save game
  Button m_saveAsMap;
  Button m_loadMap;
  Button m_loadTileSheet;
  Button m_tileLayer;
  Button m_collisionLayer;
  Button m_gameObjectLayer;

  sf::Font *m_font; // Font for the title menu

};

#endif