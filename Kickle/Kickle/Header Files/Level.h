#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include "BasicTypeDefs.h"
#include "CollisionMap.h"
#include "Configuration.h"
#include "GameObjectMap.h"
#include "TileMap.h"
#include "tinyxml.h"

class Level {
public:
  Level();
	virtual ~Level();
	
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
  bool IsTileSolid( const sf::Vector2f& position );
  bool IsTileSolid( Uint x, Uint y );

  /****************************************
  TileHasSolidObject()
  - Returns whether the tile at the specified
    location has a solid object on it.
  *****************************************/
  bool TileHasSolidObject( const sf::Vector2f &position );
  bool TileHasSolidObject( Uint x, Uint y );

  /********************************************
  GetTile()
  - Returns the id of the tile at x, y else -1
  ********************************************/
  int GetTile( int x, int y );

  /**************************************************
  SetTile()
  - Sets the tile at x, y to that tile with that id
  ***************************************************/
  void SetTile ( int x, int y, int tileId, int collisionId );

  /**************************************************
  AddGameObject( GameObject *gameObject )
  - Adds gameObject to the gameObjectMap
  ***************************************************/
  void AddGameObject( GameObject *gameObject );
  
  /**************************************************
  DetectObjectCollision()
  - Returns whether gameObject has collided with
    another object
  ***************************************************/
  GameObject *DetectObjectCollision( GameObject *gameObject );


private:
  /**************************************
	Init()
	-Called by the constructor simply sets
	up all needed variables.
	**************************************/
	void Init();

  /**************************************
	GetVectorXTile()
  - Returns x tile based on position
    given by sf::Vector2f
	**************************************/
  Uint GetVectorXTile( const sf::Vector2f &position ) const;

  /**************************************
	GetVectorYTile()
  - Returns y tile based on position
    given by sf::Vector2f
	**************************************/
  Uint GetVectorYTile( const sf::Vector2f &position ) const;

	TileMap m_tileMap;
  CollisionMap m_collisionMap;
  GameObjectMap m_gameObjectMap;
};

#endif