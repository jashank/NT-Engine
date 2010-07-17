#include "InputHandler.h"

#include "tinyxml.h"

#include "App.h"
#include "KeyManager.h"
#include "Utilities.h"

/***********************************
 * Public Methods
 * ********************************/
bool InputHandler::LoadInputList( const TiXmlElement *inputRoot ) {
  const TiXmlElement *input = inputRoot->FirstChildElement( "input" );
  if ( input ) {
    do {
      const char *keyString = input->Attribute( "key" );
      if ( keyString ) {
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
            m_keyRegistry.push_back( std::make_pair( key, func ));
          } else {
            LogErr( "No function named for input to act upon in <input>" );
            return false;
          }

        } else {
          LogErr( "Key specified in <input> not valid " + keyName );
          return false;
        }
      } else {
        LogErr( "Key not specified for input element." );
        return false;
      }
    } while ( input = input->NextSiblingElement( "input" ));
  } else {
    LogErr( "No <input> element specified in input list. Thus, not needed." );
    return false;
  }
  return true;
}


void InputHandler::ScanInput( const boost::function1<void, std::string&> &func ) {
  static App* app = App::GetApp();

  for( int i = 0; i < m_keyRegistry.size(); ++i ) {
    if( app->GetInput().IsKeyDown( m_keyRegistry[i].first.key )) {
      Key key = app->GetKey( m_keyRegistry[i].first.key );

      //Check if key has been held down long enough
      if( key.elapsedTime >= m_keyRegistry[i].first.elapsedTime ) {
        if( m_keyRegistry[i].first.startTime == key.startTime ) {
          continue; // skip because key has already been pressed
        }

        func( m_keyRegistry[i].second );

        //If the key isn't supposed to be repeated
        if( m_keyRegistry[i].first.startTime != -1 ) {
          // Make sure key won't be handled again until key is re-pressed
          m_keyRegistry[i].first.startTime = key.startTime;
        }
      }
    }
  }
}

