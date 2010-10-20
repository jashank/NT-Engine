#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string>

extern "C" {
  #include "lauxlib.h"
}

class lua_State;
class State;

/**
 * Starts off running first State loaded in and provides a Lua interface
 * for the user to control the current State and how the StateMachine will
 * progress (swith to new State, add things to current State, etc.).
 * Assumes window has already been created.
 */
class StateMachine {
 public:
  StateMachine() {}
  ~StateMachine();

  /**
   * Sets up StateMachine with State located at path passed, and creates lua
   * state for interaction between user and application. Returns true if
   * State loaded in successfully, false otherwise.
   */
  bool Setup( const std::string &filePath ); 

  /**
   * "Steps" the machine, running it through a single update.
   * @param dt delta time - amount of time to step forward
   */
  void Step( float dt );

  /**
   * Draws everything in StateMachine to the window.
   */
  void Render();

  //@{
  /**
   * Lua functions. See the State API for how these functions manipulate the
   * current state.
   * @param L lua state Object is in. Note that there is only one state in
   * application at any time.
   * @return Number of arguments pushed on the lua stack.
   */
  static int LuaLoadPath( lua_State *L );

  static int LuaReset( lua_State *L );

  static int LuaPortal( lua_State *L );

  static int LuaGetName( lua_State *L );

  static int LuaLogErr( lua_State *L );

  static int LuaCreateObject( lua_State *L );

  static int LuaDestroyObject( lua_State *L );

  static int LuaGetObject( lua_State *L );

  static int LuaGetObjects( lua_State *L );

  static int LuaGetNearestObject( lua_State *L );

  static int LuaGetObjectOnTile( lua_State *L );

  static int LuaObjectBlockingTile( lua_State *L );

  static int LuaGetTileInfo( lua_State *L );

  static int LuaTileIsCrossable( lua_State *L );

  static int LuaSetTile( lua_State *L );

  static int LuaOffsetCam( lua_State *L );

  static int LuaSetCamCenter( lua_State *L );

  static int LuaSetCamSpeed( lua_State *L );

  static int LuaSpeedUpCam( lua_State *L );

  static int LuaSlowDownCam( lua_State *L );
  //@}

 private:
  /* Names of functions when called in Lua. */
  static const luaL_Reg m_luaFuncs[];

  /* Lua state to create interaction between Lua and engine. */
  static lua_State *m_luaState;

  /* State currently being run by machine. */
  static State *m_runningState;

  /* Whether another state has been set to change to for next Step. */
  static bool m_nextStateSet;

  /* File path to next state. */
  static std::string m_nextStatePath;

  /**
   * Changes to State set in m_nextStatePath.
   */
  void NextState();
};

#endif // STATEMACHINE_H

