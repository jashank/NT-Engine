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

#ifndef NT_MATRIX2D_H
#define NT_MATRIX2D_H

#include <boost/scoped_array.hpp>

namespace nt {

/**
 * Simple 2D matrix with basic indicing and iteration operations.
 * Index via (column,row) for more convenient (x,y) access. Starts at (0,0).
 * The iteration of the matrix is by row.
 * Does not do index checking.
 */
template < typename T >
class Matrix2D {
 public:
  typedef T* iterator;

  Matrix2D( unsigned int cols, unsigned int rows )
    :m_cols(cols), m_rows(rows), m_mat( new T[cols * rows] ) {}

  /**
   * Returns element located at (col, row). 
   * Does not check if indice is valid.
   */
  const T &operator()( unsigned int col, unsigned int row ) {
    return m_mat[Index( col, row )];
  }

  /**
   * Returns same thing as (). Made to ease syntax when dealing with a
   * pointer to a Matrix2D.
   */
  const T &Get( unsigned int col, unsigned int row ) {
    return m_mat[Index( col, row )];
  }

  /**
   * Sets element at (col, row) to value passed.
   */
  void Set( unsigned int col, unsigned int row, T val ) {
    m_mat[Index( col, row )] = val;
  }
  
  /**
   * Returns iterator to (0,0) element of matrix.
   */
  iterator begin() {
    return &m_mat[0];
  }

  /**
   * Returns iterator to one past last element of matrix.
   */
  iterator end() {
    return &m_mat[m_cols * m_rows];
  }

 private:
  //@{
  /**
   * Restrict default constructor, copy constructor, and assignment.
   */
  Matrix2D();
  Matrix2D( const Matrix2D &mat );
  Matrix2D& operator=( const Matrix2D &mat );
  //@}

  /**
   * Returns index in array corresponding to coordinate passed.
   */
  int Index( unsigned int col, unsigned int row ) {
    return (m_cols * row) + col;
  }

  unsigned int m_cols; /** Columns in matrix. */
  unsigned int m_rows; /** Rows in matrix. */
  boost::scoped_array<T> m_mat; /** Array representation of matrix. */
};

} // namespace nt

#endif // NT_MATRIX2D_H

