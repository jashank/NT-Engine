/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright, Nathan Rice

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

#ifndef NT_STATE_H
#define NT_STATE_H

#include <string>
#include <map>

#include <boost/scoped_ptr.hpp>
#include <SFML/System/Clock.hpp>

#include "Camera.h"
#include "FontManager.h"
#include "ObjectManager.h"
#include "MusicManager.h"
#include "TileManager.h"

class lua_State;

namespace nt {

/**
 * A highly customizable construct representing a single "scene" in the
 * application. Each State has an XML file where the user specifies tile
 * information (layout, size, etc.), Objects that will be in the State upon
 * State creation, music to use, and portals to other States. 
 */
class State {
 public:
  /**
   * Loads in State file at file path. Camera is initially set to view
   * entire State, starting from top left tile.
   * @param filePath path to State file.
   * @param L lua state used for interaction with scripts.
   */
  State( const std::string &filePath, lua_State *L );
  ~State() {}

  /**
   * Should be called immediately after construction. Initializes components
   * that can't be initialized during construction process.
   */
  void Init();

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
   * @param alpha blending factor between previous frame and current frame.
   * Should be from [0:1].
   */
  void Render( float alpha );

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

  int LuaGetObject( lua_State *L ) const;

  int LuaGetObjects( lua_State *L ) const;

  int LuaGetNearestObject( lua_State *L ) const;

  int LuaGetNearestToObject( lua_State *L ) const;

  int LuaGetObjectOnTile( lua_State *L ) const;

  int LuaGetObjectsOnTile( lua_State *L ) const;

  int LuaGetTileInfo( lua_State *L ) const;

  int LuaSetTile( lua_State *L );

  int LuaPlayMusic( lua_State *L );

  int LuaStopMusic( lua_State *L );

  int LuaPauseMusic( lua_State *L );

  int LuaMusicIsPlaying( lua_State *L ) const;

  int LuaLoopMusic( lua_State *L );

  int LuaGetMusicVolume( lua_State *L ) const;

  int LuaSetMusicVolume( lua_State *L );

  int LuaAdjustMusicVolume( lua_State *L );

  int LuaKeepMusicPlaying( lua_State *L );

  int LuaSpanCam( lua_State *L );

  int LuaOffsetCam( lua_State *L );

  int LuaCenterCam( lua_State *L );

  int LuaSetCamSpeed( lua_State *L );

  int LuaAdjustCamSpeed( lua_State *L );
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
   * Loads State from file at file path, setting camera to view entire State.
   * @param filePath path to file to load.
   * @param L lua state to attach to members that require it
   * @return True if file loads successfully (no syntax errors in file).
   */
  bool LoadFromFile( const std::string &filePath, lua_State *L );

  /** Camera that controls what is viewed in the State. */
  boost::scoped_ptr<Camera> m_camera;

  /** Manages all fonts loaded in this State. */
  boost::scoped_ptr<FontManager> m_fontManager;

  /** Manages all Objects in this State. */
  boost::scoped_ptr<ObjectManager> m_objectManager;

  /** Manages all music in this State. */
  boost::scoped_ptr<MusicManager> m_musicManager; 

  /**
   * Manages all Tiles in this State.
   */
  boost::scoped_ptr<TileManager> m_tileManager;

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
};

} // namespace nt

#endif // NT_STATE_H

