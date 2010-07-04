#include "Tile.h"

Tile::Tile() {
}

Tile::Tile( std::string type, std::string name, int id, int cid )
            : type( type ), name( name ), id( id ), cid( cid ) {
}

Tile::~Tile() {
}
