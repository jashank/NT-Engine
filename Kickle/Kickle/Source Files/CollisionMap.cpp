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
  Initialize the collision map to all being not solid.
  */
  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      m_collisionLayout[i][j] = NOT_SOLID;
    }
  }
}


bool CollisionMap::SetCollisionMap( TiXmlElement* root ) {
  Uint mapSize = Configuration::GetMapSize();

  std::string collisionString( root->GetText() );
  std::stringstream collisionMapStream( collisionString, std::ios_base::in );
  
  int currentTile = 0;

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

  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      if ( collisionLayout[i][j] == SOLID ) {
        m_collisionLayout[i][j] = SOLID;
      } else {
        m_collisionLayout[i][j] = NOT_SOLID;
      }

    }
  }

  return true;
}


bool CollisionMap::TileIsSolid( Uint x, Uint y ) const {
  /*
  Making sure that you're not going to run off the array.
  */
  if ( Configuration::IsTileValid( x, y ) ) {
    if ( m_collisionLayout[y][x] == SOLID ) {
      return true;
    }
  } else {
    return true; // Off the map is always solid.
  }

  return false;
}


void CollisionMap::SetCollision( Uint x, Uint y, int collisionId ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    m_collisionLayout[y][x] = collisionId;
  }

}


int CollisionMap::GetCollision( Uint x, Uint y ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    return m_collisionLayout[y][x];
  } else {
    return SOLID;
  }
}