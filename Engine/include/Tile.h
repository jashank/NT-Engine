#ifndef TILE_H
#define TILE_H

#include <string>

/**
 * A base definition of a tile which consists of type,
 * name, tile id (id) and collision id (cid).
 */
class Tile {
 public:
  Tile();
  Tile( std::string type, std::string name, int id, int cid );
  ~Tile();

  /**
   * Allows for grouping of multiple tiles into a single type.
   */
  std::string type;

  /**
   * Allows for more readable code (as opposed to using the Tile's ID) when 
   * retrieving tile information from lua script.
   */
  std::string name;

  /**
   * ID of tile, should be unique. Used to quickly recognize attributes of 
   * Tile and have a clean tile layout in the State file.
   */
  int id; 

  /**
   * Collision ID of tile. A crossable tile has a collision ID of 0. Any other
   * ID (typically 1) indicates that the tile is not crossable.
   */
  int cid;
};

#endif // TILE_H

