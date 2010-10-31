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
   * Adds an element to range of positions in 'range', inclusive.
   */
  void AddElem( T elem, const IntRect &range ) {
    for ( int x = range.topLeft.x; x <= range.bottomRight.x; ++x ) {
      for ( int y = range.topLeft.y; y <= range.bottomRight.y; ++y ) {
        int i = Index( x, y );
        m_mat[i].push_back( elem );
      }
    }
  }

  /**
   * Removes element from range of positions, inclusive. It is okay if
   * element is not in one of the positions.
   */
  void RemoveElem( T elem, const IntRect &range ) {
    for ( int x = range.topLeft.x; x <= range.bottomRight.x; ++x ) {
      for ( int y = range.topLeft.y; y <= range.bottomRight.y; ++y ) {
        int i = Index( x, y );
        m_mat[i].remove( elem );
      }
    }
  }

  /**
   * Moves element from positions in lastRange to positions in currRange.
   */
  void MoveElem(
    T elem,
    const IntRect &lastRange,
    const IntRect &currRange
  ) {
    for ( int x = lastRange.topLeft.x; x <= lastRange.bottomRight.x; ++x ) {
      for ( int y = lastRange.topLeft.y; y <= lastRange.bottomRight.y; ++y ) {
        if ( !currRange.Contains( x, y ) ) {
          int i = Index( x, y );
          m_mat[i].remove( elem );
        }
      }
    }

    for ( int x = currRange.topLeft.x; x <= currRange.bottomRight.x; ++x ) {
      for ( int y = currRange.topLeft.y; y <= currRange.bottomRight.y; ++y ) {
        if ( !lastRange.Contains( x, y ) ) {
          int i = Index( x, y );
          m_mat[i].push_back( elem );
        }
      }
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
    return (m_cols * y) + x;
  }

  int m_cols; /** Columns in matrix. */
  int m_rows; /** Rows in matrix. */
  std::list<T> *m_mat; /** Array representation of matrix. */

  nt::core::IntRect m_range; /** Range currently being iterated over. */
  nt::core::IntVec m_rangeItr; /** Iterates over dimensions of range. */
  ZItr m_elemItr; /** Iterates over elements of a dimension. */
};

} // namespace core
} // namespace nt

#endif // MATRIX3D_H

