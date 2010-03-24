#ifndef COLLISION_MAP_H
#define COLLISION_MAP_H

#include "BasicTypeDefs.h"

class CollisionMap {
 public:
  CollisionMap();
  virtual ~CollisionMap();

  // Given a layout of 15 by 15, sets all the values in the collision layout 
  // to those values.
  bool SetCollisionMap( int layout[15][15] );

  // Returns true if the tile at X Y is solid, else false;
  bool IsTileSolid( Uint x, Uint y ) const;

  // Changes the value of the tile sheet to that value if it is valid. Else -1.
  void SetCollision( Uint x, Uint y, int collisionId );

  // Returns id of tile at that location.
  int GetCollision( Uint x, Uint y );

 private:
  // Sets the initial value of the collision layout to NOT_SOLID. 
  // Called by the constructor.
  void Init();

  static const Uint MAP_SIZE = 15;
  static const int SOLID = 1;
  static const int NOT_SOLID = 0;

  int m_collision_layout[MAP_SIZE][MAP_SIZE];
};

#endif