#ifndef NT_TILEMANAGER_H
#define NT_TILEMANAGER_H

#include <map>
#include <string>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "AnimSprite.h"
#include "Matrix2D.h"
#include "Rect.h"
#include "Tile.h"

namespace nt {
  class Camera;
}

class TiXmlElement;
class lua_State;

namespace nt {

/**
 * Holds and manages all Tiles in the current state. Provides functions to 
 * State API to modify the tiles and retrieve information from them. Note that
 * tile information and animations are specified in the same file (an animation
 * file).
 */
class TileManager {
 public:
  /**
   * Loads tile data from <tiles> element of State file.
   */
  TileManager( const TiXmlElement *root );

  /**
   * Updates animations for tiles on screen.
   * @param dt delta time - amount of time to step forward
   */
  void Update( float dt );

  /**
   * Renders tiles on screen.
   * @param cam Camera currently viewing the State.
   */
  void Render( float alpha, const Camera &cam );

  /**
   * Dimensions of tile in pixels. All tiles are perfect squares so only
   * necessary to return one value.
   */
  int GetTileSize() const;

  /**
   * @return IntRect with corners representing corners of map. For example,
   * a map with a bottom right corner of (5,6) would return an
   * IntRect with a bottom right corner of (5,6).
   */
  const IntRect &GetMapRect() const;

  //@{
  /**
   * Lua-related functions. See State API documentaions for details. Note that
   * these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaGetTileInfo( lua_State *L ) const;

  int LuaSetTile( lua_State *L );
  //@}

 private:
  typedef std::map<int, Tile*> idMap_type;

  static const int CROSSABLE = 0;
  static const int BLANK_TILE_ID = -1;

  //@{
  /**
   * Restrict copy constructor and assignment.
   */
  TileManager( const TileManager &manager );
  TileManager& operator=( const TileManager &manager );
  //@}

  /**
   * Loads tile information from <tiles> element of State XML file. 
   * Called by constructor.
   */
  void LoadData( const TiXmlElement *root );

  /**
   * Loads animation data for tiles.
   * @param animPath path to tile animation file.
   * @return True if load was successful (no syntax errors).
   */
  bool LoadTileAnims( const std::string &animPath );

  /**
   * Loads layout of tiles from xml element <layout> passed.
   * @param root <layout> element to load data from.
   * @return True if load was successful (no syntax errors).
   */
  bool LoadTileLayout( const TiXmlElement *layout );

  /**
   * Loads tile information from <strip> element in tile animation file.
   * @param strip <strip> element to load data from.
   * @return True if load was successful (no syntax errors).
   */
  bool LoadTileInfo( const TiXmlElement *strip );

  /**
   * Changes tile at x,y to tile associated with tile name passed. Checks to
   * make sure that x,y coordinate is valid on tile grid.
   * @param x x tile coordinate to change.
   * @param y y tile coordinate to change.
   * @param tileName type of tile associated with this tile name will be the
   * new tile at the tile coordinate passed.
   */
  void SetTile( int x, int y, const std::string &tileName );

  /**
   * Checks to make sure x,y coordinate exists on map.
   * @param x x tile coordinate to check.
   * @param y y tile coordinate to check.
   * @return Tile at x,y coordinate passed. NULL if coordinate is invalid or
   * no tile located at coordinate.
   */
  Tile *GetTile( int x, int y ) const;

  /**
   * Array of animated sprites for the different tiles.
   */
  boost::scoped_array<AnimSprite> m_tileSprites;

  /**
   * The number of different tiles loaded in.
   */
  int m_numTileTypes;

  /**
   * IntRect representation of tile map. Coordinates represent map boundaries
   * (in tiles).
   */
  IntRect m_mapRect;

  /**
   * Number of tiles in TileManager (which is equivalent to the 
   * number of tiles in the current State's tile map).
   */
  int m_numTiles;

  /**
   * Tile dimensions (in pixels). Only a single value since tiles must be
   * perfect squares. Ex: a value of 48 would indicate 48x48 tiles.
   */
  int m_tileSize;

  /**
   * 'Name of tile/Tile' pairs. 
   */
  std::map<std::string, Tile> m_tileDataName;

  /**
   * 'ID of tile/pointer to Tile' pairs.
   */
  idMap_type m_tileDataId;

  /**
   * Represents layout of tiles on 2D grid. 
   * Accessed via typical (x,y).
   */
  boost::scoped_ptr<Matrix2D<int> > m_layout;
};

} // namespace nt

#endif // NT_TILEMANAGER_H

