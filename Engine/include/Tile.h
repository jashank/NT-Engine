#ifndef NT_TILE_H
#define NT_TILE_H

#include <string>

namespace nt {

/**
 * A base definition of a tile which consists of type,
 * name, and tile id (id).
 */
class Tile {
 public:
  Tile();
  Tile( std::string type, std::string name, int id );

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
};

} // namespace nt

#endif // NT_TILE_H

