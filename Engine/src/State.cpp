#include "State.h"

#include <utility>

extern "C" {
  #include "lualib.h"
}

#include "App.h"
#include "MapLib.h"
#include "ResourceLib.h"
#include "tinyxml.h"
#include "Utilities.h"

/*******************************************
 Data Members
*******************************************/
const luaL_Reg State::LuaFuncs[] = {
  { "LoadPath", LuaLoadPath },
  { "Reset", LuaReset },
  { "Portal", LuaPortal },
  { "GetName", LuaGetName },
  { "LogErr", LuaLogErr },
  { NULL, NULL }
};


/******************************************
 * Destructor
 *****************************************/
State::~State() {
  nt::rsrc::ClearAll();
  nt::state::EndStateComm();
}

/*******************************************
 Public Member Functions
*******************************************/
bool State::Init( const std::string &filePath ) {
  if ( !LoadFromFile( filePath )) {
    return false;
  }
  nt::state::SetStateComm( this );
  return true;
}


void State::HandleEvents() {
  m_objectManager.HandleEvents();
}


void State::Update() {
  m_tileManager.Update();
  m_objectManager.Update();
  m_musicManager.Update();
}


void State::Render() {
  // The rendering order is important.
  m_tileManager.Render();
  m_objectManager.Render();
}


TileManager& State::GetTileManager() {
  return m_tileManager;
}


/*************************************
 * Lua Functions
 * ***********************************/
void State::RegisterLuaFuncs( lua_State *L  ) {
  luaL_register( L, "State", LuaFuncs );
}


int State::LuaLoadPath( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LoadPath" );
    return 0;
  }
  App::GetApp()->SetNextState( lua_tostring( L, -1 ));
  return 0;
}


int State::LuaReset( lua_State *L ) {
  App::GetApp()->SetNextState( App::GetApp()->GetCurrentState()->m_path );
  return 0;
}


int State::LuaPortal( lua_State *L ) {
  State *currentState = App::GetApp()->GetCurrentState();

  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to Portal" );
    return 0;
  }
  std::map<std::string, std::string>::const_iterator port =
    currentState->m_portals.find( lua_tostring( L, -1 ));
  if ( port != currentState->m_portals.end() ) {
    App::GetApp()->SetNextState( port->second );
  }
  return 0;
}


int State::LuaGetName( lua_State *L ) {
  lua_pushstring( L, App::GetApp()->GetCurrentState()->m_name.c_str() );
  return 1;
}


int State::LuaLogErr( lua_State *L ) {
  if ( !lua_isstring( L, -1 )) {
    LogLuaErr( "String not passed to LuaLogLuaErr" );
    return 0;
  }
  LogLuaErr( lua_tostring( L, -1 ));
  return 0;
}

/**********************************
 * Private Member Functions
 *********************************/

bool State::LoadFromFile( const std::string &filePath ) {
  TiXmlDocument doc( filePath.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );
    if ( root ) {

      TiXmlElement *elem = root->FirstChildElement( "map" );
      nt::map::LoadData( elem );

      elem = root->FirstChildElement( "tiles" );
      if ( !m_tileManager.LoadData( elem )) {
        LogErr( "Problem loading tiles in state file " + filePath );
        return false;
      }

      elem = root->FirstChildElement( "objects" );
      if ( !m_objectManager.LoadData( elem )) {
        LogErr( "Problem loading Objects in state file " + filePath );
        return false;
      }

      elem = root->FirstChildElement( "music" );
      if ( !m_musicManager.LoadData( elem )) {
        LogErr( "Problem loading music in state file " + filePath );
        return false;
      }

      elem = root->FirstChildElement( "portals" );
      const TiXmlElement *port = elem->FirstChildElement( "port" );
      if ( port ) {
        do {
          const char *name = port->Attribute( "name" );
          const char *portPath = port->Attribute( "path" );
          if ( name && portPath ) {
            m_portals.insert( std::make_pair( name, portPath ));
          } else {
            LogErr( "Name or path not specified for port in State: " + filePath );
          }
        } while ( (port = port->NextSiblingElement( "port" )) );
      }

      elem = root->FirstChildElement( "fonts" );
      const TiXmlElement *font = elem->FirstChildElement( "font" );
      if ( font ) {
        do {
          const char *path = font->Attribute( "path" );
          if ( path ) {
            nt::rsrc::LoadFont( path );
          } else {
            LogErr( "Name or path not specified for font in State: " + filePath );
          }
        } while ( (font = font->NextSiblingElement( "font" )) );
      }

    } else {
      LogErr( "<state> tag not specified in state file " + filePath );
      return false;
    }
    m_path = filePath;
    m_name = GetFileName( filePath );
    return true;
  }

  LogErr( "State file not found: " + filePath );
  return false;
}


