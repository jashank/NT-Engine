#include "Direction.h"

namespace nt {
namespace dir {

bool IsCardinal( Direction dir ) {
  // All cardinal directions are even numbers
  return (dir % 2) == 0;
}


Direction GetOpposite( Direction dir ) {
  return (dir + 4) % 8;
}


bool IsNorthward( Direction dir ) {
  return ( dir < East || dir > West );
}


bool IsSouthward( Direction dir ) {
  return ( dir > East && dir < West );
}


bool IsWestward( Direction dir ) {
  return ( dir > South );
}


bool IsEastward( Direction dir ) {
  return ( dir > North && dir < South );
}
    

} // namespace dir
} // namespace nt
