#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <SFML/Audio/Music.hpp>

#include "NamePath.h"

class TiXmlElement;
class lua_State;

/**
 * Manages music in the state providing interface to perform various
 * actions on the music playing/to be played.
 */
class MusicManager {
 public:
  MusicManager(); 
  ~MusicManager();

  /**
   * Loads music in from <music> element of State XML file.
   * @param root <music> element of file.
   * @return True if no errors in loading.
   */
  bool LoadData( const TiXmlElement *root );

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
  typedef std::map<const NamePath, boost::shared_ptr<sf::Music > > map_type;

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

  /** If true, won't stop the music when manager is destroyed. */
  bool m_keepMusicPlaying;

  /** Music currently in use (playing, stopped, whatever). */
  sf::Music *m_currentMusic;

  /** 
   * Holds pointers to sf::Music and their associated names and paths.
   */
  map_type m_music;
};

#endif // MUSIC_MANAGER_H

