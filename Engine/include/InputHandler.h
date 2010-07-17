#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <utility>
#include <vector>

#include "boost/function/function1.hpp"

#include "Key.h"

class TiXmlElement;

/*! \class InputHandler
 * - Use this in a class that needs registered input\n
 * - Registers key/function pairs (where function is
 *   name of function in lua script)\n
 * - When used, needs callback function, since lua
 *   func calling code is specific to each class\n
 */
class InputHandler {
 public:
  /// Constructor
  InputHandler() {}
  /// Destructor
  ~InputHandler() {}

  /// Loads input list from xml element root passed
  /// Returns whether list in xml file, or root passed, is valid
  bool LoadInputList( const TiXmlElement *inputRoot );

  /// Scans for key events that have occured, compares them to registry
  /// and calls function passed with matching lua function name as argument.
  /// Function passed should be a lua call to an object's script.
  void ScanInput( const boost::function1<void, std::string&> &func );

 private:
  /// Each key is paired with a lua function name
  typedef std::pair<Key, std::string> KeyEntry;

  /// Restrict copy constructor and assignment
  InputHandler( const InputHandler &handler );
  InputHandler& operator=( const InputHandler &handler );

  std::vector<KeyEntry> m_keyRegistry; // All keys registered for this handler
};

#endif
