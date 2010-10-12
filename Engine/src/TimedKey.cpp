#include "TimedKey.h"

#include "boost/assign.hpp"

#include "Utilities.h"
#include "Window.h"

/***************************
 * Initialize Extra Keys
 **************************/
KeyMap TimedKey::m_extraKeys =
  boost::assign::map_list_of
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
  ("pagedown", sfk PageDown),
  ("end", sfk End),
  ("home", sfk Home),
  ("insert", sfk Insert),
  ("delete", sfk Delete),
  ("add", sfk Add),
  ("subtract", sfk Subtract),
  ("multiply", sfk Multiply),
  ("divide", sfk Divide),
  ("left", sfk Left),
  ("right", sfk Right),
  ("up", sfk Up),
  ("down", sfk Down),
  ("numpad0", sfk Numpad0),
  ("numpad1", sfk Numpad1),
  ("numpad2", sfk Numpad2),
  ("numpad3", sfk Numpad3),
  ("numpad4", sfk Numpad4),
  ("numpad5", sfk Numpad5),
  ("numpad6", sfk Numpad6),
  ("numpad7", sfk Numpad7),
  ("numpad8", sfk Numpad8),
  ("numpad9", sfk Numpad9),
  ("f1", sfk F1),
  ("f2", sfk F2),
  ("f3", sfk F3),
  ("f4", sfk F4),
  ("f5", sfk F5),
  ("f6", sfk F6),
  ("f7", sfk F7),
  ("f8", sfk F8),
  ("f9", sfk F9),
  ("f10", sfk F10),
  ("f11", sfk F11),
  ("f12", sfk F12),
  ("f13", sfk F13),
  ("f14", sfk F14),
  ("f15", sfk F15),
  ("pause", sfk Pause);

/*******************************
 * Constructors and Destructors
 ******************************/
TimedKey::TimedKey( std::string &keyString, bool repeat, float delay )
  :m_activated( false ), 
   m_noRepActiv( false ),
   m_pressed( false ),
   m_m_repeat( repeat ), 
   m_delay( delay ) {
  std::string lwrKey = ToLowerCase( str );

  // Single character keys can easily be casted
  if ( lwrKey.size() == 1 ) {
    m_kCode = static_cast<sf::Key::Code>( keyString[0] );

  } else if (( KeyMap::iterator code = m_extraKeys.find( lwrKey )) !=
               m_extraKeys.end()) {
    m_kCode = *code;

  } else {
    // No key found so associate with Count, which doesn't match any key
    m_kCode = sfk Count;
  }
}


void TimedKey::Update() {
  if ( !nt::window::GetInput().IsKeyDown( m_kCode )) {
    m_pressed = false;
    m_activated = false;
    m_noRepeatActiv = false;
    return;
  }  

  if ( !m_pressed ) {
    m_pressed = true;
    m_clock.Reset();
  }

  if ( m_clock.GetElapsedTime() >= m_delay ) {
    if ( m_repeat ) {
      m_activated = true;
    } else if ( m_noRepeatActiv ) {
      m_activated = false;
    } else {
      m_activated = true;
      m_noRepeatActiv = true;
    }
  }
}


bool TimedKey::IsActivated() {
  return m_activated;
}
    

