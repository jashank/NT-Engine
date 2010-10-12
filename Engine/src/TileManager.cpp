#include "TileManager.h"

#include <cstring>
#include <sstream>
#include <utility>

extern "C" {
  #include "lualib.h"
}

#include "AnimData.h"
#include "AnimSprite.h"
#include "App.h"
#include "ResourceLib.h"
#include "State.h"
#include "tinyxml.h"

/******************************
Constructors and Destructors.
******************************/
TileManager::TileManager()
 : m_tileSprites( NULL ),
   m_numTileTypes( 0 ),
   m_width( 0 ),
   m_height( 0 ),
   m_numTiles( 0 ),
   m_tileDim( 0 ),
   m_layout( NULL ) {}


TileManager::~TileManager() {
  SAFEDELETEA( m_tileSprites );
  SAFEDELETE( m_layout );
}

/***************************************
Public Methods
***************************************/
bool TileManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *tileSize = dataRoot->FirstChildElement( "size" );
  tileSize->Attribute( "px", &m_tileDim );

  const TiXmlElement *anims = dataRoot->FirstChildElement( "animation" );
  const char *path = anims->Attribute( "path" );
  // Path may be empty, inferring that there are no tile animations
  if ( strcmp( path, "" ) != 0 ) {
    if ( !LoadTileAnims( path )) {
      LogErr( "Problem loading tile information from animation file." );
      return false;
    }
  }

  const TiXmlElement *layout = dataRoot->FirstChildElement( "layout" );
  if ( !LoadTileLayout( layout )) {
    LogErr( "Problem loading tile layout in state file." );
    return false;
  } 

  return true;
}


void TileManager::Update() {
  for( int i = 0; i < m_numTileTypes; ++i ) {
    m_tileSprites[i].Update();
  }
}


void TileManager::Render() {
  if ( m_tileSprites ) {
    static App* app = App::GetApp();

    static float x = 0.f;
    static float y = 0.f;
    int tile = -1;

    for ( int i = 0; i < m_width; ++i ) {
      for ( int j = 0; j < m_height; ++j ) {

        tile = *( m_layout->Get( i, j ));
        if ( tile != BLANK_TILE_ID ) {
          x = static_cast<float>( i ) * m_tileDim;
          y = static_cast<float>( j ) * m_tileDim;
          m_tileSprites[tile].SetPosition( x, y );
          app->Draw( m_tileSprites[tile] );
        }
      }
    }
  }
}


int TileManager::GetTileDim() const {
  return m_tileDim;
}


int TileManager::GetMapWidth() const {
  return m_width;
}


int TileManager::GetMapHeight() const {
  return m_height;
}


bool TileManager::TileIsCrossable( int x, int y )  const {
  if ( TileOnMap( x, y )) { 
    ConstTileInfoIter iter = m_tileDataId.find( *( m_layout->Get( x, y )));
    if ( iter != m_tileDataId.end() ) {
      return ( iter->second->cid == CROSSABLE );
    }
  }
  return false;
}


bool TileManager::TileOnMap( int x, int y ) const {
  return ( x >= 0 && x < m_width && y >= 0 && y < m_height );
}

/********************************
 * Lua Functions
 * *****************************/
int TileManager::LuaGetTileInfo( lua_State *L ) {
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

  if ( TileOnMap( tileX, tileY )) {
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


int TileManager::LuaTileIsCrossable( lua_State *L ) {
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
 
  if ( TileOnMap( tileX, tileY )) {
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

  if ( TileOnMap( tileX, tileY )) {
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
bool TileManager::LoadTileAnims( const std::string &animPath ) {
  AnimData *tileAnims = nt::rsrc::LoadAnim( animPath );

  if ( tileAnims ) {
    m_numTileTypes = tileAnims->GetNumAnims();
    m_tileSprites = new AnimSprite[m_numTileTypes];

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
  layout->Attribute( "width", &m_width );
  layout->Attribute( "height", &m_height );

  m_layout = new nt::core::Matrix2D<int>( m_width, m_height );
  nt::core::Matrix2D<int>::iterator itr = m_layout->begin();
  while ( itr != m_layout->end() ) {
    *itr = BLANK_TILE_ID;
    ++itr;
  }  

  m_numTiles = m_width * m_height;

  const char *layoutText = layout->GetText();
  // Grids with no tiles mapped will just be empty
  if ( layoutText ) {
    int column = 0;
    int row = 0;
    std::stringstream tileMapStream( layoutText, std::ios_base::in );
    int currentTile = -1;

    while ( tileMapStream >> currentTile && row < m_height ) {
      *( m_layout->Get( column, row )) = currentTile;
      ++column;
      if ( column >= m_width ) {
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
  if ( TileOnMap( x, y )) {
    std::map<std::string, Tile>::iterator tileDataItr
     = m_tileDataName.find( tileName );
    if ( tileDataItr != m_tileDataName.end() ) {
      *( m_layout->Get( x, y )) = tileDataItr->second.id;
    }
  }
}


const Tile* TileManager::GetTile( int x, int y ) const {
  if ( TileOnMap( x, y )) { 
    int id = *( m_layout->Get( x, y ));
    ConstTileInfoIter tile = m_tileDataId.find( id );
    if( tile != m_tileDataId.end() ) {
      return tile->second;
    }
  }
  return NULL;
}


void TileManager::SetCollision( int x, int y, int collisionId ) {
  if ( TileOnMap( x, y )) {
    m_tileDataId[*( m_layout->Get( x, y ))]->cid = collisionId;
  }
}

