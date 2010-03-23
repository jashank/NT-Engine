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

  /************************************************
  Init
  -Loads the resources required by LevelState
  ************************************************/
  virtual void Init();

  /************************************************
  CleanUp
  -Releases the resources acquired by LevelState
  ************************************************/
  virtual void CleanUp();

  /************************************************
  Pause
  -Pauses the LevelState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the LevelState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle LevelState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update LevelState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render LevelState's frame
  ************************************************/
  virtual void Render();

  /***********************************************
  IsTileSolid()
  -Returns whether position or tile passed is solid.
  ************************************************/
  bool IsTileSolid( const sf::Vector2f& position ) const;
  bool IsTileSolid( Uint x, Uint y ) const;

  /***********************************************
  TileHasSolidObject()
  - Returns whether position or tile passed 
    has a solid object on it.
   ************************************************/
  bool TileHasSolidObject( const sf::Vector2f &position );
  bool TileHasSolidObject( Uint x, Uint y );

  /********************************************
  GetTile()
  - Returns the id of the tile at x, y, else -1
  ********************************************/
  int GetTile( int x, int y ) const;

  /**************************************************
  SetTile()
  - Sets the tile at x, y to that tile with that id
  ***************************************************/
  void SetTile ( int x, int y, int tileId, int collisionId );

  /**************************************************
  AddGameObject()
  - Adds gameObject to the gameObjectMap
  ***************************************************/
  void AddGameObject( GameObject *gameObject );
  
  /**************************************************
  DetectObjectCollision()
  - Returns whether gameObject has collided with
    another object
  ***************************************************/
  GameObject* DetectObjectCollision( const GameObject *gameObject );

  /************************************************
  LuaCreateGameObject
  - Allows a GameObject to be created from Lua
  ************************************************/
  static int LuaCreateGameObject( lua_State *L );

  /************************************************
  LuaIsTileSolid
  - Exposes Level's IsTileSolid to Lua
  ************************************************/
  static int LuaIsTileSolid( lua_State *L );

  /************************************************
  LuaTileHasSolidObject
  - Exposes Level's TileHasSolidObject to Lua
  ************************************************/
  static int LuaTileHasSolidObject( lua_State *L );

  /************************************************
  RegisterLuaPlayFuncts()
  - Registers functions associated with the
    LevelState to Lua.
  ************************************************/
  static void RegisterLuaPlayFuncts( lua_State *L );
  
 private:	
  static const luaL_Reg luaPlayFuncts[]; // Functions to register to Lua

  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  LevelState();
  LevelState( const LevelState &title );
  LevelState& operator=( const LevelState &title );

  /***********************************************
	SetLevel()
	-Given the path to a lua script, opens it and
   tries to determine information needed about the
   level (tile layer, entity later, collision layer..)
	************************************************/
	bool SetLevel( std::string levelPath );

  /**************************************
	GetVectorXTile(), GetVectorYTile()
  - Returns x/y tile based on position
    given by sf::Vector2f
	**************************************/
  Uint GetVectorXTile( const sf::Vector2f &position ) const;
  Uint GetVectorYTile( const sf::Vector2f &position ) const;
  
  static LevelState *m_instance; // LevelState has one instance

  CollisionMap m_collisionMap;
  GameObjectMap m_gameObjectMap;
	TileMap m_tileMap;
};

#endif