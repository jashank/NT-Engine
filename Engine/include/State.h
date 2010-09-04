#ifndef STATE_H
#define STATE_H

#include <string>
#include <map>

extern "C" {
  #include "lauxlib.h"
}

#include "ObjectManager.h"
#include "SoundManager.h"
#include "TileManager.h"

class lua_State;

/**
 * A highly customizable construct representing a single "scene" in the
 * application. Each State has an XML file where the user specifies tile
 * information (layout, size, etc.), Objects that will be in the State upon
 * State creation, music to use, and portals to other States. Also provides
 * an API for manipulating the State from lua scripts. See the State Guide for
 * more information.
 */
class State {
 public:
  State() {}
  ~State() {}

  /**
   * Loads State from file at file path.
   * @param filePath path to file to load.
   * @return True if file loads successfully (no syntax errors in file).
   */
  bool LoadFromFile( const std::string &filePath );

  /**
   * Handles events for anything that handles events in the State.
   */
  void HandleEvents();

  /**
   * Updates everything that needs regular updating in the State.
   */
  void Update();

  /**
   * Renders everything that needs to be rendered in the State.
   */
  void Render();

  /**
   * @return TileManager held by State.
   */
  TileManager& GetTileManager();

  /**
   * @return ObjectManager held by State.
   */
  ObjectManager& GetObjectManager();

  /**
   * Registers Lua functions to State API for access in scripts.
   * @param L lua state that functions will be registered in.
   */
  static void RegisterLuaFuncs( lua_State *L );

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
  //@}

 private:
  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  State( const State &state );
  State& operator=( const State &state );
  //@}

  /**
   * Method names for State API to associate with methods in State class. For
   * example, "Portal" is matched with LuaPortal.
   */
  static const luaL_Reg LuaFuncs[]; 

  /** Manages all Objects in this State. */
  ObjectManager m_objectManager; /** Manages all Objects in this State. */

  /** Manages all sound in this State. */
  SoundManager m_soundManager; 

  /**
   * 'name/file path' pairs that represent portals. A portal is like a
   * nickname for the path to a State, allowing easy access to new States via 
   * the Portal function in the State API.  
   */
  std::map<std::string, std::string> m_portals; 

  /**
   * File path to this State.
   */
  std::string m_path; 

  /**
   * Name of this State (same as name of file without extension).
   */
  std::string m_name; 

  /**
   * Manages all Tiles in this State.
   */
  TileManager m_tileManager;
};

#endif // STATE_H

