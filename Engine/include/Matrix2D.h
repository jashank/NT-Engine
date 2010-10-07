#ifndef MATRIX2D_H
#define MATRIX2D_H

namespace nt {
namespace core {

/**
 * Lightweight 2D matrix with basic indicing and iteration operations.
 * Index via (column,row) for more convenient (x,y) access.
 * Does index checking.
 */
template < typename T >
class Matrix2D {
 public:
  Matrix2D( unsigned int cols, unsigned int rows )
    :m_cols(cols), m_rows(rows) {
    m_mat = new T[cols * rows];
  }

  ~Matrix2D() {
    delete m_mat;
  }

  /**
   * Returns element located at (col, row). Returns NULL if indice is invalid.
   */
  T *Matrix::operator() ( unsigned col, unsigned row ) {
    if ( col >= m_cols || row >= m_rows ) {
      return NULL;
    }
    return m_mat[m_rows*col + row];

  
 private:
  std::vector<std::vector<T> >> m_
  unsigned int m_cols;
  unsigned int m_rows;
  T *m_mat;
}

#undef // MATRIX2D_H
