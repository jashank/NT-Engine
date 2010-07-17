#include "GameState.h"

#include <utility>

extern "C" {
  #include "lualib.h"
}

#include "App.h"
#include "tinyxml.h"
#include "Utilities.h"

/*******************************************
 Data Members
*******************************************/
const luaL_Reg GameState::LuaFuncs[] = {
  { "NewState", LuaNewState },
  { "ResetState", LuaResetState },
  { "Portal", LuaPortal },
  { "GetName", LuaGetName },
  { "LogErr", LuaLogErr },
  { NULL, NULL }
};


/*******************************************
 Public Member Functions
*******************************************/
bool GameState::LoadFromFile( const std::string &path ) {
  TiXmlDocument doc( path.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );
    if ( root ) {
      TiXmlElement *elem = root->FirstChildElement( "tiles" );
      if ( elem ) {
        if ( !m_tileManager.LoadData( elem )) {
          LogErr( "Problem loading tiles in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "game_objects" );
      if ( elem ) {
        if ( !m_gameObjectManager.LoadData( elem )) {
          LogErr( "Problem loading GameObjects in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "GUI" );
      if ( elem ) {
        if ( !m_guiManager.LoadData( elem )) {
          LogErr( "Problem loading GUI in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "sound" );
      if ( elem ) {
        if ( !m_soundManager.LoadData( elem )) {
          LogErr( "Problem loading sound in state file " + path );
          return false;
        }
      }

      elem = root->FirstChildElement( "portals" );
      if ( elem ) {
        const TiXmlElement *port = elem->FirstChildElement( "port" );
        if ( port ) {
          do {
            const char *name = port->Attribute( "name" );
            const char *portPath = port->Attribute( "path" );
            if ( name && portPath ) {
              m_portals.insert( std::make_pair( name, portPath ));
            } else {
              LogErr( "Name or path not specified for port in GameState: " + path );
            }
          } while ( (port = port->NextSiblingElement( "port" )) );
        }
      }

    } else {
      LogErr( "<state> tag not specified in state file " + path );
      return false;
    }
    m_path = path;
    m_name = GetXmlFileName( path );
    return true;
  }

  LogErr( "GameState file not found: " + path );
  return false;
}


void GameState::HandleEvents() {
  m_gameObjectManager.HandleEvents();
  m_guiManager.HandleEvents();
}


void GameState::Update() {
  m_tileManager.Update();
  m_gameObjectManager.Update();
  m_guiManager.Update();
  m_soundManager.Update();
}


void GameState::Render() {
  // The rendering order is important.
  m_tileManager.Render();
  m_gameObjectManager.Render();
  m_guiManager.Render();
}


TileManager& GameState::GetTileManager() {
  return m_tileManager;
}


GameObjectManager& GameState::GetGameObjectManager() {
  return m_gameObjectManager;
}


/*************************************
 * Lua Functions
 * ***********************************/
void GameState::RegisterLuaFuncs( lua_State *L  ) {
  luaL_register( L, "Game", LuaFuncs );
}


int GameState::LuaNewState( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to NewState" );
    return luaL_error( L, "String not passed to NewState" );
  }
  App::GetApp()->SetNextState( lua_tostring( L, -1 ));
  return 0;
}


int GameState::LuaResetState( lua_State *L ) {
  App::GetApp()->SetNextState( App::GetApp()->GetCurrentState()->m_path );
  return 0;
}


int GameState::LuaPortal( lua_State *L ) {
  GameState *currentState = App::GetApp()->GetCurrentState();

  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to Portal" );
    return luaL_error( L, "String not passed to Portal" );
  }
  std::map<std::string, std::string>::const_iterator port =
    currentState->m_portals.find( lua_tostring( L, -1 ));
  if ( port != currentState->m_portals.end() ) {
    App::GetApp()->SetNextState( port->second );
  }
  return 0;
}


int GameState::LuaGetName( lua_State *L ) {
  lua_pushstring( L, App::GetApp()->GetCurrentState()->m_name.c_str() );
  return 1;
}


int GameState::LuaLogErr( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LuaLogLuaErr" );
    return luaL_error( L, "String not passed to LuaLogLuaErr" );
  }
  LogLuaErr( lua_tostring( L, -1 ));
  return 0;
}
