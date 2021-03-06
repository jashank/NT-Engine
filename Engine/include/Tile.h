/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright

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

#ifndef NT_TILE_H
#define NT_TILE_H

#include <string>

namespace nt {

/**
 * A base definition of a tile which consists of type,
 * name, and tile id (id).
 */
class Tile {
 public:
  Tile();
  Tile( std::string type, std::string name, int id );

  /**
   * Allows for grouping of multiple tiles into a single type.
   */
  std::string type;

  /**
   * Allows for more readable code (as opposed to using the Tile's ID) when 
   * retrieving tile information from lua script.
   */
  std::string name;

  /**
   * ID of tile, should be unique. Used to quickly recognize attributes of 
   * Tile and have a clean tile layout in the State file.
   */
  int id; 
};

} // namespace nt

#endif // NT_TILE_H

