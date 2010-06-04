#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "BaseState.h"

#include <SFML/Graphics/String.hpp>
class sf::Font;

#include "Button.h"
#include "BasicTypeDefs.h"

class LevelState;

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

  // Loads resources required by EditorState
  virtual void Init();

  // Releases resources acquired by EditorState
  virtual void CleanUp();

  // Pauses EditorState
  virtual void Pause();

  // Unpauses EditorState
  virtual void Resume();

  // Handles EditorState's user input events
  virtual void HandleEvents();

  // Update EditorState's frame
  virtual void Update();

  // Render EditorState's frame
  virtual void Render();

  //----GUI----

  // Side Panel
  void CreateSidePanel();
  // Buttons.
  void CreateButtons();
  // Button Actions
  static void SaveFile();
  static void LoadFile();
  static void LoadTileSheet();
  static void TileLayer();
  static void CollisionLayer();
  static void GameObjectLayer();
  static void Exit();

 private:
   void DrawGrid();

   void DrawSideBar();
   void SideBarEvents();

   void DrawToolbar();
   void ToolbarEvents();
   void GridEvents();

  // Restricts construction, copy construction, and assignment.
  EditorState();
  EditorState( const EditorState &title );
  EditorState& operator=( const EditorState &title );

  static EditorState *m_instance; // EditorState has one instance
  static LevelState *m_level; // Level to work with

  Button m_exit; // Go back to title screen.
  Button m_saveMap; // Button to save game
  Button m_loadMap;
  Button m_loadTileSheet;
  Button m_tileLayer;
  Button m_collisionLayer;
  Button m_gameObjectLayer;

  sf::Font *m_font; // Font for the title menu

};

#endif