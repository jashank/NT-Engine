#ifndef PRIORITY_RANGE_MATRIX_3D
#define PRIORITY_RANGE_MATRIX_3D

#include <queue>

#include "RangeMatrix3D.h"

namespace nt {
namespace core {

/**
 * Holds element and an integer representing its order. The lower the
 * order, the higher the priority. Only accessible by PriorityRangeMatrix3D.
 */
template < typename T >
class OrderedElem {
 public:
  bool operator<( const OrderedElem<T> &other ) {
    return order > other.order;
  }
 private:
  friend class PriorityRangeMatrix3D;
  int order;
  T elem;
};

/**
 * Has all of the exact same functionality as a RangeMatrix3D, but
 * also keeps track of order objects are added in. They can then
 * be retrieved in the order they were added in from some given range.
 */
template < typename T >
class PriorityRangeMatrix3D : public nt::core::RangeMatrix3D<OrderedElem<T> > {
 public:
  PriorityRangeMatrix3D( int cols, int rows )
   :RangeMatrix3D<OrderedElement<T> >( cols, rows ) {}

 private:
  //@{
  /**
   * Restrict default constructor, copy constructor, and assignment.
   */
  PriorityRangeMatrix3D();
  PriorityRangeMatrix3D( const PriorityRangeMatrix3D &mat );
  PriorityRangeMatrix3D& operator=( const PriorityRangeMatrix3D &mat );
  //@}

  /** Elements to return after SetRange is called. */
  std::priority_queue<OrderedElem<T> > m_toReturn;
};

} // namespace core
} // namespace nt
#endif // PRIORITY_RANGE_MATRIX_3D

