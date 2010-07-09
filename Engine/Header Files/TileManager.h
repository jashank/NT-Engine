/*
Author: Ben Wright (bwright.au@gmail.com)
Co-Authors: pretty much everybody else
*/

#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "Tile.h"

class AnimSprite;
class TiXmlElement;

class TileManager {
 public:
  TileManager();
  ~TileManager();

  // Parses data from <tile_map> section of state file
  // Returns whether load was successful.
  bool LoadData( const TiXmlElement *dataRoot );

  // Updates TileManager
  void Update();
  // Renders TileManager
	void Render();

  // Changes the value of the tile sheet to value passed if it is valid, else -1
  void SetTile( int x, int y, const std::string &tileName );

  // Returns type, name, and id of tile
  const Tile& GetTile( int x, int y );

  // Return dimensions of tile for this map (tiles are square)
  int GetTileDim() const;

  // Return number of tiles on map horizontally and vertically
  int GetMapWidth() const;
  int GetMapHeight() const;

  // Collision Stuff
  // Returns true if the tile at X Y is solid, else false;
  bool TileIsCrossable( int x, int y ) const;

  // Changes the value of the tile sheet to that value if it is valid.
  // Else -1.
  void SetCollision( int x, int y, int collisionId );

  // Returns id of tile at that location.
  int GetCollision( int x, int y ) const;

 private:
  typedef std::map< int, Tile* >::iterator TileInfoIter;
  typedef std::map< int, Tile* >::const_iterator ConstTileInfoIter;
  static const Tile NULL_TILE_INFO;

  // Restricts copy constructor, and assignment.
  TileManager( const TileManager &manager );
  TileManager& operator=( const TileManager &manager );

  // Sets up tile animations from animation file passed
  // Returns whether load was successful.
  bool LoadTileAnims( const std::string &animPath );

  // Loads layout of tiles from data in xml element passed
  // Returns whether load was successful.
  bool LoadTileLayout( const TiXmlElement *root );

  // Retrieves tile info from <strip> tag
  // Returns whether load was successful.
  bool GetTileInfo( const TiXmlElement *strip );

  AnimSprite *m_tileSprites;
  int m_numTileTypes;
  int m_width; // Number of tiles x dir
  int m_height; // Number of tiles y dir
  int m_numTiles;
  int m_tileDim;
  // Name of tile is key. Used for setting tiles.
  std::map<std::string, Tile> m_tileDataName;
  // Id of tile is key. Used for getting tiles.
  // Holds references to data stored in m_tileDataName.
  std::map<int, Tile*> m_tileDataId;
  std::vector<std::vector<int> > m_layout;

  // Collision Stuff
  static const int NOT_CROSSABLE = 1;
  static const int CROSSABLE = 0;
};

#endif
