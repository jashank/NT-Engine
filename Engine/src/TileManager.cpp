#include "TileManager.h"

#include <cstring>
#include <sstream>
#include <utility>

extern "C" {
  #include "lua.h"
}

#include <boost/shared_ptr.hpp>

#include "AnimData.h"
#include "AnimSprite.h"
#include "Camera.h"
#include "ResourceLib.h"
#include "State.h"
#include "tinyxml.h"
#include "Utilities.h"
#include "Window.h"

namespace nt {

/****************
 * Constructor
 ***************/
TileManager::TileManager( const TiXmlElement *root )
 : m_numTileTypes( 0 ),
   m_numTiles( 0 ),
   m_tileSize( 0 ) {
  LoadData( root );
}

/********************
 * Public Methods
 *******************/
void TileManager::Update( float dt ) {
  for( int i = 0; i < m_numTileTypes; ++i ) {
    m_tileSprites[i].Update( dt );
  }
}


void TileManager::Render( float alpha, const Camera &cam ) {
  if ( m_tileSprites ) {
    const IntRect &view = cam.GetAdjustedFocus( 1, 1 );
    int tLx = view.topLeft.x;
    int tLy = view.topLeft.y;
    int bRx = view.bottomRight.x;
    int bRy = view.bottomRight.y;

    float screenX = 0.0;
    float screenY = 0.0;
    int tile = -1;

    for ( int x = tLx; x <= bRx; ++x ) {
      for ( int y = tLy; y <= bRy; ++y ) {

        tile = m_layout->Get( x, y );
        if ( tile != BLANK_TILE_ID ) {
          screenX = static_cast<float>( x ) * m_tileSize;
          screenY = static_cast<float>( y ) * m_tileSize;
          m_tileSprites[tile].SetStartingPos( screenX, screenY );
          window::Draw( m_tileSprites[tile], alpha );
        }
      }
    }
  }
}


int TileManager::GetTileSize() const {
  return m_tileSize;
}


const IntRect &TileManager::GetMapRect() const {
  return m_mapRect;
}


bool TileManager::TileIsCrossable( int x, int y )  const {
  if ( m_mapRect.Contains( x, y )) { 
    idMap_type::const_iterator itr = 
      m_tileDataId.find( m_layout->Get( x, y ));

    if ( itr != m_tileDataId.end() ) {
      return ( itr->second->cid == CROSSABLE );
    }
  }
  return false;
}


/********************************
 * Lua Functions
 * *****************************/
int TileManager::LuaGetTileInfo( lua_State *L ) const {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in GetTileInfo." );
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in GetTileInfo." );
    return 0;
  }
  int tileY = lua_tointeger( L, -1 );

  if ( m_mapRect.Contains( tileX, tileY )) {
    const Tile *tile = GetTile( tileX, tileY );
    if ( tile ) {
      lua_pushstring( L, tile->type.c_str() );
      lua_pushstring( L, tile->name.c_str() );
      lua_pushinteger( L,tile->id );
      lua_pushinteger( L, tile->cid );
      return 4;
    } else {
      LogLuaErr( "Tile coordinate passed to GetTileInfo has no information." );
      return 0;
    }
  } else {
    LogLuaErr( "Tile coordinate not on map passed to GetTileInfo" );
    return 0;
  }
}


int TileManager::LuaTileIsCrossable( lua_State *L ) const {
  if ( !lua_isnumber( L, -2 ) ) {
    LogLuaErr( "Number not passed to x position in TileIsCrossable." );
    return 0;
  }
  int tileX = lua_tointeger( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to y position in TileIsCrossable." );
    return 0;
  }
  int tileY = lua_tointeger( L, -1 );
 
  if ( m_mapRect.Contains( tileX, tileY )) {
    lua_pushboolean( L, TileIsCrossable( tileX, tileY ));
    return 1;
  } else {
    LogLuaErr( "Tile location not on map passed to TileIsCrossable" );
    return 0;
  }
}


int TileManager::LuaSetTile( lua_State *L ) {
  if ( !lua_isnumber( L, -4 ) ) {
    LogLuaErr( "Number not passed to x position in SetTile." );
    return 0;
  }
  int tileX = lua_tointeger( L, -4 );

  if ( !lua_isnumber( L, -3 ) ) {
    LogLuaErr( "Number not passed to y position in SetTile." );
    return 0;
  }
  int tileY = lua_tointeger( L, -3 );

  if ( !lua_isstring( L, -2 ) ) {
    LogLuaErr( "String not passed to tile name in SetTile." );
    return 0;
  }
  std::string tileName = lua_tostring( L, -2 );

  if ( !lua_isnumber( L, -1 ) ) {
    LogLuaErr( "Number not passed to collision ID in SetTile." );
    return 0;
  }
  int collisionID = lua_tointeger( L, -1 );
  
  SetTile( tileX, tileY, tileName );
  SetCollision( tileX, tileY, collisionID );

  if ( m_mapRect.Contains( tileX, tileY )) {
    SetTile( tileX, tileY, tileName );
    SetCollision( tileX, tileY, collisionID );
  } else {
    LogLuaErr( "Tile location not on map passed to SetTile" );
  }
  return 0;
}

