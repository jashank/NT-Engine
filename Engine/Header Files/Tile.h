#ifndef TILE_H
#define TILE_H

#include <string>

class Tile {
 public:
  Tile();
  Tile( std::string type, std::string name, int id, int cid );
  ~Tile();

  // Data
  std::string type;
  std::string name;
  int id; // Tile ID
  int cid; // Collision ID
};

#endif // TILE_H
