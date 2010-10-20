#ifndef STATE_H
#define STATE_H

#include <string>
#include <map>

#include <SFML/System/Clock.hpp>

#include "Camera.h"
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
   * @param filePath path to the State file to load in
   * @param L lua state being used for application
   */
  bool Init( const std::string &filePath, lua_State *L );

  /**
   * Handles events for anything that handles events in the State.
   */
  void HandleEvents();

  /**
   * Updates everything that needs regular updating in the State.
   * @param dt delta time - amount of time to step forward
   */
  void Update( float dt );

  /**
   * Renders everything that needs to be rendered in the State.
   */
  void Render();

  /**
   * Returns file path of State file loaded into this State.
   */
  std::string GetPath() const;

  /**
   * Returns file path associated with a portal name set.
   * If no path associated returns empty string.
   */
  std::string GetPortalPath( std::string &port ) const;

  //@{
  /**
   * Lua-related functions. See State API documentation for details. Note that
   * these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaGetName( lua_State *L ) const;

  int LuaCreateObject( lua_State *L );

  int LuaDestroyObject( lua_State *L );

  int LuaGetObject( lua_State *L );

  int LuaGetObjects( lua_State *L );

  int LuaGetNearestObject( lua_State *L );

  int LuaGetObjectOnTile( lua_State *L );

  int LuaObjectBlockingTile( lua_State *L );

  int LuaGetTileInfo( lua_State *L );

  int LuaTileIsCrossable( lua_State *L );

  int LuaSetTile( lua_State *L );

  int LuaOffsetCam( lua_State *L );

  int LuaSetCamCenter( lua_State *L );

  int LuaSetCamSpeed( lua_State *L );

  int LuaSpeedUpCam( lua_State *L );

  int LuaSlowDownCam( lua_State *L );
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
   * @param L lua state to attach to members that require it
   * @return True if file loads successfully (no syntax errors in file).
   */
  bool LoadFromFile( const std::string &filePath, lua_State *L );

  /** Camera that controls what is viewed in the State. */
  Camera m_camera;

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

