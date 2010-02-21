/*
Author: Ben Wright (bwright.au@gmail.com)
*/

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "App.h"
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
private:
		static const int MAP_SIZE = 15;
		static const int NULL_TILE = -1;
		static const int TILE_SIZE = 48;
		static const int X_PAD = 50;
		static const int Y_PAD = 10;

		sf::Image m_tileSheet;
		sf::Sprite m_tileSprite;

		int m_layout[MAP_SIZE][MAP_SIZE];
};

#endif