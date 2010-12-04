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
    objMan = state->m_objectManager.get();
    tileMan = state->m_tileManager.get();
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


void CullTileRect( nt::core::IntRect &rect ) {
  if ( rect.topLeft.x < 0 ) {
    rect.topLeft.x = 0;
  }
  if ( rect.topLeft.y < 0 ) {
    rect.topLeft.y = 0;
  }

  int farTileX = GetMapWidth() - 1;
  int farTileY = GetMapHeight() - 1;

  if ( rect.bottomRight.x > farTileX ) {
    rect.bottomRight.x = farTileX;
  }
  if ( rect.bottomRight.y > farTileY ) {
    rect.bottomRight.y = farTileY;
  }
}

} // namespace state
} // namespace nt

