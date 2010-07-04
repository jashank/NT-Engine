#include "TileManager.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <utility>
#include <tr1/tuple>

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
TileManager::TileManager( const TiXmlElement *dataRoot )
 : m_tileSprites( NULL ),
   m_numTileTypes( 0 ),
   m_width( 0 ),
   m_height( 0 ),
   m_numTiles( 0 ),
   m_tileDim( 0 ) {
  LoadData( dataRoot );
}


TileManager::~TileManager() {
  SAFEDELETEA( m_tileSprites );
}


/***************************************
Public Methods
***************************************/
void TileManager::LoadData( const TiXmlElement *dataRoot ) {
  const TiXmlElement *tileSize = dataRoot->FirstChildElement( "size" );
  if ( !tileSize->Attribute( "dim", &m_tileDim )) {
    LogErr( "Tile size not specified in state file." );
  }
  LoadTileAnims( dataRoot->FirstChildElement( "anims" )->
    Attribute( "path" ));
  LoadTileLayout( dataRoot->FirstChildElement( "layout" ));
}

void TileManager::Update() {
  for( unsigned int i = 0; i < m_numTileTypes; ++i ) {
    m_tileSprites[i].Update();
  }
}

void TileManager::Render() {
  static App* app = App::GetApp();

	int tile = 0;
  static float x, y = 0.0f;
	for ( unsigned int i = 0; i < m_height; i++ ) {
		for ( unsigned int j = 0; j < m_width; j++ ) {
			tile = m_layout[i][j];
      x = static_cast<float>( j ) * m_tileDim;
      y = static_cast<float>( i ) * m_tileDim;
      m_tileSprites[tile].SetPosition( x, y );
			app->Draw( m_tileSprites[tile] );
		}
	}
}


void TileManager::SetTile( unsigned int x, unsigned int y, const std::string &tileName ) {
  std::map<std::string, Tile>::iterator tileDataItr
   = m_tileDataName.find( tileName );
  if ( tileDataItr != m_tileDataName.end() && x < m_width && y < m_height ) {
    m_layout[y][x] = tileDataItr->second.id;
  }
}


const Tile& TileManager::GetTile( unsigned int x, unsigned int y ) {
  if ( x < m_width && y < m_height ) {
    int id = m_layout[y][x];
    TileInfoIter tile = m_tileDataId.find( id );
    if( tile != m_tileDataId.end() ) {
      return *m_tileDataId[id];
    }
  }

  return NULL_TILE_INFO;
}


int TileManager::GetTileDim() {
  return m_tileDim;
}


int TileManager::GetMapWidth() {
  return m_width;
}


int TileManager::GetMapHeight() {
  return m_height;
}

/************************************
Private Methods
************************************/
void TileManager::LoadTileAnims( const std::string &animPath ) {
  std::cout << animPath << "\n";
  static App *app = App::GetApp();
  AnimData *tileAnims = app->LoadAnim( animPath );

  if ( tileAnims ) {
    m_numTileTypes = tileAnims->GetNumAnims();
    m_tileSprites = new AnimSprite[m_numTileTypes];

    for ( unsigned int i = 0; i < m_numTileTypes; ++i ) {
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
                GetTileInfo( strip );
              } while ( strip = strip->NextSiblingElement( "strip" ));
            }
          } else if ( strcmp( elem->Value(), "strip" ) == 0 ) {
            GetTileInfo( elem );
          }
        } while ( elem = elem->NextSiblingElement() );
      }
    }
  } else {
    LogErr( "Tile animation file not found." );
  }
}


void TileManager::LoadTileLayout( const TiXmlElement *root ) {
  if ( !root->Attribute( "width", &m_width ) ||
       !root->Attribute( "height", &m_height )) {
    LogErr( "Didn't specify width and height for tile layout." );
  }
  m_numTiles = m_width * m_height;

  std::stringstream tileMapStream( root->GetText(), std::ios_base::in );

  int currentTile = 0;

  for( unsigned int i=0; i < m_width; i++ ) {
      m_layout.push_back( std::vector<int>() );
    for ( unsigned int j=0; j < m_height; j++ ) {
      if ( tileMapStream >> currentTile ) {
        m_layout[i].push_back(  currentTile );
      } else {
        m_layout[i].push_back(-1);
      }
    }
  }
}

void TileManager::GetTileInfo( const TiXmlElement *strip ) {
  std::string tileName;
  std::string tileType;
  int tileId;
  int collisionId;

  const char *name = strip->Attribute( "name" );
  if ( name ) {
    tileName = name;
  } else {
    LogErr( "No name specified for tile." );
  }

  const char *type = strip->Attribute( "type" );
  if ( type ) {
    tileType = type;
  } else {
    LogErr( "No type specified for tile." );
  }

  strip->QueryIntAttribute( "id", &tileId );
  strip->QueryIntAttribute( "cid", &collisionId );

  Tile tile( tileType, tileName, tileId, collisionId );
  m_tileDataName.insert(
          std::pair<std::string, Tile>( tileName, tile ));
  m_tileDataId.insert(
          std::pair<int, Tile*>( tileId, &m_tileDataName[tileName] ));
}

// Collision Stuff
bool TileManager::TileIsCrossable( unsigned int x, unsigned int y )  {
  if ( x >= m_width || y >= m_height  ) {
    return false;
  } else {
    return (m_tileDataId[m_layout[y][x]]->cid == CROSSABLE);
  }
}

void TileManager::SetCollision( unsigned int x, unsigned int y, 
                                int collisionId ) {
  if ( x < m_width && y < m_height ) {
    m_tileDataId[m_layout[y][x]]->cid = collisionId;
  }
}

int TileManager::GetCollision( unsigned int x, unsigned int y ) {
  if ( x < m_width && y < m_height ) {
    return m_tileDataId[m_layout[y][x]]->cid;
  } else {
    return NOT_CROSSABLE;
  }
}
