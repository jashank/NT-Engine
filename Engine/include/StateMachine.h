/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef NT_STATEMACHINE_H
#define NT_STATEMACHINE_H

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
  explicit StateMachine( const std::string &filePath );
  ~StateMachine();

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

#endif // NT_STATEMACHINE_H

