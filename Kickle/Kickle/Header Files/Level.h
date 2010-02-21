#ifndef LEVEL_H
#define LEVEL_H

//TODO
// Associate Y value with render preference.

#include "TileMap.h"

class Level {
public:
	Level();
	virtual ~Level();
	
	/**************************************
	Init()
	-Called by the constructor simply sets
	up all needed variables.
	**************************************/
	void Init();

	/**************************************************************
	Render()
	- Runs m_tileMap.Render() see that method for more information
	***************************************************************/
	void Render();

	/*************************************************************************
	SetLevel()
	-Given the path to a lua script opens it an trys to determine information
	needed about the level such as the tile layer the entity layer and the
	collision layer.
	**************************************************************************/
	bool SetLevel( std::string levelPath );
private:
	TileMap m_tileMap;
	//CollisionMap m_collision_map;
};

#endif