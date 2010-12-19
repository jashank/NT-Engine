/* NT Engine - Copyright (c) 2010-2011 Alex Shipley

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

#include "Direction.h"

namespace nt {
namespace dir {

bool IsCardinal( Direction dir ) {
  // All cardinal directions are even numbers
  return (dir % 2) == 0;
}


Direction GetOpposite( Direction dir ) {
  return static_cast<Direction>( (dir + 4) % 8 );
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
