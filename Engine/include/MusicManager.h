#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <map>
#include <string>

namespace sf {
  class Music;
}

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
   * @return True if no errors.
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

  int LuaMusicIsPlaying( lua_State *L );

  int LuaLoopMusic( lua_State *L );

  int LuaSetVolume( lua_State *L );

  int LuaIncVolume( lua_State *L );

  int LuaDecVolume( lua_State *L );

  int LuaKeepMusicPlaying( lua_State *L );
  //@}

 private:
  /** 
   * Name/path pairs for music loaded in. If music has no associated name
   * then path is used as name.
   */
  std::map<std::string, std::string> m_namePaths;

  /** Music currently playing. */
  sf::Music *m_music;
};

#endif // MUSIC_MANAGER_H

