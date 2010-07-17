#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <SFML/Graphics/Rect.hpp>

#include "GUI.h"

/*****************************************************************
 * \class Button
 * A GUI, can be clicked on, and defines onClick function in Lua
 *
 * 'MousePressed' means that cursor was in pressable area when
 * button was pressed.
 * 'MouseClicked' means that cursor was released without leaving
 * the pressable area after the mouse button was pressed.
 * 'MouseReleased' means that cursor was released outside of the
 * pressable area after the mouse button was pressed.
 * **************************************************************/
class Button : public GUI {
 public:
  /// Constructor and Destructor
  Button( const std::string &filepath, int x, int y );
  ~Button() {}

  /// Handles mouse clicks for button
  virtual void HandleEvents();

 private:
  /// Restricts copy constructor and assignment.
  Button( const Button &button );
  Button& operator=( const Button &button );

  /// Loads in button specific data from xml file. Returns
  /// whether load was successful.
  bool LoadData( const std::string &filepath );

  /// Pass either "MousePressed", "MouseReleased", or "MouseClicked"
  /// to call appropriate MouseInput function from Lua.
  void LuaCallMouseInput( const std::string &inputType );

  bool m_pressedDown; // Whether the button is pressed down
  sf::IntRect m_pressable; // Pressable area of animsprite
};

#endif
