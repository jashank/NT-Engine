#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <list>

#include "Rect.h"

namespace nt {
namespace core {

/**
 * 3D matrix without index checking.
 * Provides functions to easily get objects stored in a range.
 * Note that this class is static in its first 2 dimensions (x,y) but dynamic
 * in the last dimension (z). So while you cannot go outside of the x & y
 * bounds, you can add as many objects as you please within those bounds.
 */
template < typename T >
class RangeMatrix3D {
 public:
  /**
   * Constructor creates 3 dimensional matrix bounded by first 2 dimensions.
   */
  RangeMatrix3D( int cols, int rows )
    :m_cols( cols ), m_rows( rows ) {
    int size = cols * rows;
    m_mat = new std::list<T>[size];
  }

  ~RangeMatrix3D() {
    SAFEDELETEA( m_mat );
  }

  /**
   * Sets a range in the matrix to retrieve elements from via GetElem.
   * For example, if the user were to pass (3, 5, 2, 4), GetElem would return
   * elements from (3-5, 2-4). Calling this function clears any current
   * iterations being made (i.e. GetElem will not continue to return elements
   * in a range specified in a previous call).
   */
  void SetRange( int xMin, int yMin, int xMax, int yMax ) {
    m_range.topLeft.x = xMin;
    m_range.topLeft.y = yMin;
    m_range.bottomRight.x = xMax;
    m_range.bottomRight.y = yMax;

    m_rangeItr.x = xMin;
    m_rangeItr.y = yMin;

    m_elemItr = m_mat[Index( xMin, yMin )].begin();
  }

  /**
   * Saves the current spot being iterated over. Useful for when you need
   * to do a nested iteration which requires a new SetRange call.
   */
  void SavePlace() {
    m_saveRange = m_range;
    m_saveRangeItr = m_rangeItr;
    m_saveElemItr = m_elemItr;
  }

  /**
   * If SavePlace() was called previously, goes back to spot saved at.
   * Note that moving any elements in between SavePlace and ToPlace may
   * invalidate the iterator saved (depending on where things were moved.)
   */
  void ToPlace() {
    m_range = m_saveRange;
    m_rangeItr = m_saveRangeItr;
    m_elemItr = m_saveElemItr;
  }

  /**
   * Returns next element in range last set (just at 0,0 if none set yet).
   * Iterates by column. When no more elements exist, returns NULL.
   */
  T GetElem() {
    // Counts loop iterations to keep iteration position correct
    int count = 0;

    for ( ; m_rangeItr.x <= m_range.bottomRight.x; ++m_rangeItr.x ) {
      for ( ; m_rangeItr.y <= m_range.bottomRight.y; ++m_rangeItr.y ) {

        int i = Index( m_rangeItr.x, m_rangeItr.y );
        if ( count > 0 ) {
          m_elemItr = m_mat[i].begin();
        }
        if ( m_elemItr != m_mat[i].end() ) {
          return *m_elemItr++;
        }
        ++count;
      }
      m_rangeItr.y = m_range.topLeft.y;
    }
    return NULL;
  }

  /**
   * Adds an element at specified position in matrix.
   */
  void AddElem( T elem, int x, int y ) {
    int i = Index( x, y );
    m_mat[i].push_back( elem );
  }

  /**
   * Removes element at specified position in matrix. If element isn't there
   * then does nothing.
   */
  void RemoveElem( T elem, int x, int y ) {
    int i = Index( x, y );
    m_mat[i].remove( elem );
  }

  /**
   * Moves last element returned by GetElem to position specified.
   */
  void MoveReturnedElem( int x, int y ) {
    if ( m_rangeItr.x != x || m_rangeItr.y != y ) {
      --m_elemItr;
      int newi = Index( x, y );
      m_mat[newi].push_back( *m_elemItr );

      int oldi = Index( m_rangeItr.x, m_rangeItr.y );
      m_elemItr = m_mat[oldi].erase( m_elemItr );
    }
  }

 private:
  typedef typename std::list<T>::iterator ZItr;

  //@{
  /**
   * Restrict default constructor, copy constructor, and assignment.
   */
  RangeMatrix3D();
  RangeMatrix3D( const RangeMatrix3D &mat );
  RangeMatrix3D& operator=( const RangeMatrix3D &mat );
  //@}

  /**
   * Returns coordinate's corresponding index into array.
   */
  int Index( int x, int y ) {
    return (m_cols * x) + y;
  }

  int m_cols; /** Columns in matrix. */
  int m_rows; /** Rows in matrix. */
  std::list<T> *m_mat; /** Array representation of matrix. */

  nt::core::IntRect m_range; /** Range currently being iterated over. */
  nt::core::IntVec m_rangeItr; /** Iterates over dimensions of range. */
  ZItr m_elemItr; /** Iterates over elements of a dimension. */

  nt::core::IntRect m_saveRange; /** Where SavePlace() stores range. */
  nt::core::IntVec m_saveRangeItr; /** Where SavePlace() stores position. */
  ZItr m_saveElemItr; /** Where SavePlace() stores element iterator. */
};

} // namespace core
} // namespace nt

#endif // MATRIX3D_H

