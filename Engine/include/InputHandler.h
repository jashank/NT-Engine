#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <utility>
#include <map>
#include <vector>

#include <boost/function/function1.hpp>

#include "TimedKey.h"
#include "Rect.h"

class TiXmlElement;

/**
 * Registers and handles input specified in xml files that specify an
 * input list.
 */ 
class InputHandler {
 public:
  InputHandler(); 
  ~InputHandler() {}

  /**
   * Loads <input_list> root passed, storing function-name/key 
   * pairs and mouse-event/function-name pairs in registry.
   * @param inputRoot root element of the <input_list>.
   * @return True if load was successful (no syntax errors in file).
   */
  bool LoadInputList( const TiXmlElement *inputRoot );

  //@{
  /** 
   * The following functions scan input and compare it to the InputHandler's 
   * registry to check if there are any matches. If so, a callback occurs on
   * the function passed, binding the function name in the registry to it.
   * This is used for finding the lua function name associated with a key or
   * mouse event and passing it to an Object's function.
   */ 

  /**
   * Scan input for keys down.
   * @param func the callback function (actually a function pointer). Actual
   * function must take a single string argument (the lua function name).
   */
  void ScanKeys( const boost::function1<void, std::string&> &func );

  /**
   * Scans input for mouse events.
   * @param func the callback function (actually a function pointer). Actual 
   * function must take a signel string argument (the lua function name).
   * @param mouseArea area to check to see if mouse event occured in that area.
   * Area is relative to top left corner of screen.
   */
  void ScanMouse( 
    const boost::function1<void, std::string&> &func, 
    const nt::core::FloatRect &mouseArea 
  );
  //@}

 private:
  typedef std::map<std::string, TimedKey>::iterator keyRegItr;
  typedef std::map<std::string, TimedKey>::const_iterator constKeyRegItr;
  typedef std::map<std::string, std::string>::iterator msRegItr;
  typedef std::map<std::string, std::string>::const_iterator constMsRegItr;
  typedef boost::function1<void, std::string&> funcType;

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  InputHandler( const InputHandler &handler );
  InputHandler& operator=( const InputHandler &handler );
  //@}

  /**
   * Loads key information found at input element into registry.
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
  std::map<std::string, std::string> m_mouseRegistry;

  /**
   * Keys registered for this handler, storing function-name/key pairs. 
   */
  std::map<std::string, TimedKey> m_keyRegistry; 

  /**
   * Keys held down in previous state. Used to disallow input from being
   * handled twice when a state change occurs.
   */
  std::vector<sf::Key::Code> m_prevKeys; 
};

#endif // INPUT_HANDLER_H

