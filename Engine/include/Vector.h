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

