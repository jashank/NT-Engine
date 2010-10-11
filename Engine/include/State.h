#ifndef STATE_H
#define STATE_H

#include <string>
#include <map>

#include <SFML/System/Clock.hpp>

#include "KeyManager.h"
#include "ObjectManager.h"
#include "MusicManager.h"
#include "StateComm.h"
#include "TileManager.h"

/**
 * A highly customizable construct representing a single "scene" in the
 * application. Each State has an XML file where the user specifies tile
 * information (layout, size, etc.), Objects that will be in the State upon
 * State creation, music to use, and portals to other States. 
 */
class State {
 public:
  State() {}
  ~State();

  /**
   * Initializes State via loading in State file at file path and setting up
   * communication interface among parts of State.
   */
  bool Init( const std::string &filePath );

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
  friend bool nt::state::SetStateComm( State *state );

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  State( const State &state );
  State& operator=( const State &state );
  //@}

  /**
   * Loads State from file at file path.
   * @param filePath path to file to load.
   * @return True if file loads successfully (no syntax errors in file).
   */
  bool LoadFromFile( const std::string &filePath );

  /** Manages keys registered for monitoring in this State.*/
  KeyManager m_keyManager; 

  /** Manages all Objects in this State. */
  ObjectManager m_objectManager;

  /** Manages all music in this State. */
  MusicManager m_musicManager; 

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

