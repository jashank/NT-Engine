#include "TileMap.h"

#include <cstdlib>
#include <sstream>

#include <utility>

#include "App.h"
#include "AnimData.h"
#include "AnimSprite.h"
#include "Configuration.h"

/*****************************
Constants
*****************************/
const TileMap::tileInfo TileMap::NULL_TILE_INFO = 
  std::tr1::make_tuple( "", "", -1 );

/******************************
Constructors and Destructors.
******************************/

TileMap::TileMap()
 : m_tileSprites(0),
   m_numTiles(0) {
	Init();
}

TileMap::~TileMap() {
  SAFEDELETEA( m_tileSprites );
}


/***************************************
Public Methods
***************************************/

void TileMap::LoadTileAnims( 
  const std::string& tileSheet,
  const std::string &anims ) {
  static App* app = App::GetApp();

  sf::Image& sheet = app->LoadImageW( tileSheet );
  AnimData& tileAnims = app->LoadAnim( anims );

  m_numTiles = tileAnims.GetNumAnims();
  m_tileSprites = new AnimSprite[m_numTiles];
  
  for ( Uint i = 0; i < m_numTiles; ++i ) {
    m_tileSprites[i].SetImage( sheet );
    m_tileSprites[i].SetAnimData( tileAnims );
    m_tileSprites[i].SetAnimation( i );
    m_tileSprites[i].Play();
  }

  TiXmlDocument doc( anims.c_str() );
  if ( !doc.LoadFile() ) {
    throw "Tile animation file could not load.";
  }
  TiXmlHandle handleDoc( &doc );

  TiXmlElement* root = handleDoc.FirstChildElement( "anim_strips" ).Element();
  TiXmlElement* strip = root->FirstChildElement( "strip" );

  do {
    std::string type( strip->Attribute( "type" ));
    std::string name( strip->Attribute( "name" ));
    int id = atoi( strip->Attribute( "id" ));
    tileInfo tile( type, name, id );

    m_tileDataName.insert( 
      std::pair<std::string, tileInfo>( name, tile ));
    m_tileDataId.insert(
      std::pair<int, tileInfo>( id, tile ));
  }
  while ( strip = strip->NextSiblingElement( "strip" ));
}


void TileMap::Render() {
  static App* app = App::GetApp();

  for( Uint i = 0; i < m_numTiles; ++i ) {
    m_tileSprites[i].Update();
  }

	int tile = 0;
  static float x, y = 0.0f;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			tile = m_layout[i][j]; 
			if (tile != NULL_TILE) {
        x = (float)j * TILE_SIZE + Configuration::GetXPad();
        y = (float)i * TILE_SIZE + Configuration::GetYPad();
        m_tileSprites[tile].SetPosition( x, y );
				app->Draw( m_tileSprites[tile] );
			}
		}
	}
}


void TileMap::SetTileLayout( TiXmlElement* root ) {
  Uint mapSize = Configuration::GetMapSize();

  std::string tileString( root->GetText() );
  std::stringstream tileMapStream( tileString, std::ios_base::in );

  int currentTile = 0;

  int tileLayout[15][15]; // TODO fix to dynamic
 
  for( Uint i=0; i < mapSize; i++ ) {
    for ( Uint j=0; j < mapSize; j++ ) {
      if ( tileMapStream >> currentTile ) {
        tileLayout[i][j] = currentTile;
      } else {
          tileLayout[i][j] = -1;
      }
    }
  }
	
	// Set the layout to that of the parsed in argument.
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = tileLayout[i][j];
		}
	}

}


void TileMap::SetTile( int x, int y, const std::string &tileName ) {
  m_layout[y][x] = std::tr1::get<2>( m_tileDataName[tileName] );
}


const TileMap::tileInfo& TileMap::GetTile( int x, int y ) {
  if ( x < MAP_SIZE && x >= 0 && y < MAP_SIZE && y >= 0 ) {
    int id = m_layout[y][x];
    return m_tileDataId[id];
  } else {
    return NULL_TILE_INFO;
  }
}


/*****************************************
Private Methods
*****************************************/

void TileMap::Init() {
  // Set the intial layout to null.
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = NULL_TILE;
		}
	}
}