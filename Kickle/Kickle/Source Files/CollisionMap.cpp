#include "CollisionMap.h"

#include <sstream>

#include "App.h"
#include "Configuration.h"

/****************************
Constructors and Destructors
*****************************/
CollisionMap::CollisionMap() {
  Init();
}

CollisionMap::~CollisionMap() {
}

/************************************************************
Public Member Functions
************************************************************/

void CollisionMap::Init() {
  /*
  Initialize the collision map to all being crossable.
  */
  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      m_collisionLayout[i][j] = CROSSABLE;
    }
  }
}


bool CollisionMap::SetCollisionMap( TiXmlElement* root ) {
  unsigned int mapSize = Configuration::GetMapSize();

  std::string collisionString( root->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );
  
  int currentTile = 0;

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

  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      ( collisionLayout[i][j] == CROSSABLE ) ? 
          m_collisionLayout[i][j] = CROSSABLE : 
          m_collisionLayout[i][j] = NOT_CROSSABLE;
    }
  }

  return true;
}


bool CollisionMap::TileIsCrossable( unsigned int x, unsigned int y ) const {
  if ( !Configuration::IsTileValid( x, y )) {
    return false;
  } else {
    return m_collisionLayout[y][x] == CROSSABLE;
  }
}


void CollisionMap::SetCollision( unsigned int x, unsigned int y, int collisionId ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    m_collisionLayout[y][x] = collisionId;
  }

}


int CollisionMap::GetCollision( unsigned int x, unsigned int y ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    return m_collisionLayout[y][x];
  } else {
    return NOT_CROSSABLE;
  }
}