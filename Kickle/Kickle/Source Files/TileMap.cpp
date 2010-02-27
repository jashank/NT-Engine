#include "TileMap.h"

/******************************
Constructors and Destructors.
******************************/
TileMap::TileMap() {
	Init();
}

TileMap::~TileMap() {
}

/**********************************************
Init()
- Called by the constructor and initializes the
	tile map.
***********************************************/
void TileMap::Init() {
	// Do nothing.
}

/***********************************************
Render()
- Called by the Level class and renders the map
************************************************/
void TileMap::Render() {
  static App* app = App::GetApp();
	/*
	The current tile refers to the integer identifier that specifies
	which 48x48 tile on the 720x720 sprite sheet is selected.
	*/
	int current_tile = 0;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			current_tile = m_layout[i][j]; 
			if (current_tile != NULL_TILE) {
				/*
				Given the layouts current tile determines which tile it is
				refering to in the level sprite sheet. Note this relies on
				the properties of integer division.
				*/
				m_tileSprite.SetSubRect(
					sf::IntRect((current_tile%MAP_SIZE)*TILE_SIZE, 
					(current_tile/MAP_SIZE)*TILE_SIZE, 
					((current_tile%MAP_SIZE)*TILE_SIZE+TILE_SIZE), 
					((current_tile/MAP_SIZE)*TILE_SIZE+TILE_SIZE)));
				
				/*
				The sprite is then placed on the desired location on the map. Note the
				inverse of the J and I so that the map reads correctly.
				*/
        m_tileSprite.SetPosition( (float)j*TILE_SIZE+Config::X_PAD, (float)i*TILE_SIZE+Config::Y_PAD );
				/*
				Once we find the sprites sub rect on the level sprite sheet and
				have its position to be placed we draw it to the screen.
				*/
				app->Draw( m_tileSprite );
			}
		}
	}
}

/************************************************
SetTileMap()
- Called by the Level class trys to grab and load
	the tile map sheet and sets the buffer as given.
	The Tile Map does not deal directly with LUNA.
*************************************************/
void TileMap::SetTileMap( std::string mapPath, int layout[MAP_SIZE][MAP_SIZE] ) {
	m_tileSprite.SetImage( App::GetApp()->LoadImageW( mapPath ) );
	
	// Set the layout to that of the parsed in argument.
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = layout[i][j];
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
int TileMap::GetTile( int x, int y ) {
  if ( x < MAP_SIZE && x >= 0 && y < MAP_SIZE && y >= 0) {
    return m_layout[y][x];
  } else {
    return NULL_TILE;
  }
}