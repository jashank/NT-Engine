#ifndef TIMED_STRING_H
#define TIMED_STRING_H

#include <string>

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>

#include "Lerpable.h"

namespace nt {

/**
 * Provides similar functionality to sf::String using a lot of code from
 * SFML (particularly Render), but allows for timed on-screen output of 
 * characters stored.
 */
class TimedString : public Lerpable {
 public:
  TimedString();

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

  /**
   * Given a user-defined name of a font or the path to the font, sets
   * the font for this string.
   */
  void SetFont( const std::string &fontName );

  /**
   * Sets size of string, default is 30 pt.
   */
  void SetSize( float size );

 private:
  /** Overrides Lerpable's Render. */
  void Render( sf::RenderTarget &target ) const;

  /** Font that string is displayed in. */
  boost::shared_ptr<const sf::Font> m_font;

  /** Size of text, default 30 pt. */
  float m_size;

  /** Time between output of characters. */
  float m_printTime;

  /**
   * Timer to monitor how long it has been since last character was output.
   */
  sf::Clock m_timer;

  /** Text stored that isn't ready to be output. */
  std::string m_buffer;

  /** Text to output to screen. */
  std::string m_output;
};

} // namespace nt

#endif // TIMED_STRING_H
