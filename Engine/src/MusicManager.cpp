#include "MusicManager.h"

extern "C" {
  #include "lua.h"
}

#include "ResourceLib.h"
#include "tinyxml.h"
#include "Utilities.h"

/*****************************
 * Constructor
 ****************************/ 
MusicManager::MusicManager()
  :m_keepMusicPlaying( false ), m_currentMusic( NULL ) {}

MusicManager::~MusicManager() {
  if ( m_currentMusic && !m_keepMusicPlaying ) {
    m_currentMusic->Stop();
  }
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
        NamePath key;
        key.path = path;
        if ( name ) {
          key.name = name;
        }
        m_music.insert(
          map_type::value_type( key, nt::rsrc::LoadMusic( key.path ))
        );

      } else {
        LogErr( "Path not specified for song element in State file." );
        return false;
      }

    } while ( (song = song->NextSiblingElement( "song" )) );
  }
  return true;
}


int MusicManager::LuaPlayMusic( lua_State *L ) {
  int args = lua_gettop( L );

  switch( args ) {
    case 0: {
      if ( m_currentMusic ) {
        m_currentMusic->Play();
      }
      return 0;
    }
    case 1: {
      if ( !lua_isstring( L, -1 )) {
        LogLuaErr( "String not passed as name argument to PlayMusic." );
        return 0;
      }
      sf::Music *music = GetMusic( lua_tostring( L, -1 ));
      if ( music ) {
        StopAndSet( music );
        m_currentMusic->Play();
      }
      return 0;
    }
    case 2: {
      if ( !lua_isstring( L, -2 ) || !lua_isnumber( L, -1 )) {
        LogLuaErr( "Bad arguments passed to PlayMusic." );
        return 0;
      }
      sf::Music *music = GetMusic( lua_tostring( L, -2 ));
      if ( music ) {
        StopAndSet( music );
        m_currentMusic->SetVolume( lua_tonumber( L, -1 ));
        m_currentMusic->Play();
      }
      return 0;
    }

    default: { return 0; }
  }
}


/**********************
 * Private Methods
 *********************/
sf::Music *MusicManager::GetMusic( const std::string &nameOrPath ) const {
  const NamePath key( nameOrPath, nameOrPath );

  map_type::const_iterator music = m_music.find( key );
  if ( music != m_music.end() ) {
    return music->second.get();
  }
  return NULL;
}


void MusicManager::StopAndSet( sf::Music *music ) {
  if ( m_currentMusic ) {
    m_currentMusic->Stop();
  }
  m_currentMusic = music;
}

