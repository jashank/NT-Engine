#ifndef WINDOW_H
#define WINDOW_H

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

#endif // WINDOW_H

