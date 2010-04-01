#include "KeyManager.h"

#include <cctype>
#include <algorithm>

#include "App.h"

KeyManager::KeyManager()
 : m_input( 0 ) {
}

const std::string KeyManager::m_extraKeys[] = {
//"a", "b", "c", "d", "e", "f", "g",
//"h", "i", "j", "k", "l", "m", "n",
//"o", "p", "q", "r", "s", "t", "u",
//"v", "w", "x", "y", "z",
//"0", "1", "2", "3", "4",
//"5", "6", "7", "8", "9", 
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

bool KeyManager::InterpretKey( const std::string &keyString, sf::Key::Code &keyResult ) {
  std::string lower = keyString;
  std::transform( 
        lower.begin(), 
        lower.end(),
        lower.begin(),
        std::tolower );

  //A = 'a', B = 'b', C = 'c', D = 'd', E = 'e', F = 'f', G = 'g',
  //H = 'h', I = 'i', J = 'j', K = 'k', L = 'l', M = 'm', N = 'n',
  //O = 'o', P = 'p', Q = 'q', R = 'r', S = 's', T = 't', U = 'u',
  //V = 'v', W = 'w', X = 'x', Y = 'y', Z = 'z',
  //Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3', Num4 = '4',
  //Num5 = '5', Num6 = '6', Num7 = '7', Num8 = '8', Num9 = '9', 
  //If it has only 1 character it should be one of the sf::Keys above
  if( lower.size() == 1 ) {
    keyResult = static_cast<sf::Key::Code>( lower[0] );
    return true;
  }  

  //sf::Key::Code::X
  
  //Calculate the amount of extra keys in the sf::key enum
  // Count - Escape(note escape is the first "extra" key in enum)
  static Uint count = static_cast<Uint>( sf::Key::Count ) -
    static_cast<Uint>( sf::Key::Escape );

  
  for( Uint i = 0; i < count; ++i ) {
    if( lower == m_extraKeys[i] ) {
      keyResult = static_cast<sf::Key::Code>( i + 
        static_cast<Uint>(sf::Key::Escape) ); 
      return true;
    }
  }

  return false;//not a valid keyString
}


float KeyManager::GetKeyTime( sf::Key::Code key ) const {
  if( m_input ) {
    for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
      if( key == m_keys[i].first ) {
        return m_keys[i].second;
      }
    }
  }
  return 0.0f;
}


void KeyManager::Init( const sf::Input &input ) {
  m_input = &input;
}


void KeyManager::RegisterKey( sf::Key::Code key ) {
  for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
    if( key == m_keys[i].first ) {
      return;
    }
  }
  m_keys.push_back( std::make_pair( key, 0.0f ) );
}


void KeyManager::Update() {
  float deltatime = App::GetApp()->GetDeltaTime();
  for( int i = 0; i < static_cast<int>(m_keys.size()); ++i ) {
    if( m_input->IsKeyDown( m_keys[i].first ) ) {
      m_keys[i].second += deltatime;
    }
    else {
      m_keys[i].second = 0.0f;
    }
  }
}