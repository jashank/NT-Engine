/*
Author: Ben Wright (bwright.au@gmail.com)
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "App.h"
#include "Configuration.h"

#include <SFML/Graphics.hpp>
#include <string>

class TileMap {
public:
	TileMap();
	virtual ~TileMap();
	
	/**********************************************
	Init()
	- Called by the constructor and initializes the
	  tile map.
	***********************************************/
	void Init();

	/*********************************************
	Render()
	- Called by the Level class and renders the map
	**********************************************/
	void Render();

	/************************************************
	SetTileMap()
	- Called by the Level class trys to grab and load
	  the tile map sheet and sets the buffer as given.
	  The Tile Map does not deal directly with LUNA.
	*************************************************/
	bool SetTileMap( std::string mapPath , int layout[15][15] );

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
  int GetTile( int x, int y );

private:
		static const int MAP_SIZE = 15;
		static const int NULL_TILE = -1;
		static const int TILE_SIZE = 48;

		sf::Image m_tileSheet;
		sf::Sprite m_tileSprite;

		int m_layout[MAP_SIZE][MAP_SIZE];
};

#endif