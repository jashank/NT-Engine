#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include "BaseState.h"

#include <string>

#include <SFML/Graphics.hpp>

#include "BasicTypeDefs.h"
#include "CollisionMap.h"
#include "GameObjectMap.h"
#include "TileMap.h"

class GameObject;

/************************************************
LevelState
- State for levels in game.
************************************************/
class LevelState : public BaseState {
 public:
  virtual ~LevelState();

  static LevelState* GetInstance();
  static void DestroyInstance();

  // Loads the resources required by LevelState
  virtual void Init();

  // Releases the resources acquired by LevelState
  virtual void CleanUp();

  // Pauses the LevelState
  virtual void Pause();

  // Unpauses the LevelState
  virtual void Resume();

  // Handle LevelState's user input events
  virtual void HandleEvents();

  // Update LevelState's frame
  virtual void Update();

  // Render LevelState's frame
  virtual void Render();

  // Returns whether 'position' or 'tile' passed is solid
  bool TileIsSolid( const sf::Vector2f& position ) const;
  bool TileIsSolid( Uint x, Uint y ) const;

  // Returns whether 'position' or 'tile' passed has a solid object on it
  bool TileHasGridObject( const sf::Vector2f &position );
  bool TileHasGridObject( Uint x, Uint y );

  // Returns the id of the tile at x, y, else -1
  int GetTile( int x, int y ) const;

  // Sets the tile at x, y to that tile with that id
  void SetTile ( int x, int y, int tileId, int collisionId );
  
  // Detects whether 'gameObject' has collided with any other GameObjects.
  // Returns GameObject it has collided with, or NULL if no collision.
  GameObject* DetectObjectCollision( const GameObject *gameObject );

  /*********************************************************
   Lua Functions
  *********************************************************/

  // Allows a GameObject to be created from Lua
  static int LuaCreateGameObject( lua_State *L );

  // Exposes Level's IsTileSolid to Lua
  static int LuaTileIsSolid( lua_State *L );

  // Exposes Level's TileHasSolidObject to Lua
  static int LuaTileHasGridObject( lua_State *L );

  // Allows Lua to grab a GameObject from the Level
  static int LuaGetGameObject( lua_State *L );

  // Registers functions associated with the LevelState to Lua
  static void RegisterLuaLevelFuncts( lua_State *L );
  
 private:	
  static const luaL_Reg luaLevelFuncts[]; // Functions to register to Lua

  // Restricts construction, copy construction, and assignment.
  LevelState();
  LevelState( const LevelState &title );
  LevelState& operator=( const LevelState &title );

	// Given the path to a lua script, opens it and tries to determine 
  // information needed about the level (TileMap, CollisionMap, etc.)
	bool SetLevel( std::string levelPath );

  // Returns x/y tile based on position given by sf::Vector2f
  Uint GetVectorXTile( const sf::Vector2f &position ) const;
  Uint GetVectorYTile( const sf::Vector2f &position ) const;
  
  static LevelState *m_instance; // LevelState has one instance

  CollisionMap m_collisionMap;
  GameObjectMap m_gameObjectMap;
	TileMap m_tileMap;
};

#endif