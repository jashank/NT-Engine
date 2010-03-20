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

	/*********************************************
	LoadTileAnims()
  - Loads the tilesheet
	- Loads the tile animations from an xml file
	**********************************************/
  void LoadTileAnims( const std::string& tileSheet, const std::string &anims );

	/*********************************************
	Render()
	- Called by the Level class and renders the map
	**********************************************/
	void Render();

	/************************************************
	SetTileLayout()
	- Sets the layout of the tiles in the map
	*************************************************/
	void SetTileLayout( int layout[15][15] );

  /************************************************************
  SetTile()
  - Changes the value of the tile sheet to that value if it is 
  valid else -1.
  ************************************************************/
  void SetTile( int x, int y, int tileId );
  /**********************************************
  GetTile()
  - Returns the id of the tile at that location.
  **********************************************/
  int GetTile( int x, int y ) const;

private:
  /**********************************************
  Init()
	- Called by the constructor and initializes the
	  tile map.
	***********************************************/
	void Init();
  static const int MAP_SIZE = 15;
	static const int NULL_TILE = -1;
	static const int TILE_SIZE = 48;

  AnimSprite *m_tileSprites;
  Uint m_numTiles;

	int m_layout[MAP_SIZE][MAP_SIZE];
};

#endif