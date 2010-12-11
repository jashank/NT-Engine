#include "MusicManager.h"

extern "C" {
  #include "lua.h"
}

#include "Utilities.h"

namespace nt {

/*****************************
 * Constructor
 ****************************/ 
MusicManager::MusicManager( const TiXmlElement *root )
  :m_music( "song" ),
   m_keepMusicPlaying( false ), 
   m_currentMusic( NULL ) {
  m_music.LoadData( root );
}

MusicManager::~MusicManager() {
  if ( m_currentMusic && !m_keepMusicPlaying ) {
    m_currentMusic->Stop();
  }
}

/*****************************
 * Public Methods
 ****************************/
int MusicManager::LuaPlayMusic( lua_State *L ) {
  int args = lua_gettop( L );

  switch( args ) {
    case 0: {
      if ( m_currentMusic ) {
        SafePlay( m_currentMusic );
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
        CheckAndPlay( music );
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
        CheckAndPlay( music );
        m_currentMusic->SetVolume( lua_tonumber( L, -1 ));
      }
      return 0;
    }

    default: { return 0; }
  }
}


int MusicManager::LuaStopMusic( lua_State *L ) {
  if ( m_currentMusic ) {
    m_currentMusic->Stop();
  }
  return 0;
}


int MusicManager::LuaPauseMusic( lua_State *L ) {
  if ( m_currentMusic ) {
    m_currentMusic->Pause();
  }
  return 0;
}


int MusicManager::LuaMusicIsPlaying( lua_State *L ) const {
  if ( m_currentMusic ) {
    lua_pushboolean( L, m_currentMusic->GetStatus() == sf::Music::Playing );
  } else {
    lua_pushboolean( L, false );
  }
  return 1;
}


int MusicManager::LuaLoopMusic( lua_State *L ) {
  if ( !lua_isboolean( L, -1 )) {
    LogLuaErr( "Boolean not passed to Music.Loop" );
    return 0;
  }
  if ( m_currentMusic ) {
    m_currentMusic->SetLoop( lua_toboolean( L, -1 ));
  }
  return 0;
}


int MusicManager::LuaGetMusicVolume( lua_State *L ) const {
  if ( m_currentMusic ) {
    lua_pushnumber( L, m_currentMusic->GetVolume() );
    return 1;
  } else {
    return 0;
  }
}


int MusicManager::LuaSetMusicVolume( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to Music.SetVolume" );
    return 0;
  }
  if ( m_currentMusic ) {
    m_currentMusic->SetVolume( lua_tonumber( L, -1 ));
  }
  return 0;
}


int MusicManager::LuaAdjustMusicVolume( lua_State *L ) {
  if ( !lua_isnumber( L, -1 )) {
    LogLuaErr( "Number not passed to Music.AdjustVolume" );
    return 0;
  }
  if ( m_currentMusic ) {
    float vol = m_currentMusic->GetVolume();
    m_currentMusic->SetVolume( vol + lua_tonumber( L, -1 ));
  }
  return 0;
}


int MusicManager::LuaKeepMusicPlaying( lua_State *L ) {
  if ( !lua_isboolean( L, -1 )) {
    LogLuaErr( "Boolean not passed to Music.KeepPlaying" );
    return 0;
  }
  m_keepMusicPlaying = lua_toboolean( L, -1 );
  return 0;
}

/**********************
 * Private Methods
 *********************/
sf::Music *MusicManager::GetMusic( const std::string &nameOrPath ) const {
  return m_music.GetVal( nameOrPath ).get();
}


void MusicManager::SafePlay( sf::Music *music ) {
  if ( music->GetStatus() != sf::Music::Playing ) {
    music->Play();
  }
}


void MusicManager::CheckAndPlay( sf::Music *music ) {
  if ( m_currentMusic != music ) {
    StopAndSet( music );
  }
  SafePlay( music );
}


void MusicManager::StopAndSet( sf::Music *music ) {
  if ( m_currentMusic ) {
    m_currentMusic->Stop();
  }
  m_currentMusic = music;
}

} // namespace nt

