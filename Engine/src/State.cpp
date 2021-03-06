/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "State.h"

#include <utility>

extern "C" {
  #include "lua.h"
}

#include "tinyxml.h"
#include "Utilities.h"

namespace nt {

/******************************************
 * Destructor
 *****************************************/
State::State( const std::string &filePath, lua_State *L ) {
  LoadFromFile( filePath, L );
}

/*******************************************
 Public Member Functions
*******************************************/
void State::Init() {
  m_objectManager->Init();
}


void State::HandleEvents() {
  m_objectManager->HandleEvents( *m_camera );
}


void State::Update( float dt ) {
  m_camera->Update( dt );
  m_tileManager->Update( dt );
  m_objectManager->Update( dt, *m_camera );
}


void State::Render( float alpha ) {
  // The rendering order is important.
  m_tileManager->Render( alpha, *m_camera );
  m_objectManager->Render( alpha, *m_camera );
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

int State::LuaGetTileInfo( lua_State *L ) const {
  return m_tileManager->LuaGetTileInfo( L );
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
  return m_camera->LuaSpan( L );
}

int State::LuaOffsetCam( lua_State *L ) {
  return m_camera->LuaOffset( L );
}

int State::LuaCenterCam( lua_State *L ) {
  return m_camera->LuaSetCenter( L );
}

int State::LuaSetCamSpeed( lua_State *L ) {
  return m_camera->LuaSetSpeed( L );
}

int State::LuaAdjustCamSpeed( lua_State *L ) {
  return m_camera->LuaAdjustSpeed( L );
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

      const IntRect &mapRect = m_tileManager->GetMapRect();
      const int tileSize = m_tileManager->GetTileSize();

      m_camera.reset( new Camera( mapRect, tileSize ) );

      elem = root->FirstChildElement( "objects" );
      m_objectManager.reset( 
        new ObjectManager( elem, mapRect, tileSize, L )
      );

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

} // namespace nt

