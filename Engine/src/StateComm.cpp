#include "StateComm.h"

#include "ObjectManager.h"
#include "State.h"
#include "TileManager.h"

namespace {
  ObjectManager *objMan = NULL;
  TileManager *tileMan = NULL;
}


namespace nt {
namespace state {

bool SetStateComm( State *state ) {
  if ( !objMan && !tileMan ) {
    objMan = &state->m_objectManager;
    tileMan = &state->m_tileManager;
    return true;
  }
  return false;
}


void EndStateComm() {
  objMan = NULL;
  tileMan = NULL;
}


bool TileIsOpen( int x, int y ) {
  if ( objMan && tileMan ) {
    if ( InRange( x, y )) {
      return ( !objMan->ObjectBlockingTile( x, y ) && 
                tileMan->TileIsCrossable( x, y ));
    }
  }
  return false;
}


int GetTileSize() {
  if ( tileMan ) {
    return tileMan->GetTileDim();
  }
  return 0;
}


int GetMapWidth() {
  if ( tileMan ) {
    return tileMan->GetMapWidth();
  }
  return 0;
}


int GetMapHeight() {
  if ( tileMan ) {
    return tileMan->GetMapHeight();
  }
  return 0;
}


int InRange( int x, int y ) {
  if ( tileMan ) {
    return ( x >= 0 && x < tileMan->GetMapWidth() &&
             y >= 0 && y < tileMan->GetMapHeight());
  }
  return false;
}

} // namespace state
} // namespace nt

