#include "KeyManager.h"

#include <algorithm>
#include <cctype>

#include <SFML/Window/Input.hpp>

#include "Window.h"

/******************************
Constant Members
******************************/
const std::string KeyManager::m_extraKeys[] = {
"escape",
"lcontrol",
"lshift",
"lalt",
"lsystem",      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
"rcontrol",
"rshift",
"ralt",
"rsystem",      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
"menu",
"lbracket",     ///< [
"rbracket",     ///< ]
"semicolon",    ///< ;
"comma",        ///< ,
"period",       ///< .
"quote",        ///< '
"slash",        ///< /
"backslash",
"tilde",        ///< ~
"equal",        ///< =
"dash",         ///< -
"space","return","back","tab","pageup","pagedown","end","home","insert","delete",
"add",          ///< +
"subtract",     ///< -
"multiply",     ///< *
"divide",       ///< /
"left",         ///< Left arrow
"right",        ///< Right arrow
"up",           ///< Up arrow
"down",         ///< Down arrow
"numpad0","numpad1","numpad2","numpad3","numpad4",
"numpad5","numpad6","numpad7","numpad8","numpad9",
"f1","f2","f3","f4","f5","f6","f7","f8","f9","f10","f11","f12","f13","f14","f15",
"pause",
};

/*************************
Constructor
*************************/
KeyManager::KeyManager( sf::Input *input )
 : m_input( &nt::window::GetInput() ) {}

/*************************
Public Methods
*************************/
bool KeyManager::InterpretKey( std::string keyString, sf::Key::Code &keyResult ) {
  keyString = ToLowerCase( keyString );

  //A = 'a', B = 'b', C = 'c', D = 'd', E = 'e', F = 'f', G = 'g',
  //H = 'h', I = 'i', J = 'j', K = 'k', L = 'l', M = 'm', N = 'n',
  //O = 'o', P = 'p', Q = 'q', R = 'r', S = 's', T = 't', U = 'u',
  //V = 'v', W = 'w', X = 'x', Y = 'y', Z = 'z',
  //Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3', Num4 = '4',
  //Num5 = '5', Num6 = '6', Num7 = '7', Num8 = '8', Num9 = '9',
  //If it has only 1 character it should be one of the sf::Keys above
  if( keyString.size() == 1 ) {
    keyResult = static_cast<sf::Key::Code>( keyString[0] );
    return true;
  }

  static const int ESCAPE_KEY =
    static_cast<int>( sf::Key::Escape );

  //Calculate the amount of extra keys in the sf::key enum
  // Count - Escape(note escape is the first "extra" key in enum)
  static int count =
    static_cast<int>( sf::Key::Count ) - ESCAPE_KEY;

  //Loops through the extraKeys array comparing to keyString
  for( int i = 0; i < count; ++i ) {
    if( keyString == m_extraKeys[i] ) {
      keyResult = static_cast<sf::Key::Code>( i + ESCAPE_KEY );
      return true;
    }
  }

  return false;//not a valid keyString
}


bool KeyManager::IsKeyDown( sf::Key::Code kCode ) {
  return m_input->IsKeyDown( kCode );
}


Key KeyManager::GetKey( sf::Key::Code key ) const {
  if( m_input ) {
    for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
      if( key == m_keys[i].key ) {
        return m_keys[i];
      }
    }
  }
  return Key();
}


void KeyManager::RegisterKey( sf::Key::Code key ) {
  for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
    if( key == m_keys[i].key ) {
      return;
    }
  }
  m_keys.push_back( Key( key ) );
}


void KeyManager::Update( float currentTime ) {
  for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
    if( m_input->IsKeyDown( m_keys[i].key )) {
      timeDiff = currentTime - m_keys[i].elapsedTime;
      m_keys[i].elapsedTime += timeDiff;

      if( m_keys[i].startTime == 0.0f ) {
        m_keys[i].startTime = currentTime;
      }
    }
    else {
      m_keys[i].elapsedTime = 0.0f;
      m_keys[i].startTime = 0.0f;
    }
  }
}
