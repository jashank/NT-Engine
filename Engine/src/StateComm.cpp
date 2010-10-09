#include "StateComm.h"

#include "MapLib.h"
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
    if ( nt::map::InRange( x, y )) {
      return ( !objMan->ObjectBlockingTile( x, y ) && 
                tileMan->TileIsCrossable( x, y ));
    }
  }
  return false;
}


} // namespace state
} // namespace nt

