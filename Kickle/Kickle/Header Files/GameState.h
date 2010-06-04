#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

class TileManager;
class CollisionManager;
class GameObjectManager;
class SoundManager;

/********************************************************************
GameState
 - Generic class for all states in the game. Data defined in XML file.
   Provides functions to Lua, allowing it access information on, and
   manipulate the state.
********************************************************************/
class GameState {
 public:
  // Constructs GameState. Doesn't load in any data.
  GameState();
  ~GameState();

  // Loads GameState from file located at path passed. 
  bool LoadFromFile( const std::string &path );

  // Handles events generated by user
  void HandleEvents();

  // Updates to changes made since last frame
  void Update();

  // Renders current frame
  void Render();

  // Returns state's tile Manager
  TileManager* GetTileManager() const;

  // Returns state's collision Manager
  CollisionManager* GetCollisionManager() const;

  // Returns state's GameObjectManager
  GameObjectManager* GetGameObjectManager() const;

  // Returns whether tile specified has a grid object
  bool TileHasGridObject( unsigned int x, unsigned int y );

  //-------------------------------------------------------
  // Lua Functions
  //-------------------------------------------------------

  // Returns state's lua state (pointer because lua functions take pointer)
  lua_State* GetLuaState();

  // Creates an GameObject given its XML file. Reports error if file is invalid.
  static int LuaCreateGameObject( lua_State *L );
  
  // Destroys GameObject passed. Does nothing if GameObject doesn't exist.
  static int LuaDestroyGameObject( lua_State *L );

  // Returns a GameObject given the GameObject's "type". NULL if there is none.
  static int LuaGetGameObject( lua_State *L );

  // Returns GameObject located on tile. NULL if none exists.
  static int LuaGetGameObjectOnTile( lua_State *L );

  // Returns type, name, and id of tile (in that order).
  // ("", "", -1) if no tile is invalid.
  static int LuaGetTileInfo( lua_State *L );

  // Returns whether tile specified is crossable. False if tile is invalid.
  static int LuaTileIsCrossable( lua_State *L );

  // Returns whether tile specified has a gridlocked GameObject on it. False if tile is invalid.
  static int LuaTileHasGridObject( lua_State *L );

  // Sets tile specified to tile associated with name passed.
  // Also must pass collisionID, 0 for crossable, 1 for not crossable. 
  static int LuaSetTile( lua_State *L );

  // Changes state to unmodified version of state defined by XML file passed.
  // Reports error if file is invalid.
  static int LuaNewState( lua_State *L );
  
 private:
  static const luaL_Reg LuaGameStateFuncts[]; // Functions to register to Lua

  // Restricts copy constructor, and assignment.
  GameState( const GameState &state );
  GameState& operator=( const GameState &state );

  TileManager *m_tileManager;
  CollisionManager *m_collisionManager;
  GameObjectManager *m_gameObjectManager;
  // GUI Storage
  SoundManager *m_soundManager;
  lua_State *m_luaState;
};

#endif