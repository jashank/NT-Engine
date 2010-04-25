/*
Author: Ben Wright (bwright.au@gmail.com)
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <map>
#include <string>
#include <tuple>

#include "BasicTypeDefs.h"
#include "tinyxml.h"

class AnimSprite;

class TileMap {
 public:
  // Type, name, id of tile
  typedef std::tr1::tuple<std::string, std::string, int> tileInfo;

	TileMap();
	virtual ~TileMap();

  // Loads the tilesheet and tile animations from xml file
  void LoadTileAnims( const std::string& tileSheet, const std::string &anims );

  // Renders the map (called by LevelState)
	void Render();

  // Sets the layout of the tiles in the map
	void SetTileLayout( TiXmlElement* root );

  // Changes the value of the tile sheet to value passed if it is valid, else -1
  void SetTile( int x, int y, const std::string &tileName );

  // Returns type, name, and id of tile
  const TileMap::tileInfo& GetTile( int x, int y );

 private:
  // Called by constructor and initializes tilemap.
	void Init();

  static const int MAP_SIZE = 15;
	static const int NULL_TILE = -1;
	static const int TILE_SIZE = 48;
  static const tileInfo NULL_TILE_INFO;

  AnimSprite *m_tileSprites;
  Uint m_numTiles;

	int m_layout[MAP_SIZE][MAP_SIZE];
  // Name of tile is key. Used for setting tiles.
  std::map<std::string, tileInfo> m_tileDataName;
  // Id of tile is key. Used for getting tiles.
  std::map<int, tileInfo> m_tileDataId;
};

#endif