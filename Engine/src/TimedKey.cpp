#include "TimedKey.h"

#include "boost/assign.hpp"
#include <SFML/Window/Input.hpp>

#include "Utilities.h"
#include "Window.h"

/***************************
 * Initialize Extra Keys
 **************************/
TimedKey::KeyMap TimedKey::m_extraKeys =
  boost::assign::map_list_of
  ("escape", sf::Key::Escape)
  ("lcontrol", sf::Key::LControl)
  ("lshift", sf::Key::LShift)
  ("lalt", sf::Key::LAlt)
  ("lsystem", sf::Key::LSystem) // OS Specific
  ("rcontrol", sf::Key::RControl)
  ("rshift", sf::Key::RShift)
  ("ralt", sf::Key::RAlt)
  ("rsystem", sf::Key::RSystem)
  ("menu", sf::Key::Menu)
  ("lbracket", sf::Key::LBracket)
  ("rbracket", sf::Key::RBracket)
  ("semicolon", sf::Key::SemiColon)
  ("comma", sf::Key::Comma)
  ("period", sf::Key::Period)
  ("quote", sf::Key::Quote)
  ("slash", sf::Key::Slash)
  ("backslash", sf::Key::BackSlash)
  ("tilde", sf::Key::Tilde)
  ("equal", sf::Key::Equal)
  ("dash", sf::Key::Dash)
  ("space", sf::Key::Space)
  ("return", sf::Key::Return)
  ("back", sf::Key::Back)
  ("tab", sf::Key::Tab)
  ("pageup", sf::Key::PageUp)
  ("pagedown", sf::Key::PageDown)
  ("end", sf::Key::End)
  ("home", sf::Key::Home)
  ("insert", sf::Key::Insert)
  ("delete", sf::Key::Delete)
  ("add", sf::Key::Add)
  ("subtract", sf::Key::Subtract)
  ("multiply", sf::Key::Multiply)
  ("divide", sf::Key::Divide)
  ("left", sf::Key::Left)
  ("right", sf::Key::Right)
  ("up", sf::Key::Up)
  ("down", sf::Key::Down)
  ("numpad0", sf::Key::Numpad0)
  ("numpad1", sf::Key::Numpad1)
  ("numpad2", sf::Key::Numpad2)
  ("numpad3", sf::Key::Numpad3)
  ("numpad4", sf::Key::Numpad4)
  ("numpad5", sf::Key::Numpad5)
  ("numpad6", sf::Key::Numpad6)
  ("numpad7", sf::Key::Numpad7)
  ("numpad8", sf::Key::Numpad8)
  ("numpad9", sf::Key::Numpad9)
  ("f1", sf::Key::F1)
  ("f2", sf::Key::F2)
  ("f3", sf::Key::F3)
  ("f4", sf::Key::F4)
  ("f5", sf::Key::F5)
  ("f6", sf::Key::F6)
  ("f7", sf::Key::F7)
  ("f8", sf::Key::F8)
  ("f9", sf::Key::F9)
  ("f10", sf::Key::F10)
  ("f11", sf::Key::F11)
  ("f12", sf::Key::F12)
  ("f13", sf::Key::F13)
  ("f14", sf::Key::F14)
  ("f15", sf::Key::F15)
  ("pause", sf::Key::Pause);

/*******************************
 * Constructors and Destructors
 ******************************/
TimedKey::TimedKey( std::string &keyString, bool repeat, float delay )
  :m_activated( false ), 
   m_noRepeatActiv( false ),
   m_pressed( false ),
   m_repeat( repeat ), 
   m_delay( delay ) {
  std::string lwrKey = ToLowerCase( keyString );

  // Single character keys can easily be casted
  if ( lwrKey.size() == 1 ) {
    m_kCode = static_cast<sf::Key::Code>( keyString[0] );

  } else {
    KeyMap::iterator keyPair = m_extraKeys.find( lwrKey );
    
    if ( keyPair != m_extraKeys.end() ) {
      m_kCode = keyPair->second;
    } else {
      // No key found so associate with Count, which doesn't match any key
      m_kCode = sf::Key::Count;
    }
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


bool TimedKey::IsActivated() const {
  return m_activated;
}
    

bool TimedKey::IsDown() const {
  return nt::window::GetInput().IsKeyDown( m_kCode );
}

