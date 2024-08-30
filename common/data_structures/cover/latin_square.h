#pragma once

#include "common/base.h"
#include "common/data_structures/cover/dlx.h"
#include "common/linear_algebra/matrix.h"

namespace ds {
namespace cover {
class LatinSquare {
 protected:
  size_t n;
  DLX dlx;

 public:
  void Init(size_t size) {
    n = size;
    const size_t dlcolumns = 3 * n * n;
    const size_t dlrows = n * n * n;
    dlx.Init(dlrows, dlcolumns);
    for (size_t i = 0; i < dlrows; ++i) {
      const size_t r = Row(i), c = Column(i), v = Value(i);
      dlx.Add(i, v * n + r);
      dlx.Add(i, n * n + r * n + c);
      dlx.Add(i, 2 * n * n + c * n + v);
    }
  }

  size_t DLRow(size_t row, size_t column, size_t value) const {
    return value + n * (column + n * row);
  }

  size_t Row(size_t dlrow) const { return dlrow / (n * n); }

  size_t Column(size_t dlrow) const { return (dlrow / n) % n; }

  size_t Value(size_t dlrow) const { return dlrow % n; }

  void Set(size_t row, size_t column, size_t value) {
    dlx.SelectRow(DLRow(row, column, value));
  }

  void EraseLast() { dlx.ReleaseLastRow(); }

  bool SearchAny() { return dlx.SearchAny(); }

  la::Matrix<size_t> GetSolution() const {
    la::Matrix<size_t> m(n);
    for (size_t dlrow : dlx.SelectedRows())
      m(Row(dlrow), Column(dlrow)) = Value(dlrow);
    return m;
  }
};
}  // namespace cover
}  // namespace ds
