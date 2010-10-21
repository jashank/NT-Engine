#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <map>
#include <string>

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
}

#include "Matrix2D.h"
#include "Tile.h"

class AnimSprite;
class Camera;
class TiXmlElement;

/**
 * Holds and manages all Tiles in the current state. Provides functions to 
 * State API to modify the tiles and retrieve information from them. Note that
 * tile information and animations are specified in the same file (an animation
 * file).
 */
class TileManager {
 public:
  TileManager();
  ~TileManager();

  /**
   * Loads tile information from <tiles> element of State XML file. 
   * @param dataRoot parent element of tile information.
   * @return True if load was successful (no syntax errors). 
   */
  bool LoadData( const TiXmlElement *dataRoot );

  /**
   * Updates animations for tiles on screen.
   * @param dt delta time - amount of time to step forward
   */
  void Update( float dt );

  /**
   * Renders tiles on screen.
   */
  void Render( const Camera &cam );

  /**
   * @return Dimensions of tiles in this State (since tiles are square, only
   * necessary to return one integer).
   */
  int GetTileDim() const;

  /**
   * @return Width (in tiles) of tile map.
   */
  int GetMapWidth() const;

  /**
   * @return Height (in tiles) of tile map.
   */
  int GetMapHeight() const;

  /**
   * Checks to make sure coordinate passed is valid.
   * @param x x tile coordinate to check.
   * @param y y tile coordinate to check.
   * @return True if tile at (x,y) can be crossed.
   */
  bool TileIsCrossable( int x, int y ) const;

  /**
   * @param x x tile coordinate to check.
   * @param y y tile coordinate to check.
   * @return True if tile exists on map at (x, y).
   */
  bool TileOnMap( int x, int y ) const;

  //@{
  /**
   * Lua-related functions. See State API documentaions for details. Note that
   * these all return the number of arguments that the caller should
   * return to Lua.
   */
  int LuaGetTileInfo( lua_State *L );

  int LuaTileIsCrossable( lua_State *L );

  int LuaSetTile( lua_State *L );
  //@}

 private:
  typedef std::map< int, Tile* >::iterator TileInfoIter;
  typedef std::map< int, Tile* >::const_iterator ConstTileInfoIter;

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
  const Tile* GetTile( int x, int y ) const;

  /**
   * Checks to make sure x,y coordinate exists on map.
   * @param x x tile coordinate to change.
   * @param y y tile coordinate to change.
   * @param collisionId collision ID to set to tile. 0 is crossable, anything
   * else is not crossable. 
   */
  void SetCollision( int x, int y, int collisionId );

  /**
   * Array of animated sprites for the different tiles.
   */
  AnimSprite *m_tileSprites;

  /**
   * The number of different tiles loaded in.
   */
  int m_numTileTypes;

  /**
   * Number of tiles on x-axis.
   */
  int m_width; 

  /**
   * Number of tiles on y-axis.
   */
  int m_height; 

  /**
   * Number of tiles in TileManager (which is equivalent to the 
   * number of tiles in the current State's tile map).
   */
  int m_numTiles;

  /**
   * Tile dimensions (in pixels). Only a single value since tiles must be
   * perfect squares. Ex: a value of 48 would indicate 48x48 tiles.
   */
  int m_tileDim;

  /**
   * 'Name of tile/Tile' pairs. 
   */
  std::map<std::string, Tile> m_tileDataName;

  /**
   * 'ID of tile/pointer to Tile' pairs.
   */
  std::map<int, Tile*> m_tileDataId;

  /**
   * Represents layout of tiles on 2D grid. 
   * Accessed via typical (x,y).
   */
  nt::core::Matrix2D<int> *m_layout;
};

#endif // TILEMANAGER_H

