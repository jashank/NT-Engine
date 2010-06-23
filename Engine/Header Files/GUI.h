#ifndef GUI_H
#define GUI_H

#include <string>

#include "boost/function/function1.hpp"

#include "AnimSprite.h"
#include "InputHandler.h"

class GameState;

/******************************************
 * GUI
 * Abstract base class for gui components
 * ***************************************/
class GUI {
 public:
  // Loads data from file passed, and puts GUI at screen location given
  GUI( const std::string &filepath, int x, int y );
  // Destructor pure virtual to make this class abstract
  virtual ~GUI() = 0;

  // Handles input defined by user in XML file
  virtual void HandleEvents();

  // Renders AnimSprite that all GUI share
  virtual void Render();

  // Lunar requirements
  static const char className[];

 private:
  // Restrict copy constructor and destructor
  GUI( const GUI& );
  GUI& operator=( const GUI& );

  // Loads GUI data from xml file. Returns true if load was successful.
  bool LoadData( const std::string &filepath );

  // Initializes lua for this gui component
  void InitLua();

  // Calls function named funcName from GUI's lua script
  void CallScriptFunc( std::string &funcName );

  // Function pointer to CallScriptFunc. Used with InputHandler.
  const boost::function1<void, std::string&> m_ptrCallScriptFunc;

  AnimSprite m_sprite; // Sprite representing GUI
  static GameState *m_state; // State GUI is currently in
  InputHandler m_input; // Handles key events for this GUI
  int m_id; // ID for lua
  std::string m_luaPath; // Path to lua script
  std::string m_name; // Name given to GUI component from xml file
};

#endif
