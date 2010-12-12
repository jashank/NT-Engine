#include "Direction.h"

namespace nt {
namespace dir {

bool IsCardinal( Direction dir ) {
  switch( dir ) {
    case North:
    case East:
    case South:
    case West: {
      return true;
    }
    default: {
      return false;
    }
  }
}


Direction GetOpposite( Direction dir ) {
  switch( dir ) {
    case North: return South;
    case Northeast: return Southwest;
    case East: return West;
    case Southeast: return Northwest;
    case South: return North;
    case Southwest: return Northeast;
    case West: return East;
    case Northwest: return Southeast;
    default: return North; // arbitrary
  }
}
