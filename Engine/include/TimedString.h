#ifndef TIMED_STRING_H
#define TIMED_STRING_H

#include <string>

#include <SFML/Graphics/String.hpp>
#include <SFML/System/Clock.hpp>

namespace nt {
  namespace core {

/**
 * Provides same functionality as sf::String, but allows for timed on-screen
 * output of characters stored.
 */
class TimedString : public sf::String {
 public:
  TimedString();
  ~TimedString() {}

  /**
   * Stores text in buffer for future output.
   * @param text to store in buffer.
   */
  void BufferText( const std::string &text );

  /**
   * Sets time (in seconds) between output of characters.
   * @param time between each character output. Unit = seconds.
   */
  void SetPrintTime( float time );

  /**
   * Starts the printing process, initiating timer to use for output. If the
   * time set between character output is 0, whole string will be output.
   */
  void StartPrint();

  /**
   * Updates printing, checking if enough time has passed to output another
   * character.
   */
  void UpdatePrint();

 private:
  /**
   * Time between output of characters.
   */
  float m_printTime;

  /**
   * Timer to monitor how long it has been since last character was output.
   */
  sf::Clock m_timer;

  /** 
   * Buffer for characters before they are set to sf::String for rendering.
   */
  std::string m_buffer; 
};

} // namespace core
} // namespace nt

#endif // TIMED_STRING_H
