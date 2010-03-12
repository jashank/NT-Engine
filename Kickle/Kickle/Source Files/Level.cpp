#include <iostream>
#include <sstream>
#include <string>

#include "Level.h"
#include "Utilities.h"

Level::Level() {
  Init();
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

bool Level::IsTileSolid( const sf::Vector2f& position ) const {
  Uint tileSize = Configuration::GetTileSize();
  Uint tileX = (Uint)( position.x - Configuration::GetXPad() ) / tileSize;
  Uint tileY = (Uint)( position.y - Configuration::GetYPad() ) / tileSize;

  return ( m_collisionMap.IsTileSolid( tileX, tileY ) );
}

bool Level::IsTileSolid( int x, int y ) const {
   return m_collisionMap.IsTileSolid( x, y );
}

bool Level::SetLevel( std::string levelPath ) {

  TiXmlDocument doc ( levelPath.c_str() );
  
  if ( !doc.LoadFile() ) {
    return false;
  }

  TiXmlElement* root = doc.FirstChildElement( "map" );

  // Load in tiles.
  Uint mapSize = Configuration::GetMapSize();

  std::string tileString( root->FirstChildElement( "tile_layout" )->GetText() );
  std::stringstream tileMapStream( tileString, std::ios_base::in );

  int tileLayout[15][15]; // TODO fix to dynamic
 
  int currentTile = 0;
  for( Uint i=0; i < mapSize; i++ ) {
    for ( Uint j=0; j < mapSize; j++ ) {
      if ( tileMapStream >> currentTile ) {
        tileLayout[i][j] = currentTile;
      } else {
          tileLayout[i][j] = -1;
      }
    }
  }
  // Get the sprite sheet
  std::string tileSheet( root->FirstChildElement( "tile_sheet" )->GetText() );
  // Load In Collision Layout
  std::string collisionString( root->FirstChildElement( "collision_layout" )->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );

  int collisionLayout[15][15]; // TODO fix to dynamic
 
  for( Uint i=0; i < mapSize; i++ ) {
    for ( Uint j=0; j < mapSize; j++ ) {
      if ( collisionMapStream >> currentTile ) {
          collisionLayout[i][j] = currentTile;
      } else {
          collisionLayout[i][j] = -1;
      }
    }
  }

  // GameObjectMap parses its section by itself instead of in this method.
 if ( root->FirstChildElement( "game_objects" ) ) {
    m_gameObjectMap.SetGameObjectMap( root->FirstChildElement( "game_objects" ) );
 }

	// Send them to the appropriate functions.
	m_tileMap.SetTileMap( tileSheet, tileLayout );
  m_collisionMap.SetCollisionMap( collisionLayout );

	return true;
}


int Level::GetTile( int x, int y ) {
  return m_tileMap.GetTile( x, y );
}


void Level::SetTile (int x, int y, int tileId, int collisionId ) {
  m_tileMap.SetTile( x, y, tileId );
  m_collisionMap.SetCollision( x, y, collisionId );
}


void Level::AddGameObject( GameObject *gameObject ) {
  m_gameObjectMap.AddGameObject( gameObject );
}


GameObject *Level::DetectObjectCollision( GameObject *gameObject ) {
  return m_gameObjectMap.DetectCollision( gameObject );
}
