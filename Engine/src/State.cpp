#include "State.h"

#include <utility>

extern "C" {
  #include "lualib.h"
}

#include "ResourceLib.h"
#include "tinyxml.h"
#include "Utilities.h"

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
bool State::Init( const std::string &filePath, lua_State *L ) {
  if ( !LoadFromFile( filePath, L )) {
    return false;
  }
  nt::state::SetStateComm( this );

  m_clock.Reset();
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


std::string State::GetPath() const {
  return m_path;
}


std::string State::GetPortalPath( std::string &port ) const {
  std::map<std::string, std::string>::const_iterator pair =
    m_portals.find( port );

  if ( pair != m_portals.end() ) {
    return pair->second;
  } else {
    return "";
  }
}

/*************************************
 * Lua Functions
 * ***********************************/
int State::LuaGetName( lua_State *L ) const {
  lua_pushstring( L, m_name.c_str() );
  return 1;
}


int State::LuaCreateObject( lua_State *L ) {
  return m_objectManager.LuaCreateObject( L );
}

int State::LuaDestroyObject( lua_State *L ) {
  return m_objectManager.LuaDestroyObject( L );
}

int State::LuaGetObject( lua_State *L ) {
  return m_objectManager.LuaGetObject( L );
}

int State::LuaGetObjects( lua_State *L ) {
  return m_objectManager.LuaGetObjects( L );
}

int State::LuaGetNearestObject( lua_State *L ) {
  return m_objectManager.LuaGetNearestObject( L );
}

int State::LuaGetObjectOnTile( lua_State *L ) {
  return m_objectManager.LuaGetObjectOnTile( L );
}

int State::LuaObjectBlockingTile( lua_State *L ) {
  return m_objectManager.LuaObjectBlockingTile( L );
}

int State::LuaGetTileInfo( lua_State *L ) {
  return m_tileManager.LuaGetTileInfo( L );
}

int State::LuaTileIsCrossable( lua_State *L ) {
  return m_tileManager.LuaTileIsCrossable( L );
}

int State::LuaSetTile( lua_State *L ) {
  return m_tileManager.LuaSetTile( L );
}

/**********************************
 * Private Member Functions
 *********************************/
bool State::LoadFromFile( const std::string &filePath, lua_State *L ) {
  TiXmlDocument doc( filePath.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );
    if ( root ) {

      TiXmlElement *elem = root->FirstChildElement( "tiles" );
      if ( !m_tileManager.LoadData( elem )) {
        LogErr( "Problem loading tiles in state file " + filePath );
        return false;
      }

      // Set state comm for this load for ObjectManager to know map dimensions
      nt::state::SetStateComm( this );
      elem = root->FirstChildElement( "objects" );
      if ( !m_objectManager.LoadData( elem, L )) {
        LogErr( "Problem loading Objects in state file " + filePath );
        return false;
      }
      nt::state::EndStateComm();

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


