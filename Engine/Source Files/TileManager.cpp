#include "TileManager.h"

#include <iostream>
#include <sstream>
#include <utility>
#include <tr1/tuple>

#include "AnimData.h"
#include "AnimSprite.h"
#include "App.h"
#include "Config.h"
#include "tinyxml.h"

/******************************
Constant Members
******************************/
const TileManager::tileInfo TileManager::NULL_TILE_INFO =
std::tr1::make_tuple( "", "", -1 );


/******************************
Constructors and Destructors.
******************************/
TileManager::TileManager( const TiXmlElement *dataRoot )
 : m_tileSprites( NULL ),
   m_numTiles( 0 ) {
  LoadData( dataRoot );
}


TileManager::~TileManager() {
  SAFEDELETEA( m_tileSprites );
}


/***************************************
Public Methods
***************************************/
void TileManager::LoadData( const TiXmlElement *dataRoot ) {
  LoadTileAnims( dataRoot->FirstChildElement( "tile_anims" )->
    Attribute( "path" ));
  LoadTileLayout( dataRoot->FirstChildElement( "tile_layout" ));
}

void TileManager::Update() {
  for( unsigned int i = 0; i < m_numTiles; ++i ) {
    m_tileSprites[i].Update();
  }
}

void TileManager::Render() {
  static App* app = App::GetApp();

	int tile = 0;
  static float x, y = 0.0f;
	for ( unsigned int i = 0; i < Config::GetMapSize(); i++ ) {
		for ( unsigned int j = 0; j < Config::GetMapSize(); j++ ) {
			tile = m_layout[i][j];
      x = static_cast<float>( j ) * Config::GetTileSize() + Config::GetXPad();
      y = static_cast<float>( i ) * Config::GetTileSize() + Config::GetYPad();
      m_tileSprites[tile].SetPosition( x, y );
			app->Draw( m_tileSprites[tile] );
		}
	}
}


void TileManager::SetTile( unsigned int x, unsigned int y, const std::string &tileName ) {
  std::map<std::string, tileInfo>::iterator tileDataItr
   = m_tileDataName.find( tileName );
  if ( tileDataItr != m_tileDataName.end() && Config::IsTileValid( x, y)) {
    m_layout[y][x] = std::tr1::get<2>( tileDataItr->second );
  }
}


const TileManager::tileInfo& TileManager::GetTile( unsigned int x, unsigned int y ) {
  if ( Config::IsTileValid( x, y )) {
    int id = m_layout[y][x];
    TileInfoIter tile = m_tileDataId.find( id );
    if( tile != m_tileDataId.end() ) {
      return *m_tileDataId[id];
    }
  }

  return NULL_TILE_INFO;
}

/************************************
Private Methods
************************************/
void TileManager::LoadTileAnims( const std::string &animPath ) {
  std::cout << animPath << "\n";
  static App *app = App::GetApp();
  AnimData *tileAnims = app->LoadAnim( animPath );

  if ( tileAnims ) {
    m_numTiles = tileAnims->GetNumAnims();
    m_tileSprites = new AnimSprite[m_numTiles];

    for ( unsigned int i = 0; i < m_numTiles; ++i ) {
      m_tileSprites[i].SetAnimData( tileAnims );
      m_tileSprites[i].SetAnimation( i );
      m_tileSprites[i].Play();
    }

    TiXmlDocument doc( animPath.c_str() );
    doc.LoadFile();
    TiXmlHandle handleDoc( &doc );

    TiXmlElement *sheet = handleDoc.FirstChildElement( "sheet" ).Element();
    do {
      TiXmlElement *strip = sheet->FirstChildElement( "strip" );
      do {
        std::string type( strip->Attribute( "type" ));
        std::string name( strip->Attribute( "name" ));
        int id ;
        strip->Attribute( "id", &id );

        tileInfo tile( type, name, id );

        m_tileDataName.insert(
          std::pair<std::string, tileInfo>( name, tile ));
        m_tileDataId.insert(
          std::pair<int, tileInfo*>( id, &m_tileDataName[name] ));
      } while ( strip = strip->NextSiblingElement( "strip" ));
    } while ( sheet = sheet->NextSiblingElement( "sheet" ));
  } else {
    LogErr( "Tile animation file not found." );
  }
}


void TileManager::LoadTileLayout( const TiXmlElement *root ) {
  unsigned int mapSize = Config::GetMapSize();

  std::stringstream tileMapStream( root->GetText(), std::ios_base::in );

  int currentTile = 0;

  for( unsigned int i=0; i < mapSize; i++ ) {
    for ( unsigned int j=0; j < mapSize; j++ ) {
      if ( tileMapStream >> currentTile ) {
        m_layout[i][j] = currentTile;
      } else {
        m_layout[i][j] = -1;
      }
    }
  }
}
