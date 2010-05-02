#ifndef COLLISION_MAP_H
#define COLLISION_MAP_H

#include "tinyxml.h"

class CollisionMap {
 public:
  CollisionMap();
  virtual ~CollisionMap();

  // Given a layout of 15 by 15, sets all the values in the collision layout 
  // to those values.
  bool SetCollisionMap( TiXmlElement* root );

  // Returns true if the tile at X Y is solid, else false;
  bool TileIsCrossable( unsigned int x, unsigned int y ) const;

  // Changes the value of the tile sheet to that value if it is valid. Else -1.
  void SetCollision( unsigned int x, unsigned int y, int collisionId );

  // Returns id of tile at that location.
  int GetCollision( unsigned int x, unsigned int y );

 private:
  // Sets the initial value of the collision layout to NOT_SOLID. 
  // Called by the constructor.
  void Init();

  static const unsigned int MAP_SIZE = 15;
  static const int NOT_CROSSABLE = 1;
  static const int CROSSABLE = 0;

  int m_collisionLayout[MAP_SIZE][MAP_SIZE];
};

#endif