#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BaseState.h"

#include "BasicTypeDefs.h"
#include "GameObject.h"
#include "Level.h"

/************************************************
PlayState
-Inherits from BaseState
-Contains all TitleState Content
************************************************/
class PlayState : public BaseState {
 public:
  virtual ~PlayState();

  static BaseState* GetInstance();
  static void DestroyInstance();

  /************************************************
  Init
  -Loads the resources required by TitleState
  ************************************************/
  virtual void Init();

  /************************************************
  CleanUp
  -Releases the resources acquired by TitleState
  ************************************************/
  virtual void CleanUp();

  /************************************************
  Pause
  -Pauses the PlayState
  ************************************************/
  virtual void Pause();

  /************************************************
  Resume
  -Unpauses the PlayState
  ************************************************/
  virtual void Resume();

  /************************************************
  HandleEvents
  -Handle PlayState's user input events
  ************************************************/
  virtual void HandleEvents();

  /************************************************
  Update
  -Update PlayState's frame
  ************************************************/
  virtual void Update();

  /************************************************
  Render
  -Render PlayState's frame
  ************************************************/
  virtual void Render();

  /************************************************
  GetLevel
  -Returns current level in PlayState
  ************************************************/
  Level& GetLevel();

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
    PlayState to Lua.
  ************************************************/
  static void RegisterLuaPlayFuncts( lua_State *L );
  
 private:	
  /************************************************
  Restricts construction, copy construction, 
  and assignment.
  ************************************************/
  PlayState();
  PlayState( const PlayState &title );
  PlayState& operator=( const PlayState &title );

  static const luaL_Reg luaPlayFuncts[]; // Functions to register to Lua
  
  static PlayState *m_instance; // PlayState has one instance
  Level m_level;
};

#endif