/************************************
Private Methods
************************************/
void TileManager::LoadData( const TiXmlElement *root ) {
  const TiXmlElement *tileSize = root->FirstChildElement( "size" );
  tileSize->Attribute( "px", &m_tileSize );

  const TiXmlElement *anims = root->FirstChildElement( "animation" );
  const char *path = anims->Attribute( "path" );
  // Path may be empty, inferring that there are no tile animations
  if ( strcmp( path, "" ) != 0 ) {
    if ( !LoadTileAnims( path )) {
      LogErr( "Problem loading tile information from animation file." );
    }
  }

  const TiXmlElement *layout = root->FirstChildElement( "layout" );
  if ( !LoadTileLayout( layout )) {
    LogErr( "Problem loading tile layout in state file." );
  } 
}


bool TileManager::LoadTileAnims( const std::string &animPath ) {
  const boost::shared_ptr<AnimData> &tileAnims =
    rsrc::LoadResource<AnimData>( animPath );

  if ( tileAnims ) {
    m_numTileTypes = tileAnims->GetNumAnims();
    m_tileSprites.reset( new AnimSprite[m_numTileTypes] );

    for ( int i = 0; i < m_numTileTypes; ++i ) {
      m_tileSprites[i].SetAnimData( tileAnims );
      m_tileSprites[i].SetAnimation( i );
      m_tileSprites[i].Play();
    }

    TiXmlDocument doc( animPath.c_str() );
    doc.LoadFile();
    TiXmlHandle handleDoc( &doc );

    TiXmlElement *root = handleDoc.FirstChildElement( "animations" ).Element();
    TiXmlElement *sheet = root->FirstChildElement( "sheet" );
    if ( sheet ) {
      TiXmlElement *elem = sheet->FirstChildElement();
      if ( elem ) {
        do {
          if ( !LoadTileInfo( elem )) {
            LogErr( "Couldn't retrieve tile information from tile animation"
                    "file: " + animPath );
            return false;
          }
        } while ( (elem = elem->NextSiblingElement()) );
      }
    }
  } else {
    LogErr( "Problem loading tile animation file: " + animPath );
  }
  return true;
}


bool TileManager::LoadTileLayout( const TiXmlElement *layout ) {
  int width = 0;
  int height = 0;
  layout->Attribute( "width", &width );
  layout->Attribute( "height", &height );

  m_layout.reset( new Matrix2D<int>( width, height ));
  Matrix2D<int>::iterator itr = m_layout->begin();
  while ( itr != m_layout->end() ) {
    *itr = BLANK_TILE_ID;
    ++itr;
  }  

  m_mapRect.Scale( width, height );
  m_numTiles = width * height;

  const char *layoutText = layout->GetText();
  // Grids with no tiles mapped will just be empty
  if ( layoutText ) {
    int column = 0;
    int row = 0;
    std::stringstream tileMapStream( layoutText, std::ios_base::in );
    int currentTile = -1;

    while ( tileMapStream >> currentTile && row < height ) {
      m_layout->Set( column, row, currentTile );
      ++column;
      if ( column >= width ) {
        column = 0;
        ++row;
      }
    }
  }
       
  return true;
}


bool TileManager::LoadTileInfo( const TiXmlElement *strip ) {
  std::string tileName, tileType;
  int tileId = 0;
  int tileCid = 0;

  const char *name = strip->Attribute( "name" );
  if ( name ) {
    tileName = name;
  } else {
    LogErr( "No name specified for tile." );
    return false;
  }

  const char *type = strip->Attribute( "type" );
  if ( type ) {
    tileType = type;
  } else {
    LogErr( "No type specified for tile." );
    return false;
  }

  strip->QueryIntAttribute( "id", &tileId );
  strip->QueryIntAttribute( "cid", &tileCid );

  Tile tile( tileType, tileName, tileId, tileCid );
  m_tileDataName.insert(
    std::pair<std::string, Tile>( tileName, tile ));
  m_tileDataId.insert(
    std::pair<int, Tile*>( tileId, &m_tileDataName[tileName] ));

  return true;
}


void TileManager::SetTile( int x, int y, const std::string &tileName ) {
  if ( m_mapRect.Contains( x, y )) {
    std::map<std::string, Tile>::iterator tileDataItr
     = m_tileDataName.find( tileName );
    if ( tileDataItr != m_tileDataName.end() ) {
      m_layout->Set( x, y, tileDataItr->second.id );
    }
  }
}


Tile *TileManager::GetTile( int x, int y ) const {
  if ( m_mapRect.Contains( x, y )) { 
    int id = m_layout->Get( x, y );
    idMap_type::const_iterator tile = m_tileDataId.find( id );
    if( tile != m_tileDataId.end() ) {
      return tile->second;
    }
  }
  return NULL;
}


void TileManager::SetCollision( int x, int y, int collisionId ) {
  if ( m_mapRect.Contains( x, y )) {
    m_tileDataId[m_layout->Get( x, y )]->cid = collisionId;
  }
}

} // namespace nt
