#include "TileMap.h"

#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

#include "App.h"
#include "AnimData.h"
#include "AnimSprite.h"
#include "Configuration.h"
#include "LevelState.h"

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

/**********************************************
Init()
- Called by the constructor and initializes the
	tile map.
***********************************************/
void TileMap::Init() {
  // Set the intial layout to null.
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = NULL_TILE;
		}
	}
}

/*********************************************
LoadTileAnims()
- Loads the tilesheet
- Loads the tile animations from an xml file
**********************************************/
void TileMap::LoadTileAnims( 
  const std::string& tileSheet,
  const std::string &anims ) {
  static App* app = App::GetApp();

  sf::Image& sheet = app->LoadImageW( tileSheet );
  AnimData& tileAnims = app->LoadAnim( anims );

  m_numTiles = tileAnims.GetNumAnims();
  m_tileSprites = new AnimSprite[m_numTiles];
  
  for( Uint i = 0; i < m_numTiles; ++i ) {
    m_tileSprites[i].SetImage( sheet );
    m_tileSprites[i].SetAnimData( tileAnims );
    m_tileSprites[i].SetAnimation( i );
    m_tileSprites[i].Play();
  }

  lua_State* L = LevelState::GetInstance()->GetLuaState();
  std::string tableName( "Tiles" );  
  lua_getglobal( L, tableName.c_str() );
  
  //if the table(tableName) hasn't been created yet
  if( lua_isnil( L, -1 ) ) {
    TiXmlDocument doc ( anims.c_str() );
    
    if ( !doc.LoadFile() ) {
      throw "Blahblahblah I am lazy.";
    }
    TiXmlHandle handleDoc( &doc ); //handle to the xml doc
  
    TiXmlElement* root = handleDoc.FirstChildElement( "anim_strips" ).Element();

    lua_newtable( L ); //Create the lua table
    int n = lua_gettop( L ); //save the table's index
   
    TiXmlElement* strip = root->FirstChildElement( "strip" );
    Uint i = 0;
    for( ; strip; ++i, strip = strip->NextSiblingElement( "strip" ) ) {
        std::string animName( strip->Attribute( "name" ) );

        std::transform( animName.begin(), animName.end(), 
          animName.begin(), std::toupper );

        lua_pushstring( L, animName.c_str() ); //push the key
        lua_pushnumber( L, i ); //push the value
        lua_rawset( L, n ); //add key/value to the table
    }
    
    //Assign the new table we just made to 
    //a lua variable called tableName
    lua_setglobal( L, tableName.c_str() );
  }
}

/***********************************************
Render()
- Called by the Level class and renders the map
************************************************/
void TileMap::Render() {
  static App* app = App::GetApp();


  //Updates animated tiles
  for( Uint i = 0; i < m_numTiles; ++i ) {
    m_tileSprites[i].Update();
  }

	/*
	The current tile refers to the integer identifier that specifies
	which 48x48 tile on the 720x720 sprite sheet is selected.
	*/
	int tile = 0;
  static float x, y = 0.0f;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			tile = m_layout[i][j]; 
			if (tile != NULL_TILE) {
				/*
				Given the layouts current tile determines which tile it is
				refering to in the level sprite sheet. Note this relies on
				the properties of integer division.
				*/

				//m_tileSprites[tile].SetSubRect(
				//	sf::IntRect((tile%MAP_SIZE)*TILE_SIZE, 
				//	(tile/MAP_SIZE)*TILE_SIZE, 
				//	((tile%MAP_SIZE)*TILE_SIZE+TILE_SIZE), 
				//	((tile/MAP_SIZE)*TILE_SIZE+TILE_SIZE)));
				
				/*
				The sprite is then placed on the desired location on the map. Note the
				inverse of the J and I so that the map reads correctly.
				*/
        x = (float)j * TILE_SIZE + Configuration::GetXPad();
        y = (float)i * TILE_SIZE + Configuration::GetYPad();
        m_tileSprites[tile].SetPosition( x, y );

				/*
				Once we find the sprites sub rect on the level sprite sheet and
				have its position to be placed we draw it to the screen.
				*/
				app->Draw( m_tileSprites[tile] );
			}
		}
	}
}

	/************************************************
	SetTileLayout()
	- Sets the layout of the tiles in the map
	*************************************************/
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

/************************************************************
SetTile()
- Changes the value of the tile sheet to that value if it is 
 valid else -1.
************************************************************/
void TileMap::SetTile( int x, int y, int tileId ) {
  if ( x < MAP_SIZE && x >= 0 && y < MAP_SIZE && y >= 0) {
    m_layout[y][x] = tileId;
  }

}

/**********************************************
GetTile()
- Returns the id of the tile at that location.
**********************************************/
int TileMap::GetTile( int x, int y ) const {
  if ( x < MAP_SIZE && x >= 0 && y < MAP_SIZE && y >= 0) {
    return m_layout[y][x];
  } else {
    return NULL_TILE;
  }
}