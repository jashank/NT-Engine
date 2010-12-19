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

#ifndef NT_MATRIX3D_H
#define NT_MATRIX3D_H

#include <list>

#include <boost/scoped_array.hpp>

#include "Rect.h"

namespace nt {

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
    :m_cols( cols ), m_rows( rows ), m_mat( new std::list<T>[cols * rows] ) {}

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
   * Returns pointer to next element in range last set 
   * (just at 0,0 if none set yet).
   * Iterates by column. When no more elements exist, returns NULL.
   */
  const T *GetElem() {
    // Counts loop iterations to keep iteration position correct
    int count = 0;

    for ( ; m_rangeItr.x <= m_range.bottomRight.x; ++m_rangeItr.x ) {
      for ( ; m_rangeItr.y <= m_range.bottomRight.y; ++m_rangeItr.y ) {

        int i = Index( m_rangeItr.x, m_rangeItr.y );
        if ( count > 0 ) {
          m_elemItr = m_mat[i].begin();
        }
        if ( m_elemItr != m_mat[i].end() ) {
          return &(*m_elemItr++);
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
  void AddElem( const T &elem, const IntRect &range ) {
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
  void RemoveElem( const T &elem, const IntRect &range ) {
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
    const T &elem,
    const IntRect &lastRange,
    const IntRect &currRange
  ) {
    if ( lastRange != currRange ) {
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
  boost::scoped_array<std::list<T> > m_mat;

  IntRect m_range; /** Range currently being iterated over. */
  IntPoint m_rangeItr; /** Iterates over dimensions of range. */
  ZItr m_elemItr; /** Iterates over elements of a dimension. */
};

} // namespace nt

#endif // NT_MATRIX3D_H

