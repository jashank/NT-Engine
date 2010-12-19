/* NT Engine - Copyright (c) 2010-2011 Alex Shipley, Ben Wright

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

#ifndef NT_VECTOR_H
#define NT_VECTOR_H

#include "Direction.h"

namespace nt {

/**
 * Vector has a magnitude with a type of T and a direction.
 */
template<typename T>
struct Vector {
  /**
   * Default initializer for magnitude, direction is North.
   */
  Vector() : direction( dir::North ), magnitude(T()) {}

  dir::Direction direction;
  T magnitude;
};

//@{
/**
 * Common Vector types.
 */
typedef Vector<int> IntVec;
typedef Vector<float> FloatVec;
//@}
 

} // namespace nt

#endif // NT_VECTOR_H

