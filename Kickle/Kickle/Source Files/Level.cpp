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
  unsigned int tileSize = App::GetApp()->GetConfig()->GetTileSize();
  return m_collisionMap.IsTileSolid( ((int)position.x-App::GetApp()->GetConfig()->GetXPad())/tileSize , 
                                      ((int)position.y-App::GetApp()->GetConfig()->GetYPad())/tileSize );
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
  unsigned int mapSize = App::GetApp()->GetConfig()->GetMapSize();

  std::string tileString( root->FirstChildElement( "tile_layout" )->GetText() );
  std::stringstream tileMapStream( tileString, std::ios_base::in );

  int tileLayout[15][15]; // TODO fix to dynamic
 
  int currentTile = 0;
  for( unsigned int i=0; i < mapSize; i++ ) {
    for ( unsigned int j=0; j < mapSize; j++ ) {
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
 
  for( unsigned int i=0; i < mapSize; i++ ) {
    for ( unsigned int j=0; j < mapSize; j++ ) {
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

/*****************************************
GetGameObject()
- Returns a pointer to the game object at
  the location. Else returns Null
*****************************************/
GameObject *Level::GetGameObject( unsigned int x, unsigned int y ) const {
  return m_gameObjectMap.GetGameObject( x, y );
}

GameObject *Level::GetGameObject( sf::Vector2f position ) const {
  return GetGameObject((unsigned int)(position.x -
                                        App::GetApp()->GetConfig()->GetXPad()) /
                                        App::GetApp()->GetConfig()->GetTileSize(),
                                        (unsigned int)(position.y -
                                        App::GetApp()->GetConfig()->GetYPad()) /
                                        App::GetApp()->GetConfig()->GetTileSize());
}