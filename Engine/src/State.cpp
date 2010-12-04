#include "State.h"

#include <utility>

extern "C" {
  #include "lua.h"
}

#include "tinyxml.h"
#include "Utilities.h"

/******************************************
 * Destructor
 *****************************************/
State::~State() {
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

  return true;
}


void State::HandleEvents() {
  m_objectManager->HandleEvents( m_camera );
}


void State::Update( float dt ) {
  m_camera.Update( dt );
  m_tileManager->Update( dt );
  m_objectManager->Update( dt, m_camera );
}


void State::Render( float alpha ) {
  // The rendering order is important.
  m_tileManager->Render( alpha, m_camera );
  m_objectManager->Render( alpha, m_camera );
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
  return m_objectManager->LuaCreateObject( L );
}

int State::LuaDestroyObject( lua_State *L ) {
  return m_objectManager->LuaDestroyObject( L );
}

int State::LuaGetObject( lua_State *L ) const {
  return m_objectManager->LuaGetObject( L );
}

int State::LuaGetObjects( lua_State *L ) const {
  return m_objectManager->LuaGetObjects( L );
}

int State::LuaGetNearestObject( lua_State *L ) const {
  return m_objectManager->LuaGetNearestObject( L );
}

int State::LuaGetNearestToObject( lua_State *L ) const {
  return m_objectManager->LuaGetNearestToObject( L );
}

int State::LuaGetObjectOnTile( lua_State *L ) const {
  return m_objectManager->LuaGetObjectOnTile( L );
}

int State::LuaGetObjectsOnTile( lua_State *L ) const {
  return m_objectManager->LuaGetObjectsOnTile( L );
}

int State::LuaObjectBlockingTile( lua_State *L ) const {
  return m_objectManager->LuaObjectBlockingTile( L );
}

int State::LuaGetTileInfo( lua_State *L ) const {
  return m_tileManager->LuaGetTileInfo( L );
}

int State::LuaTileIsCrossable( lua_State *L ) const {
  return m_tileManager->LuaTileIsCrossable( L );
}

int State::LuaSetTile( lua_State *L ) {
  return m_tileManager->LuaSetTile( L );
}

int State::LuaPlayMusic( lua_State *L ) {
  return m_musicManager->LuaPlayMusic( L );
}

int State::LuaStopMusic( lua_State *L ) {
  return m_musicManager->LuaStopMusic( L );
}

int State::LuaPauseMusic( lua_State *L ) {
  return m_musicManager->LuaPauseMusic( L );
}

int State::LuaMusicIsPlaying( lua_State *L ) const {
  return m_musicManager->LuaMusicIsPlaying( L );
}

int State::LuaLoopMusic( lua_State *L ) {
  return m_musicManager->LuaLoopMusic( L );
}

int State::LuaGetMusicVolume( lua_State *L ) const {
  return m_musicManager->LuaGetMusicVolume( L );
}

int State::LuaSetMusicVolume( lua_State *L ) {
  return m_musicManager->LuaSetMusicVolume( L );
}

int State::LuaAdjustMusicVolume( lua_State *L ) {
  return m_musicManager->LuaAdjustMusicVolume( L );
}

int State::LuaKeepMusicPlaying( lua_State *L ) {
  return m_musicManager->LuaKeepMusicPlaying( L );
}

int State::LuaSpanCam( lua_State *L ) {
  return m_camera.LuaSpan( L );
}

int State::LuaOffsetCam( lua_State *L ) {
  return m_camera.LuaOffset( L );
}

int State::LuaCenterCam( lua_State *L ) {
  return m_camera.LuaSetCenter( L );
}

int State::LuaSetCamSpeed( lua_State *L ) {
  return m_camera.LuaSetSpeed( L );
}

int State::LuaAdjustCamSpeed( lua_State *L ) {
  return m_camera.LuaAdjustSpeed( L );
}

/**********************************
 * Private Member Functions
 *********************************/
bool State::LoadFromFile( const std::string &filePath, lua_State *L ) {
  TiXmlDocument doc( filePath.c_str() );

  if ( doc.LoadFile() ) {
    TiXmlElement *root = doc.FirstChildElement( "state" );
    if ( root ) {

      TiXmlElement *elem = root->FirstChildElement( "music" );
      m_musicManager.reset( new MusicManager( elem ));

      elem = root->FirstChildElement( "fonts" );
      m_fontManager.reset( new FontManager( elem ));

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

      elem = root->FirstChildElement( "tiles" );
      m_tileManager.reset( new TileManager( elem ));

      // Set state comm temporarily for ObjectManager and Camera
      // to access TileManager
      nt::state::SetStateComm( this );

      m_camera.Span(
        m_tileManager->GetMapWidth() - 1,
        m_tileManager->GetMapHeight() - 1
      );

      elem = root->FirstChildElement( "objects" );
      m_objectManager.reset( new ObjectManager( elem, L ));

      // End temporary StateComm set
      nt::state::EndStateComm();

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

