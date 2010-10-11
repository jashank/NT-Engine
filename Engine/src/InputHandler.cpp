#include "InputHandler.h"

#include <algorithm>

#include "KeyManager.h"
#include "tinyxml.h"
#include "Utilities.h"

/*********************************
 * Constructors and Destructors
 * ******************************/
InputHandler::InputHandler()
  : m_mousePressed( false ) {}

/***********************************
 * Public Methods
 * ********************************/
bool InputHandler::LoadInputList( const TiXmlElement *inputRoot ) {
  const TiXmlElement *input = inputRoot->FirstChildElement( "input" );
  if ( input ) {
    do {
      const char *keyString = input->Attribute( "key" );
      if ( keyString ) {
        LoadKey( input, keyString );
      } else {
        const char *eventString = input->Attribute( "mouse" );
        if ( eventString ) {
          LoadMouseEvent( input, eventString );
        }
      }
    } while ( (input = input->NextSiblingElement( "input" )) );

    KeysDownToPrevKeys();
  } else {
    LogErr( "No <input> element specified in input list. Thus, not needed." );
    return false;
  }
  return true;
}


void InputHandler::ScanKeys( const funcType &func ) {
  // Any keys held down from the previous state that have been released and
  // pressed again should be removed from the prevKeys vector.
  for ( unsigned int i = 0; i < m_prevKeys.size(); ++i ) {
    if ( !app->GetInput()->IsKeyDown( m_prevKeys[i] )) {
      m_prevKeys.erase( m_prevKeys.begin() + i );
    }
  }

  for ( keyRegItr itr = m_keyRegistry.begin(); itr != m_keyRegistry.end();
        ++itr ) {
    sf::Key::Code code = (*itr).second.key;
    if( app->GetInput()->IsKeyDown( code ) &&
        std::find( m_prevKeys.begin(), m_prevKeys.end(), code ) ==
          m_prevKeys.end()) {
      Key key = app->GetKey( (*itr).second.key );

      //Check if key has been held down long enough
      if( key.elapsedTime >= (*itr).second.elapsedTime ) {
        if( (*itr).second.startTime == key.startTime ) {
          continue; // skip because key has already been pressed
        }

        std::string funcName = (*itr).first;
        func( funcName );

        //If the key isn't supposed to be repeated
        if( (*itr).second.startTime != -1 ) {
          // Make sure key won't be handled again until key is re-pressed
          (*itr).second.startTime = key.startTime;
        }
      }
    }
  }
}


void InputHandler::ScanMouse( 
  const funcType &func, 
  const nt::core::FloatRect &mouseArea 
) {
  if ( !m_mouseRegistry.empty() ) {
    std::string eventString = "";

    const sf::Input *input = App::GetApp()->GetInput();
    int x = input->GetMouseX();
    int y = input->GetMouseY();
    bool mouseDown = input->IsMouseButtonDown( sf::Mouse::Left );

    if ( mouseArea.Contains( x, y )) {
      if ( mouseDown ) {
        eventString = "mousepressed";
        m_mousePressed = true;
      } else if ( !mouseDown && m_mousePressed ) {
        eventString = "mouseclicked";
        m_mousePressed = false;
      }
    } else if ( !mouseDown && m_mousePressed ) {
      eventString = "mousereleased";
      m_mousePressed = false;
    }

    constMsRegItr msItr = m_mouseRegistry.find( eventString );
    if ( msItr != m_mouseRegistry.end() ) {
      std::string funcName = msItr->second;
      func( funcName );
    } 
  }
}


/***************************
 * Private
 **************************/
void InputHandler::KeysDownToPrevKeys() {
  for ( constKeyRegItr itr = m_keyRegistry.begin(); itr != m_keyRegistry.end();
        ++itr ) {
    sf::Key::Code code = (*itr).second.key;
    if ( App::GetApp()->GetInput()->IsKeyDown( code )) {
        m_prevKeys.push_back( code );
    }
  }
}


bool InputHandler::LoadKey( const TiXmlElement *input, const char *keyString ) {
  sf::Key::Code k;
  std::string keyName = keyString;
  if ( KeyManager::InterpretKey( keyName, k )) {
    App::GetApp()->RegisterKey( k );
    Key key( k );

    const char *repeat = input->Attribute( "repeat" );
    if ( repeat ) {
      key.startTime = ( ToLowerCase( repeat ) == "true" ) ? -1.0f : 0.0f;
    } else {
      LogErr( "Repeat attribute not given in <input>." );
      return false;
    }

    input->QueryFloatAttribute( "delay", &key.elapsedTime );

    const char *func = input->Attribute( "function" );
    if ( func ) {
      m_keyRegistry.insert( std::make_pair( func, key ));
    } else {
      LogErr( "No function named for input to act upon in <input>" );
      return false;
    }

  } else {
    LogErr( "Key specified in <input> not valid " + keyName );
    return false;
  }
  return true;
}


bool InputHandler::LoadMouseEvent( 
  const TiXmlElement *input, 
  const char *eventString 
) {
  const char *func = input->Attribute( "function" );
  if ( func ) {
    m_mouseRegistry.insert( std::make_pair( ToLowerCase( eventString ), func ));
  } else {
    LogErr( "No function name specified for input to act upon in <input>" );
    return false;
  }
  return true;
}
  

