#include "MapLib.h"

namespace nt {
namespace map {

void IncPoint( int &x, int &y, int width, int height ) {
  ++x;
  if ( x >= width ) {
    x = 0;
    ++y;
  }
  if ( y >= height ) {
    x = -1;
    y = -1;
  }
}

} // namespace map
} // namespace nt

