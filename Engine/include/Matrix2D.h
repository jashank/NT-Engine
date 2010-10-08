#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <vector>

namespace nt {
namespace core {

/**
 * Simple 2D matrix with basic indicing and iteration operations.
 * Index via (column,row) for more convenient (x,y) access. Starts at (0,0).
 * Does not do index checking.
 */
template < typename T >
class Matrix2D {
 public:
  typedef T* iterator;

  Matrix2D( unsigned int cols, unsigned int rows )
    :m_cols(cols), m_rows(rows) {
    m_mat = new T[cols * rows];
  }

  ~Matrix2D() {
    delete[] m_mat;
  }

  /**
   * Returns pointer to element located at (col, row). 
   * Does not check if indice is valid.
   */
  T *operator() ( unsigned int col, unsigned int row ) {
    return m_mat[(m_col * row) + col];
  }

  /**
   * Returns (0,0) element of matrix.
   */
  iterator begin() {
    return &m_mat[0];
  }

  /**
   * Returns one past last element of matrix.
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

  unsigned int m_cols; /** Columns in matrix. */
  unsigned int m_rows; /** Rows in matrix. */
  T* m_mat; /** Array representation of matrix. */
};

} // namespace core
} // namespace nt

#undef // MATRIX2D_H

