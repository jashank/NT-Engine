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

#ifndef NT_TIMED_STRING_H
#define NT_TIMED_STRING_H

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

#endif // NT_TIMED_STRING_H
