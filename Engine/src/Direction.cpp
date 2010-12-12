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

} // namespace dir
} // namespace nt
