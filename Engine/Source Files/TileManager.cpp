#include "TileManager.h"

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
const TileManager::tileInfo TileManager::NULL_TILE_INFO =
std::tr1::make_tuple( "", "", -1 );


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
  std::map<std::string, tileInfo>::iterator tileDataItr
   = m_tileDataName.find( tileName );
  if ( tileDataItr != m_tileDataName.end() && x < m_width && y < m_height ) {
    m_layout[y][x] = std::tr1::get<2>( tileDataItr->second );
  }
}


const TileManager::tileInfo& TileManager::GetTile( unsigned int x, unsigned int y ) {
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
  if ( !root->Attribute( "width", &m_width ) ||
       !root->Attribute( "height", &m_height )) {
    LogErr( "Didn't specify width and height for tile layout." );
  }
  m_numTiles = m_width * m_height;

  std::stringstream tileMapStream( root->GetText(), std::ios_base::in );

  int currentTile = 0;

  for( unsigned int i=0; i < m_width; i++ ) {
    for ( unsigned int j=0; j < m_height; j++ ) {
      if ( tileMapStream >> currentTile ) {
        m_layout[i][j] = currentTile;
      } else {
        m_layout[i][j] = -1;
      }
    }
  }
}
