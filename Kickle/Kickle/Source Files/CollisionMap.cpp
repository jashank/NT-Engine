#include "CollisionMap.h"

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
      m_collision_layout[i][j] = NOT_SOLID;
    }
  }
}


bool CollisionMap::SetCollisionMap( int layout[MAP_SIZE][MAP_SIZE] ) {
  for ( int i = 0; i < MAP_SIZE; i++ ) {
    for ( int j = 0; j < MAP_SIZE; j++ ) {
      if ( layout[i][j] == SOLID ) {
        m_collision_layout[i][j] = SOLID;
      } else {
        m_collision_layout[i][j] = NOT_SOLID;
      }

    }
  }
  return true;
}


bool CollisionMap::IsTileSolid( Uint x, Uint y ) const {
  /*
  Making sure that your not going to run off the array.
  */
  if ( Configuration::IsTileValid( x, y ) ) {
    if ( m_collision_layout[y][x] == SOLID ) {
      return true;
    }
  } else {
    return true; // Off the map is always solid.
  }

  return false;
}


void CollisionMap::SetCollision( Uint x, Uint y, int collisionId ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    m_collision_layout[y][x] = collisionId;
  }

}


int CollisionMap::GetCollision( Uint x, Uint y ) {
  if ( Configuration::IsTileValid( x, y ) ) {
    return m_collision_layout[y][x];
  } else {
    return SOLID;
  }
}