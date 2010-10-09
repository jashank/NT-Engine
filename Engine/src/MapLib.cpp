#include "MapLib.h"

#include "tinyxml.h"

// Map data
namespace {
  int tileSize = 0;
  int mapWidth = 0;
  int mapHeight = 0;
}


namespace nt {
namespace map {

void LoadData( const TiXmlElement *mapRoot ) {
  const TiXmlElement *size = mapRoot->FirstChildElement( "tilesize" );
  size->Attribute( "px", &tileSize );

  const TiXmlElement *dims = mapRoot->FirstChildElement( "dims" );
  dims->Attribute( "width", &mapWidth );
  dims->Attribute( "height", &mapHeight );
}


int GetTileSize() {
  return tileSize;
}


int GetWidth() {
  return mapWidth;
}


int GetHeight() {
  return mapHeight;
}


bool InRange( int x, int y ) {
  return ( x >= 0 && x < mapWidth && y >= 0 && y < mapHeight );
}

} // namespace map
} // namespace nt

