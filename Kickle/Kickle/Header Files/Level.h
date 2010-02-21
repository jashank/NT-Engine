#ifndef LEVEL_H
#define LEVEL_H

//TODO
// Associate Y value with render preference.

#include "TileMap.h"

class Level {
public:
	Level();
	virtual ~Level();

	void Init();
	void Render();

	bool SetLevel( std::string levelPath );
private:
	TileMap m_tileMap;
	//CollisionMap m_collision_map;
};

#endif