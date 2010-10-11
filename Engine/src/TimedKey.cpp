#include "TimedKey.h"

#include "boost/assign.hpp"

/***************************
 * Initialize Extra Keys
 **************************/
KeyMap TimedKey::m_extraKeys =
  map_list_of
  ("escape", sfk Escape),
  ("lcontrol", sfk LControl),
  ("lshift", sfk LShift),
  ("lalt", sfk LAlt),
  ("lsystem", sfk LSystem), // OS Specific
  ("rcontrol", sfk RControl),
  ("rshift", sfk RShift),
  ("ralt", sfk RAlt),
  ("rsystem", sfk RSystem),
  ("menu", sfk Menu),
  ("lbracket", sfk LBracket),
  ("rbracket", sfk RBracket),
  ("semicolon", sfk SemiColon),
  ("comma", sfk Comma),
  ("period", sfk Period),
  ("quote", sfk Quote),
  ("slash", sfk Slash),
  ("backslash", sfk BackSlash),
  ("tilde", sfk Tilde),
  ("equal", sfk Equal),
  ("dash", sfk Dash),
  ("space", sfk Space),
  ("return", sfk Return),
  ("back", sfk Back),
  ("tab", sfk Tab),
  ("pageup", sfk PageUp),
  ("pagedown", sfk PageDown)

