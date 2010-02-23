#include "CollisionMap.h"
#include <iostream>
/****************************
Constructors and Destructors
*****************************/
CollisionMap::CollisionMap() {
  Init();
}

CollisionMap::~CollisionMap() {
}

/*************************************************************
Init()
- Sets the initial value of the collision layout to NOT_SOLID.
   Called by the constructor.
 **************************************************************/
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

/******************************************************************
SetCollisionMap()
- Given a layout of 15 by 15 sets all the values in the collision
  layout to those values.
******************************************************************/
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

/*****************************************************************
IsSolid()
- Returns true if the tile at X Y is solid, else false;
*****************************************************************/
bool CollisionMap::IsTileSolid( int x, int y ) {
  /*
  Making sure that your not going to run off the array.
  */
  if ( x < MAP_SIZE && x >= 0 && y < MAP_SIZE && y >= 0) {
    if ( m_collision_layout[y][x] == SOLID ) {
      return true;
    }
  } else {
    return true; // Off the map is always solid.
  }
  return false;
}