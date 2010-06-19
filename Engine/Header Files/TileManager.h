/*
Author: Ben Wright (bwright.au@gmail.com)
Co-Authors: pretty much everybody else
*/

#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <map>
#include <string>
#include <tr1/tuple>

class AnimSprite;
class TiXmlElement;

class TileManager {
 public:
  // Type, name, id of tile
  typedef std::tr1::tuple<std::string, std::string, int> tileInfo;

  // Calls LoadData to initialize tile manager
  explicit TileManager( const TiXmlElement *dataRoot );
  ~TileManager();

  // Parses data from <tile_map> section of state file
  void LoadData( const TiXmlElement *dataRoot );

  // Updates TileManager
  void Update();
  // Renders TileManager
	void Render();

  // Changes the value of the tile sheet to value passed if it is valid, else -1
  void SetTile( unsigned int x, unsigned int y, const std::string &tileName );

  // Returns type, name, and id of tile
  const TileManager::tileInfo& GetTile( unsigned int x, unsigned int y );

 private:
  typedef std::map< int, tileInfo* >::iterator TileInfoIter;
  static const tileInfo NULL_TILE_INFO;

  // Restricts copy constructor, and assignment.
  TileManager( const TileManager &manager );
  TileManager& operator=( const TileManager &manager );

  // Sets up tile animations from animation file passed
  void LoadTileAnims( const std::string &animPath );

  // Loads layout of tiles from data in xml element passed
	void LoadTileLayout( const TiXmlElement *root );

  AnimSprite *m_tileSprites;
  unsigned int m_numTiles;

	int m_layout[15][15];
  // Name of tile is key. Used for setting tiles.
  std::map<std::string, tileInfo> m_tileDataName;
  // Id of tile is key. Used for getting tiles.
  // Holds references to data stored in m_tileDataName.
  std::map<int, tileInfo*> m_tileDataId;
};

#endif
