#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string>

#include <boost/scoped_ptr.hpp>

#include "State.h"

class lua_State;
class luaL_Reg;

namespace nt {

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
   * @param alpha blending factor between previous frame and current frame.
   * Should be from [0:1].
   */
  void Render( float alpha );

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

  static int LuaGetNearestToObject( lua_State *L );

  static int LuaGetObjectOnTile( lua_State *L );

  static int LuaGetObjectsOnTile( lua_State *L );

  static int LuaGetTileInfo( lua_State *L );

  static int LuaSetTile( lua_State *L );

  static int LuaPlayMusic( lua_State *L );

  static int LuaStopMusic( lua_State *L );

  static int LuaPauseMusic( lua_State *L );

  static int LuaMusicIsPlaying( lua_State *L );

  static int LuaLoopMusic( lua_State *L );

  static int LuaGetMusicVolume( lua_State *L );

  static int LuaSetMusicVolume( lua_State *L );

  static int LuaAdjustMusicVolume( lua_State *L );

  static int LuaKeepMusicPlaying( lua_State *L );

  static int LuaSpanCam( lua_State *L );

  static int LuaOffsetCam( lua_State *L );

  static int LuaCenterCam( lua_State *L );

  static int LuaSetCamSpeed( lua_State *L );

  static int LuaAdjustCamSpeed( lua_State *L );
  //@}

 private:
  /* Names of functions in State library. */
  static const luaL_Reg m_luaStateFuncs[];

  /* Names of functions in Map library. */
  static const luaL_Reg m_luaMapFuncs[];

  /* Names of functions in Music library. */
  static const luaL_Reg m_luaMusicFuncs[];

  /* Names of functions in Camera library. */
  static const luaL_Reg m_luaCamFuncs[];

  /* Names of functions in Error library. */
  static const luaL_Reg m_luaErrorFuncs[];

  /* Lua state to create interaction between Lua and engine. */
  static lua_State *m_luaState;

  /* State currently being run by machine. */
  static boost::scoped_ptr<State> m_runningState;

  /* Whether another state has been set to change to for next Step. */
  static bool m_nextStateSet;

  /* File path to next state. */
  static std::string m_nextStatePath;

  /**
   * Changes to State set in m_nextStatePath.
   */
  void NextState();
};

} // namespace nt

#endif // STATEMACHINE_H

