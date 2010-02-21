#include "TileMap.h"

TileMap::TileMap() {
}

TileMap::~TileMap() {
}

void TileMap::Init() {
}

void TileMap::Render() {
	int current_tile = 0;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			current_tile = m_layout[i][j]; 
			if (current_tile != NULL_TILE) {
				m_tileSprite.SetSubRect(sf::IntRect((current_tile%MAP_SIZE)*TILE_SIZE, (current_tile/MAP_SIZE)*TILE_SIZE, 
									((current_tile%MAP_SIZE)*TILE_SIZE+TILE_SIZE), ((current_tile/MAP_SIZE)*TILE_SIZE+TILE_SIZE)));
				m_tileSprite.SetPosition( (float)j*TILE_SIZE, (float)i*TILE_SIZE );
				App::GetApp()->Draw( m_tileSprite );
			}
		}
	}
}

bool TileMap::SetTileMap( std::string mapPath, int layout[MAP_SIZE][MAP_SIZE] ) {
	if ( m_tileSheet.LoadFromFile( mapPath ) ) {
		m_tileSprite.SetImage( m_tileSheet );
	} else {
		return false;
	}

	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			 m_layout[i][j] = layout[i][j];
		}
	}

	return true;
}