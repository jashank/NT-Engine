#include "TileManager.h"

#include <cstdio>
#include <sstream>
#include <utility>

#include "AnimData.h"
#include "AnimSprite.h"
#include "App.h"
#include "tinyxml.h"

/******************************
Constant Members
******************************/
const Tile TileManager::NULL_TILE_INFO = Tile( "", "", -1, NOT_CROSSABLE );
/******************************
Constructors and Destructors.
******************************/
TileManager::TileManager()
 : m_tileSprites( NULL ),
   m_numTileTypes( 0 ),
   m_width( 0 ),
   m_height( 0 ),
   m_numTiles( 0 ),
   m_tileDim( 0 ) {}


TileManager::~TileManager() {
  SAFEDELETEA( m_tileSprites );
}


/***************************************
Public Methods
***************************************/
bool TileManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *tileSize = dataRoot->FirstChildElement( "size" );
  if ( !tileSize->Attribute( "px", &m_tileDim )) {
    LogErr( "Tile size not specified in state file." );
    return false;
  }

  const TiXmlElement *anims = dataRoot->FirstChildElement( "animation" );
  if ( anims ) {
    const char *path = anims->Attribute( "path" );
    if ( path ) {
      if ( !LoadTileAnims( path )) {
        LogErr( "Problem loading tile information from animation file." );
        return false;
      }
    } else {
      LogErr( "No path specified in <animation> tag for tiles in state file." );
    }
  } else {
    LogErr( "No <animation> tag for tiles in state file." );
    return false;
  }

  const TiXmlElement *layout = dataRoot->FirstChildElement( "layout" );
  if ( layout ) {
    if ( !LoadTileLayout( layout )) {
      LogErr( "Problem loading tile layout in state file." );
      return false;
    }
  } else {
    LogErr( "No <layout> tag for tiles in state file." );
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
  static App* app = App::GetApp();

	int tile = 0;
  static float x, y = 0.0f;
	for ( int i = 0; i < m_height; i++ ) {
		for ( int j = 0; j < m_width; j++ ) {
			tile = m_layout[i][j];
      x = static_cast<float>( j ) * m_tileDim;
      y = static_cast<float>( i ) * m_tileDim;
      m_tileSprites[tile].SetPosition( x, y );
			app->Draw( m_tileSprites[tile] );
		}
	}
}


void TileManager::SetTile( int x, int y, const std::string &tileName ) {
  std::map<std::string, Tile>::iterator tileDataItr
   = m_tileDataName.find( tileName );
  if ( tileDataItr != m_tileDataName.end() && x < m_width && y < m_height ) {
    m_layout[y][x] = tileDataItr->second.id;
  }
}


const Tile& TileManager::GetTile( int x, int y ) {
  if ( x < m_width && y < m_height ) {
    int id = m_layout[y][x];
    TileInfoIter tile = m_tileDataId.find( id );
    if( tile != m_tileDataId.end() ) {
      return *m_tileDataId[id];
    }
  }

  return NULL_TILE_INFO;
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


// Collision Stuff
bool TileManager::TileIsCrossable( int x, int y )  const {
  if ( x < m_width && y < m_height  ) {
    ConstTileInfoIter iter = m_tileDataId.find( m_layout[y][x] );
    if ( iter != m_tileDataId.end() ) {
      return ( iter->second->cid == CROSSABLE);
    }
  }
  return false;
}


void TileManager::SetCollision( int x, int y, int collisionId ) {
  if ( x < m_width && y < m_height ) {
    m_tileDataId[m_layout[y][x]]->cid = collisionId;
  }
}


int TileManager::GetCollision( int x, int y ) const {
  if ( x < m_width && y < m_height ) {
    ConstTileInfoIter iter = m_tileDataId.find( m_layout[y][x] );
    if ( iter != m_tileDataId.end() ) {
      return iter->second->cid;
    }
  }
  return NOT_CROSSABLE;
}

/************************************
Private Methods
************************************/
bool TileManager::LoadTileAnims( const std::string &animPath ) {
  // Robust error handling not needed as much in this func b/c
  // already done when loading animation file
  static App *app = App::GetApp();
  AnimData *tileAnims = app->LoadAnim( animPath );

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

    TiXmlElement *sheet = handleDoc.FirstChildElement( "sheet" ).Element();
    if ( sheet ) {
      TiXmlElement *elem = sheet->FirstChildElement();
      if ( elem ) {
        do {
          if ( strcmp( elem->Value(), "common" ) == 0 ) {
            TiXmlElement *strip = elem->FirstChildElement( "strip" );
            if ( strip ) {
              do {
                if ( !GetTileInfo( strip )) {
                  LogErr( "Couldn't retrieve tile information from tile"
                      "animation file: " + animPath );
                  return false;
                }
              } while ( strip = strip->NextSiblingElement( "strip" ));
            }
          } else if ( strcmp( elem->Value(), "strip" ) == 0 ) {
            if ( !GetTileInfo( elem )) {
              LogErr( "Couldn't retrive tile information from tile"
                  "animation file: " + animPath );
              return false;
            }
          }
        } while ( elem = elem->NextSiblingElement() );
      }
    }
  } else {
    LogErr( "Problem loading tile animation file: " + animPath );
  }
  return true;
}


bool TileManager::LoadTileLayout( const TiXmlElement *root ) {
  if ( !root->Attribute( "width", &m_width ) ||
       !root->Attribute( "height", &m_height )) {
    LogErr( "Didn't specify width and height for tile layout." );
    return false;
  }
  m_numTiles = m_width * m_height;

  std::stringstream tileMapStream( root->GetText(), std::ios_base::in );

  int currentTile = 0;

  for( int i = 0; i < m_width; i++ ) {
    m_layout.push_back( std::vector<int>() );
    for ( unsigned int j = 0; j < m_height; j++ ) {
      if ( tileMapStream >> currentTile ) {
        m_layout[i].push_back( currentTile );
      } else {
        m_layout[i].push_back( -1 );
      }
    }
  }

  return true;
}


bool TileManager::GetTileInfo( const TiXmlElement *strip ) {
  std::string tileName;
  std::string tileType;
  int tileId;
  int tileCid;

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
