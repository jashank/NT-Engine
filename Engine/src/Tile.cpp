#include "Tile.h"

namespace nt {

Tile::Tile() : id( 0 ) {}


Tile::Tile( std::string type, std::string name, int id )
            : type( type ), name( name ), id( id ) {}

} // namespace nt
