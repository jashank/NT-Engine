/*
Author: Ben Wright (bwright.au@gmail.com)
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <string>

#include "BasicTypeDefs.h"

class AnimSprite;

class TileMap {
public:
	TileMap();
	virtual ~TileMap();

  // Loads the tilesheet and tile animations from xml file
  void LoadTileAnims( const std::string& tileSheet, const std::string &anims );

  // Renders the map (called by LevelState)
	void Render();

  // Sets the layout of the tiles in the map
	void SetTileLayout( int layout[15][15] );

  // Changes the value of the tile sheet to that value if it is valid, else -1.
  void SetTile( int x, int y, int tileId );

  // Returns id of tile at specified location.
  int GetTile( int x, int y ) const;

private:
  // Called by constructor and initializes tilemap.
	void Init();

  static const int MAP_SIZE = 15;
	static const int NULL_TILE = -1;
	static const int TILE_SIZE = 48;

  AnimSprite *m_tileSprites;
  Uint m_numTiles;

	int m_layout[MAP_SIZE][MAP_SIZE];
};

#endif