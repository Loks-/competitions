#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace opt {
namespace approximation {
template <class T = double>
class Table {
 protected:
  std::vector<T> v;
  double minx, maxx, step;

 public:
  template <class TFunction>
  Table(TFunction& f, double _minx, double _maxx, size_t size)
      : minx(_minx), maxx(_maxx) {
    assert(size > 0);
    assert(minx < maxx);
    v.reserve(size + 1);
    step = (maxx - minx) / size;
    for (unsigned i = 0; i <= size; ++i)
      v.push_back(f((i * maxx + (size - i) * minx) / size));
  }

  const T& operator()(double x) const {
    if (x <= minx) return v[0];
    size_t i = (x - minx) / step;
    return v[std::min(i, v.size() - 1)];
  }
};
}  // namespace approximation
}  // namespace opt
