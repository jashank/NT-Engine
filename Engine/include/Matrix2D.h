#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <boost/scoped_array.hpp>

namespace nt {
namespace core {

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

} // namespace core
} // namespace nt

#endif // MATRIX2D_H

