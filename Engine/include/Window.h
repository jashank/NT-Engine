/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_WINDOW_H
#define NT_WINDOW_H

#include <string>

namespace nt {
  class Lerpable;
}

namespace sf {
  class Event;
  class Input;
}

/**
 * Contains interface for creating initial application window, retrieving
 * information from it, and destroying it.
 */
namespace nt {
namespace window {

  /**
   * Create window for application. IMPORTANT: The rest of the functions will 
   * break the application unless a window has been created.
   * @param title displayed at top of window.
   * @param width width of window in pixels.
   * @param height height of window in pixels.
   */
  void Create( 
    std::string &title, 
    int width, 
    int height 
  );

  /**
   * Closes window that has been created.
   */
  void Close();

  /**
   * Draws lerpable object to the window using alpha value for lerping.
   */
  void Draw( nt::Lerpable &toDraw, float alpha );

  /**
   * Clears graphical contents of window for another rendering.
   * Should be called every frame.
   */
  void Clear();

  /**
   * Displays contents drawn on window. Should be called every frame.
   */
  void Display();

  /**
   * Returns true if window is open.
   */
  bool IsOpen();

  /**
   * Pops most recent SFML event off of event stack and puts it in
   * event passed. Returns true if there are still events on stack.
   */
  bool GetEvent( sf::Event &event );

  /**
   * Retrieves SFML input manager from window.
   */
  const sf::Input &GetInput();

  /**
   * Offsets window's view, adjusting contents being looked at.
   * @param x amount in pixels to adjust in x direction.
   * @param y amount in pixels to adjust in y direction.
   */
  void OffsetView( float x, float y );

  /**
   * Resets the window's view to its starting position (0, 0).
   */
  void ResetView();

} // namespace window
} // namespace nt

#endif // NT_WINDOW_H

