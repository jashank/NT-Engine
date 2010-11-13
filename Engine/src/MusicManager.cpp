#include "MusicManager.h"

#include <utility>

extern "C" {
  #include "lua.h"
}

#include <SFML/Audio/Music.hpp>

#include "ResourceLib.h"
#include "tinyxml.h"
#include "Utilities.h"

/*****************************
 * Constructor and Destructor
 ****************************/
MusicManager::MusicManager()
  :m_music( NULL ) {}

MusicManager::~MusicManager() {
  m_music = NULL;
}

/*****************************
 * Public Methods
 ****************************/
bool MusicManager::LoadData( const TiXmlElement *root ) {
  const TiXmlElement *song = root->FirstChildElement( "song" );
  if ( song ) {
    do {
      const char *name = song->Attribute( "name" );
      const char *path = song->Attribute( "path" );

      if ( path ) {
        if ( nt::rsrc::LoadMusic( path )) {

          std::pair<std::string, std::string> pair;
          if ( name ) {
            pair = std::make_pair( name, path );
          } else {
            pair = std::make_pair( path, path );
          }
          m_namePaths.insert( pair );
        }
      } else {
        LogErr( "Path not specified for song element in State file." );
        return false;
      }

    } while ( (song = song->NextSiblingElement( "song" )) );
  }
  return true;
}

