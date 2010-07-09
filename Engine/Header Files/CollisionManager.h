#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

class TiXmlElement;

class CollisionManager {
 public:
  CollisionManager();

  // Parses data from <collision_map> section of state file
  void LoadData( const TiXmlElement *dataRoot );

  // Returns true if the tile at X Y is solid, else false;
  bool TileIsCrossable( int x, int y ) const;

  // Changes the value of the tile sheet to that value if it is valid. Else -1.
  void SetCollision( int x, int y, int collisionId );

  // Returns id of tile at that location.
  int GetCollision( int x, int y );

 private:
  static const int NOT_CROSSABLE = 1;
  static const int CROSSABLE = 0;

  // Restricts copy constructor, and assignment.
  CollisionManager( const CollisionManager &manager );
  CollisionManager& operator=( const CollisionManager &manager );

  int m_collisionLayout[15][15];
  int m_width;
  int m_height;
};

#endif
