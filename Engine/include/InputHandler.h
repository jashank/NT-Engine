/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

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

#ifndef NT_INPUT_HANDLER_H
#define NT_INPUT_HANDLER_H

#include <string>
#include <utility>
#include <map>
#include <vector>

#include <boost/function/function1.hpp>

#include "TimedKey.h"
#include "Rect.h"

class TiXmlElement;

namespace nt {

/**
 * Registers and handles input specified in xml files that specify an
 * input list.
 */ 
class InputHandler {
 public:
  InputHandler(); 
  ~InputHandler() {}

  /**
   * Loads <input_list> root passed, storing function-name/Key pairs
   * pairs and mouse-event/function-name pairs in registry. Keys initially
   * held down on loading will not be updated until after they have been 
   * released.
   * @param inputRoot root element of the <input_list>.
   * @return True if load was successful (no syntax errors in file).
   */
  bool LoadInputList( const TiXmlElement *inputRoot );

  /**
   * Updates all TimedKeys stored.
   */
  void Update();

  /**
   * Calls functions associated with any activated keys registered.
   * @param func the callback function to pass function name associated
   * with key to.
   */
  void ScanKeys( const boost::function1<void, std::string&> &func ) const;

  /**
   * Calls functions associated with any current mouse events registered.
   * @param func the callback function to pass function name associated with
   * mouse event to.
   * @param mouseArea area to check to see if mouse event occured in that area.
   * Area is relative to top left corner of screen.
   */
  void ScanMouse( 
    const boost::function1<void, std::string&> &func, 
    const FloatRect &mouseArea 
  );

 private:
  typedef std::multimap<std::string, TimedKey> keymap_type;
  typedef std::multimap<std::string, std::string> mousemap_type;
  typedef boost::function1<void, std::string&> func_type;

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  InputHandler( const InputHandler &handler );
  InputHandler& operator=( const InputHandler &handler );
  //@}

  /**
   * Loads key information found at input element into registry.
   * Any loaded keys currently held down will be marked, and won't be updated
   * until after they have been released.
   * @param input element at which information for key is found.
   * @param keyString name of key found in input element.
   * @return Whether load succeeded (no parsing problems).
   */
  bool LoadKey( const TiXmlElement *input, const char *keyString );

  /**
   * Loads mouse event information found at input element into registry.
   * @param input element at which information for mouse event is found.
   * @param eventString name of mouse event found in input element.
   * @return Whether load succeeded (no parsing problems).
   */
  bool LoadMouseEvent( const TiXmlElement *input, const char *eventString );


  /**
   * Whether mouse is currently pressed down.
   */
  bool m_mousePressed;

  /**
   * Mouse events registered for this handler, storing 
   * mouse-event/function-name pairs. Possible mouse events are "MouseClicked",
   * "MousePressed", and "MouseReleased".
   */
  mousemap_type m_mouseRegistry;

  /**
   * Keys registered for this handler, storing function name/TimedKey pairs.
   */
  keymap_type m_keyRegistry; 

  /**
   * Keys already held down when key data was loaded in.
   */
  keymap_type m_prevKeys; 
};

} // namespace nt

#endif // NT_INPUT_HANDLER_H

