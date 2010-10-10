#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string>

class State;

/**
 * Starts off running first State loaded in and provides a Lua interface
 * for the user to control the current State and how the StateMachine will
 * progress (swith to new State, add things to current State, etc.).
 */
class StateMachine {
 public:
  StateMachine() {}
  ~StateMachine();

  /**
   * Sets up StateMachine with State located at path passed. Returns true if
   * State loaded in successfully, false otherwise.
   */
  bool Setup( const std::string &filePath ); 

  /**
   * Runs StateMachine with State currently loaded in. The machine will run
   * until the user exits the application window or exits via Lua.
   */
  void Run();

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
  //@}

 private:
  static State *m_runningState = NULL;
};

#endif // STATEMACHINE_H

