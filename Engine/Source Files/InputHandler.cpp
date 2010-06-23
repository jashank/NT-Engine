#include "InputHandler.h"

#include "tinyxml.h"

#include "App.h"
#include "KeyManager.h"
#include "Utilities.h"

/***********************************
 * Public Methods
 * ********************************/
bool InputHandler::LoadInputList( const TiXmlElement *inputRoot ) {
  for ( const TiXmlElement *input = inputRoot->FirstChildElement( "input" );
        input; input = input->NextSiblingElement( "input" ) ) {

    std::string keyString = input->Attribute( "key" );
    sf::Key::Code k;
    if( !KeyManager::InterpretKey( keyString, k ) ) {
      //Invalid key attribute in an input xml tag
      return false;
    }
    App::GetApp()->RegisterKey( k );
    Key key( k );

    std::string repeat = input->Attribute( "repeat" );
    ToLowerCase( repeat );
    key.startTime = ( repeat == "true" ) ? -1.0f : 0.0f;

    std::string delay = input->Attribute( "delay" );
    input->QueryFloatAttribute( "delay", &key.elapsedTime );

    std::string function = input->Attribute( "function" );

    m_keyRegistry.push_back( std::make_pair( key, function ));
  }
  return true;
}


void InputHandler::ScanEvents( const boost::function1<void, std::string&> &func ) {
  static App* app = App::GetApp();

  for( unsigned int i = 0; i < m_keyRegistry.size(); ++i ) {
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

