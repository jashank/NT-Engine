#include "InputHandler.h"

#include <algorithm>

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
  } else {
    LogErr( "No <input> element specified in input list. Thus, not needed." );
    return false;
  }
  return true;
}


void InputHandler::ScanKeys( const funcType &func ) {
  for ( keyRegItr itr = m_keyRegistry.begin(); itr != m_keyRegistry.end();
        ++itr ) {
    itr->second.Update();
    if ( itr->second.IsActivated() ) {
      std::string funcName = itr->first;
      func( funcName );
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
bool InputHandler::LoadKey( 
  const TiXmlElement *input, 
  const char *keyString 
) {
  std::string keyName = keyString;

  bool repeat = false;
  const char *repeatStr = input->Attribute( "repeat" );
  if ( repeatStr ) {
    repeat = ( ToLowerCase( repeatStr ) == "true" );
  } else {
    LogErr( "Repeat attribute not given in <input>." );
    return false;
  }

  float delay = 0.0;
  input->QueryFloatAttribute( "delay", &delay );

  TimedKey key( keyName, repeat, delay )

  const char *func = input->Attribute( "function" );
  if ( func ) {
    m_keyRegistry.insert( std::make_pair( func, key ));
  } else {
    LogErr( "No function named for input to act upon in <input>" );
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
  

