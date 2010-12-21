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

#ifndef NT_MUSIC_MANAGER_H
#define NT_MUSIC_MANAGER_H

#include <SFML/Audio/Music.hpp>

#include "NamePathManager.h"

class lua_State;
class TiXmlElement;

namespace nt {

/**
 * Manages music in the state providing interface to perform various
 * actions on the music playing/to be played.
 */
class MusicManager {
 public:
  /**
   * Loads data from <music> element of State file.
   */
  explicit MusicManager( const TiXmlElement *root ); 
  ~MusicManager();

  //@{
  /**
   * Lua-related functions. See State API documentation for details. Note that
   * these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaPlayMusic( lua_State *L );

  int LuaStopMusic( lua_State *L );

  int LuaPauseMusic( lua_State *L );

  int LuaMusicIsPlaying( lua_State *L ) const;

  int LuaLoopMusic( lua_State *L );

  int LuaGetMusicVolume( lua_State *L ) const;

  int LuaSetMusicVolume( lua_State *L );

  int LuaAdjustMusicVolume( lua_State *L );

  int LuaKeepMusicPlaying( lua_State *L );
  //@}

 private:
  /**
   * Returns pointer to sf::Music associated with string passed. String may
   * be the music's name or file path. Returns NULL if none found.
   */
  sf::Music *GetMusic( const std::string &nameOrPath ) const;

  /**
   * Checks to see if music is already playing before playing it.
   * Assumes music passed is not NULL.
   */
  void SafePlay( sf::Music *music );

  /**
   * Checks to see if music passed is the same as the current music. If it
   * isn't then changes to music passed and plays it. Otherwise, safely
   * plays the current music. Assumes music passed is not NULL.
   */
  void CheckAndPlay( sf::Music *music );

  /**
   * Stops music currently playing and assigns sf::Music pointer passed to it.
   * Assumes music passed is not NULL.
   */
  void StopAndSet( sf::Music *music );

  /** Holds all music loaded in. */
  NamePathManager<sf::Music> m_music;

  /** If true, won't stop the music when manager is destroyed. */
  bool m_keepMusicPlaying;

  /** Music currently in use (playing, stopped, whatever). */
  sf::Music *m_currentMusic;
};

} // namespace nt

#endif // NT_MUSIC_MANAGER_H

