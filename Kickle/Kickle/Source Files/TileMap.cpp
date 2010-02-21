#include "TileMap.h"

TileMap::TileMap() {
	Init();
}

TileMap::~TileMap() {
}

void TileMap::Init() {
	// Do nothing.
}

void TileMap::Render() {
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
				m_tileSprite.SetPosition( (float)j*TILE_SIZE, (float)i*TILE_SIZE );
				/*
				Once we find the sprites sub rect on the level sprite sheet and
				have its position to be placed we draw it to the screen.
				*/
				App::GetApp()->Draw( m_tileSprite );
			}
		}
	}
}

bool TileMap::SetTileMap( std::string mapPath, int layout[MAP_SIZE][MAP_SIZE] ) {
	// Check that the tile sheet exists. If it doesn't return false.
	if ( m_tileSheet.LoadFromFile( mapPath ) ) {
		m_tileSprite.SetImage( m_tileSheet );
	} else {
		return false;
	}
	
	// Set the layout to that of the parsed in argument.
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = layout[i][j];
		}
	}

	return true;
}