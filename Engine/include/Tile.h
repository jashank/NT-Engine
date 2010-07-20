#ifndef TILE_H
#define TILE_H

#include <string>

/**
 * Tile class. A base definition of a tile which consists of type,
 * name, tile id (id) and collision id (cid). We may be moving this
 * to a structure in the future to optimize preformance and since
 * it does not really fit the class type.
 */

class Tile {
 public:
  /// Constructor
  Tile();
  /// Constructor with member definitions
  Tile( std::string type, std::string name, int id, int cid );
  /// Destructors
  ~Tile();

  // Data
  std::string type;
  std::string name;
  int id; // Tile ID
  int cid; // Collision ID
};

#endif // TILE_H
