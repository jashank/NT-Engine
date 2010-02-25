#include <iostream>
#include <sstream>
#include <string>

#include "Level.h"
#include "Utilities.h"

Level::Level() {
}

Level::~Level() {
}

void Level::Init() {
}

void Level::Render() {
	m_tileMap.Render();
}

bool Level::IsTileSolid( const sf::Vector2f& position ) {
  // TODO cleanup.
  return m_collisionMap.IsTileSolid( ((int)position.x+24-Config::X_PAD)/48 , ((int)position.y+48-Config::Y_PAD)/48 );
}

bool Level::SetLevel( std::string levelPath ) {
  TiXmlDocument doc ( levelPath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlHandle handleDoc( &doc );
  TiXmlElement* root = handleDoc.FirstChildElement("map").Element();

  // Load in tiles.
  std::string tile_string( root->FirstChildElement("tile_layout")->GetText() );
  std::stringstream tile_layout( tile_string, std::ios_base::in );

  int layout[15][15];
 
  int current_tile = 0;
  for( unsigned int i=0; i < Config::MAP_SIZE; i++ ) {
    for ( unsigned int j=0; j < Config::MAP_SIZE; j++ ) {
      if ( tile_layout >> current_tile ) {
          layout[i][j] = current_tile;
      } else {
          layout[i][j] = -1;
      }
    }
  }
  // Get the sprite sheet
  std::string tile_sheet( root->FirstChildElement("tile_sprite_sheet")->GetText() );
  
  // Load In Collision Layout
  std::string collision_string( root->FirstChildElement("collision_layout")->GetText() );
  std::stringstream collision_layout( collision_string, std::ios_base::in );

  int collisionLayout[15][15];
 
  for( unsigned int i=0; i < Config::MAP_SIZE; i++ ) {
    for ( unsigned int j=0; j < Config::MAP_SIZE; j++ ) {
      if ( collision_layout >> current_tile ) {
          collisionLayout[i][j] = current_tile;
      } else {
          collisionLayout[i][j] = -1;
      }
    }
  }

	// Send them to the appropriate functions.
	bool tileMapLoaded = m_tileMap.SetTileMap( tile_sheet ,layout );
  m_collisionMap.SetCollisionMap( collisionLayout );

	return tileMapLoaded;
}

/********************************************
GetTile()
- Returns the id of the tile at x, y else -1
********************************************/
int Level::GetTile( int x, int y ) {
  return m_tileMap.GetTile( x, y );
}

/**************************************************
SetTile()
- Sets the tile at x, y to that tile with that id
***************************************************/
void Level::SetTile (int x, int y, int tileId, int collisionId ) {
  m_tileMap.SetTile( x, y, tileId );
  m_collisionMap.SetCollision( x, y, collisionId );
}