#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "CollisionMap.h"
#include "GameObjectMap.h"
#include "Configuration.h"
#include "tinyxml.h"

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


  /***************************************************************
  Update()
  - Runs m_gameObject.Update see that method for more information
  ****************************************************************/
  void Update();

	/**************************************************************
	Render()
	- Runs m_tileMap.Render() see that method for more information
  - Runs m_collisionMap.Render()
  - Runs m_gameObject.Render()

	***************************************************************/
	void Render();

	/*************************************************************************
	SetLevel()
	-Given the path to a lua script opens it an trys to determine information
	needed about the level such as the tile layer the entity layer and the
	collision layer.
	**************************************************************************/
	bool SetLevel( std::string levelPath );

  /****************************************
  IsTileSolid()
  -Wrapper function for the collision map.
  *****************************************/
  bool IsTileSolid( const sf::Vector2f& position ) const;

   /****************************************
  IsTileSolid()
  -Wrapper function for the collision map.
  *****************************************/
  bool IsTileSolid( int x, int y ) const;

  /********************************************
  GetTile()
  - Returns the id of the tile at x, y else -1
  ********************************************/
  int GetTile( int x, int y );

  /**************************************************
  SetTile()
  - Sets the tile at x, y to that tile with that id
  ***************************************************/
  void SetTile (int x, int y, int tileId, int collisionId );

private:
	TileMap m_tileMap;
  CollisionMap m_collisionMap;
  GameObjectMap m_gameObjectMap;
};

#endif