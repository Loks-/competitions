#pragma once

#include "common/base.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/matrix_static_size.h"

template <typename TMatrix>
inline unsigned Rows(const TMatrix& A) {
  return A.Rows();
}

template <typename TMatrix>
inline unsigned Columns(const TMatrix& A) {
  return A.Columns();
}

template <typename TValue, unsigned rows_, unsigned columns_>
inline unsigned Rows(const la::MatrixStaticSize<TValue, rows_, columns_>&) {
  return rows_;
}

template <typename TValue, unsigned rows_, unsigned columns_>
inline unsigned Columns(const la::MatrixStaticSize<TValue, rows_, columns_>&) {
  return columns_;
}

template <class TMatrix>
inline void MatrixMultPointers(const TMatrix& A, const TMatrix& B,
                               TMatrix& output) {
  using TValue = typename TMatrix::TValue;
  assert((Columns(B) == Rows(A)) && (Rows(output) == Rows(A)) &&
         (Columns(output) == Columns(B)));
  const unsigned rows = Rows(A), columns = Columns(A),
                 columns2 = Columns(output);
  output.Clear();
  const TValue* pA = A.begin();
  for (unsigned i = 0; i < rows; ++i) {
    const TValue* pB = B.begin();
    for (unsigned j = 0; j < columns; ++j) {
      const TValue& vA = *pA++;
      for (TValue *pC = output.begin() + i * columns2, *pCend = pC + columns2;
           pC < pCend;)
        *pC++ += *pB++ * vA;
    }
  }
}

template <class TMatrix>
inline void MatrixMultLoops(const TMatrix& A, const TMatrix& B,
                            TMatrix& output) {
  assert((Rows(B) == Columns(A)) && (Rows(output) == Rows(A)) &&
         (Columns(output) == Columns(B)));
  output.Clear();
  for (unsigned i = 0; i < Rows(A); ++i) {
    for (unsigned j = 0; j < Columns(A); ++j) {
      for (unsigned k = 0; k < Columns(B); ++k)
        output(i, k) += A(i, j) * B(j, k);
    }
  }
}
