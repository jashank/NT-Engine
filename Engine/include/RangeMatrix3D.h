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
  Matrix3D( int cols, int rows )
    :m_cols( x ), m_rows( y ) {
    int size = cols * rows;
    m_mat = new T[size];

    for ( int i = 0; i < size; ++i ) {
      m_mat[i] = std::list<T> vec;
    }
  }

  ~Matrix3D() {
    SAFEDELETEA( m_mat );
  }

  /**
   * Sets a range in the matrix to retrieve elements from via GetElem.
   * For example, if the user were to pass (3, 5, 2, 4), GetElem would return
   * elements from (3-5, 2-4). Calling this function clears any current
   * iterations being made (i.e. GetElem will not continue to return elements
   * in a range specified in a previous call).
   */
  void SetRange( int xMin, int xMax, int yMin, int yMax ) {
    m_range.topLeft.x = xMin;
    m_range.topLeft.y = yMin;
    m_range.bottomRight.x = xMax;
    m_range.bottomRight.y = yMax;

    m_itr.x = xMin;
    m_itr.y = yMin;
    m_itr.z = 0;
  }

  /**
   * Returns next element in range last set (just at 0 if none set yet).
   * Iterates by column. When no more elements exist, returns NULL.
   */
  T *GetElem() {
    for ( ; m_itr.x <= m_range.bottomRight.x; ++m_itr.x ) {
      for ( ; m_itr.y <= m_range.bottomRight.y; ++m_itr.y ) {

        int i = (m_cols * m_itr.x) + m_itr.y;
        if ( m_itr.z < m_mat[i].size() ) {
          return &(m_mat[i][m_itr.z++]);
        } else {
          m_itr.z = 0;
        }
      }
    }
    return NULL;
  }

  // GET POSITION OF LAST ELEMENT RETURNED
             
 private:
  //@{
  /**
   * Restrict default constructor, copy constructor, and assignment.
   */
  Matrix3D();
  Matrix3D( const Matrix3D &mat );
  Matrix3D& operator=( const Matrix3D &mat );
  //@}

  int m_cols; /** Columns in matrix. */
  int m_rows; /** Rows in matrix. */
  T* m_mat /** Array representation of matrix. */

  nt::core::IntRect m_range; /** Range currently being iterated over. */
  nt::core::IntVec m_itr; /** Iterates over dimensions of range. */
};

#endif // MATRIX3D_H
