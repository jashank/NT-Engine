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

void Level::Update() {
  m_gameObjectMap.Update();
}

void Level::Render() {
  // The Rendering order is important.
	m_tileMap.Render();
  m_gameObjectMap.Render();
}

bool Level::IsTileSolid( const sf::Vector2f& position ) {
  // Y is + 48 so it measures from the bottom of its feet.
  return m_collisionMap.IsTileSolid( ((int)position.x-App::GetApp()->GetConfig()->GetXPad())/48 , ((int)position.y-App::GetApp()->GetConfig()->GetYPad()+48)/48 );
}

bool Level::IsTileSolid( int x, int y ) {
   return m_collisionMap.IsTileSolid( x, y );
}

bool Level::SetLevel( std::string levelPath ) {

  TiXmlDocument doc ( levelPath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement("map");

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
  std::string tile_sheet( root->FirstChildElement("tile_sheet")->GetText() );
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

  // GameObjectMap parses its section by itself instead of in this method.
 if ( root->FirstChildElement("game_objects") ) {
    m_gameObjectMap.SetGameObjectMap( root->FirstChildElement("game_objects") );
 }

	// Send them to the appropriate functions.
	m_tileMap.SetTileMap( tile_sheet ,layout );
  m_collisionMap.SetCollisionMap( collisionLayout );

	return true;
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