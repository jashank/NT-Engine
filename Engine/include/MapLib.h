#ifndef MAPLIB_H
#define MAPLIB_H

class TiXmlElement;


/**
 * Functions to load map dimensions from <map> element of State file and
 * retrieve data loaded in. Also provides some utility functions related
 * to map size and dimensions.
 */
namespace nt {
namespace map {

/**
 * Loads map tile size, and width/height (in tiles) from <map> element of
 * State file.
 */
void LoadData( const TiXmlElement *mapRoot );

/**
 * Returns size of a tile (in pixels). Only a single number because all tiles
 * are perfect squares.
 */
int GetTileSize();

/**
 * Returns width of map (in tiles).
 */
int GetWidth();

/**
 * Returns height of map (in tiles).
 */
int GetHeight(); 

/**
 * Returns whether tile coordinate passed exists on map.
 * @param x x-coordinate of tile location.
 * @param y y-coordinate of tile location.
 */
bool InRange( int x, int y );

} // namespace map
} // namespace nt

#endif // MAPLIB_H

